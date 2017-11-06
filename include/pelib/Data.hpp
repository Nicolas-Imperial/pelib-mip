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


#include <string>

#ifndef PELIB_DATA
#define PELIB_DATA

#if DEBUG
#define trace(var) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"." << endl
#else
#define trace(var)
#endif

namespace pelib
{
	/** Base class of any data structure that can be parsed, output or held in a pelib collection **/
	class Data
	{
		public:
			/** Destructor **/
			virtual ~Data();

			/** returns a pointer to a copy of this Data instance **/
			virtual
			Data*
			clone() const = 0;
			
		protected:
		private:		
	};
}

#endif
