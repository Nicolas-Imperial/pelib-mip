/*
 * string.c
 *
 *	Created on: 20 Feb 2012
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
#include <math.h>
#include <string.h>

#include <pelib/string.h>
#include <pelib/const_string.h>

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

/*
const_string*
pelib_alloc_struct(const_string)()
{
	return 1;
}
*/

int
pelib_alloc_buffer(const_string)(const_string *str, size_t n)
{
	*str = malloc(sizeof(char) * n);
	return (*str != NULL);
}

int
pelib_set_buffer(const_string)(const_string *str, void* buffer, size_t n)
{
	*str = buffer;
	return 1;
}

int
pelib_init(const_string)(const_string *val)
{
	*val = NULL;
	return 1;
}

int
pelib_destroy(const_string)(const_string val)
{
	/* Do nothing */
	return 1;
}

int
pelib_compare(const_string)(const_string a, const_string b)
{
	return strcmp(a, b);
}

int
pelib_copy(const_string)(const_string a, const_string *b)
{
	// TODO: dirty: prefer a copy constructor
	return pelib_copy(string)((string)a, (string*)b);
}

char*
pelib_string(const_string)(const_string a)
{
	string cpy;
	pelib_alloc_buffer(string)(&cpy, 1 + strlen(a) + 1 + 1);
	cpy[0] = '"';
	string cpy_ptr = &(cpy[1]);
	pelib_copy(string)((string)a, &cpy_ptr);
	cpy[1 + strlen(a)] = '"';
	cpy[1 + strlen(a) + 1] = '\0';
	return cpy;
}

char*
pelib_string_detail(const_string)(const_string a, int level)
{
	return pelib_string(const_string)(a);
}

FILE*
pelib_printf(const_string)(FILE* stream, const_string a)
{
	fprintf(stream, "%s\n", a);
	return stream;
}

// Now include the generic set implementation
#define SET_T const_string
#include "pelib/set.c"

// Now include the generic set implementation
#define PELIB_PAIR_KEY_T const_string
#define PELIB_PAIR_VALUE_T const_string
#include "pelib/pair.c"

#define PELIB_ITERATOR_T const_string
#include "pelib/iterator.c"

#define PELIB_ITERATOR_T pelib_pair_t(const_string, const_string)
#include "pelib/iterator.c"

// Now include the generic set implementation
#define MAP_KEY_T const_string
#define MAP_VALUE_T const_string
#include "pelib/map.c"

// Now include the generic set implementation
#define SET_T pelib_pair_t(const_string, const_string)
#include "pelib/set.c"

