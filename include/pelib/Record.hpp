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

#include <pelib/Data.hpp>

#ifndef PELIB_RECORD
#define PELIB_RECORD
namespace pelib
{
	/** Ancestor class for container classes of derivatives of pelib::Data **/
	class Record
	{
		public:
			/** Destructor **/
			virtual ~Record() = 0;
			virtual Record* clone() const = 0;
		protected:
		/** For some interesting reason, a pointer to Record pointing to a derived class is resolved as a Record by typeinfo if there is no virtual function in the base class.
		This is an empty virtual function to make sure type_info can resolve the right class name
		**/
		virtual void do_nothing() {}
		private:
	};
}

#endif
