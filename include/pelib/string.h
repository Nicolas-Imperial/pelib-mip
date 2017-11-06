/*
 * stringeger.h
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

#ifndef STRING_H_
#define STRING_H_

typedef char* string;

#define STRUCT_T string
#include <pelib/structure.h>
#define DONE_string 1 

#define PELIB_ITERATOR_T string
#include <pelib/iterator.h>
#define DONE_pelib_iterator_string 1

#define SET_T string
#include <pelib/set.h>
#define DONE_set_string 1

#define PELIB_PAIR_KEY_T string
#define PELIB_PAIR_VALUE_T string
#include <pelib/pair.h>
#define DONE_pelib_pair_string_string 1 

#define PELIB_ITERATOR_T pelib_pair_t(string, string)
#include <pelib/iterator.h>
#define DONE_pelib_iterator_pelib_pair_string_string 1 

#define MAP_KEY_T string
#define MAP_VALUE_T string
#include <pelib/map.h>
#define DONE_map_string_string 1

#define SET_T pelib_pair_t(string, string)
#include <pelib/set.h>
#define DONE_set_string_string 1

#endif /* STRING_H_ */
