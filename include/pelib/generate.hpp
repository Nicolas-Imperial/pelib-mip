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

#include <pelib/Record.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CROWN_H
#define CROWN_H
	const pelib::Record* pelib_generate(size_t argc, char **argv);
	std::string pelib_description(size_t argc, char **argv);
	void pelib_delete(pelib::Record *sched);
#endif

#ifdef __cplusplus
}
#endif
