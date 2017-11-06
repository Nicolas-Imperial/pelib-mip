/*
 * chareger.h
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

#ifndef CHAR_H_
#define CHAR_H_

#define STRUCT_T char
#include <pelib/structure.h>
#define DONE_char 1 

#define ARRAY_T char
#include <pelib/array.h>
#define DONE_array_char 1

#define PELIB_ITERATOR_T char
#include <pelib/iterator.h>
#define DONE_pelib_iterator_char 1

#define SET_T char
#include <pelib/set.h>
#define DONE_set_char 1

#define PELIB_PAIR_KEY_T char
#define PELIB_PAIR_VALUE_T char
#include <pelib/pair.h>
#define DONE_pelib_pair_char_char 1 

#define PELIB_ITERATOR_T pelib_pair_t(char, char)
#include <pelib/iterator.h>
#define DONE_pelib_iterator_pelib_pair_char_char 1 

#define MAP_KEY_T char
#define MAP_VALUE_T char
#include <pelib/map.h>
#define DONE_map_char_char 1

#define SET_T pelib_pair_t(char, char)
#include <pelib/set.h>
#define DONE_set_char_char 1

#define CFIFO_T char
#include <pelib/fifo.h>
#define DONE_cfifo_char 1 

#define CFIFO_ARRAY_T char
#include <pelib/fifo_array.h>
#define DONE_cfifo_array_char 1

#endif /* CHAR_H_ */

#ifdef __cplusplus
}
#endif
