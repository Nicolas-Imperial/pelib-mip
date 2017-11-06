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
#include <pelib/DataParser.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>
#include <pelib/NoDecimalFloatException.hpp>

#ifndef PELIB_AMPLOUTPUTSCALAR
#define PELIB_AMPLOUTPUTSCALAR

namespace pelib
{
	/** Parser and output class for a pelib::Scalar in AMPL output format **/
	template <class Value>
	class
	AmplOutputScalar: public AmplOutputData
	{
		public:
			/** Constuctor
				@param strict If true and parsing floating point values, fails in any element parsed is an integer. An integer written as a float (e.g. 52.0) doesn't make the parser to fail.
			**/
			AmplOutputScalar(bool strict = true)
			{
				this->strict = strict;
			}
			
			/** Returns a pointer to a copy of this instance **/
			virtual
			AmplOutputScalar*
			clone() const
			{
				return new AmplOutputScalar();
			}

			/** Returns a boost::regex string able to match a scalar in AMPL output format and extract its name and value **/
			virtual
			std::string
			getDetailedPattern()
			{
				return "(\\w[\\w\\d_]*)\\s*=\\s*([-+\\w\\d][-\\w\\d_\\.+]*)";
			}

			/** Returns a boost::regex string able to match a scalar in AMPL output format **/
			virtual
			std::string
			getGlobalPattern()
			{
				return "\\w[\\w\\d_]*\\s*=\\s*[-+\\w\\d][-\\w\\d_\\.+]*";
			}

			/** Reads input stream and builds an instance of pelib::Scalar containing the name and value read **/
			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

				boost::cmatch match;
				if(!boost::regex_match(str.c_str(), match, boost::regex(std::string("(?:.*?)").append(getDetailedPattern()))))
				{
					throw ParseException(std::string("String \"").append(str).append("\" doesn't match regex \"").append(getDetailedPattern()).append("\". "));
				}

				Scalar<Value> *scalar = new Scalar<Value>(match[1], AlgebraDataParser::convert<Value>(match[2], strict));
				return scalar;
			}

			/** Writes the name and value of a pelib::Scalar to output stream in AMPL output format **/
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

				stream << scalar->getName() << " = " << val << std::endl;
			}
	
		protected:
			bool strict;
		private:		
	};
}

#endif
