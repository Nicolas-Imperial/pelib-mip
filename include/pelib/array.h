/*
 * array.h
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
#if PELIB_CONCAT_2(DONE_array_, ARRAY_T) == 0

#define array(elem) PELIB_CONCAT_2(array_, elem)
#define array_t(elem) PELIB_CONCAT_2(array(elem), _t)

#define pelib_array_loadfilename(elem) PELIB_CONCAT_3(pelib_, array(elem), _loadfilename)
#define pelib_array_loadfilenamebinary(elem) PELIB_CONCAT_3(pelib_, array(elem), _loadfilenamebinary)
#define pelib_array_preloadfilenamebinary(elem) PELIB_CONCAT_3(pelib_, array(elem), _preloadfilenamebinary)
#define pelib_array_loadfilenamewindowbinary(elem) PELIB_CONCAT_3(pelib_, array(elem), _loadfilenamewindowbinary)
#define pelib_array_storefilename(elem) PELIB_CONCAT_3(pelib_, array(elem), _storefilename)
#define pelib_array_storefilenamebinary(elem) PELIB_CONCAT_3(pelib_, array(elem), _storefilenamebinary)
#define pelib_array_checkascending(elem) PELIB_CONCAT_3(pelib_, array(elem), _checkascending)
#define pelib_array_read(elem) PELIB_CONCAT_3(pelib_, array(elem), _read)
#define pelib_array_write(elem) PELIB_CONCAT_3(pelib_, array(elem), _write)
#define pelib_array_append(elem) PELIB_CONCAT_3(pelib_, array(elem), _append)
#define pelib_array_length(elem) PELIB_CONCAT_3(pelib_, array(elem), _length)
#define pelib_array_capacity(elem) PELIB_CONCAT_3(pelib_, array(elem), _capacity)
#define pelib_array_compare(elem) PELIB_CONCAT_3(pelib_, array(elem), _compare)

/** Generic static array **/
struct array(ARRAY_T)
  {
    size_t capacity; /** Maximum elements in the array **/
    size_t length; /** Number of elements currently stored in the array **/
    ARRAY_T* data; /** Address where elements are stored **/
  };
typedef struct array(ARRAY_T) array_t(ARRAY_T);

#define STRUCT_T array_t(ARRAY_T)
#include <pelib/structure.h>

/** Load an instance of array from a text file. The capacity of the array equals the number of elements loaded **/
array_t(ARRAY_T)* pelib_array_loadfilename(ARRAY_T)(char*);

/** Load an instance of array from a binary file. The capacity of the array equals the number of elements loaded **/
array_t(ARRAY_T)* pelib_array_loadfilenamebinary(ARRAY_T)(char*);

/** Load the number of elements in a array from a binary file, but not the data itself. The capacity of the array equals the number of elements loaded **/
array_t(ARRAY_T)* pelib_array_preloadfilenamebinary(ARRAY_T)(char*);

/** Load only a portion of the data stored in a binary file. The capacity of the array equals the number of elements loaded
	@param Index of the first element to be loaded. This element is stored at offset 0 in this array instance
	@param to Index of the last element to be loaded
**/
array_t(ARRAY_T)* pelib_array_loadfilenamewindowbinary(ARRAY_T)(char*, size_t from, size_t to);
/** Stores an array in a text file **/
int
pelib_array_storefilename(ARRAY_T)(array_t(ARRAY_T)*, char*);
/** Stores an array in a binary file **/
int
pelib_array_storefilenamebinary(ARRAY_T)(array_t(ARRAY_T)*, char*);
/** Returns 0 if elements in an array are not sorted in non-decreasing order **/
int
pelib_array_checkascending(ARRAY_T)(array_t(ARRAY_T)*);
/** Returns the i-th element in the array, starting at 0 **/
ARRAY_T
pelib_array_read(ARRAY_T)(array_t(ARRAY_T)*, size_t i);
/** Places element elem at index i, starting at 0 **/
int
pelib_array_write(ARRAY_T)(array_t(ARRAY_T)*, size_t i, ARRAY_T elem);
/** Write elem in array instance at index defined by the array's length
	@return 0 if this array is already full
**/
int
pelib_array_append(ARRAY_T)(array_t(ARRAY_T)*, ARRAY_T elem);
/** Returns the length of the array, that is the number of elements stored in it **/
size_t
pelib_array_length(ARRAY_T)(array_t(ARRAY_T)*);
/** Returns the maximum number of elements this array can hold **/
size_t
pelib_array_capacity(ARRAY_T)(array_t(ARRAY_T)*);
/** Compares all elements of two arrays one by one
	@return If a1 and a2 have the same number of elements, for the first pelib_pair of elements from both arrays a1 and a2 that are different, return -1 if the element of a1 is lower than the element of a2, 1 otherwise. Returns 0 if all elements are identical. If a1 has less elements than a2, then returns -1 other wise returns 1.
**/
int
pelib_array_compare(ARRAY_T)(array_t(ARRAY_T)* a1, array_t(ARRAY_T)* a2);

#endif /* ARRAY_H_ */

// Make sure the symbol does not propagate any further
#undef ARRAY_T
