/*
 * integer.h
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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef INTEGER_H_
#define INTEGER_H_

#define STRUCT_T int
#include <pelib/structure.h>
#define DONE_int 1 

#define ARRAY_T int
#include <pelib/array.h>
#define DONE_array_int 1

#define PELIB_ITERATOR_T int
#include <pelib/iterator.h>
#define DONE_pelib_iterator_int 1

#define SET_T int
#include <pelib/set.h>
#define DONE_set_int 1

#define PELIB_PAIR_KEY_T int
#define PELIB_PAIR_VALUE_T int
#include <pelib/pair.h>
#define DONE_pelib_pair_int_int 1 

#define PELIB_ITERATOR_T pelib_pair_t(int, int)
#include <pelib/iterator.h>
#define DONE_pelib_iterator_pelib_pair_int_int 1 

#define MAP_KEY_T int
#define MAP_VALUE_T int
#include <pelib/map.h>
#define DONE_map_int_int 1

#define SET_T pelib_pair_t(int, int)
#include <pelib/set.h>
#define DONE_set_int_int 1

#define CFIFO_T int
#include <pelib/fifo.h>
#define DONE_cfifo_int 1 

#define CFIFO_ARRAY_T int
#include <pelib/fifo_array.h>
#define DONE_cfifo_array_int 1

#endif /* INTEGER_H_ */

#ifdef __cplusplus
}
#endif
