/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>
#include <boost/algorithm/string.hpp>

#include <pelib/ParseException.hpp>
#include <pelib/NoDecimalFloatException.hpp>
#include <pelib/Algebra.hpp>
#include <pelib/AlgebraData.hpp>
#include <pelib/DataParser.hpp>

#ifndef PELIB_ALGEBRADATAPARSER
#define PELIB_ALGEBRADATAPARSER

namespace pelib
{
	/** Base class of any class able to parse a text into a data structure that can be contained in an instance of Algebra. This class must be derived to be used **/
	class AlgebraDataParser : public DataParser
	{
		public:
			/** Converts a string into an instance of T and throws a ParseException upon conversion error. If T is a numeric base type (int, float and derivatives) and strict is true, then an attempt to convert a string that matches an integer value into a float or double results in throwing a NoDecimalException **/
			template <class Target>
			static
			Target
			convert(std::string element, bool strict = 0)
			{
				Target out;
				bool infinity, positive;

				if(is_integer(typeid(out)) || is_decimal(typeid(out)))
				{
					boost::algorithm::to_lower(element);
					if(element.compare("+inf") == 0 || element.compare("inf") == 0)
					{
						out = std::numeric_limits<Target>::max();
						infinity = true;
						positive = true;
					}
					else
					{
						if(element.compare("-inf") == 0)
						{
							out = std::numeric_limits<Target>::min();
							infinity = true;
							positive = false;
						}
						else
						{
							std::istringstream converter(element);
							converter >> out;
							infinity = false;

							if(converter.fail())
							{
								throw ParseException(std::string("Couln't convert literal \"").append(element).append("\" into type \"").append(typeid(out).name()).append("\"."));
							}

							std::stringstream ss;
							ss << out;
							double val;
							ss >> val;

							positive = val >= 0;
						}
					}
				}
				else
				{
					std::istringstream converter(element);
					converter >> out;

					if(converter.fail())
					{
						throw ParseException(std::string("Couln't convert literal \"").append(element).append("\" into type \"").append(typeid(out).name()).append("\"."));
					}
				
					return out;	
				}

#if TRACE
				std::cerr << "String element: \"" << element << "\"" << std::endl;
				std::cerr << "Strict checking: \"" << strict << "\"" << std::endl;
				std::cerr << "Let us proceed with optional checking" << std::endl;
#endif
				if(strict)
				{
#if TRACE
					std::cerr << "OK so we have to check" << std::endl;
#endif
					// We asked a floating-point conversion, but provided a integer
					//if(strcmp(typeid(out).name(), "f") == 0 || strcmp(typeid(out).name(), "d") == 0)
					if(is_decimal(typeid(out)) && ! is_integer(typeid(out)))
					{
#if TRACE
						std::cerr << "So we are asking for a decimal conversion" << std::endl;
#endif
						// let's try to parse against a fixed-point value
						//match("[-+]?\\d+\\.\\d+", element);
						boost::cmatch match;
						if(!boost::regex_match(element.c_str(), match, boost::regex("[-+]?\\d+\\.\\d+")))
						{
#if TRACE
							// This should bin in the else clause
							//std::cerr << "\"" << element << "\" passed the fixed-point format matching" << std::endl;
#endif
							// OK so it doesn't parse a fixed-point notation
							// Then I suppose it was a scientific notation; let's see if it indeed denotes a decimal digit
							double val;
							std::stringstream ss;
							ss << out;
							ss >> val;
							long long int int_test = (long long int)floor(val);
							//std::istringstream converter(element);
							//converter >> int_test;

#if TRACE
							std::cerr << "\"" << element << "\" is not at fixed-point format, may be scientific notation" << std::endl;
#endif

							if(int_test == val && !infinity)
							{
								// Integer-converted and floating-point were equal, then it was an integer, you fool
#if TRACE
								std::cerr << "The decimal part of \"" << element << "\" is nul, therefore we have an integer" << std::endl;
#endif
								throw NoDecimalFloatException(std::string("Asked a decimal conversion, but \"").append(element).append("\" is integer."), val);
							}

							if(infinity && std::numeric_limits<Target>::has_infinity)
							{
								ss.str("");
								ss.clear(); // Clear state flags.
								ss << std::numeric_limits<Target>::infinity();
								ss >> val;

								ss.str("");
								ss.clear();
								ss << (positive ? val : -1 * val);
								ss >> out;
							}
#if TRACE
							std::cerr << "Passed the scientific format matching" << std::endl;
#endif
						}
#if TRACE
						std::cerr << "OK I valid this conversion" << std::endl;
#endif
					}
						
					// We asked an integer conversion, but provided a float
					//if(strcmp(typeid(out).name(), "f") != 0 && strcmp(typeid(out).name(), "d") != 0)
					if(! is_decimal(typeid(out)) && is_integer(typeid(out)))
					{
#if TRACE
						std::cerr << "So we are asking for a integer conversion" << std::endl;
#endif
						// let's try to parse against a fixed-point value
						//match("[+-]?\\d+\\.\\d+", element);
						boost::cmatch match;
						if(!boost::regex_match(element.c_str(), match, boost::regex("[+-]?\\d+\\.\\d+")))
						{
#if TRACE
							std::cerr << "It is not fixed-point format, good" << std::endl;
#endif
							// Then I suppose it was a scientific notation; let's see if it indeed denotes an integer digit
							if(!infinity)
							{
								double int_test;
								std::istringstream converter(element);
								converter >> int_test;

								long long int val;
								std::stringstream ss;
								ss << out;
								ss >> val;

								if(int_test != val)
								{
#if TRACE
									std::cerr << "But this is not an integer anyway" << std::endl;
#endif
									// Integer-converted and floating-point were equal, then it was an integer, you fool
									throw ParseException(std::string("Asked an integer conversion, but \"").append(element).append("\" is decimal."));
								}
#if TRACE
								std::cerr << "OK I valid this conversion" << std::endl;
#endif
								return out;
							}
							else
							{
								if(infinity && std::numeric_limits<Target>::has_infinity)
								{
									long long int val;
									std::stringstream ss;
									ss << std::numeric_limits<Target>::infinity();
									ss >> val;
	
									ss.str("");
									ss.clear();
									ss << (positive ? val : -1 * val);
									ss >> out;
	
									return out;
								}
								else
								{
									return out;
								}
							}
						}
#if TRACE
						std::cerr << "But element was fixed-point format" << std::endl;
#endif
						throw ParseException(std::string("Asked an integer conversion, but \"").append(element).append("\" is decimal."));
					}
				}
				else
				{
					if(infinity && std::numeric_limits<Target>::has_infinity)
					{
						std::stringstream ss;
					
						if(is_decimal(typeid(out)))
						{
							long double val;
							ss << std::numeric_limits<Target>::infinity();
							ss >> val;
							ss.str("");
							ss.clear();
							ss << (positive ? val : -1 * val);
						}
						else
						{
							long long int val;
							ss << std::numeric_limits<Target>::infinity();
							ss >> val;
							ss.str("");
							ss.clear();
							ss << (positive ? val : -1 * val);
						}

						ss >> out;
						return out;
					}
						else
					{
						return out;
					}
				}

				return out;
			}

