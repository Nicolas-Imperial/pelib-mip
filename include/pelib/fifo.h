/*
 * fifo.h
 *
 *  Created on: 26 Jan 2012
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

#include <stddef.h>

#include <pelib/template.h>

#ifndef FIFO_H
#define FIFO_H

/** Last operation performed on an instance of circular fifo **/
enum pelib_fifo_operation
{
	PELIB_CFIFO_PUSH,
	PELIB_CFIFO_POP
};
typedef enum pelib_fifo_operation pelib_fifo_operation_t;
#endif

#if PELIB_CONCAT_2(DONE_cfifo_, CFIFO_T) == 0

#define cfifo(elem) PELIB_CONCAT_2(cfifo_, elem)
#define cfifo_t(elem) PELIB_CONCAT_2(cfifo(elem), _t)

#define pelib_cfifo_push(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _push)
#define pelib_cfifo_fill(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _fill)
#define pelib_cfifo_pop(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _pop)
#define pelib_cfifo_discard(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _discard)
#define pelib_cfifo_peek(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _peek)
#define pelib_cfifo_last(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _last)
#define pelib_cfifo_is_full(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _is_full)
#define pelib_cfifo_is_empty(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _is_empty)
#define pelib_cfifo_string_content(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _string_content)
#define pelib_cfifo_printf_content(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _printf_content)
#define pelib_cfifo_pushmem(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _pushmem)
#define pelib_cfifo_popmem(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _popmem)
#define pelib_cfifo_peekmem(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _peekmem)
#define pelib_cfifo_peekaddr(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _peekaddr)
#define pelib_cfifo_writeaddr(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _writeaddr)
#define pelib_cfifo_popfifo(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _popfifo)
#define pelib_cfifo_length(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _length)
#define pelib_cfifo_last(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _last)
#define pelib_cfifo_capacity(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _capacity)

/** generic circular fifo **/
struct cfifo(CFIFO_T)
{
	size_t read; /** index of the next element to read at the next pop operation **/
	size_t write; /** index of the next element able to receive new data at the next push operation **/
	pelib_fifo_operation_t last_op; /** Last operation, push or pop, performed on this circular fifo. Set to PELIB_CFIFO_POP at fifo initialization **/
	size_t capacity; /** Maximum number of elements this fifo can hold at a given time **/
	volatile CFIFO_T *buffer; /** Pointer to the memory portion that hold the data **/
};
typedef struct cfifo(CFIFO_T) cfifo_t(CFIFO_T);

#define STRUCT_T cfifo_t(CFIFO_T)
#include <pelib/structure.h>

#define PELIB_FIFO_ELEM_MAX_CHAR        11

/** Pushes a new element the tail of the fifo **/
int
pelib_cfifo_push(CFIFO_T)(cfifo_t(CFIFO_T)*, CFIFO_T);
size_t
/** Assumes n elements have been written to the fifo and set the write structire member accordingly **/
pelib_cfifo_fill(CFIFO_T)(cfifo_t(CFIFO_T)*, size_t n);

/** Returns the head of the fifo and set the head to the next element **/
CFIFO_T
pelib_cfifo_pop(CFIFO_T)(cfifo_t(CFIFO_T)*);

/** Discards n elements from the head of the fifo **/
size_t
pelib_cfifo_discard(CFIFO_T)(cfifo_t(CFIFO_T)*, size_t n);

/** Reads the head element but doesn't set the hed to the next element
	@param offset Skip the first offset elements before reading
**/
CFIFO_T
pelib_cfifo_peek(CFIFO_T)(cfifo_t(CFIFO_T)*, size_t offset);

/** Returns an address that contains elements to be read. If num is not NULL and if there is at least one element available, writes the number of elements available in address pointer by num. If no element is available, returns NULL and writes nothing in other parameters. Note that *num may take a lower value than pelib_cfifo_length() if pelib_cfifo_length() >= 2. In this case, if remaining is not null, then remaining is set to point to the the rest of the data; otherwise it is set to NULL. **/
CFIFO_T*
pelib_cfifo_peekaddr(CFIFO_T)(cfifo_t(CFIFO_T)*, size_t offset, size_t *num, CFIFO_T **remaining);

/** Returns an address that can hold new elements. If num is not NULL and if there is at least one memory element available, writes the number of additional elements the fifo can store in address pointer by num. If no memory is available, returns NULL and writes nothing in other parameters. Note that *num may take a lower value than pelib_cifo_capacity() - pelib_cfifo_length(). In this case and if remaining is non-null, remaining points to the extra memory buffer. Otherwise, it is set to NULL. **/
CFIFO_T*
pelib_cfifo_writeaddr(CFIFO_T)(cfifo_t(CFIFO_T)*, size_t *num, CFIFO_T **remaining);

/** Returns 0 if at least one push operation can be performed on the fifo **/
int
pelib_cfifo_is_full(CFIFO_T)(cfifo_t(CFIFO_T)*);

/** Returns 0 if there is at least one element stored in the fifo **/
int
pelib_cfifo_is_empty(CFIFO_T)(cfifo_t(CFIFO_T)*);

/** Pushes all n elements stored in some memory address and update
	@param Address of the first element to be pushed
	@param n Number of elements to push
	@return Number of elements that could be pushed. Lower than n if there is not enough space to write all elements
**/
size_t
pelib_cfifo_pushmem(CFIFO_T)(cfifo_t(CFIFO_T)*, CFIFO_T* addr, size_t n);

/** Pops n elements and copy them into memory pointed by addr and update the fifo head element
	@param addr Address where the first element popped is written
	@param n Number of elements to pop
	@return Number of elements actually popped. May be lower than n if there are not enough elemennts stored
**/
size_t
pelib_cfifo_popmem(CFIFO_T)(cfifo_t(CFIFO_T)*, CFIFO_T* addr, size_t n);

/** Copies the n first elements into addr but do not discard any element in the fifo
	@param n Number of elements to peek from fifo
	@param offset Number of elements to skip before reading
	@return Number of elemetns that could actually be read
**/
size_t
pelib_cfifo_peekmem(CFIFO_T)(cfifo_t(CFIFO_T)*, CFIFO_T*, size_t n, size_t offset);

/** Pops n elements from fifo and pushes them into fifo dest
	@param dest Fifo that receives the elements popped
	@param n number of elements to pop
	@return Number of elements actually pushed in fifo dest
**/
size_t
pelib_cfifo_popfifo(CFIFO_T)(cfifo_t(CFIFO_T)*, cfifo_t(CFIFO_T)* dest, size_t n);

/** Returns the number of elements available in the fifo **/
size_t
pelib_cfifo_length(CFIFO_T)(cfifo_t(CFIFO_T)*);

/** Returns the number that was last written in the fifo **/
CFIFO_T
pelib_cfifo_last(CFIFO_T)(cfifo_t(CFIFO_T)*);

/** Returns the maximum number of elements this fifo can contain **/
size_t
pelib_cfifo_capacity(CFIFO_T)(cfifo_t(CFIFO_T)*);
#endif
#undef CFIFO_T
