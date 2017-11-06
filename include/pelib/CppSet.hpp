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

#include <pelib/CppData.hpp>
#include <pelib/Set.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_CPPSET
#define PELIB_CPPSET

namespace pelib
{
	/** C++ code generator for a Set **/
	template <class Value>
	class CppSet: public CppData
	{
		typedef std::set<Value, std::less<Value>, std::allocator<Value> > SetType;
		
		public:
			CppSet() : CppData()
			{
				// Do nothing
			}
	
			/** Returns a copy of the class instance **/		
			virtual
			CppSet*
			clone() const
			{
				return new CppSet();
			}

			/** Generate C++ code that declares a C++ set instance and fills it with the data given in the pelib::Set object instance passed **/
			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Set<Value> *set = dynamic_cast<const Set<Value>*>(data);
				if(set == NULL) throw CastException("parameter \"data\" was not of type \"Set<Value>\".");
				
				// Dummy key and value and string for their type
				Value value;
				std::string value_str = type_name(typeid(value));

				// Declare a map to contain values
				o << "std::set<" << value_str.c_str() << "> set_" << set->getName() << ";" << std::endl;

				// Fill the map with values
				std::set<Value> stdset = set->getValues();
				for(typename std::set<Value>::iterator iter = stdset.begin(); iter != stdset.end(); iter++)
				{
					value = *iter;
					//value_str = type_name(typeid(value)).c_str();
					
					o << "set_" << set->getName() << ".insert(" << value << ");" << std::endl;
				}
				
				o << "pelib::Set<" << value_str.c_str() << "> " << set->getName() << "(\"" << set->getName() << "\", set_" << set->getName() << ");" << std::endl;
			}

		protected:
		private:	
	};
}

#endif
