/*
 * fifo_array.c
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

cfifo_t(CFIFO_ARRAY_T)*
pelib_cfifo_from_array(CFIFO_ARRAY_T)(array_t(CFIFO_ARRAY_T) *array)
  {

  cfifo_t(CFIFO_T) *fifo;
    fifo = pelib_alloc_struct(cfifo_t(CFIFO_ARRAY_T))();
    fifo->buffer = array->data;
    if(array->data == NULL) exit(81);
    fifo->capacity = pelib_array_capacity(CFIFO_ARRAY_T)(array);
    fifo->read = 0;   
    fifo->write = pelib_array_length(CFIFO_ARRAY_T)(array) % fifo->capacity; 

    if (pelib_array_length(CFIFO_ARRAY_T)(array) > 0)
      {
        fifo->last_op = PELIB_CFIFO_PUSH;
      }
    else
      {
        fifo->last_op = PELIB_CFIFO_POP;
      }

    pelib_free_struct(array_t(CFIFO_ARRAY_T))(array);

    return fifo;
  }

array_t(CFIFO_ARRAY_T)*
pelib_array_from_cfifo(CFIFO_ARRAY_T)(cfifo_t(CFIFO_ARRAY_T) *cfifo)
{
	array_t(CFIFO_T) *array;
	array = pelib_alloc_collection(array_t(CFIFO_ARRAY_T))(pelib_cfifo_length(CFIFO_ARRAY_T)(cfifo));
	pelib_init(array_t(CFIFO_ARRAY_T))(array);

	array->length = pelib_cfifo_popmem(CFIFO_ARRAY_T)(cfifo, array->data, pelib_array_capacity(CFIFO_ARRAY_T)(array));

	pelib_free(cfifo_t(CFIFO_ARRAY_T))(cfifo);

    return array;
  }
