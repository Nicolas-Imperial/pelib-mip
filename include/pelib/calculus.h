/*
 Copyright 2015 Nicolas Melot

 This file is part of Crown.

 Crown is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Crown is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Crown. If not, see <http://www.gnu.org/licenses/>.

*/

#include <map>
#include <pelib/Algebra.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CROWN_H
#define CROWN_H
	const pelib::Algebra* pelib_calculus(const std::map<const std::string&, const pelib::Algebra> &data, size_t argc, char**argv, std::map<const std::string, double>&);
	void pelib_delete(pelib::Algebra *data);
#endif

#ifdef __cplusplus
}
#endif
