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

#include <map>
#include <pelib/Record.hpp>

#ifndef PELIB_SOLVER
#define PELIB_SOLVER
namespace pelib
{
	/** Ancestor class for container classes of derivatives of pelib::Data **/
	class Solver
	{
		public:
			/** Destructor **/
			virtual ~Solver() = 0;
			virtual const Record* solve() const = 0;
			virtual const Record* solve(std::map<const std::string, double> &statistics) const = 0;
		protected:
		/** For some interesting reason, a pointer to Solver pointing to a derived class is resolved as a Solver by typeinfo if there is no virtual function in the base class.
		This is an empty virtual function to make sure type_info can resolve the right class name
		**/
		virtual void do_nothing() {}
		private:
	};
}

#endif
