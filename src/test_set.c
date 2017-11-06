/*
 * test_set.c
 *
 *	Created on: 13 Jan 2012
 *	Copyright 2012 Nicolas Melot
 *
 * This file is part of pelib.
 * 
 *		 pelib is free software: you can redistribute it and/or modify
 *		 it under the terms of the GNU General Public License as published by
 *		 the Free Software Foundation, either version 3 of the License, or
 *		 (at your option) any later version.
 * 
 *		 pelib is distributed in the hope that it will be useful,
 *		 but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 *		 GNU General Public License for more details.
 * 
 *		 You should have received a copy of the GNU General Public License
 *		 along with pelib. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <pelib/unit.h>
#include <pelib/integer.h>

#define LENGTH	8
#define FILENAME_BINARY	"set_binary.bin"
#define VALUE	10
#define SKIP	6

#if 10
#define debug(var) printf("[%s:%s:%d] %s = \"%s\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#define debug_addr(var) printf("[%s:%s:%d] %s = \"%p\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#define debug_int(var) printf("[%s:%s:%d] %s = \"%d\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#define debug_size_t(var) printf("[%s:%s:%d] %s = \"%zu\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#else
#define debug(var)
#define debug_addr(var)
#define debug_int(var)
#define debug_size_t(var)
#endif

pelib_set_t(pelib_pair_t(int, int)) set;
int value;

int insertion[] = {3, 6, 7, 5, 3, 5, 6, 2};

void
init()
{
	unsigned int i;
	pelib_init(pelib_set_t(pelib_pair_t(int, int)))(&set);
	value = VALUE;

	for (i = 0; i < LENGTH; i++)
	{
		pelib_pair_t(int, int) pelib_pair;
		pelib_pair.key = insertion[i];
		pelib_pair.value = i;
		pelib_set_insert(pelib_pair_t(int, int))(&set, pelib_pair);
	}
}

void
cleanup()
{
	pelib_destroy(pelib_set_t(pelib_pair_t(int, int)))(set);
}

void setup() {};
void teardown() {};

static void
test_string()
{
	char *ref = "[(2,7):(3,0):(3,4):(5,3):(5,5):(6,1):(6,6):(7,2)]";
	char *str = pelib_string(pelib_set_t(pelib_pair_t(int, int)))(set);
	assert_equals_str(str, ref);
	free(str);
}

void
run()
{
	test(test_string);
}

