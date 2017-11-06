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

#include <pelib/AlgebraData.hpp>
#include <pelib/PelibException.hpp>

#ifndef PELIB_SCALAR
#define PELIB_SCALAR

namespace pelib
{
	/** Algebraic scalar value associated to a string name **/
	template <class Value>
	class Scalar: public AlgebraData
	{
		public:
			/** Constructor: associates a single value to a string name **/
			Scalar(const std::string name, const Value value, precision prec = leave) : AlgebraData(name, prec)
			{
				this->value = value;
			}

			/** Copy constructor **/
			Scalar(const Scalar<Value>* scalar) : AlgebraData(scalar->getName(), scalar->getPrecision())
			{
				// Do nothing
			}

			/** Returns a pointer to a copy of this scalar **/
			virtual
			Scalar*
			clone() const
			{
				return new Scalar<Value>(name, value, prec);
			}

			/** Return the value associated to this instance **/
			virtual
			const Value&
			getValue() const
			{
				return value;
			}
			
			void	
			merge(const AlgebraData* ptr)
			{
				// Only allow merging if both this and ptr are of same name and type
				if(ptr->getName().compare(this->getName()) == 0 && std::string(typeid(*ptr).name()).compare(typeid(Scalar<Value>).name()) == 0)
				{
					Scalar<Value> *scalar = (Scalar<Value>*)ptr;
					this->value = scalar->getValue();
				}
				else
				{
					throw PelibException(std::string("Cannot merge data \"") + ptr->getName() + "\" with " + typeid(Value).name() + " scalar of name \"" + this->getName() + "\".");
				}
			}
		protected:
			/** Value help by this instance **/
			Value value;
		private:		
	};
}

#endif
