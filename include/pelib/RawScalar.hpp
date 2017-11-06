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


#include <pelib/RawData.hpp>
#include <pelib/DataParser.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>
#include <pelib/NoDecimalFloatException.hpp>

#ifndef PELIB_RAWSCALAR
#define PELIB_RAWSCALAR

namespace pelib
{
	/** Parser and output class for a pelib::Scalar in AMPL output format **/
	template <class Value>
	class
	RawScalar: public RawData
	{
		public:
			/** Returns a pointer to a copy of this instance **/
			virtual
			RawScalar*
			clone() const
			{
				return new RawScalar();
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

				stream << val << std::endl;
			}
	
		protected:
		private:		
	};
}

#endif
