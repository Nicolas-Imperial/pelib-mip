/*
 * pelib_pair.c
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

#if !(defined PELIB_PAIR_KEY_T && defined PELIB_PAIR_VALUE_T)
#error Using generic pelib_pair without key or value types
#endif

pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T)*
pelib_alloc_struct(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))()
{
	pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T)* pelib_pair;

	pelib_pair = malloc(sizeof(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T)));
	assert(pelib_pair != NULL);

	return pelib_pair;
}

int
pelib_init(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T)* pelib_pair)
{
	int a = pelib_init(PELIB_PAIR_KEY_T)(&pelib_pair->key);
	return a && pelib_init(PELIB_PAIR_VALUE_T)(&pelib_pair->value);
}

int
pelib_copy(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T) src, pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T)* dst)
{
	int a = pelib_copy(PELIB_PAIR_KEY_T)(src.key, &dst->key);	
	return a && pelib_copy(PELIB_PAIR_VALUE_T)(src.value, &dst->value);
}

#define pelib_pair_length_debug printf("[PELIB:%s:%s:%d] i = %d\n", __FILE__, __FUNCTION__, __LINE__, i);
#define pelib_pair_length_pre_debug printf("[PELIB:%s:%s:%d] length = %d\n", __FILE__, __FUNCTION__, __LINE__, pelib_pelib_pair_length(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T)(&pelib_pair));
char*
pelib_string(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T) pelib_pair)
{
	char *key = pelib_string(PELIB_PAIR_KEY_T)(pelib_pair.key);
	char *value = pelib_string(PELIB_PAIR_VALUE_T)(pelib_pair.value);

	char *elem = malloc((strlen(key) + strlen(value) + 4) * sizeof(char*));
	elem[0] = '(';
	strcpy(&elem[1], key);
	elem[1 + strlen(key)] = ',';
	strcpy(&elem[1 + strlen(key) + 1], value);
	elem[1 + strlen(key) + 1 + strlen(value)] = ')';
	elem[1 + strlen(key) + 1 + strlen(value) + 1] = '\0';
	free(key);
	free(value);

	return elem;
}

char*
pelib_string_detail(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T) pelib_pair, int level)
{
	if(level == 0)
	{
		return pelib_string(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair);
	}
	else
	{
		char *key = pelib_string(PELIB_PAIR_KEY_T)(pelib_pair.key);
		char *value = pelib_string(PELIB_PAIR_VALUE_T)(pelib_pair.value);

		char *elem = malloc((strlen(key) + strlen(value) + 4) * sizeof(char*));
		elem[0] = '(';
		strcpy(&elem[1], key);
		elem[1 + strlen(key)] = ',';
		strcpy(&elem[1 + strlen(key) + 1], value);
		elem[1 + strlen(key) + 1 + strlen(value)] = ')';
		elem[1 + strlen(key) + 1 + strlen(value) + 1] = '\0';
		free(key);
		free(value);

		return elem;
	}
}

FILE*
pelib_printf(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(FILE* stream, pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T) pelib_pair)
{
	char * str;
	str = pelib_string(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

FILE*
pelib_printf_detail(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(FILE* stream, pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T) pelib_pair, int level)
{
	char * str;
	str = pelib_string_detail(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair, level);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

int
pelib_free(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T)* pelib_pair)
{
	return pelib_free_struct(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair);
}

int
pelib_free_struct(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T)* pelib_pair)
{
	pelib_destroy(PELIB_PAIR_KEY_T)(pelib_pair->key);
	pelib_destroy(PELIB_PAIR_VALUE_T)(pelib_pair->value);
	free(pelib_pair);
	return 0;
}

int
pelib_compare(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T) a1, pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T) a2)
{
	int a = pelib_compare(PELIB_PAIR_KEY_T)(a1.key, a2.key);
	int b = pelib_compare(PELIB_PAIR_VALUE_T)(a1.value, a2.value);
	return a == 0 ? b : a;
}

int
pelib_destroy(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T))(pelib_pair_t(PELIB_PAIR_KEY_T, PELIB_PAIR_VALUE_T) set)
{
	pelib_destroy(PELIB_PAIR_KEY_T)(set.key);
	pelib_destroy(PELIB_PAIR_VALUE_T)(set.value);
	return 1;
}

#undef PELIB_PAIR_KEY_T
#undef PELIB_PAIR_VALUE_T
