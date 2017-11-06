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


#include <set>

#include <pelib/AmplInputData.hpp>
#include <pelib/Set.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_AMPLINPUTSET
#define PELIB_AMPLINPUTSET

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;

namespace pelib
{
	/** Parser and output class for Set in AMPL input data text format **/
	template <class Value, class Key = size_t>
	class AmplInputSet: public AmplInputData
	{
		/** Type of the set data manipulated in member functions **/
		typedef std::set<Value, std::less<Value>, std::allocator<Value> > SetType;
		
		public:
			/** Constructor
				@param strict If true and Value is a floating-point numeric type, fails if one element is an integer. An integer written as a float (e.g. 52.0) doesn't make the parser to fail
			**/
			AmplInputSet(bool strict = true)
			{
				this->strict = strict;
			}
			
			/** Returns a pointer to a copy of this class instance **/
			virtual
			AmplInputSet*
			clone() const
			{
				return new AmplInputSet();
			}
			
			/** Parse input stream in AMPL input data format into a pointer to an instance of pelib::Set
				@param in Input stream in AMPL input data format to be parsed
				@return An instance of pelib::Set holding all values parsed from in **/
			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				SetType values;
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

				// TODO: investigate why using AlgebraDataParser results in random results
				boost::cmatch match;
				if(!boost::regex_match(str.c_str(), match, boost::regex(getDetailedPattern())))
				{
					throw ParseException(std::string("String \"").append(str).append("\" doesn't match regex \"").append(getDetailedPattern()).append("\". "));
				}
				//boost::cmatch match(AlgebraDataParser::match(getDetailedPattern(), str));
				//boost::cmatch match = AlgebraDataParser::match(getDetailedPattern(), str);
				
				boost::regex param_set("\\s*([^\\s]+)");
				std::string keystr = match[2];
				std::string remain = match[3];
				const int subs[] = {1};
				boost::sregex_token_iterator iter = make_regex_token_iterator(remain, param_set, subs, boost::regex_constants::match_default);
				boost::sregex_token_iterator end;

				int integer_values = 0, total_values = 0;
				for(; iter != end; ++iter )
				{
					Value value;
					
					try
					{
						value = AlgebraDataParser::convert<Value>(*iter, strict);
					}
					catch(NoDecimalFloatException &e)
					{
						std::stringstream ss;
						ss << e.getValue();
						ss >> value;
						integer_values++;
					}

					values.insert(value);
					total_values++;
				}

				// If all values could have been parsed as integer, then this is obviously an integer vector rather to a float one
				if(integer_values == total_values)
				{
					//throw NoDecimalFloatException(std::string("Set only composed of integer-parsable values."), 0);
					throw ParseException(std::string("Set only composed of integer-parsable values."));
				}

				if(keystr.compare(std::string()) == 0)
				{
					return new Set<Value>(match[1], values);
				}
				else
				{
					Key key;

					try
					{
						key = AlgebraDataParser::convert<Key>(keystr, strict);
					}
					catch(NoDecimalFloatException &e)
					{
						std::stringstream ss;
						ss << e.getValue();
						ss >> key;
					}

					return new Set<Value, Key>(match[1], key, values);
				}
			}

			/** Output all values of instance of pelib::set into output stream in AMPL input data format
				@param o Output stream to write to in AMPL input data format
				@param data Instance of pelib::Set whose values are to be written to output stream
			 **/
			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{
				const Set<Value, Key> *set = dynamic_cast<const Set<Value>*>(data);
				if(set == NULL) throw CastException("parameter \"data\" was not of type \"Set<Value>\".");				
				
				for(typename Set<Value, Key>::SetOfSetsType::const_iterator i = set->getSubsets().begin(); i != set->getSubsets().end(); i++)
				{
					if(set->isOneSet())
					{
						o << "set " << set->getName() << " :=";
					}
					else
					{
						o << "set " << set->getName() << "[" << i->first << "] :=";
					}
					
					SetType values = i->second;
					for(typename std::set<Value>::const_iterator iter = values.begin(); iter != values.end(); iter++)
					{
						o << " " << *iter;
					}

					o << ";" << std::endl;
				}
			}

			/** Returns a boost::regex regular expression that matches a Set in AMPL input data format and able to extract its elements **/
			virtual
			std::string
			getDetailedPattern()
			{
				return "set\\s*([^\\s\\n\\[\\]]+)\\s*(?:\\[([^\\s\\n]+)\\])?\\s*:=(.+)";
			}

			/** Returns a boost::regex regular expression that matches a Set in AMPL input data format **/
			virtual
			std::string
			getGlobalPattern()
			{
				return "set\\s*[^\\s\\n\\[\\]]+\\s*(?:\\[[^\\s\\n]+\\])?\\s*:=.+";
			}

		protected:
			/** Defines if parsin operation should fail upon the parsing of an integer when the Set holds floating-point numeric values **/
			bool strict;
		private:	
	};
}

#undef debug
#endif
