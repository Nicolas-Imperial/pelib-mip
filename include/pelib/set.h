/*
 * set.h
 *
 *  Created on: 5 Sep 2011
 *  Copyright 2011 Nicolas Melot
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
#if PELIB_CONCAT_2(DONE_set_, SET_T) == 0

//#define PELIB_ITERATOR_T SET_T
//#include <pelib/iterator.h>

#define pelib_set(elem) PELIB_CONCAT_2(pelib_set_, elem)
#define pelib_set_t(elem) PELIB_CONCAT_2(pelib_set(elem), _t)

#define pelib_set_first(elem) PELIB_CONCAT_3(pelib_, pelib_set(elem), _first)
#define pelib_set_last(elem) PELIB_CONCAT_3(pelib_, pelib_set(elem), _last)
#define pelib_set_insert(elem) PELIB_CONCAT_3(pelib_, pelib_set(elem), _insert)
#define pelib_set_find(elem) PELIB_CONCAT_3(pelib_, pelib_set(elem), _find)
#define pelib_set_compare(elem) PELIB_CONCAT_3(pelib_, pelib_set(elem), _compare)

/** Generic static set **/
struct pelib_set(SET_T)
{
	pelib_iterator_t(SET_T) *first, *last, *middle; /** Address where elements are stored **/
};
typedef struct pelib_set(SET_T) pelib_set_t(SET_T);

#define STRUCT_T pelib_set_t(SET_T)
#include <pelib/structure.h>

/* Returns the first element in set */
SET_T*
pelib_set_first(SET_T)(pelib_set_t(SET_T)*);

/* Returns the first element in set */
SET_T*
pelib_set_last(SET_T)(pelib_set_t(SET_T)*);

/* Places an element in set */
int
pelib_set_insert(SET_T)(pelib_set_t(SET_T)*, SET_T);

/* Find an element in set */
SET_T*
pelib_set_find(SET_T)(pelib_set_t(SET_T)*, SET_T);

/** Compares all elements of two sets one by one
	@return If a1 and a2 have the same number of elements, for the first pelib_pair of elements from both sets a1 and a2 that are different, return -1 if the element of a1 is lower than the element of a2, 1 otherwise. Returns 0 if all elements are identical. If a1 has less elements than a2, then returns -1 other wise returns 1.
**/
int
pelib_set_compare(SET_T)(pelib_set_t(SET_T)* a1, pelib_set_t(SET_T)* a2);

#endif /* SET_H_ */

// Make sure the symbol does not propagate any further
#undef SET_T
