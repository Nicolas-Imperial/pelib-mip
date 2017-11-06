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
#include <set>

#include <pelib/AlgebraData.hpp>
#include <pelib/PelibException.hpp>

#ifndef PELIB_SET
#define PELIB_SET

namespace pelib
{
	/** Mathematical Set **/
	template <class Value, class Key = size_t> 
	class Set: public AlgebraData
	{
		public:
			/** Type of the container that holds all values in this set. **/
			typedef std::set<Value> SetType;
			typedef std::map<Key, SetType> SetOfSetsType;

			/** Constructor of a Set with name and values passed as parameters **/
			Set(const std::string name, const SetType values) : AlgebraData(name)
			{
				oneSet = 1;
				if(std::string(typeid(Key).name()).compare(std::string(typeid(size_t).name())) == 0)
				{
					this->values.insert(std::pair<size_t, SetType>(1, values)); 
				}
				else
				{
					throw PelibException(std::string("Trying to build a Set of one subset with an invalid subset key (got key type \"") + typeid(Key).name() + "\", expected \"" + typeid(size_t).name() + "\".");
				}
			}

			/** Constructor of a Set with name, key and values passed as parameters **/
			Set(const std::string name, const Key &key, const SetType values) : AlgebraData(name)
			{
				oneSet = 0;
				this->values.insert(std::pair<Key, SetType>(key, values)); 
			}

			/** Constructor of a Set with name and values passed as parameters **/
			Set(const std::string name, const SetOfSetsType values) : AlgebraData(name), values(values)
			{
				oneSet = 0;
				// Do nothing
			}

			/** Copy constructor **/
			Set(const Set<Value, Key>* set): AlgebraData(set->getName()), values(set->getSubsets())
			{
				oneSet = set->oneSet;
			}

			/** Returns a pointer to a copy of the Set instance **/
			virtual
			Set*
			clone() const
			{
				return new Set<Value, Key>(this);
			}

			/** Returns an instance of pelib::SetType containing all values in this Set. **/
			virtual
			const SetType&
			getValues() const
			{
				if(values.size() == 1)
				{
					return values.begin()->second;
				}
				else
				{
					throw PelibException("Using exists method on Set of several sets where the method is valid on of there is only one subset.");
				}
			}

			/** Returns true if an element equal to elem is found in this set. **/
			virtual
			bool
			exists(Value elem) const
			{
				if(values.size() == 1)
				{
					return getValues().find(elem) != getValues().end();
				}
				else
				{
					throw PelibException("Using exists method on Set of several sets where the method is valid on of there is only one subset.");
				}
			}

			/** Returns the number of elements in this set **/
			virtual
			size_t
			getSize() const
			{
				return values.size();
			}

			const SetOfSetsType&
			getSubsets() const
			{
				return values;
			}
			
			void
			merge(const AlgebraData *ptr)
			{
				// Only allow merging if both this and ptr are of same name and type
				if(ptr->getName().compare(this->getName()) == 0 && std::string(typeid(*ptr).name()).compare(typeid(Set<Value, Key>).name()) == 0)
				{
					Set<Value, Key> *set = (Set<Value, Key>*)ptr;
					oneSet = 0;
					for(typename SetOfSetsType::iterator i = set->values.begin(); i != set->values.end(); i++)
					{
						if(this->values.find(i->first) != this->values.end())
						{
							this->values.erase(this->values.find(i->first));
						}
						// Add the new row in this matrix
						this->values.insert(std::pair<Key, SetType>(i->first, i->second));
					}
				}
				else
				{
					throw PelibException(std::string("Cannot merge data \"") + ptr->getName() + "\" with " + typeid(Value).name() + " set of name \"" + this->getName() + "\".");
				}
			}

			/** Return true if this set is composed of exactly 1 subset **/
			bool
			isOneSet() const
			{
				return oneSet;
			}

		protected:
			/** Value container in this set **/
			SetOfSetsType values;
			bool oneSet;
		private:		
	};
}

#endif
