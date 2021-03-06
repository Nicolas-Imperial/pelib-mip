/*
 * float.c
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

#include <pelib/float.h>

int
pelib_copy(float)(float s1, float * s2)
  {
    *s2 = s1;

    return 1;
  }

int
pelib_init(float)(float *val)
  {
    *val = 0;

    return 1;
  }

int
pelib_compare(float)(float a, float b)
  {
    return a - b;
  }

char*
pelib_string(float)(float a)
  {
    size_t size;
    char* str;

	if(a > 0)
	{
		size = (float)log10(a);
	}
	else
	{
		size = 1;
	}
    str = malloc(sizeof(char) * (size + 2));
    sprintf(str, "%f", a);

    return str;
  }

char*
pelib_string_detail(float)(float a, int level)
{
	return pelib_string(float)(a);
}

size_t
pelib_fread(float)(float* buffer, FILE* stream)
{
// TODO: adapt for flat number parsing
/*
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
*/
	fprintf(stderr, "[%s:%s:%d][ERROR] Not implemented. Aborting.\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

// Now include the generic array implementation
#define ARRAY_T float
#include "pelib/array.c"

// Now include the generic fifo implementation
#define CFIFO_T float
#include "pelib/fifo.c"

// Now include the generic fifo implementation
#define CFIFO_ARRAY_T float
#include "pelib/fifo_array.c"
