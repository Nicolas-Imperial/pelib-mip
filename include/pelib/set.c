/*
 * set.c
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

#include <pelib/size_t.h>

#ifndef SET_T
#error Using generic set without a type
#endif

// Now include the generic set implementation
//#define PELIB_ITERATOR_T SET_T
//#include "pelib/iterator.c"

#ifdef debug
#undef debug
#undef debug_addr
#undef debug_int
#undef debug_size_t
#endif

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

pelib_set_t(SET_T)*
pelib_alloc_struct(pelib_set_t(SET_T))()
{
	pelib_set_t(SET_T)* set;

	set = malloc(sizeof(pelib_set_t(SET_T)));
	assert(set != NULL);

	return set;
}

pelib_set_t(SET_T)*
pelib_alloc(pelib_set_t(SET_T))()
{
	pelib_set_t(SET_T)* set;

	set = malloc(sizeof(pelib_set_t(SET_T)));
	assert(set != NULL);

	return set;
}

int
pelib_init(pelib_set_t(SET_T))(pelib_set_t(SET_T)* set)
{
	set->first = NULL;
	set->middle = NULL;
	set->last = NULL;

	return 1;
}

#define pelib_set_insert_element(elem) PELIB_CONCAT_3(pelib_, pelib_set(elem), _insert_element)
static
int
pelib_set_insert_element(SET_T)(pelib_set_t(SET_T) *set, pelib_iterator_t(SET_T) *elem)
{
	pelib_iterator_t(SET_T) *current = set->first, *previous = NULL;

	// If there is at least one element, then browse the list until we find the right spot
	// Otherwise, place the element as first and last, that is first and last pointers point
	// the the element and next and previous pointers of this element point to nothing.
	if(current != NULL)
	{
		// browse the list until we reach the end or until we find the last element lower 
		// than the element to insert
		while(current != NULL && pelib_compare(SET_T)(elem->value, current->value) > 0)
		{
			previous = current;
			current = current->next;
		}

		/* Comment out the next block if you want to implement a multiset */
		if(current != 0 && pelib_compare(SET_T)(elem->value, current->value) == 0)
		{
			return 0;
		}

		// Insert element
		elem->previous = previous;
		elem->next = current;
		if(previous != NULL)
		{
			previous->next = elem;
		}

		// Update tail if the element is insert last
		// Otherwise update next element's pointer to
		// previous element (the newly inserted one)
		if(current == NULL)
		{
			set->last = elem;
		}
		else
		{
			if(current == set->first)
			{
				set->first = elem;
			}
			current->previous = elem;
		}
	}
	else
	{
		elem->next = NULL;
		elem->previous = NULL;
		set->first = elem;
		set->last = elem;
	}

	return 1;
}

int
pelib_set_insert(SET_T)(pelib_set_t(SET_T)* set, SET_T value)
{
	pelib_iterator_t(SET_T) *new = pelib_alloc(pelib_iterator_t(SET_T))();
	pelib_init(pelib_iterator_t(SET_T))(new);
	pelib_copy(SET_T)(value, &new->value);
	if(!pelib_set_insert_element(SET_T)(set, new))
	{
		pelib_free(pelib_iterator_t(SET_T))(new);
		return 0;
	}
	else
	{
		return 1;
	}
}

int
pelib_copy(pelib_set_t(SET_T))(pelib_set_t(SET_T) src, pelib_set_t(SET_T)* dst)
{
	size_t i;

	pelib_iterator_t(SET_T) *elem = src.last;
	while(elem != NULL)
	{
		pelib_iterator_t(SET_T) *cpy = pelib_alloc(pelib_iterator_t(SET_T))();
		pelib_copy(SET_T)(elem->value, &cpy->value);
		pelib_set_insert_element(SET_T)(dst, cpy);
		elem = elem->previous;
	}

	return 0;
}

#define set_length_debug printf("[PELIB:%s:%s:%d] i = %d\n", __FILE__, __FUNCTION__, __LINE__, i);
#define set_length_pre_debug printf("[PELIB:%s:%s:%d] length = %d\n", __FILE__, __FUNCTION__, __LINE__, pelib_set_length(SET_T)(&set));
char*
pelib_string(pelib_set_t(SET_T))(pelib_set_t(SET_T) set)
{
	char *str, *grow, *elem;
	size_t i;
	str = malloc(3 * sizeof(char));
	sprintf(str, "[");

	pelib_iterator_t(SET_T) *el = set.first;
	while(el != NULL && el->next != NULL)
	{
		elem = pelib_string(SET_T)(el->value);
		grow = malloc((strlen(str) + 1) * sizeof(char));
		sprintf(grow, "%s", str);

		free(str);
		str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
		sprintf(str, "%s%s:", grow, elem);
		free(elem);
		free(grow);
		el = el->next;
	}
	while(el != NULL)
	{
		elem = pelib_string(SET_T)(el->value);
		grow = malloc((strlen(str) + 1) * sizeof(char));
		sprintf(grow, "%s", str);

		free(str);
		str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
		sprintf(str, "%s%s", grow, elem);
		free(elem);
		free(grow);

		el = el->next;
	}

	grow = malloc((strlen(str) + 1) * sizeof(char));
	sprintf(grow, "%s", str);
	sprintf(str, "%s]", grow);
	free(grow);

	return str;
}

