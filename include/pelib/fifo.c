/*
 * fifo.c
 *
 *  Created on: 26 Jan 2012
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#ifndef CFIFO_T
#error Using generic fifo without a type
#endif

// Aliases for static functions
#define state(elem) PELIB_CONCAT_3(cfifo_, elem, _state)
#define is_in_content(elem) PELIB_CONCAT_2(is_in_content_, elem)
#define continuous_read_length(elem) PELIB_CONCAT_2(continuous_read_length_, elem)
static size_t continuous_read_length(CFIFO_T)(cfifo_t(CFIFO_T) *fifo);
#define continuous_write_length(elem) PELIB_CONCAT_2(continuous_write_length_, elem)
static size_t continuous_write_length(CFIFO_T)(cfifo_t(CFIFO_T) *fifo);

#define CFIFO_EMPTY "[]"
#define CFIFO_SEPARATOR ":"
#define CFIFO_BEGIN "["
#define CFIFO_END "]"

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
#define debug_size_t(var) printf("[%s:%s:%d] %s = \"%zu\"\n", __FILE__, __FUNCTION__, __LINE__, #var, (size_t)(var)); fflush(NULL)
#else
#define debug(var)
#define debug_addr(var)
#define debug_int(var)
#define debug_size_t(var)
#endif

enum state
{
  NORMAL, REVERSE, EMPTY, FULL
};
typedef enum state state_t;

int
pelib_alloc_buffer(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T)* cfifo, size_t capacity)
{
	cfifo->buffer = (CFIFO_T*)malloc(sizeof(CFIFO_T) * capacity);
	cfifo->capacity = capacity;

	return 1;
}

cfifo_t(CFIFO_T)*
pelib_alloc_struct(cfifo_t(CFIFO_T))()
{
	cfifo_t(CFIFO_T) *fifo;
	fifo = (cfifo_t(CFIFO_T)*)malloc(sizeof(cfifo_t(CFIFO_T)));

	return fifo;
}

cfifo_t(CFIFO_T)*
pelib_alloc_collection(cfifo_t(CFIFO_T))(size_t size)
{
	cfifo_t(CFIFO_T) *fifo;
	fifo = pelib_alloc_struct(cfifo_t(CFIFO_T))();
	if(fifo != NULL)
	{
		if(pelib_alloc_buffer(cfifo_t(CFIFO_T))(fifo, size) == 0)
		{
			pelib_free_struct(cfifo_t(CFIFO_T))(fifo);
			return NULL;
		}
	}
		

	return fifo;
}

int
pelib_init(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T)* fifo)
{
	fifo->last_op = PELIB_CFIFO_POP;
	fifo->read = 0;
	fifo->write = 0;

	return 1;
}

int
pelib_free(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T)* fifo)
{
	pelib_free_buffer(cfifo_t(CFIFO_T))(fifo);
	return pelib_free_struct(cfifo_t(CFIFO_T))(fifo);
}

int
pelib_free_struct(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T)* fifo)
{
	free((void*)fifo->buffer);

	return 1;
}

int
pelib_free_buffer(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T)* fifo)
{
	free(fifo);
	return 1;
}

FILE*
pelib_printf(cfifo_t(CFIFO_T))(FILE* stream, cfifo_t(CFIFO_T) fifo)
{
	char* str;
	str = pelib_string(cfifo_t(CFIFO_T))(fifo);
	printf("%s\n", str);
	free(str);

	return stream;
}

FILE*
pelib_printf_detail(cfifo_t(CFIFO_T))(FILE* stream, cfifo_t(CFIFO_T) fifo, int level)
{
	char* str;
	str = pelib_string_detail(cfifo_t(CFIFO_T))(fifo, level);
	printf("%s\n", str);
	free(str);

	return stream;
}

static state_t
state(CFIFO_T)(cfifo_t(CFIFO_T) *cfifo)
{
/*
	debug_size_t((size_t)cfifo->read);
	debug_size_t((size_t)cfifo->write);
	debug_size_t((size_t)cfifo->last_op);
*/
    if (cfifo->read == cfifo->write)
      {
        if (cfifo->last_op == PELIB_CFIFO_POP)
          {
            return EMPTY;
          }
        else if (cfifo->last_op == PELIB_CFIFO_PUSH)
          {
            return FULL;
          }
        else
          {
            fprintf(stdout, "[ERROR:%s:%s:%i] Illegal state at fifo %p\n", __FILE__, __FUNCTION__, __LINE__, cfifo);
            return EMPTY;
          }
      }
    else if (cfifo->read < cfifo->write)
      {
        return NORMAL;
      }
    else
      {
        return REVERSE;
      }
  }

