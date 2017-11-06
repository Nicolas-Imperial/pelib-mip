/*
 * size_t.c
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

#include <math.h>
#include <stdlib.h>

#include <pelib/size_t.h>

int
pelib_copy(size_t)(size_t s1, size_t * s2)
{
  *s2 = s1;

  return 1;
}

int
pelib_init(size_t)(size_t *var)
{
  *var = 0;

  return 1;
}

int
pelib_compare(size_t)(size_t a, size_t b)
{
  return a - b;
}

char*
pelib_string(size_t)(size_t a)
  {
    size_t size;
    char* str;

	if(a > 0)
	{
		size = (int)log10(a);
	}
	else
	{
		size = 1;
	}
    str = malloc(sizeof(char) * (size + 2));
    sprintf(str, "%zu", a);

    return str;
  }

char*
pelib_string_detail(size_t)(size_t a, int level)
{
	return pelib_string(size_t)(a);
}

size_t
pelib_fread(size_t)(size_t* buffer, FILE* stream)
{
  size_t total;
  size_t num;
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

// Now include the generic stack implementation
#define STACK_T size_t
#include <pelib/stack.c>

// Generic circular fifos
#define CFIFO_T size_t
#include <pelib/fifo.c>

