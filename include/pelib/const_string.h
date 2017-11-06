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

#ifndef CONST_STRING_H_
#define CONST_STRING_H_

typedef const char* const_string;

#define STRUCT_T const_string
#include <pelib/structure.h>
#define DONE_const_string 1 

#define PELIB_ITERATOR_T const_string
#include <pelib/iterator.h>
#define DONE_pelib_iterator_const_string 1

#define SET_T const_string
#include <pelib/set.h>
#define DONE_set_const_string 1

#define PELIB_PAIR_KEY_T const_string
#define PELIB_PAIR_VALUE_T const_string
#include <pelib/pair.h>
#define DONE_pelib_pair_const_string_const_string 1 

#define PELIB_ITERATOR_T pelib_pair_t(const_string, const_string)
#include <pelib/iterator.h>
#define DONE_pelib_iterator_pelib_pair_const_string_const_string 1 

#define MAP_KEY_T const_string
#define MAP_VALUE_T const_string
#include <pelib/map.h>
#define DONE_map_const_string_const_string 1

#define SET_T pelib_pair_t(const_string, const_string)
#include <pelib/set.h>
#define DONE_set_const_string_const_string 1

#endif /* STRING_H_ */
