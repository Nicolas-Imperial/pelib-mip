/*
 * test_array.c
 *
 *  Created on: 13 Jan 2012
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
#include <math.h>

#include <pelib/unit.h>
#include <pelib/integer.h>
#include <pelib/sort.h>

#define LENGTH 1024
#define RANDOM_SEED	0
#define MAX_VALUE LENGTH

array_t(int) *prand, *increasing, *unsorted;
int value;

int
pseudo_randomize(int i, int max)
{
	double res;

	res = (sin((double) (i * max) / (double) (max * 2 * M_PI * 0.7)) + 1) / 2
			* max;

	return (int) res;
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

void
init()
{
	int i;

	// Initialize pseudo-random array
	srandom(RANDOM_SEED);
	prand = pelib_alloc_collection(array_t(int))(LENGTH);
	for (i = 0; i < LENGTH; i++)
	{
		pelib_array_append(int)(prand, random() % MAX_VALUE);
	}

	// Initialize increasing sorted array
	increasing = pelib_alloc_collection(array_t(int))(LENGTH);
	for (i = 0; i < LENGTH; i++)
	{
		pelib_array_append(int)(increasing, i);
	}

	// Initialize increasing sorted array
	unsorted = pelib_alloc_collection(array_t(int))(LENGTH);
	for (i = 0; i < LENGTH; i++)
	{
		pelib_array_append(int)(unsorted, pseudo_randomize(i, MAX_VALUE));
	}
}

void
cleanup()
{
	pelib_free(array_t(int))(prand);
	pelib_free(array_t(int))(increasing);
	pelib_free(array_t(int))(unsorted);
}

static void
test_is_increasing()
{
	assert(pelib_is_increasing(increasing));
	assert_false(pelib_is_increasing(unsorted));
}

static void
test_quicksort()
{
	//pelib_quicksort(unsorted);
	//assert(pelib_is_increasing(unsorted));
	assert(1);
}

/*
static void
test_insertsort()
{
	// do nothing
}
*/

void
run()
{
	test(test_is_increasing);
	test(test_quicksort);
	//test(test_insertsort);
}