// returns 1 if index is in data area
// returns 3 if index is in position of read index
// returns 7 if index is in both read and write position, and in data area (fifo full)
// returns -7 if index is in both read and write position not in data area (fifo empty)
// returns -1 if index is not in data area
// returns -5 if index is in position of write index but not in data area
// ERRORS:
// returns -2 if index is in position of read index but not read, but is not in data area
// returns 5 if index is in position of write index but not read, but still is in data area
// returns 6 if in both read and write indexes but cannot determine if in data area or not
// returns 0 in case of other errors
/**
 * Returns an integer whose bits represent (if set):
 * 0: always set to 1
 * 1: index in position of read index
 * 2: index in position of write index
 * value is made negative if index is not in data area
 */
static int
is_in_content(CFIFO_T)(cfifo_t(CFIFO_T) *fifo, size_t index)
  {
    int res;
    size_t read;
    state_t state;

    read = fifo->read;
    state = state(CFIFO_T)(fifo);

    res = 1;
    res += index == read ? 2 : 0;
    res += index == fifo->write ? 4 : 0;

    switch (state)
      {
        case NORMAL:
        if (index < read || index >= fifo->write)
          {
            res = -1 * res;
          }
        break;
        case REVERSE:
        if (index >= fifo->write && index < read)
          {
            res = -1 * res;
          }
        break;
        case EMPTY:
        res = -1 * res;
        break;
        case FULL:
        // Do nothing
        break;
        default:
        fprintf(stderr, "[ERROR:%s:%s:%i] Illegal cfifo state", __FILE__, __FUNCTION__, __LINE__);
        return 0;
        break;
      }

    return res;
  }

char*
pelib_string(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T) fifo)
  {
    return pelib_string_detail(cfifo_t(CFIFO_T))(fifo, 0);
  }

char*
pelib_string_detail(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T) fifo, int level)
{
  char *str, *elem;
  unsigned int i;
  int status;

  str = (char*)malloc(sizeof(char) * ((PELIB_FIFO_ELEM_MAX_CHAR + 1) * fifo.capacity)
      + 4);

  sprintf(str, "[");
  for (i = 0; i < fifo.capacity; i++)
  {
    status = is_in_content(CFIFO_T)(&fifo, i);
    if (abs(status) & 2)
    {
      sprintf(str, "%s>", str);
    }
    if (abs(status) & 4)
    {
      sprintf(str, "%s>", str);
    }
    if(status > 0)
    {
      elem = pelib_string_detail(CFIFO_T)(fifo.buffer[i], level);
      sprintf(str, "%s%s", str, elem);
      free(elem);
    }
    else
    {
      sprintf(str, "%s.", str);
    }
    if (i < fifo.capacity - 1)
    {
      sprintf(str, "%s:", str);
    }
  }
  sprintf(str, "%s]", str);

  return str;
}

int
pelib_cfifo_push(CFIFO_T)(cfifo_t(CFIFO_T)* fifo, CFIFO_T elem)
  {
    if (fifo->capacity > 0 && 
	state(CFIFO_T)(fifo) != FULL)
      {
        memcpy((void*)&(fifo->buffer[fifo->write]), &elem, sizeof(CFIFO_T));

        fifo->write = (fifo->write + 1) % fifo->capacity;
        fifo->last_op = PELIB_CFIFO_PUSH;

        return 1;
      }
    else
      {
        return 0;
      }
  }

