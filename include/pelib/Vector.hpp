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

#include <pelib/PelibException.hpp>
#include <pelib/AlgebraData.hpp>

#ifndef PELIB_VECTOR
#define PELIB_VECTOR

namespace pelib
{
	/** Models an algebraic vector of data **/
	template <class Key, class Value> 
	class Vector: public AlgebraData
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			/** Constructor
				@param name name associated to this vector
				@param values Values held in the vector associated to an ordered set of indexes
			**/
			Vector(const std::string name, const VectorType values) : AlgebraData(name), values(values)
			{
				// Do nothing
			}

			/** Copy constructor **/
			Vector(const Vector<Key, Value>* vector): AlgebraData(vector->getName()), values(vector->getValues())
			{
				// Do nothing
			}

			/** Returns a pointer to a copy of this Vector **/
			virtual
			Vector*
			clone() const
			{
				return new Vector<Key, Value>(name, values);
			}

			/** Returns all values held in this Vector, associated to their key **/
			virtual
			const std::map<Key, Value>&
			getValues() const
			{
				return values;
			}

			/** Returns a single value in the Vector thanks to the key associated to the value **/
			virtual
			const Value&
			find(Key key) const
			{
				return values.find(key)->second;
			}

			/** Returns the number of elements stored in the Vector **/
			virtual
			size_t
			getSize() const
			{
				return values.size();
			}
			
			void
			merge(const AlgebraData *ptr)
			{
				// Only allow merging if both this and ptr are of same name and type
				if(ptr->getName().compare(this->getName()) == 0 && std::string(typeid(*ptr).name()).compare(typeid(Vector<Key, Value>).name()) == 0)
				{
					Vector<Key, Value> *vector = (Vector<Key, Value>*)ptr;
					for(typename VectorType::iterator i = vector->values.begin(); i != vector->values.end(); i++)
					{
						if(this->getValues().find(i->first) != this->getValues().end())
						{
							this->values.erase(this->values.find(i->first));
						}
						// Add the new row in this matrix
						this->values.insert(std::pair<Key, Value>(i->first, i->second));
					}
				}
				else
				{
					throw PelibException(std::string("Cannot merge data \"") + ptr->getName() + "\" with " + typeid(Value).name() + " vector of name \"" + this->getName() + "\".");
				}
			}
		protected:
			VectorType values;
		private:		
	};
}

#endif
