/*
 * integer.c
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

#include <stdlib.h>
#include <string.h>

#include <pelib/pelib_string.h>

int
pelib_copy(pelib_string)(pelib_string s1, pelib_string *s2)
{
	strcpy((char*)*s2, (char*)s1);

	return 1;
}

int
pelib_init(pelib_string)(pelib_string *val)
{
	**val = '\0';

	return 1;
}

int
pelib_compare(pelib_string)(pelib_string a, pelib_string b)
{
	return strcmp((char*)a, (char*)b);
}

char*
pelib_string(pelib_string)(pelib_string a)
{
	size_t size;
	char* str;

	str = malloc(sizeof(char) * pelib_string_length(a));
	sprintf(str, "%s", a);

	return str;
}

size_t
pelib_string_length(pelib_string str)
{
	return strlen(str);
}

char*
pelib_string_detail(pelib_string)(pelib_string a, int level)
{
	return pelib_string(pelib_string)(a);
}

FILE*
pelib_printf(pelib_string)(FILE* stream, pelib_string a)
{
	char* str = pelib_string(pelib_string)(a);
	fprintf(stream, "%s", str);
	free(str);

	return stream;
}
