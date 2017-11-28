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
#include <pelib/AmplInputData.hpp>
#include <pelib/Vector.hpp>
#include <pelib/CastException.hpp>
#include <pelib/NoDecimalFloatException.hpp>

#ifndef PELIB_AMPLINPUTVECTOR
#define PELIB_AMPLINPUTVECTOR

#ifdef debug
#undef debug
#endif

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;

namespace pelib
{
	/** Parser and Outputer of a Pelib::Vector in AMPL input format **/
	template <class Key, class Value>
	class AmplInputVector: public AmplInputData
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			/**
				Constructor
				@param strict if Value is a floating-point number, fails if any element parsed is an integer (but succeeds if it is written as a float, i.e., 52.0
			**/
			AmplInputVector(bool strict = true)
			{
				this->strict = strict;
			}
			
			/** Returns a pointer to a copy of this class instance **/
			virtual
			AmplInputVector*
			clone() const
			{
				return new AmplInputVector();
			}
			
#if 0
// Get precise desriptive string of a type
// https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>

template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}
#endif
			/** Parses the input stream into a instance of Pelib::Vector
				@param in input stream in AMPL input data format to be parsed
			 **/
			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				VectorType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				boost::cmatch match; // = AlgebraDataParser::match(getDetailedPattern(), str);
				if(!boost::regex_match(str.c_str(), match, boost::regex(getDetailedPattern())))
				{
					throw ParseException(std::string("String \"").append(str).append("\" doesn't match regex \"").append(getDetailedPattern()).append("\". "));
				}
				
				boost::regex param_vector("(?:\\s*([^\\s]+)\\s+([^\\s]+))");
				std::string remain = match[2];
				const int subs[] = {1, 2};
				boost::sregex_token_iterator iter = make_regex_token_iterator(remain, param_vector, subs, boost::regex_constants::match_default);
				boost::sregex_token_iterator end;

				int integer_values = 0, total_values = 0;
				for(; iter != end; ++iter )
				{
					Key key;
					Value value;

					try
					{
						key = AlgebraDataParser::convert<Key>(*iter++, strict);
					} catch(NoDecimalFloatException &e)
					{
						std::ostringstream ss;
						ss << e.getValue();
						throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));
					}
					
					try
					{
						value = AlgebraDataParser::convert<Value>(*iter, strict);
					} catch(NoDecimalFloatException &e)
					{
						std::stringstream ss;
						ss << e.getValue();
						ss >> value;
						integer_values++;
					}

					values.insert(std::pair<Key, Value>(key, value));
					total_values++;
				}

				// If all values could have been parsed as integer, then this is obviously an integer vector rather to a float one
				if(integer_values == total_values && is_decimal(typeid(Value)))
				{
					//throw NoDecimalFloatException(std::string("Vector only composed of integer-parsable values."), 0);
					/*
					Key key;
					Value value;
					debug(type_name<decltype(key)>());
					debug(type_name<decltype(value)>());
					*/
					throw ParseException(std::string("Vector only composed of integer-parsable values."));
				}

				return new Vector<Key, Value>(match[1], values);
			}

			/** Dumps the content of a pelib::Vector into an output stream in AMPL input data format
				@param o Output stream where is written the vector in AMPL input data format
				@param data pelib::Vector to be written to output stream
			**/
			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{
				const Vector<Key, Value> *vector = dynamic_cast<const Vector<Key, Value>*>(data);
				if(vector == NULL) throw CastException("parameter \"data\" was not of type \"Vector<Key, Value>\".");
					
				o << "param: " << vector->getName() << " :=" << std::endl;
				VectorType values = vector->getValues();
				
				for(typename std::map<Key, Value>::const_iterator iter = values.begin(); iter != values.end(); iter++)
				{
					o << iter->first << " " << iter->second << std::endl;
				}
			
				o << ";" << std::endl;				
			}

			/** Returns a boost::regex regular expression that matches a AMPL input data vector data structure and can extract all its elements **/
			virtual
			std::string
			getDetailedPattern()
			{
				return "param\\s*:\\s*([^\\s\\n]+)\\s*:=(.*)";
			}

			/** Returns a boost::regex regular expression that matches a AMPL input data vector data structure **/
			virtual
			std::string
			getGlobalPattern()
			{
				return "param\\s*:\\s*[^\\s\\n]+\\s*:=.*";
			}

		protected:
			/** Internal record of the strict or not strict parsing policy of this vector parser instance **/
			bool strict;
		private:	
	};
}

#undef debug
#endif
