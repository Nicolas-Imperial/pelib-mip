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


#include <pelib/CppDataOutput.hpp>

#ifndef PELIB_CPPDATA
#define PELIB_CPPDATA

namespace pelib
{
	/** Base class of classes that generate C++ code from instances derives from pelib::AlgebraData **/
	class CppData: public CppDataOutput
	{
		public:
			/** Returns a pointer to a copy of this class **/
			virtual
			CppData*
			clone() const = 0;

		protected:
			/**  **/
			const std::string
			/** Returns the C++ type string to use in a C/C++ source code, associated to a C++ type
				@param type Type of some C++ object obtained thanks to typeid()
			**/
			type_name(const std::type_info &type) const;
		private:		
	};
}

#endif
