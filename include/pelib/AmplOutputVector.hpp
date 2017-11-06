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


#include <pelib/AmplOutputData.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_AMPLOUTPUTVECTOR
#define PELIB_AMPLOUTPUTVECTOR

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;

namespace pelib
{
	/** Parser and output class in AMPL output format for pelib algebraic Vector data structure **/
	template <class Key, class Value>
	class AmplOutputVector: public AmplOutputData
	{
		/** Type of a pelib::Vector's internal value collection **/
		typedef std::map<Key, Value> VectorType;
		
		public:
			/** Constructor
				@param strict If true and parsing a floating-point values-collection, fails if any element is an integer but succeeds if integers are written with a trailing decimal 0 (e.g. 52.0)
			**/
			AmplOutputVector(bool strict = true)
			{
				this->strict = strict;
			}

			/** Returns a pointer to a opy of this class instance **/
			virtual
			AmplOutputVector*
			clone() const
			{
				return new AmplOutputVector();
			}

			/** Uses a boost::regex regular expression to match a vector written in AMPL output format and extract its name and value **/
			virtual
			std::string
			getDetailedPattern()
			{
				return "(\\w[\\w\\d_]*)\\s*\\[\\*\\]\\s*:=\\s*((?:[-+\\w\\d][-\\w\\d_+\\.]*\\s+[-+\\w\\d][-\\w\\d_\\.+]*[\\s\\n]+)+)";
			}

			/** Uses a boost::regex regular expression to match a vector written in AMPL output format **/
			virtual
			std::string
			getGlobalPattern()
			{
				return "\\w[\\w\\d_]*\\s*\\[\\*\\]\\s*:=.+";
			}

			/** Reads input stream in AMPL output format and builds an instance of pelib::Vector containing all values
				@param in Input stream to read AMPL output text from
				@return An instance of pelib::Vector containing all values read from input
			**/
			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				VectorType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				//std::cerr << str << std::endl;
				//std::cerr << std::string("(?:.*?)").append(getDetailedPattern()) << std::endl;
				boost::cmatch match; // = AlgebraDataParser::match(std::string("(?:.*?)").append(getDetailedPattern()), str);
				if(!boost::regex_match(str.c_str(), match, boost::regex(std::string("(?:.*?)").append(getDetailedPattern()))))
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

					std::string key_str = *iter++;
					std::string val_str = *iter;

					try
					{
						key = AlgebraDataParser::convert<Key>(key_str, strict);
					} catch(NoDecimalFloatException &e)
					{
						std::ostringstream ss;
						ss << e.getValue();
						throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));		
					}
					
					try
					{
						value = AlgebraDataParser::convert<Value>(val_str, strict);
					} catch(NoDecimalFloatException &e)
					{
						float float_value = e.getValue();
						std::stringstream ss;
						ss << float_value;
						ss >> value;
						integer_values++;
					}

					values.insert(std::pair<Key, Value>(key, value));
					total_values++;
				}

				// If all values could have been parsed as integer, then this is obviously an integer vector rather to a float one
				if(integer_values == total_values)
				{
					//throw NoDecimalFloatException(std::string("Vector only composed of integer-parsable values."), 0);
					throw ParseException(std::string("Vector only composed of integer-parsable values."));
				}

				return new Vector<Key, Value>(match[1], values);
			}

			/** Writes an instance of pelib::Vector into an output stream in AMPL output format
				@param o Output stream
				@param data Instance of pelib::Vector to be written
			**/
			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Vector<Key, Value> *vector = dynamic_cast<const Vector<Key, Value>*>(data);
				if(vector == NULL) throw CastException("parameter \"data\" was not of type \"Vector<Key, Value>\".");
				
				o << vector->getName() << " [*] := " << std::endl;

				// warning: "iter++" makes valgrind unhappy, dealing with free'd memory
				for(typename std::map<Key, Value>::const_iterator iter = vector->getValues().begin(); iter != vector->getValues().end(); iter++)
				{
					o << iter->first << "\t" << iter->second << std::endl;
				}
				
				o << ";" << std::endl;		
			}

		protected:
			/** Defines if parsing operations are performed in strict mode (see constructor) **/
			bool strict;
		private:	
	};
}

#undef debug
#endif
