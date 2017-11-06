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
#include <pelib/Record.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PELIB_LIBPARSER
#define PELIB_LIBPARSER

// /!\ the content of argv is freed after this function is run
/** Parses the content of an input stream with string options and produces a class instance derived from pelib::Record. Used for dynamic library parsers **/
pelib::Record* pelib_parse(std::istream& cin, size_t argc, char** argv, const std::map<std::string, pelib::Record*> &inputs);

/** Deletes a Record produced by pelib_parse **/
void pelib_delete(pelib::Record*);

#endif

#ifdef __cplusplus
}
#endif