			// This is causing too much problem, probably due to a bug in boost::regex, even when inlining
			// Inline manually instead
#if 0
			/** Returns all matches of regex found in str **/
			static inline
			boost::cmatch
			match(std::string regex, std::string str);
#endif

			/** Returns a boost regular expression that extracts all data in the text to parse, that is stored in the corresponding data structure produced, such as its name or its values. **/
			virtual
			std::string
			getDetailedPattern() = 0;

			/** Returns a boost regular expression that matches the string representation of an algebraic data structure **/
			virtual
			std::string
			getGlobalPattern() = 0;

			/** Parses a text stream into a new instance derived from AlgebraData **/
			virtual
			AlgebraData*
			parse(std::istream &in) = 0;
		protected:
			static bool is_decimal(const std::type_info &var)
			{
				return (std::string(var.name()).compare(std::string(typeid(float).name())) == 0) ||
					(std::string(var.name()).compare(std::string(typeid(double).name())) == 0) ||
					(std::string(var.name()).compare(std::string(typeid(long double).name())) == 0);
			}

			static bool is_integer(const std::type_info &var)
			{
				/*
					List obtained by running the following C program
					#include <iostream>
					#include <cstdlib>
					#include <typeinfo>

					using namespace std;

					#define show(type) cout << #type << " " << typeid(type).name() << endl;

					int
					main(int argc, char **argv)
					{
						show(int);
						show(unsigned int);
						show(long int);
						show(long unsigned int);
						show(long long int);
						show(long long unsigned int);
						cout << endl;
						show(size_t);
						show(ptrdiff_t);
						show(signed size_t);
						show(unsigned ptrdiff_t);
						show(long size_t);
						show(long long size_t);
						show(long ptrdiff_t);
						show(long long ptrdiff_t);
						cout << endl;
						show(float);
						show(double);
						show(long double);
						return EXIT_SUCCESS;
					}

					./test | rev | sort -k 1 | uniq -w 1 | cut -f 1 -d ' ' --complement | rev
				*/

				return (std::string(var.name()).compare(std::string(typeid(int).name())) == 0) ||
					(std::string(var.name()).compare(std::string(typeid(unsigned int).name())) == 0) ||
					(std::string(var.name()).compare(std::string(typeid(long size_t).name())) == 0) ||
					(std::string(var.name()).compare(std::string(typeid(size_t).name())) == 0) ||
					(std::string(var.name()).compare(std::string(typeid(long long size_t).name())) == 0) ||
					(std::string(var.name()).compare(std::string(typeid(long long unsigned int).name())) == 0);
			}			
		private:
			// Nothing private
	};
}

#endif
