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
#include <pelib/Vector.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_RAWVECTOR
#define PELIB_RAWVECTOR

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;

namespace pelib
{
	/** Parser and output class in AMPL output format for pelib algebraic Vector data structure **/
	template <class Key, class Value>
	class RawVector: public RawData
	{
		public:
			/** Returns a pointer to a opy of this class instance **/
			virtual
			RawVector*
			clone() const
			{
				return new RawVector();
			}

			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Vector<Key, Value> *vector = dynamic_cast<const Vector<Key, Value>*>(data);
				if(vector == NULL) throw CastException("parameter \"data\" was not of type \"Vector<Key, Value>\".");
				// warning: "iter++" makes valgrind unhappy, dealing with free'd memory
				for(typename std::map<Key, Value>::const_iterator iter = vector->getValues().begin(); iter != vector->getValues().end(); iter++)
				{
					o << iter->second << " ";
				}
				
				o << std::endl;		
			}

		protected:
		private:	
	};
}

#undef debug
#endif
