/*
 * pelib_iterator.h
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

#ifdef __cplusplus
extern "C"
{
#endif

#include <pelib/template.h>
#if PELIB_CONCAT_2(DONE_pelib_iterator_, PELIB_ITERATOR_T) == 0

#define pelib_iterator(elem) PELIB_CONCAT_2(pelib_iterator_, elem)
#define pelib_iterator_t(elem) PELIB_CONCAT_2(pelib_iterator(elem), _t)

#define pelib_pelib_iterator_next(value) PELIB_CONCAT_3(pelib_, pelib_iterator(value), _next)
#define pelib_pelib_iterator_read(value) PELIB_CONCAT_3(pelib_, pelib_iterator(value), _read)
#define pelib_pelib_iterator_distance(value) PELIB_CONCAT_3(pelib_, pelib_iterator(value), _distance)

/** Generic static pelib_iterator **/
struct pelib_iterator(PELIB_ITERATOR_T)
{
	struct pelib_iterator(PELIB_ITERATOR_T) *previous, *next;
	PELIB_ITERATOR_T value;
};
typedef struct pelib_iterator(PELIB_ITERATOR_T) pelib_iterator_t(PELIB_ITERATOR_T);

pelib_iterator_t(PELIB_ITERATOR_T)*
pelib_pelib_iterator_next(PELIB_ITERATOR_T)(pelib_iterator_t(PELIB_ITERATOR_T)*);

PELIB_ITERATOR_T
pelib_pelib_iterator_read(PELIB_ITERATOR_T)(pelib_iterator_t(PELIB_ITERATOR_T)*);

size_t
pelib_pelib_iterator_distance(PELIB_ITERATOR_T)(pelib_iterator_t(PELIB_ITERATOR_T) *start, pelib_iterator_t(PELIB_ITERATOR_T) *stop);

#define STRUCT_T pelib_iterator_t(PELIB_ITERATOR_T)
#include <pelib/structure.h>

#endif /* PELIB_ITERATOR_H_ */

// Make sure the symbol does not propagate any further
#undef PELIB_ITERATOR_T

#ifdef __cplusplus
}
#endif
