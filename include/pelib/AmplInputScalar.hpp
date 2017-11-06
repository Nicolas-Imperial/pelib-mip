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


#include <pelib/AmplInputData.hpp>
#include <pelib/DataParser.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>
#include <pelib/NoDecimalFloatException.hpp>
#ifndef PELIB_AMPLINPUTSCALAR
#define PELIB_AMPLINPUTSCALAR

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;

namespace pelib
{
	/** Base class for a parser or output class of a scalar in AMPL input data format **/
	template <class Value>
	class
	AmplInputScalar: public AmplInputData
	{
		public:
			/** Constructor
				@param strict If true and parsing floating point values, fails when reading any integer value if not written with a trailing decimal 0, e.g. 52.0
			**/
			AmplInputScalar(bool strict = true)
			{
				this->strict = strict;
			}
			
			/** Return a pointer to a copy of this class instance **/
			virtual
			AmplInputScalar*
			clone() const
			{
				return new AmplInputScalar();
			}
			
			/** Parse input stream in AMPL input format and builds an instance of a derivative class of pelib::AlgebraData containing the values read **/
			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				boost::cmatch match;
				// TODO: Investigate why regex doesn't return correct string when called in AlgebraDataParser
				if(!boost::regex_match(str.c_str(), match, boost::regex(getDetailedPattern())))
				{
					throw ParseException(std::string("String \"").append(str).append("\" doesn't match regex \"").append(getDetailedPattern()).append("\". "));
				}
				/*
				try
				{
					match = AlgebraDataParser::match(getDetailedPattern(), str);
	} catch(NoDecimalFloatException &e)
				{
					std::ostringstream ss;
					ss << e.getValue();
					throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));
				}
				*/

				std::string match1 = match[1];
				std::string match2 = match[2];
				Value val = AlgebraDataParser::convert<Value>(match2, strict);
				Scalar<Value> *scalar = new Scalar<Value>(match1, val);
				return scalar;
			}

			/** Writes an instance of class that derives from pelib::AlgebraData to output stream
				@param stream Output stream that receives the written output of AlgebraData
				@param data Instance of a class derived from AlgebraData to write to output stream
			**/
			virtual
			void
			dump(std::ostream &stream, const AlgebraData *data) const
			{
				const Scalar<Value> *scalar = dynamic_cast<const Scalar<Value>* >(data);
				if(scalar == NULL) throw CastException("parameter \"data\" was not of type \"Scalar<Value>\".");

				Value val = scalar->getValue();
				if(scalar->getPrecision() == AlgebraData::higher)
				{
					val = AlgebraData::fixPrecision(val, stream.precision());
				}

				stream << "param " << scalar->getName() << " := " << val << ";" << std::endl;
			}

			/** Matches a data structure written in AMPL input data format and extract its name and content **/
			virtual
			std::string
			getDetailedPattern()
			{
				return "param\\s+([^\\s\\n]*)\\s*:=\\s*([^\\s]+)\\s*";
			}

			/** Matches a data structure written in AMPL input data format **/
			virtual
			std::string
			getGlobalPattern()
			{
				return "param\\s+[^\\s\\n]*\\s*:=.+";
			}
	
		protected:
			/** Defines if parsing should be performed in strict mode **/
			bool strict;
		private:		
	};
}

#undef debug
#endif