size_t
pelib_cfifo_fill(CFIFO_T)(cfifo_t(CFIFO_T)* fifo, size_t num)
{
	size_t length = pelib_cfifo_capacity(CFIFO_T)(fifo) - pelib_cfifo_length(CFIFO_T)(fifo);
	num = length < num ? length : num;

	fifo->write = (fifo->write + num) % fifo->capacity;

	if(num > 0)
	{
		fifo->last_op = PELIB_CFIFO_PUSH;
	}

	return num;
}

CFIFO_T*
pelib_cfifo_peekaddr(CFIFO_T)(cfifo_t(CFIFO_T)* fifo, size_t offset, size_t *num, CFIFO_T **addr)
  {
    if ((fifo->capacity > 0 && offset < pelib_cfifo_length(CFIFO_T)(fifo) && state(CFIFO_T)(fifo) != EMPTY))
      {
	size_t avail = continuous_read_length(CFIFO_T)(fifo);
	if(num != NULL)
	{
		*num = avail;
	}
	if(addr != NULL)
	{
 		if((fifo->read + offset + avail) % fifo->capacity != fifo->write)
		{
			*addr = (CFIFO_T*)&(fifo->buffer[(fifo->read + offset + avail) % fifo->capacity]);
		}
		else
		{
			*addr = NULL;
		}
	}

/*
	debug_size_t(fifo->read);
	debug_size_t(offset);
	debug_size_t(fifo->read + offset);
	debug_size_t((fifo->read + offset) % fifo->capacity);
        debug_size_t(fifo->buffer);
        debug_size_t(&(fifo->buffer[(fifo->read + offset) % fifo->capacity]));
*/
        return (CFIFO_T*)&(fifo->buffer[(fifo->read + offset) % fifo->capacity]);
      }
    else
      {
	if(num != NULL)
	{
		*num = 0;
	}
        return NULL;
      }
  }

CFIFO_T*
pelib_cfifo_writeaddr(CFIFO_T)(cfifo_t(CFIFO_T)* fifo, size_t *num, CFIFO_T **addr)
  {
    if ((fifo->capacity - pelib_cfifo_length(CFIFO_T)(fifo) > 0) || 1)
      {
	size_t avail = continuous_write_length(CFIFO_T)(fifo);
	if(num != NULL)
	{
		*num = avail;
	}
	if(addr != NULL)
	{
 		if((fifo->write + avail) % fifo->capacity != fifo->read)
		{
			*addr = (CFIFO_T*)&(fifo->buffer[(fifo->write + avail) % fifo->capacity]);
		}
		else
		{
			*addr = NULL;
		}
	}

        return (CFIFO_T*)&(fifo->buffer[fifo->write]);
      }
    else
      {
        return NULL;
      }
  }

CFIFO_T
pelib_cfifo_pop(CFIFO_T)(cfifo_t(CFIFO_T)* fifo)
  {
    CFIFO_T *ptr,  res;

    ptr = pelib_cfifo_peekaddr(CFIFO_T)(fifo, 0, NULL, NULL);
    if (ptr != NULL)
      {
        fifo->read = (fifo->read + 1) % fifo->capacity;
        fifo->last_op = PELIB_CFIFO_POP;

	res = *ptr;

        return res;
      }
    else
      {
	CFIFO_T def;
	pelib_init(CFIFO_T)(&def);
	return def;
      }
  }

size_t
pelib_cfifo_discard(CFIFO_T)(cfifo_t(CFIFO_T) *fifo, size_t num)
{
	int length = pelib_cfifo_length(CFIFO_T)(fifo);
	num = num < (size_t) length ? num : (size_t)length;

	fifo->read = (fifo->read + num) % fifo->capacity;

	if(num > 0)
	{
		fifo->last_op = PELIB_CFIFO_POP;
	}

	return num;
}

CFIFO_T
pelib_cfifo_peek(CFIFO_T)(cfifo_t(CFIFO_T)* cfifo, size_t offset)
  {
    CFIFO_T *ptr, res;
    ptr = pelib_cfifo_peekaddr(CFIFO_T)(cfifo, offset, NULL, NULL);
    if (ptr != NULL)
      {
	res = *ptr;
        return res;
      }
    else
      {
	CFIFO_T def;
	pelib_init(CFIFO_T)(&def);
	return def;
      }
  }

