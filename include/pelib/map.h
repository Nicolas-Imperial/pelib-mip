/*
 * map.h
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

#if PELIB_CONCAT_4(DONE_map_, MAP_KEY_T, _, MAP_VALUE_T) == 0
#define map(key, value) PELIB_CONCAT_4(map_, key, _, value)
#define map_t(key, value) PELIB_CONCAT_2(map(key, value), _t)
#define map_pelib_iterator(key, value) PELIB_CONCAT_2(map_, pelib_iterator(pelib_pair_t(key, value)))
#define map_pelib_iterator_t(key, value) PELIB_CONCAT_2(map_pelib_iterator(key, value), _t)

/*
#define PELIB_PAIR_KEY_T MAP_KEY_T
#define PELIB_PAIR_VALUE_T MAP_VALUE_T
#include <pelib/pair.h>
*/

/*
#define PELIB_ITERATOR_T pelib_pair_t(MAP_KEY_T, MAP_VALUE_T)
#include <pelib/iterator.h>
*/

#define pelib_map_begin(key, value) PELIB_CONCAT_3(pelib_, map(key, value), _begin)
#define pelib_map_end(key, value) PELIB_CONCAT_3(pelib_, map(key, value), _end)
#define pelib_map_next(key, value) PELIB_CONCAT_3(pelib_, map(key, value), _next)
#define pelib_map_read(key, value) PELIB_CONCAT_3(pelib_, map(key, value), _read)
#define pelib_map_insert(key, value) PELIB_CONCAT_3(pelib_, map(key, value), _insert)
#define pelib_map_find(key, value) PELIB_CONCAT_3(pelib_, map(key, value), _find)
#define pelib_map_compare(key, value) PELIB_CONCAT_3(pelib_, map(key, value), _compare)
#define pelib_map_size(key, value) PELIB_CONCAT_3(pelib_, map(key, value), _size)

/** Generic static map **/
struct map(MAP_KEY_T, MAP_VALUE_T)
{
	pelib_iterator_t(pelib_pair_t(MAP_KEY_T, MAP_VALUE_T)) *first, *last, *middle; /** Address where elements are stored **/
};
typedef struct map(MAP_KEY_T, MAP_VALUE_T) map_t(MAP_KEY_T, MAP_VALUE_T);
typedef struct pelib_iterator(pelib_pair_t(MAP_KEY_T, MAP_VALUE_T)) map_pelib_iterator_t(MAP_KEY_T, MAP_VALUE_T);

#define STRUCT_T map_t(MAP_KEY_T, MAP_VALUE_T)
#include <pelib/structure.h>

/* Returns the first element in map */
pelib_iterator_t(pelib_pair_t(MAP_KEY_T, MAP_VALUE_T))*
pelib_map_begin(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T)*);

/* Returns the first element in map */
pelib_iterator_t(pelib_pair_t(MAP_KEY_T, MAP_VALUE_T))*
pelib_map_end(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T)*);

/* Places an element in map */
int
pelib_map_insert(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T)*, pelib_pair_t(MAP_KEY_T, MAP_VALUE_T));

/* Find an element in map */
pelib_iterator_t(pelib_pair_t(MAP_KEY_T, MAP_VALUE_T))*
pelib_map_find(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T)*, MAP_KEY_T);

/** Compares all elements of two maps one by one
	@return If a1 and a2 have the same number of elements, for the first pelib_pair of elements from both maps a1 and a2 that are different, return -1 if the element of a1 is lower than the element of a2, 1 otherwise. Returns 0 if all elements are identical. If a1 has less elements than a2, then returns -1 other wise returns 1.
**/
int
pelib_map_compare(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T)* a1, map_t(MAP_KEY_T, MAP_VALUE_T)* a2);

pelib_iterator_t(pelib_pair_t(MAP_KEY_T, MAP_VALUE_T))*
pelib_map_next(MAP_KEY_T, MAP_VALUE_T)(pelib_iterator_t(pelib_pair_t(MAP_KEY_T, MAP_VALUE_T))*);

pelib_pair_t(MAP_KEY_T, MAP_VALUE_T)
pelib_map_read(MAP_KEY_T, MAP_VALUE_T)(pelib_iterator_t(pelib_pair_t(MAP_KEY_T, MAP_VALUE_T))*);

size_t
pelib_map_size(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T)*);

#endif

// Make sure the symbol does not propagate any further
#undef MAP_KEY_T 
#undef MAP_VALUE_T

#ifdef __cplusplus
}
#endif
