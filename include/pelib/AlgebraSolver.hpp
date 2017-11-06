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

#include <pelib/Solver.hpp>
#include <pelib/Algebra.hpp>

#ifndef PELIB_ALGEBRASOLVER
#define PELIB_ALGEBRASOLVER
namespace pelib
{
	class AlgebraSolver : public Solver
	{
		public:
			virtual pelib::Algebra solve(const std::map<const std::string, const Algebra> &data) const = 0;
			virtual pelib::Algebra solve(const std::map<const std::string, const Algebra> &data, std::map<const std::string, double> &statistics) const = 0;
			virtual const pelib::Algebra* solve() const = 0;
			virtual const pelib::Algebra* solve(std::map<const std::string, double> &statistics) const = 0;
		protected:
		private:
	};
}

#endif
