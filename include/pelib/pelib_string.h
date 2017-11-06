/*
 * pelib_string.h
 *
 *  Created on: 20 Feb 2012
 *  Copyright 2012 Nicolas Melot
 *
 * This file is part of pelib.
 * 
 *     pelib is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     pelib is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with pelib. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef PELIB_STRING_H_
#define PELIB_STRING_H_

typedef char* pelib_string;

#define STRUCT_T pelib_string
#include <pelib/structure.h>
#define DONE_pelib_string

/** Returns the number of characters contained in the string, exclusing null terminator character \0 **/
size_t
pelib_string_length(pelib_string str);

#endif /* PELIB_STRING_H_ */
