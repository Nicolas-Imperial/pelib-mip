/*
 * pelib_iterator.c
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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifndef PELIB_ITERATOR_T
#error Using generic pelib_iterator without a type
#endif

pelib_iterator_t(PELIB_ITERATOR_T)*
pelib_alloc_struct(pelib_iterator_t(PELIB_ITERATOR_T))()
{
	pelib_iterator_t(PELIB_ITERATOR_T)* pelib_iterator;

	pelib_iterator = malloc(sizeof(pelib_iterator_t(PELIB_ITERATOR_T)));
	assert(pelib_iterator != NULL);

	return pelib_iterator;
}

pelib_iterator_t(PELIB_ITERATOR_T)*
pelib_alloc(pelib_iterator_t(PELIB_ITERATOR_T))()
{
	pelib_iterator_t(PELIB_ITERATOR_T)* pelib_iterator;

	pelib_iterator = malloc(sizeof(pelib_iterator_t(PELIB_ITERATOR_T)));
	assert(pelib_iterator != NULL);

	return pelib_iterator;
}

int
pelib_init(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator_t(PELIB_ITERATOR_T)* pelib_iterator)
{
	pelib_iterator->previous = NULL;
	pelib_iterator->next = NULL;
	return pelib_init(PELIB_ITERATOR_T)(&pelib_iterator->value);
}

int
pelib_copy(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator_t(PELIB_ITERATOR_T) src, pelib_iterator_t(PELIB_ITERATOR_T)* dst)
{
	size_t i;

	*dst = src;
	return pelib_copy(PELIB_ITERATOR_T)(src.value, &dst->value);
}

#define pelib_iterator_length_debug printf("[PELIB:%s:%s:%d] i = %d\n", __FILE__, __FUNCTION__, __LINE__, i);
#define pelib_iterator_length_pre_debug printf("[PELIB:%s:%s:%d] length = %d\n", __FILE__, __FUNCTION__, __LINE__, pelib_pelib_iterator_length(PELIB_ITERATOR_T)(&pelib_iterator));
char*
pelib_string(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator_t(PELIB_ITERATOR_T) pelib_iterator)
{
	char *value = pelib_string(PELIB_ITERATOR_T)(pelib_iterator.value);
	char *elem = malloc((strlen(value) + 3) * sizeof(char*));
	strcpy(&elem[1], value);
	elem[0] = '{';
	elem[strlen(value)] = '}';
	elem[strlen(value) + 1] = '\0';
	free(value);

	return elem;
}

char*
pelib_string_detail(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator_t(PELIB_ITERATOR_T) pelib_iterator, int level)
{
	if(level == 0)
	{
		return pelib_string(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator);
	}
	else
	{
		char *value = pelib_string_detail(PELIB_ITERATOR_T)(pelib_iterator.value, level);
		char *elem = malloc((strlen(value) + 3) * sizeof(char*));
		strcpy(&elem[1], value);
		elem[0] = '{';
		elem[strlen(value)] = '}';
		elem[strlen(value) + 1] = '\0';
		free(value);

		return elem;
	}
}

FILE*
pelib_printf(pelib_iterator_t(PELIB_ITERATOR_T))(FILE* stream, pelib_iterator_t(PELIB_ITERATOR_T) pelib_iterator)
{
	char * str;
	str = pelib_string(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

FILE*
pelib_printf_detail(pelib_iterator_t(PELIB_ITERATOR_T))(FILE* stream, pelib_iterator_t(PELIB_ITERATOR_T) pelib_iterator, int level)
{
	char * str;
	str = pelib_string_detail(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator, level);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

int
pelib_destroy(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator_t(PELIB_ITERATOR_T) pelib_iterator)
{
	pelib_destroy(PELIB_ITERATOR_T)(pelib_iterator.value);
	return 1;
}

int
pelib_free(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator_t(PELIB_ITERATOR_T)* pelib_iterator)
{
	return pelib_free_struct(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator);
}

int
pelib_free_struct(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator_t(PELIB_ITERATOR_T)* pelib_iterator)
{
	free(pelib_iterator);
	return 0;
}

int
pelib_compare(pelib_iterator_t(PELIB_ITERATOR_T))(pelib_iterator_t(PELIB_ITERATOR_T) a1, pelib_iterator_t(PELIB_ITERATOR_T) a2)
{
	return pelib_compare(PELIB_ITERATOR_T)(a1.value, a2.value);
}

pelib_iterator_t(PELIB_ITERATOR_T)*
pelib_pelib_iterator_next(PELIB_ITERATOR_T)(pelib_iterator_t(PELIB_ITERATOR_T) *el)
{
	if(el != NULL)
	{
		return el->next;
	}
	else
	{
		return NULL;
	}
}

PELIB_ITERATOR_T
pelib_pelib_iterator_read(PELIB_ITERATOR_T)(pelib_iterator_t(PELIB_ITERATOR_T) *el)
{
	return el->value;
}

size_t
pelib_pelib_iterator_distance(PELIB_ITERATOR_T)(pelib_iterator_t(PELIB_ITERATOR_T) *start, pelib_iterator_t(PELIB_ITERATOR_T) *stop)
{
	size_t distance = 0;

	pelib_iterator_t(PELIB_ITERATOR_T) *i = start;

	while(pelib_compare(pelib_iterator_t(PELIB_ITERATOR_T))(*i, *stop) != 0)
	{
		i = pelib_pelib_iterator_next(PELIB_ITERATOR_T)(start);
		distance++;
	}
	
	return distance;
}

#undef PELIB_ITERATOR_T
