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
#include <map>
#include <pelib/Record.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PELIB_LIBOUTPUT
#define PELIB_LIBOUTPUT

// /!\ the content of argv is freed after this function is run
/** Writes objects contained in records to output stream. Options can be passed using the string array argv and the number of arguments in argc. Note that call elements of argv and argv itself are freed after this function terminates.
**/
void pelib_dump(std::ostream& cout, const std::map<std::string, pelib::Record*> &records, size_t argc, char** argv);

#endif

#ifdef __cplusplus
}
#endif