int
pelib_cfifo_is_full(CFIFO_T)(cfifo_t(CFIFO_T)* cfifo)
  {
    return state(CFIFO_T)(cfifo) == FULL;
  }

int
pelib_cfifo_is_empty(CFIFO_T)(cfifo_t(CFIFO_T)* cfifo)
  {
    return state(CFIFO_T)(cfifo) == EMPTY;
  }

size_t
pelib_cfifo_pushmem(CFIFO_T)(cfifo_t(CFIFO_T) *fifo, CFIFO_T* mem, size_t num)
{
	size_t left, length, pushed;
	switch(state(CFIFO_T)(fifo))
	{
		case EMPTY:
		case NORMAL:			
			left = fifo->capacity - fifo->write;
			length = left < num ? left : num;

			memcpy((void*)&fifo->buffer[fifo->write], mem, length * sizeof(CFIFO_T));

			// Record amount of elements pushed
			pushed = length;
			// Update fifo's write pointer
      			fifo->write = (fifo->write + length) % fifo->capacity;

			if(pushed > 0)
			{
				fifo->last_op = PELIB_CFIFO_PUSH;
			}
			
			// If left was not enough to accomodate the memory buffer to write
			if(left < num)			
			{
				// Now we are in the reverse mode, just call the function again
				pushed += pelib_cfifo_pushmem(CFIFO_T)(fifo, mem + length, num - length);
			}

			return pushed;
		break;
		
		case FULL:
		case REVERSE:
			// Copy from write to read
			left = fifo->read - fifo->write;
			length = left < num ? left : num;
			memcpy((void*)&(fifo->buffer[fifo->write]), mem, length * sizeof(CFIFO_T));

			fifo->write += length;
			pushed = length;

			if(pushed > 0)
			{
				fifo->last_op = PELIB_CFIFO_PUSH;
			}

			return pushed;
		break;

		default:
			return 0;
		break;
	}
}

size_t
pelib_cfifo_popmem(CFIFO_T)(cfifo_t(CFIFO_T) *fifo, CFIFO_T* mem, size_t num)
{
	size_t i;
	size_t left, length, popped;
	volatile int* ptr;
	size_t intlength;

	switch(state(CFIFO_T)(fifo))
	{
		case FULL:
		case REVERSE:
			// Copy from write to end of buffer
			left = fifo->capacity - fifo->read;
			length = left < num ? left : num;

                        memcpy(mem, (void*)&(fifo->buffer[fifo->read]), length * sizeof(CFIFO_T));
                        
                        intlength = (length * sizeof(CFIFO_T)) / sizeof(int) ;
                        ptr = (volatile int*) (&fifo->buffer[fifo->read]);

			// Record amount of elements pushed
			popped = length;

			// Update fifo's write pointer
			fifo->read = (fifo->read + length) % fifo->capacity;

			if(popped > 0)
			{
				fifo->last_op = PELIB_CFIFO_POP;
			}
			
			// If left was not enough to accomodate the memory buffer to write
			if(left < num)
			{
				// Now we are in the reverse mode, just call the function again
				popped += pelib_cfifo_popmem(CFIFO_T)(fifo, mem + length, num - length);
			}

			return popped;
		break;
		
		case EMPTY:
		case NORMAL:
			// Copy from write to read
			left = fifo->write - fifo->read;
			length = left < num ? left : num;
			memcpy(mem, (void*)&(fifo->buffer[fifo->read]), length * sizeof(CFIFO_T));
                        
                        intlength = (length * sizeof(CFIFO_T)) / sizeof(int) ;

                        ptr = (volatile int*) (&fifo->buffer[fifo->read]);
			fifo->read += length;
			popped = length;

			if(popped > 0)
			{
				fifo->last_op = PELIB_CFIFO_POP;
			}

			return popped;
		break;

		default:
			return 0;
		break;
	}
}

