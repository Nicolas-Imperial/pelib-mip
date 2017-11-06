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
#include <pelib/integer.h>

#define CAPACITY 10
#define VALUE   10
#define TO_TAIL 7
#define FROM_HEAD 4

cfifo_t(int) *cfifo;
int value;
int reverse[CAPACITY], normal[CAPACITY], different[CAPACITY];

void
init()
{
	unsigned int i;

	cfifo = pelib_alloc_collection(cfifo_t(int))(CAPACITY);
	pelib_init(cfifo_t(int))(cfifo);
	value = VALUE;

	for (i = 0; i < CAPACITY; i++)
	{
		pelib_cfifo_push(int)(cfifo, i + VALUE);
		normal[i] = VALUE + TO_TAIL + i;
		reverse[i] = VALUE + CAPACITY + FROM_HEAD + i;
		different[i] = -1;
	}
}

void
cleanup()
{
	pelib_free(cfifo_t(int))(cfifo);
}

void setup() {}
void teardown() {}

static void
cfifo_from_array()
{
	int i;
	array_t(int) *array;
	cfifo_t(int) *fifo;
	array = pelib_alloc_collection(array_t(int))(10);
	for (i = 0; i < 10; i++)
	{
		pelib_array_append(int)(array, i);
	}

	fifo = pelib_cfifo_from_array(int)(array);

	assert(fifo != NULL);
	assert(fifo->write == fifo->read);

	for (i = 0; i < 10; i++)
	{
		assert(pelib_cfifo_pop(int)(fifo) == i);
	}
}

static void
array_from_cfifo()
{
	int i;
	array_t(int) *array;
	cfifo_t(int) *fifo;

	fifo = pelib_alloc_collection(cfifo_t(int))(10);
	pelib_init(cfifo_t(int))(fifo);
	for (i = 0; i < 10; i++)
	{
		pelib_cfifo_push(int)(fifo, i);
	}

	array = pelib_array_from_cfifo(int)(fifo);

	assert(array != NULL);
	assert(pelib_array_length(int)(array) == pelib_array_capacity(int)(array));
	assert(pelib_array_length(int)(array) == 10);

	for (i = 0; i < 10; i++)
	{
		assert(pelib_array_read(int)(array, i) == i);
	}
}

void
run()
{
	test(cfifo_from_array);
	test(array_from_cfifo);
}
