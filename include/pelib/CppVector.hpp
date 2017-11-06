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


#include <pelib/CppData.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_CPPVECTOR
#define PELIB_CPPVECTOR

namespace pelib
{
	/** Output class for pelib::Vector that generate C++ code of a Algebraic Vector **/
	template <class Key, class Value>
	class CppVector: public CppData
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			/** Constructor **/
			CppVector() : CppData()
			{
				// Do nothing
			}

			/** Returns a pointer to a copy of this class instance **/
			virtual
			CppVector*
			clone() const
			{
				return new CppVector();
			}

			/** Generates C++ code that produce the same instance of pelib::Vector as given as parameter and writes it to output stream
				@param o Output stream that receives the generated C++ code
				@param data Instance of pelib::Vector that is translated to C++ code
			**/
			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Vector<Key, Value> *vector = dynamic_cast<const Vector<Key, Value>*>(data);
				if(vector == NULL) throw CastException("parameter \"data\" was not of type \"Vector<Key, Value>\".");
				
				// Dummy key and value and string for their type
				Key key;
				Value value;
				std::string key_str = type_name(typeid(key));
				std::string value_str = type_name(typeid(value));

				// Declare a map to contain values
				o << "std::map<" << key_str << ", " << value_str << "> map_" << vector->getName() << ";" << std::endl;

				// Fill the map with values
				std::map<Key, Value> map = vector->getValues();
				for(typename std::map<Key, Value>::iterator iter = map.begin(); iter != map.end(); iter++)
				{
					key = iter->first;
					value = iter->second;
					//key_str = type_name(typeid(key)).c_str();
					//value_str = type_name(typeid(value)).c_str();
					
					o << "map_" << vector->getName() << ".insert(std::pair<" << key_str.c_str() << ", " << value_str.c_str() << ">(" << key << ", " << value << "));" << std::endl;
				}
				
				o << "pelib::Vector<" << key_str << ", " << value_str << "> " << vector->getName() << "(\"" << vector->getName() << "\", map_" << vector->getName() << ");" << std::endl;	
			}

		protected:
		private:	
	};
}

#endif
