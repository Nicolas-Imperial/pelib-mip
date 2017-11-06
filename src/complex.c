/*
 * complex.c
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


#include <pelib/complex.h>

int
pelib_copy
( complex_t)(complex_t s1, complex_t * s2)
{
  s2->r = s1.r;
  s2->im = s1.im;

  return 1;
}

int
pelib_init
( complex_t)(complex_t *cplx)
{
  cplx->im = 0;
  cplx->r = 0;

  return 1;
}

int
pelib_compare
( complex_t)(complex_t a, complex_t b)
{
  if (a.r == b.r)
    {
      return a.im - b.im;
    }
  else
    {
      return a.r - b.r;
    }
}

FILE*
pelib_printf(complex_t)(FILE* stream, complex_t a)
{
	char *str = pelib_string(complex_t)(a);
	fprintf(stream, "%s\n", str);
	free(str);
	return stream;
}

char*
pelib_string(complex_t)(complex_t a)
  {
    size_t size;
    char* str;

    size = (int)(a.r > 0 ? log10(a.r) : 1) + (int)(a.im > 0 ? log10(a.im) : 1) + sizeof(char) * 3;
    str = malloc(sizeof(char) * (size + 1));
    sprintf(str, "(%i,%i)", a.r, a.im);

    return str;
  }

char*
pelib_string_detail(complex_t)(complex_t a, int level)
{
	return pelib_string(complex_t)(a);
}

size_t
pelib_fread(complex_t)(complex_t* buffer, FILE* stream)
{
  size_t total, total_both;
  int num;
  int read, has_more;
  complex_t ret;

  read = 0;
  has_more = 1;
  num = 0;
  total = 0;
  total_both = 0;

  // Read real part
  while (read != ' ')
    {
      if ((has_more = fread(&read, sizeof(char), 1, stream)) == 0 || read == ' ')
        {
          break;
        }

      total += has_more;
      num = num * 10 + read - '0';
    }
  ret.r = num;
  total_both = total;
  read = 0;

  // Read imaginary part
  // If cannot read imaginary part, then leave buffer untouched
  // And only return the number of characters read for the real part
  if (has_more && (has_more = fread(&read, sizeof(char), 1, stream)) != 0)
  {
    while (read != ' ')
    {
      if ((has_more = fread(&read, sizeof(char), 1, stream)) == 0 || read == ' ')
        {
          break;
        }

      total += has_more;
      num = num * 10 + read - '0';
    }

  ret.im = num;
  *buffer = ret;
  }

  return total_both;
}

// Now include the generic stack implementation
#define STACK_T complex_t
#include <pelib/stack.c>
#define ARRAY_T complex_t
#include <pelib/array.c>
#define CFIFO_T complex_t
#include <pelib/fifo.c>