char*
pelib_string_detail(pelib_set_t(SET_T))(pelib_set_t(SET_T) set, int level)
{
	if(level == 0)
	{
		return pelib_string(pelib_set_t(SET_T))(set);
	}
	else
	{
		// Bring it on
		char *str, *grow, *elem;
		size_t i;
		str = malloc(3 * sizeof(char));
		sprintf(str, "[");
		pelib_iterator_t(SET_T) *el = set.first;
		
		while(el != NULL && el->next != NULL)
		{
			elem = pelib_string_detail(SET_T)(el->value, level);
			grow = malloc((strlen(str) + 1) * sizeof(char));
			sprintf(grow, "%s", str);

			free(str);
			str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
			sprintf(str, "%s%s:", grow, elem);
			free(elem);
			free(grow);
		}
		while(el != NULL)
		{
			elem = pelib_string_detail(SET_T)(el->value, level);
			grow = malloc((strlen(str) + 1) * sizeof(char));
			sprintf(grow, "%s", str);

			free(str);
			str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
			sprintf(str, "%s%s", grow, elem);
			free(elem);
			free(grow);
		}

		grow = malloc((strlen(str) + 1) * sizeof(char));
		sprintf(grow, "%s", str);
		sprintf(str, "%s]", grow);
		free(grow);

		return str;
	}
}

FILE*
pelib_printf(pelib_set_t(SET_T))(FILE* stream, pelib_set_t(SET_T) set)
{
	char * str;
	str = pelib_string(pelib_set_t(SET_T))(set);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

FILE*
pelib_printf_detail(pelib_set_t(SET_T))(FILE* stream, pelib_set_t(SET_T) set, int level)
{
	char * str;
	str = pelib_string_detail(pelib_set_t(SET_T))(set, level);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

int
pelib_destroy(pelib_set_t(SET_T))(pelib_set_t(SET_T) set)
{
	pelib_iterator_t(SET_T) *elem = set.first;
	while(elem != NULL)
	{
		pelib_iterator_t(SET_T) *current = elem;
		elem = elem->next;
		pelib_destroy(pelib_iterator_t(SET_T))(*current);
		pelib_free(pelib_iterator_t(SET_T))(current);		
	}
	return 1;
}

int
pelib_free_buffer(pelib_set_t(SET_T))(pelib_set_t(SET_T)* set)
{
	pelib_iterator_t(SET_T) *elem = set->first;
	while(elem != NULL)
	{
		pelib_iterator_t(SET_T) *current = elem;
		elem = elem->next;
		pelib_destroy(pelib_iterator_t(SET_T))(*current);
		pelib_free(pelib_iterator_t(SET_T))(current);		
	}
	return 1;
}

int
pelib_free_struct(pelib_set_t(SET_T))(pelib_set_t(SET_T)* set)
{
	free(set);
	return 1;
}

int
pelib_free(pelib_set_t(SET_T))(pelib_set_t(SET_T)* set)
{
	pelib_free_buffer(pelib_set_t(SET_T))(set);
	pelib_free_struct(pelib_set_t(SET_T))(set);
	return 1;
}

int
pelib_compare(pelib_set_t(SET_T))(pelib_set_t(SET_T) a1, pelib_set_t(SET_T) a2)
{
	pelib_iterator_t(SET_T) *e1 = a1.first;
	pelib_iterator_t(SET_T) *e2 = a2.first;

	while(e1 != NULL && e2 != NULL && pelib_compare(pelib_iterator_t(SET_T))(*e1, *e2) == 0)
	{
		e1 = e1->next;
		e2 = e2->next;
	}

	if(e1 == NULL && e2 != NULL)
	{
		return 1;
	}
	else if(e1 != NULL && e2 == NULL)
	{
		return -1;
	}
	else return pelib_compare(pelib_iterator_t(SET_T))(*e1, *e2);
}

#undef SET_T