size_t
pelib_cfifo_peekmem(CFIFO_T)(cfifo_t(CFIFO_T)* fifo, CFIFO_T* mem, size_t num, size_t offset)
{
	size_t left, length;
	cfifo_t(CFIFO_T) copy = *fifo;
	copy.read = (copy.read + offset) % copy.capacity;

	switch(state(CFIFO_T)(&copy))
	{
		case FULL:
		case REVERSE:
			// Copy from write to end of buffer
			left = copy.capacity - copy.read;
			length = left < num ? left : num;

			memcpy(mem, (void*)&(copy.buffer[copy.read]), length * sizeof(CFIFO_T));

			// If left was not enough to accomodate the memory buffer to write
			if((left) < num && left > 0)
			{
				// Now we are in the reverse mode, just call the function again
				length += pelib_cfifo_peekmem(CFIFO_T)(&copy, mem + length, num - length, length);
			}

			return length;
		break;
		
		case EMPTY:
		case NORMAL:
			// Copy from write to read
			left = copy.write - copy.read;
			length = left < num ? left : num;

			memcpy(mem, (void*)&(copy.buffer[copy.read]), length * sizeof(CFIFO_T));

			return length;
		break;

		default:
			return 0;
		break;
	}
}

static
size_t
continuous_read_length(CFIFO_T)(cfifo_t(CFIFO_T)* fifo)
{
	switch(state(CFIFO_T)(fifo))
	{
		case FULL:
		case REVERSE:
			return fifo->capacity - fifo->read;
		break;
		
		case EMPTY:
			return 0;
		break;
		case NORMAL:
			return fifo->write - fifo->read;
		break;

		default:
			return -1;
		break;
	}
}

static
size_t
continuous_write_length(CFIFO_T)(cfifo_t(CFIFO_T)* fifo)
{
	switch(state(CFIFO_T)(fifo))
	{
		case FULL:
			return 0;
		break;
		case REVERSE:
			return fifo->read - fifo->write;
		break;
		
		case EMPTY:
		case NORMAL:
			return fifo->capacity - fifo->write;
		break;

		default:
			return -1;
		break;
	}
}

size_t
pelib_cfifo_popfifo(CFIFO_T)(cfifo_t(CFIFO_T)* src, cfifo_t(CFIFO_T)* tgt, size_t num)
{
	size_t requested = num;

	while(num > 0 && !pelib_cfifo_is_empty(CFIFO_T)(src) && !pelib_cfifo_is_full(CFIFO_T)(tgt))
	{
		size_t readl = continuous_read_length(CFIFO_T)(src);
		volatile size_t writel = continuous_write_length(CFIFO_T)(tgt);
		size_t length = readl < writel ? readl : writel;
		length = num < length ? num : length;
		memcpy((void*)&(tgt->buffer[tgt->write]), (void*)&(src->buffer[src->read]), length * sizeof(CFIFO_T));
		num -= length;
		pelib_cfifo_discard(CFIFO_T)(src, length);
		pelib_cfifo_fill(CFIFO_T)(tgt, length);
	}

	return requested - num;
}

size_t
pelib_cfifo_length(CFIFO_T)(cfifo_t(CFIFO_T) *cfifo)
{
	switch(state(CFIFO_T)(cfifo))
	{
		case EMPTY:
			return 0;
		break;
		case NORMAL:
			return cfifo->write - cfifo->read;
		break;
		case REVERSE:
			return cfifo->capacity - (cfifo->read - cfifo->write);
		break;
		case FULL:
			return cfifo->capacity;
		break;
	}

	return 0;
}

CFIFO_T
pelib_cfifo_last(CFIFO_T)(cfifo_t(CFIFO_T) *cfifo)
{	
	size_t length = pelib_cfifo_length(CFIFO_T)(cfifo);

	if(pelib_cfifo_length(CFIFO_T)(cfifo) <= 0)
	{  
		CFIFO_T def;
		pelib_init(CFIFO_T)(&def);
		return def;
	}
	else
	{
		CFIFO_T* ptr = pelib_cfifo_peekaddr(CFIFO_T)(cfifo, length - 1, NULL, NULL);
		return *ptr;
	}
}

size_t
pelib_cfifo_capacity(CFIFO_T)(cfifo_t(CFIFO_T)* cfifo)
{
	return cfifo->capacity;
}

