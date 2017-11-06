/*
 * fifo_test.c
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <pelib/unit.h>
#include <pelib/size_t.h>

#define CAPACITY 10
#define VALUE   10
#define TO_TAIL 7
#define FROM_HEAD 4
#define DISCARD (CAPACITY - 7)
#define OFFSET 1 // Must fit between 0 and FROM_HEAD, TO_TAIL and CAPACITY and FROM_HEAD and TO_TAIL

cfifo_t(size_t) *cfifo;
size_t value;
size_t reverse[CAPACITY], normal[CAPACITY], different[CAPACITY];

void
init()
{
	unsigned int i;

	cfifo = pelib_alloc_collection(cfifo_t(size_t))(CAPACITY);
	pelib_init(cfifo_t(size_t))(cfifo);
	value = VALUE;

	for (i = 0; i < CAPACITY; i++)
	{
		pelib_cfifo_push(size_t)(cfifo, i + VALUE);
		normal[i] = VALUE + TO_TAIL + i;
		reverse[i] = VALUE + CAPACITY + FROM_HEAD + i;
		different[i] = -1;
	}
}

void
cleanup()
{
	pelib_free(cfifo_t(size_t))(cfifo);
}

void
setup()
{
	// do nothing
}

void
teardown()
{
	// do nothing
}

static void
test_pop()
{
	unsigned int i;

	i = pelib_cfifo_pop(size_t)(cfifo);

	assert(i == VALUE);
}

/*
static void
test_pop_too_much()
{
	unsigned int i;

	for (i = 0; i < CAPACITY + 1; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}
}
*/

static void
test_push()
{
	unsigned int i;

	for (i = 0; i < CAPACITY; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	for (i = 0; i < CAPACITY; i++)
	{
		pelib_cfifo_push(size_t)(cfifo, VALUE * 2 + i);
	}

	assert(pelib_cfifo_peek(size_t)(cfifo, 0) == VALUE * 2);
}

static void
test_push_too_much()
{
	unsigned int i, res;

	for (i = 0; i < CAPACITY + 1; i++)
	{
		res = pelib_cfifo_push(size_t)(cfifo, VALUE);
	}

	assert(res == 0);
}

static void
test_is_full_or_empty()
{
	unsigned int i;

	assert(pelib_cfifo_is_full(size_t)(cfifo) == 1);
	assert(pelib_cfifo_is_empty(size_t)(cfifo) == 0);

	for (i = 0; i < (CAPACITY + 1) / 2; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	assert(pelib_cfifo_is_full(size_t)(cfifo) == 0);
	assert(pelib_cfifo_is_empty(size_t)(cfifo) == 0);

	for (; i < (CAPACITY + 1); i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	assert(pelib_cfifo_is_full(size_t)(cfifo) == 0);
	assert(pelib_cfifo_is_empty(size_t)(cfifo) == 1);
}

static void
test_string()
{
	char* str;

	// Full fifo with read_ptr and write_ptr at the edge [>>10:11:12:13:14:15:16:17:18:19]
	str = pelib_string(cfifo_t(size_t))(*cfifo);
	assert_equals_str(str, "[>>10:11:12:13:14:15:16:17:18:19]");
	free(str);

	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);

	// Not full fifo, reverse mode with read_ptr in the middle [>.:.:.:.:.:.:>16:17:18:19]
	str = pelib_string(cfifo_t(size_t))(*cfifo);
	assert_equals_str(str, "[>.:.:.:.:.:.:>16:17:18:19]");
	free(str);

	pelib_cfifo_push(size_t)(cfifo, 2);
	pelib_cfifo_push(size_t)(cfifo, 5);

	// Not full fifo, reverse mode with write_ptr and read_ptr in the middle [2:5:>.:.:.:.:>16:17:18:19]
	str = pelib_string(cfifo_t(size_t))(*cfifo);
	assert_equals_str(str, "[2:5:>.:.:.:.:>16:17:18:19]");
	free(str);

	pelib_cfifo_push(size_t)(cfifo, 2);
	pelib_cfifo_push(size_t)(cfifo, 5);
	pelib_cfifo_push(size_t)(cfifo, 7);
	pelib_cfifo_push(size_t)(cfifo, 4);

	// Full fifo with write_ptr and read_ptr in the middle [2:5:2:5:7:4:>>16:17:18:19]
	str = pelib_string(cfifo_t(size_t))(*cfifo);
	assert_equals_str(str, "[2:5:2:5:7:4:>>16:17:18:19]");
	free(str);

	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);

	// Not full fifo with read_ptr at the edge and write_ptr in the middle [>2:5:2:5:7:4:>.:.:.:.]
	str = pelib_string(cfifo_t(size_t))(*cfifo);
	assert_equals_str(str, "[>2:5:2:5:7:4:>.:.:.:.]");
	free(str);

	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);

	// Empty fifo with write_ptr and read_ptr in the middle [.:.:.:.:.:.:>>.:.:.:.]
	str = pelib_string(cfifo_t(size_t))(*cfifo);
	assert_equals_str(str, "[.:.:.:.:.:.:>>.:.:.:.]");
	free(str);

	pelib_cfifo_push(size_t)(cfifo, 1);
	pelib_cfifo_push(size_t)(cfifo, 6);
	pelib_cfifo_push(size_t)(cfifo, 9);
	pelib_cfifo_push(size_t)(cfifo, 8);

	// Not full fifo, normal mode with write_ptr and read_ptr in the middle [>.:.:.:.:.:.:>1:6:9:8]
	str = pelib_string(cfifo_t(size_t))(*cfifo);
	assert_equals_str(str, "[>.:.:.:.:.:.:>1:6:9:8]");
	free(str);

	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);
	pelib_cfifo_pop(size_t)(cfifo);

	// Empty fifo with write_ptr and read_ptr in edge [>>.:.:.:.:.:.:.:.:.:.]
	str = pelib_string(cfifo_t(size_t))(*cfifo);
	assert_equals_str(str, "[>>.:.:.:.:.:.:.:.:.:.]");
	free(str);

	pelib_cfifo_push(size_t)(cfifo, 1);
	pelib_cfifo_push(size_t)(cfifo, 6);
	pelib_cfifo_push(size_t)(cfifo, 9);
	pelib_cfifo_push(size_t)(cfifo, 8);

	// Not full fifo, normal mode with write_ptr in the middle amd and read_ptr at the edge [>1:6:9:8<:.:.:.:.:.:.]
	str = pelib_string(cfifo_t(size_t))(*cfifo);
	assert_equals_str(str, "[>1:6:9:8:>.:.:.:.:.:.]");
	free(str);
}

