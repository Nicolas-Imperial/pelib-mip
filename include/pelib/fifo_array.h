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

#include <pelib/template.h>

#if PELIB_CONCAT_2(DONE_cfifo_array_, CFIFO_ARRAY_T) == 0

#define pelib_cfifo_from_array(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _from_array)
#define pelib_array_from_cfifo(elem) PELIB_CONCAT_3(pelib_, array(elem), _from_cfifo)

/** Allocates and initialize an instance of pelib fifo from the data of a pelib array. The new fifo contains all data and free space in the pelib array **/
cfifo_t(CFIFO_ARRAY_T)*
pelib_cfifo_from_array(CFIFO_ARRAY_T)(array_t(CFIFO_ARRAY_T)*);

/** Allocates and initialize an instance of pelib array from the data of a pelib fifo. The new array's capacity is the number of elements already in the fifo. The new array has no free element left (the append operation cannot succeed) **/
array_t(CFIFO_ARRAY_T)*
pelib_array_from_cfifo(CFIFO_ARRAY_T)(cfifo_t(CFIFO_ARRAY_T)*);

#endif
#undef CFIFO_ARRAY_T
