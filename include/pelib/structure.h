/*
 * structure.h
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

#include <stdio.h>
#include <stddef.h>

#include <pelib/template.h>

// Prevents several code generation for the same data structure
#if PELIB_CONCAT_2(DONE_, STRUCT_T) == 0

#ifndef STRUCT_T
#error Using generic structure without a type
#endif

#define pelib_alloc_buffer(elem) PELIB_CONCAT_3(pelib_,elem,_alloc_buffer)
#define pelib_set_buffer(elem) PELIB_CONCAT_3(pelib_,elem,_set_buffer)
#define pelib_alloc_struct(elem) PELIB_CONCAT_3(pelib_,elem,_alloc_struct)
#define pelib_alloc(elem) PELIB_CONCAT_3(pelib_,elem,_alloc)
#define pelib_alloc_collection(elem) PELIB_CONCAT_3(pelib_,elem,_alloc_collection)
#define pelib_alloc_from(elem) PELIB_CONCAT_3(pelib_,elem,_alloc_from)
#define pelib_init(elem) PELIB_CONCAT_3(pelib_,elem,_init)
#define pelib_copy(elem) PELIB_CONCAT_3(pelib_,elem,_copy)
#define pelib_free_buffer(elem) PELIB_CONCAT_3(pelib_,elem,_free_buffer)
#define pelib_free_struct(elem) PELIB_CONCAT_3(pelib_,elem,_free_struct)
#define pelib_free(elem) PELIB_CONCAT_3(pelib_,elem,_free)
#define pelib_destroy(elem) PELIB_CONCAT_3(pelib_,elem,_destroy)
#define pelib_compare(elem) PELIB_CONCAT_3(pelib_,elem,_compare)
#define pelib_printf(elem) PELIB_CONCAT_3(pelib_,elem,_printf)
#define pelib_printf_detail(elem) PELIB_CONCAT_3(pelib_,elem,_printf_detail)
#define pelib_fwrite(elem) PELIB_CONCAT_3(pelib_,elem,_fwrite)
#define pelib_fread(elem) PELIB_CONCAT_3(pelib_,elem,_fread)
#define pelib_string(elem) PELIB_CONCAT_3(pelib_,elem,_string)
#define pelib_string_detail(elem) PELIB_CONCAT_3(pelib_,elem,_string_detail)

/** Allocates the base structure of a pelib object
	@return A pointer to the newly allocated base structure
**/
STRUCT_T *
pelib_alloc_struct(STRUCT_T)();

/** Allocates the buffer of a pelib collection
	@param obj Pointer to pelib colelction that will receive the buffer
	@param n Number of element this buffer should accept
**/
int
pelib_alloc_buffer(STRUCT_T)(STRUCT_T* obj, size_t n);

/** Replaces the buffer of a pelib collection with a new memory address. The reference to the old buffer is lost and the old buffer is not freed
	@param obj Pelib collection whose buffer is replaced
	@param buffer New buffer for the pelib collection
	@param n Number of element the new buffer can hold
	@return 0 if the operation failed and non-zero otherwise
**/
int
pelib_set_buffer(STRUCT_T)(STRUCT_T* obj, void* buffer, size_t n);
/** Allocate a new pelib simple data structure
	@return A pointer to a newly allocated data structure
**/
STRUCT_T *
pelib_alloc(STRUCT_T)();

/** Allocates a new pelib collection without initializing it
	@param n Number of element the collection can hold after instanciation
	@return Pointer to newly instanciated pelib collection
**/
STRUCT_T *
pelib_alloc_collection(STRUCT_T)(size_t n);

/** Allocate a new pelib collection and uses an existing memory location as buffer
	@param buffer Address of the memory to use as buffer
	@param n Number of elements the new collection can hold
	@return Newly allocated pelib collection
**/
STRUCT_T *
pelib_alloc_from(STRUCT_T)(void* buffer, size_t n);

/** Initializes a newly allocated pelib object
	@param obj Pelib object instance to be initialized
	@return 0 if the operation failed and non-zero otherwise
**/
int
pelib_init(STRUCT_T)(STRUCT_T *obj);

/** Copies a pelib object instance to another
	@param src Pelib object source
	@param dst Pelib object that receives the copy
**/
int
pelib_copy(STRUCT_T)(STRUCT_T src, STRUCT_T* dst);

/** Frees the memory associated to the base structure of a pelib object
	@param obj Pelib object to be freed
	@return 0 if the operation failed and non-zero otherwise
**/
int
pelib_free_struct(STRUCT_T)(STRUCT_T *obj);

/** Frees the memory associated to the buffer of a pelib collection
	@param Pelib collection whose buffer is to be freed
	@return 0 if the operation failed and non-zero otherwise
**/
int
pelib_free_buffer(STRUCT_T)(STRUCT_T *);

/** Frees a comple pelib simple or collection object, including its buffer
	@return 0 if the operation failed and non-zero otherwise
**/
int
pelib_free(STRUCT_T)(STRUCT_T *);

/** Destroys a pelib object before its memory is freed
	@return 0 if the operation failed and non-zero otherwise
**/
int
pelib_destroy(STRUCT_T)(STRUCT_T);

/** Compares two pelib object
	@return -1 if a < b, 1 if a > b and 0 if a == b
**/
int
pelib_compare(STRUCT_T)(STRUCT_T a, STRUCT_T b);

/** Writes a simple human-readable representation of a pelib object instance to some stream
	@param str Output stream to write the object to
	@param obj Pelib object to be written
	@return output stream after the pelib object was written
**/
FILE*
pelib_printf(STRUCT_T)(FILE* str, STRUCT_T obj);

/** Writes a detailed human-redable representation of a pelib object instance to some stream
	@param str Output stream to write the object to
	@param obj Pelib object to be written
	@param lvl Level of details to be written to output stream. For a collection, will write all elements in the collection with detail level = lvl - 1. Useful to display collections of complex objects and visualize all objects in details in a single line of code
	@return Output stream after the object instance was written
**/
FILE*
pelib_printf_detail(STRUCT_T)(FILE* str, STRUCT_T obj, int lvl);

/** Writes a pelib object instance to output stream. pelib_fread must be able to read back the written object instance and store it to memory
	@param obj Pelib object instance to be written
	@param str output stream that receives the written pelib object instance
	@return Number of bytes written
**/
size_t
pelib_fwrite(STRUCT_T)(STRUCT_T obj, FILE* str);

/** Reads a pelib object from input stream produced by pelib_fwrite and write it in an existing pelib object instance
	@param obj Pointer to object that receives the data read from input stream
	@param str Input stream whose content is produced by pelib_fwrite to read data from
	@return Number of bytes read
**/
size_t
pelib_fread(STRUCT_T)(STRUCT_T* obj, FILE* str);

/** Returns a simple human-readable string representation of an instance of pelib object **/
char*
pelib_string(STRUCT_T)(STRUCT_T);

/** Returns a detailed view human-readable string representation of an instance of pelib object. For a collection, the same function is run for all pelib object instances held is the collection with a lower level value.
	@param obj Pelib object to be written to
	@param lvl Level of details to be generated
**/
char*
pelib_string_detail(STRUCT_T)(STRUCT_T, int);

#endif
#undef STRUCT_T