static void
cfifo_pushmem_full()
{
	int ret, i;

	// Testing if adding more elements to fifo results in 0 and keeps fifo full
	ret = pelib_cfifo_pushmem(size_t)(cfifo, normal, CAPACITY);
	assert(pelib_cfifo_is_full(size_t)(cfifo));
	assert(ret == 0);

	for(i = 0; i < CAPACITY; i++)
	{
		assert(pelib_cfifo_pop(size_t)(cfifo) == i + VALUE);
	}
}

static void
cfifo_pushmem_normal()
{
	unsigned int i, ret;

	// Empty fifo
	for(i = 0; i < CAPACITY; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	// Fill it till TO_HEAD
	for(i = 0; i < TO_TAIL; i++)
	{
		pelib_cfifo_push(size_t)(cfifo, i + VALUE);
	}

	// Now fifo is reverse, write_ptr start at buffer + FROM_HEAD and read_ptr at buffer - TO_TAIL
	ret = pelib_cfifo_pushmem(size_t)(cfifo, normal, CAPACITY - TO_TAIL);
	assert(pelib_cfifo_is_full(size_t)(cfifo));
	assert(ret == CAPACITY - TO_TAIL);

	for(i = 0; i < CAPACITY; i++)
	{
		assert(pelib_cfifo_pop(size_t)(cfifo) == i + VALUE);
	}
}

static void
cfifo_pushmem_reverse()
{
	unsigned int i, ret;

	// Now fifo is at normal state, there are TO_TAIL free spots just before the end of buffer
	for(i = 0; i < TO_TAIL; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	// Now fifo has CAPACITY - TO_TAIL elements, stored at the end of its buffer
	for(i = 0; i < FROM_HEAD; i++)
	{
		pelib_cfifo_push(size_t)(cfifo, i + VALUE + CAPACITY);
	}

	// Now fifo is reverse, write_ptr start at buffer + FROM_HEAD and read_ptr at buffer - TO_TAIL
	ret = pelib_cfifo_pushmem(size_t)(cfifo, reverse, TO_TAIL - FROM_HEAD);
	assert(pelib_cfifo_is_full(size_t)(cfifo));
	assert(ret == TO_TAIL - FROM_HEAD);


	for(i = 0; i < CAPACITY; i++)
	{
		assert(pelib_cfifo_pop(size_t)(cfifo) == i + VALUE + TO_TAIL);
	}
}

static void
cfifo_pushmem_normal_and_reverse()
{
	unsigned int i, ret;

	// Empty fifo
	for(i = 0; i < CAPACITY; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	// Fill it till TO_TAIL
	for(i = 0; i < TO_TAIL; i++)
	{
		pelib_cfifo_push(size_t)(cfifo, i + VALUE);
	}

	// Pop to FROM_HEAD
	for(i = 0; i < FROM_HEAD; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	// Now fifo is normal, write_ptr start at buffer + TO_TAIL and read_ptr at buffer - FROM_HEAD
	ret = pelib_cfifo_pushmem(size_t)(cfifo, normal, CAPACITY - TO_TAIL + FROM_HEAD);
	assert(pelib_cfifo_is_full(size_t)(cfifo));
	assert(ret == CAPACITY - TO_TAIL + FROM_HEAD);

	for(i = 0; i < CAPACITY; i++)
	{
		assert(pelib_cfifo_pop(size_t)(cfifo) == i + VALUE + FROM_HEAD);
	}
}

static void
cfifo_pushmem_too_much()
{
	unsigned int i, ret;

	// Empty fifo
	for(i = 0; i < CAPACITY; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	// Fill it till TO_TAIL
	for(i = 0; i < TO_TAIL; i++)
	{
		pelib_cfifo_push(size_t)(cfifo, i + VALUE);
	}

	// Now fifo is inormal, write_ptr start at buffer + TO_TAIL and read_ptr at buffer - FROM_HEAD
	ret = pelib_cfifo_pushmem(size_t)(cfifo, normal, CAPACITY - TO_TAIL + FROM_HEAD);
	assert(pelib_cfifo_is_full(size_t)(cfifo));
	assert(ret == CAPACITY - TO_TAIL);

	for(i = 0; i < CAPACITY; i++)
	{
		assert(pelib_cfifo_pop(size_t)(cfifo) == i + VALUE);
	}
}

void
cfifo_popmem_simple()
{
	int ret, i;

	ret = pelib_cfifo_popmem(size_t)(cfifo, different, CAPACITY);
	assert(ret == CAPACITY);
	assert(pelib_cfifo_is_empty(size_t)(cfifo));

	for(i = 0; i < CAPACITY; i++)
	{
		assert(different[i] == cfifo->buffer[i]);
	}
}

void
cfifo_popmem_partial()
{
	int ret, i;

	ret = pelib_cfifo_popmem(size_t)(cfifo, different, CAPACITY / 2);
	assert(ret == CAPACITY / 2);
	assert(!pelib_cfifo_is_empty(size_t)(cfifo));
	assert(!pelib_cfifo_is_full(size_t)(cfifo));

	for(i = 0; i < CAPACITY / 2; i++)
	{
		assert(different[i] == cfifo->buffer[i]);
	}
}

void
cfifo_popmem_reverse()
{
	int i;

	for(i = 0; i < TO_TAIL; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	for(i = 0; i < FROM_HEAD; i++)
	{
		pelib_cfifo_push(size_t)(cfifo, i + VALUE + CAPACITY);
	}

	pelib_cfifo_popmem(size_t)(cfifo, different, CAPACITY - TO_TAIL + FROM_HEAD);

	assert(pelib_cfifo_is_empty(size_t)(cfifo));
	
	for(i = 0; i < CAPACITY - TO_TAIL + FROM_HEAD; i++)
	{
		assert(different[i] == i + VALUE + TO_TAIL);
	}
}

void
cfifo_popmem_too_much()
{	
	int i, ret;

	for(i = 0; i < TO_TAIL; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	ret = pelib_cfifo_popmem(size_t)(cfifo, different, CAPACITY);

	assert(pelib_cfifo_is_empty(size_t)(cfifo));
	assert(ret == CAPACITY - TO_TAIL);

	for(i = 0; i < CAPACITY - TO_TAIL; i++)
	{
		assert(different[i] == i + VALUE + TO_TAIL);
	}
}

void
test_peekmem_simple()
{
	int ret, i;

	ret = pelib_cfifo_peekmem(size_t)(cfifo, different, CAPACITY, 0);
	assert(ret == CAPACITY);
	assert(pelib_cfifo_is_full(size_t)(cfifo));

	for(i = 0; i < CAPACITY; i++)
	{
		assert(different[i] == cfifo->buffer[i]);
	}
}

void
test_peekmem_partial()
{
	int ret, i;

	ret = pelib_cfifo_peekmem(size_t)(cfifo, different, CAPACITY / 2, 0);
	assert(ret == CAPACITY / 2);
	assert(!pelib_cfifo_is_empty(size_t)(cfifo));
	assert(pelib_cfifo_is_full(size_t)(cfifo));

	for(i = 0; i < CAPACITY / 2; i++)
	{
		assert(different[i] == cfifo->buffer[i]);
	}
}

void
test_peekmem_reverse()
{
	size_t i;

	for(i = 0; i < TO_TAIL; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	for(i = 0; i < FROM_HEAD; i++)
	{
		pelib_cfifo_push(size_t)(cfifo, i + VALUE + CAPACITY);
	}

	pelib_cfifo_peekmem(size_t)(cfifo, different, CAPACITY - TO_TAIL + FROM_HEAD, 0);
	assert(pelib_cfifo_length(size_t)(cfifo) == CAPACITY - TO_TAIL + FROM_HEAD);
	
	for(i = 0; i < CAPACITY - TO_TAIL + FROM_HEAD; i++)
	{
		assert_equals_size_t(different[i], i + VALUE + TO_TAIL);
	}
}

void
test_peekmem_too_much()
{	
	size_t ret, i;

	for(i = 0; i < TO_TAIL; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}

	ret = pelib_cfifo_peekmem(size_t)(cfifo, different, CAPACITY, 0);

	assert_equals_size_t(pelib_cfifo_length(size_t)(cfifo), (size_t)(CAPACITY - TO_TAIL));
	assert_equals_size_t(ret, (size_t)(CAPACITY - TO_TAIL));

	for(i = 0; i < CAPACITY - TO_TAIL; i++)
	{
		assert(different[i] == i + VALUE + TO_TAIL);
	}
}

void
cfifo_length()
{
	assert(pelib_cfifo_length(size_t)(cfifo) == CAPACITY);

	pelib_cfifo_popmem(size_t)(cfifo, normal, FROM_HEAD);
	assert(pelib_cfifo_length(size_t)(cfifo) == CAPACITY - FROM_HEAD);

	pelib_cfifo_popmem(size_t)(cfifo, normal, TO_TAIL - FROM_HEAD);
	assert(pelib_cfifo_length(size_t)(cfifo) == CAPACITY - TO_TAIL);

	pelib_cfifo_popmem(size_t)(cfifo, normal, CAPACITY - TO_TAIL);
	assert(pelib_cfifo_length(size_t)(cfifo) == 0);

	pelib_cfifo_pushmem(size_t)(cfifo, normal, FROM_HEAD);
	assert(pelib_cfifo_length(size_t)(cfifo) == FROM_HEAD);

	pelib_cfifo_pushmem(size_t)(cfifo, normal, TO_TAIL - FROM_HEAD);
	assert(pelib_cfifo_length(size_t)(cfifo) == TO_TAIL);
	
	pelib_cfifo_pushmem(size_t)(cfifo, normal, CAPACITY - TO_TAIL);
	assert(pelib_cfifo_length(size_t)(cfifo) == CAPACITY);
}

void
cfifo_last()
{
	int i;

	assert(pelib_cfifo_last(size_t)(cfifo) == VALUE + CAPACITY - 1);
	pelib_cfifo_pop(size_t)(cfifo);
	assert(pelib_cfifo_last(size_t)(cfifo) == VALUE + CAPACITY - 1);
	for(i = 0; i < CAPACITY - 1; i++)
	{
		pelib_cfifo_pop(size_t)(cfifo);
	}
	assert(pelib_cfifo_last(size_t)(cfifo) == 0);
	
	pelib_cfifo_push(size_t)(cfifo, 3);
	assert(pelib_cfifo_last(size_t)(cfifo) == 3);

	pelib_cfifo_push(size_t)(cfifo, 7);
	assert(pelib_cfifo_last(size_t)(cfifo) == 7);
}

void
test_discard()
{
	size_t res;

	// Do not discard anything
	res = pelib_cfifo_discard(size_t)(cfifo, 0);
	assert(res == 0);
	assert(pelib_cfifo_length(size_t)(cfifo) == CAPACITY);
	
	// Discard a few values
	res = pelib_cfifo_discard(size_t)(cfifo, DISCARD);
	assert(res == DISCARD);
	assert(pelib_cfifo_length(size_t)(cfifo) == CAPACITY - DISCARD);

	// Discard all remaining values
	res = pelib_cfifo_discard(size_t)(cfifo, CAPACITY - DISCARD);
	assert(res == CAPACITY - DISCARD);
	assert(pelib_cfifo_length(size_t)(cfifo) == 0);

	// Discard even more values
	res = pelib_cfifo_discard(size_t)(cfifo, DISCARD);
	assert(res == 0);
	assert(pelib_cfifo_length(size_t)(cfifo) == 0);
}

void
test_discard_too_much()
{
	size_t res;

	// Discard more elements than in fifo
	res = pelib_cfifo_discard(size_t)(cfifo, CAPACITY + DISCARD);
	assert(res == CAPACITY);
	assert(pelib_cfifo_length(size_t)(cfifo) == 0);
}

void
test_fill()
{
	size_t res;

	// Get rid of everything in fifo
	pelib_cfifo_discard(size_t)(cfifo, CAPACITY);

	// Do not fill at all
	res = pelib_cfifo_fill(size_t)(cfifo, 0);
	assert(res == 0);
	assert(pelib_cfifo_length(size_t)(cfifo) == 0);
	
	// Fill a few values
	res = pelib_cfifo_fill(size_t)(cfifo, DISCARD);
	assert(res == DISCARD);
	assert(pelib_cfifo_length(size_t)(cfifo) == DISCARD);

	// Fill as much as remaining free spots
	res = pelib_cfifo_fill(size_t)(cfifo, CAPACITY - DISCARD);
	assert(res == CAPACITY - DISCARD);
	assert(pelib_cfifo_length(size_t)(cfifo) == CAPACITY);
	assert(pelib_cfifo_is_full(size_t)(cfifo));

	// Fill with even more values
	res = pelib_cfifo_fill(size_t)(cfifo, DISCARD);
	assert(res == 0);
	assert(pelib_cfifo_length(size_t)(cfifo) == CAPACITY);
	assert(pelib_cfifo_is_full(size_t)(cfifo));
}

void
test_fill_too_much()
{
	size_t res;

	// Empty fifo
	pelib_cfifo_discard(size_t)(cfifo, CAPACITY);

	// Fill with more value than capacity contains
	res = pelib_cfifo_fill(size_t)(cfifo, CAPACITY + DISCARD);

	assert(res == CAPACITY);
	assert(pelib_cfifo_length(size_t)(cfifo) == CAPACITY);
	assert(pelib_cfifo_is_full(size_t)(cfifo));
}

void
test_popfifo_simple()
{
	size_t res;
	int i;

	cfifo_t(size_t) *recipient;
	recipient = pelib_alloc_collection(cfifo_t(size_t))(CAPACITY);
	pelib_init(cfifo_t(size_t))(recipient);

	// Pop nothing
	res = pelib_cfifo_popfifo(size_t)(cfifo, recipient, 0);
	assert(res == 0);
	assert(pelib_cfifo_is_full(size_t)(cfifo));
	assert(pelib_cfifo_is_empty(size_t)(recipient));

	// Pop everything
	res = pelib_cfifo_popfifo(size_t)(cfifo, recipient, CAPACITY);
	assert(res == CAPACITY);
	assert(pelib_cfifo_is_full(size_t)(recipient));
	assert(pelib_cfifo_is_empty(size_t)(cfifo));

	for(i = 0; i < CAPACITY; i++)
	{
		assert(pelib_cfifo_peek(size_t)(recipient, i) == i + VALUE)
	}

	// Try to pop more, but first add values to cfifo
	for(i = 0; i < TO_TAIL; i++)
	{
		pelib_cfifo_push(size_t)(cfifo, i);
	}

	// Pop everything added
	res = pelib_cfifo_popfifo(size_t)(cfifo, recipient, TO_TAIL);
	assert(res == 0);
	assert(pelib_cfifo_is_full(size_t)(recipient));
	assert(!pelib_cfifo_is_empty(size_t)(cfifo));

	// Empty source and recipient fifo and try to insert again
	pelib_cfifo_discard(size_t)(recipient, CAPACITY);
	pelib_cfifo_discard(size_t)(cfifo, CAPACITY);
	res = pelib_cfifo_popfifo(size_t)(cfifo, recipient, CAPACITY);
	assert(res == 0);
	assert(pelib_cfifo_is_empty(size_t)(recipient));
	assert(pelib_cfifo_is_empty(size_t)(cfifo));

	// Good ol' cleanup
	pelib_free(cfifo_t(size_t))(recipient);
}

void
test_popfifo_complex()
{
	int i;
	size_t res;

	// Make reference fifo in reverse state
	pelib_cfifo_discard(size_t)(cfifo, TO_TAIL);
	for(i = 0; i < FROM_HEAD; i++)
	{
		pelib_cfifo_push(size_t)(cfifo, i + VALUE + CAPACITY);
	}

	// Make a recipient fifo in reverse state, but less free space
	cfifo_t(size_t) *recipient;
	recipient = pelib_alloc_collection(cfifo_t(size_t))(CAPACITY);
	pelib_init(cfifo_t(size_t))(recipient);

	for(i = 0; i < CAPACITY; i++)
	{
		pelib_cfifo_push(size_t)(recipient, i + VALUE);
	}
	pelib_cfifo_discard(size_t)(recipient, TO_TAIL - OFFSET);
	for(i = 0; i < FROM_HEAD + OFFSET; i++)
	{
		pelib_cfifo_push(size_t)(recipient, i + VALUE + CAPACITY);
	}

	res = pelib_cfifo_popfifo(size_t)(cfifo, recipient, CAPACITY);
	assert(res == TO_TAIL - FROM_HEAD - 2 * OFFSET);
	assert(pelib_cfifo_length(size_t)(recipient) == CAPACITY);
	assert(pelib_cfifo_is_full(size_t)(recipient));
	assert(!pelib_cfifo_is_full(size_t)(cfifo));
	assert(!pelib_cfifo_is_empty(size_t)(cfifo));
	assert(pelib_cfifo_length(size_t)(cfifo) == FROM_HEAD + CAPACITY - TO_TAIL - (TO_TAIL - FROM_HEAD - 2 * OFFSET));

	/* TODO: Make sure recipient indeed has the expected values */

	// Good ol' cleanup
	pelib_free(cfifo_t(size_t))(recipient);
}

void
run()
{
	test(test_pop);
	//test(test_pop_too_much);
	test(test_discard);
	test(test_discard_too_much);
	test(test_push);
	test(test_push_too_much);
	test(test_fill);
	test(test_fill_too_much);
	test(test_is_full_or_empty);
	test(test_string);
	//test(test_string_content);
	test(cfifo_pushmem_full);
	test(cfifo_pushmem_normal);
	test(cfifo_pushmem_reverse);
	test(cfifo_pushmem_normal_and_reverse);
	test(cfifo_pushmem_too_much);
	test(cfifo_popmem_simple);
	test(cfifo_popmem_partial);
	test(cfifo_popmem_reverse);
	test(cfifo_popmem_too_much);
	test(test_peekmem_simple);
	test(test_peekmem_partial);
	test(test_peekmem_reverse);
	test(test_peekmem_too_much);
	test(cfifo_length);
	test(cfifo_last);
	test(test_popfifo_simple);
	test(test_popfifo_complex);
}
