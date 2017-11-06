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
#include <math.h>

#include <pelib/integer.h>

int
pelib_copy(int)(int s1, int * s2)
  {
    *s2 = s1;

    return 1;
  }

int
pelib_init(int)(int *val)
  {
    *val = 0;

    return 1;
  }

int
pelib_destroy(int)(int val)
  {
	/* Do nothing */
    return 1;
  }

int
pelib_compare(int)(int a, int b)
  {
    return a - b;
  }

char*
pelib_string(int)(int a)
  {
    size_t size;
    char* str;

    size = (int)(a > 0 ? log10(a) : 1);
    str = malloc(sizeof(char) * (size + 2));
    sprintf(str, "%i", a);

    return str;
  }

char*
pelib_string_detail(int)(int a, int level)
{
	return pelib_string(int)(a);
}

size_t
pelib_fread(int)(int* buffer, FILE* stream)
{
  size_t total;
  int num;
  int read, has_more;

  read = 0;
  has_more = 1;
  num = 0;
  total = 0;

  while (read != ' ')
    {
      if ((has_more = fread(&read, sizeof(char), 1, stream)) == 0 || read == ' ')
        {
          break;
        }

      total += has_more;
      num = num * 10 + read - '0';
    }

  *buffer = num;
  return total;
}

FILE*
pelib_printf(int)(FILE* stream, int a)
{
	char *str = pelib_string(int)(a);
	fprintf(stream, "%s\n", str);
	free(str);
	return stream;
}

// Now include the generic array implementation
#define ARRAY_T int
#include "pelib/array.c"

// Now include the generic set implementation
#define SET_T int
#include "pelib/set.c"

// Now include the generic set implementation
#define PELIB_PAIR_KEY_T int
#define PELIB_PAIR_VALUE_T int
#include "pelib/pair.c"

#define PELIB_ITERATOR_T int
#include "pelib/iterator.c"

#define PELIB_ITERATOR_T pelib_pair_t(int, int)
#include "pelib/iterator.c"

// Now include the generic set implementation
#define MAP_KEY_T int
#define MAP_VALUE_T int
#include "pelib/map.c"

// Now include the generic set implementation
#define SET_T pelib_pair_t(int, int)
#include "pelib/set.c"

// Now include the generic fifo implementation
#define CFIFO_T int
#include "pelib/fifo.c"

// Now include the generic fifo implementation
#define CFIFO_ARRAY_T int
#include "pelib/fifo_array.c"
