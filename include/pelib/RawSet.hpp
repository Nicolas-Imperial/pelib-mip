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

#include <pelib/RawData.hpp>
#include <pelib/Set.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_RAWSET
#define PELIB_RAWSET

namespace pelib
{
	/*** AMPL Output format parser and output class for pelib::Set class instances ***/
	template <class Value, class Key = size_t>
	class RawSet: public RawData
	{
		/*** Internal format of data collection handled ***/
		typedef std::set<Value, std::less<Value>, std::allocator<Value> > SetType;
		
		public:
			/** Returns a pointer to a copy of this class instance **/
			virtual
			RawSet*
			clone() const
			{
				return new RawSet();
			}

			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Set<Value, Key> *set = dynamic_cast<const Set<Value>*>(data);
				if(set == NULL) throw CastException("parameter \"data\" was not of type \"Set<Value>\".");				
				
				for(typename Set<Value, Key>::SetOfSetsType::const_iterator i = set->getSubsets().begin(); i != set->getSubsets().end(); i++)
				{
					SetType values = i->second;
					for(typename std::set<Value>::const_iterator iter = values.begin(); iter != values.end(); iter++)
					{
						o << *iter << " ";
					}

					o << std::endl;
				}
			}

		protected:
		private:	
	};
}

#endif
