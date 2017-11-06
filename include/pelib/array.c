/*
 * array.c
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

#ifndef ARRAY_T
#error Using generic array without a type
#endif

#if 0
#define read_value(elem) PELIB_CONCAT_2(read_value_, elem)
static int
read_value(FILE * file, ARRAY_T* buffer)
{
  assert(file != NULL);
  int total;
//ARRAY_T num;
//  ARRAY_T read, has_more;

//  read = 0;
//  has_more = 1;
//  num = 0;
  total = 0;

  //while (read != ' ')
//    {
//      if ((has_more = pelib_fread(ARRAY_T)(&read, 1, file)) == 0 || read == ' ')
	total = pelib_fread(ARRAY_T)(buffer, 1, file);
//        {
//          break;
//        }

//      total += has_more;
// TODO: deport to integer.c and fix this method to become generic (fread reads one element from input stream)
//      num = num * 10 + read - '0';
//    }

//  *buffer = num;
  return total;
}
#endif

array_t(ARRAY_T)*
pelib_alloc_struct(array_t(ARRAY_T))()
  {
    array_t(ARRAY_T)* array;

    array = malloc(sizeof(array_t(ARRAY_T)));
    assert(array != NULL);

    array->length = 0;
    array->capacity = 0;

    return array;
  }

int
pelib_alloc_buffer(array_t(ARRAY_T))(array_t(ARRAY_T)* array, size_t size)
  {
    array->capacity = size;

    if(array->capacity > 0)
    {
      array->data = calloc(array->capacity, sizeof(ARRAY_T));
      if(array->data == NULL)
      {
        return 0;
      }
      assert(array->data != NULL && size > 0 || array->data == NULL && size <= 0);
    }

    return 1;
  }

int
pelib_set_buffer(array_t(ARRAY_T))(array_t(ARRAY_T)* array, void *buffer, size_t size)
{
	array->data = buffer;
	array->capacity = size;

	return 1;
}

array_t(ARRAY_T)*
pelib_alloc_collection(array_t(ARRAY_T))(size_t n)
  {
    array_t(ARRAY_T) *array;
    array = pelib_alloc_struct(array_t(ARRAY_T))();
    if(array != NULL)
    {
      if(pelib_alloc_buffer(array_t(ARRAY_T))(array, n) == 0)
      {
        pelib_free_struct(array_t(ARRAY_T))(array);        
        return NULL;
      }
    }

    return array;
  }

array_t(ARRAY_T)*
pelib_alloc_from(array_t(ARRAY_T))(void* buffer, size_t size)
{
    array_t(ARRAY_T) *array;
    array = pelib_alloc_struct(array_t(ARRAY_T))();
    if(array != NULL)
    {
      pelib_set_buffer(array_t(ARRAY_T))(array, buffer, size);
      return array;
    }

   return NULL;
}

int
pelib_init(array_t(ARRAY_T))(array_t(ARRAY_T)* array)
{
	array->length = 0;
	return 1;
}

int
pelib_copy(array_t(ARRAY_T))(array_t(ARRAY_T) src, array_t(ARRAY_T)* dst)
{
	size_t i;

	*dst = src;
	dst->data = malloc(sizeof(ARRAY_T) * src.capacity);
	
	for(i = 0; i < pelib_array_length(ARRAY_T)(&src); i++)
	{
		pelib_copy(ARRAY_T)(src.data[i], &dst->data[i]);
	}

	return 0;
}

array_t(ARRAY_T)*
pelib_array_loadfilename(ARRAY_T)(char * filename)
{
  size_t i;
  FILE * h;
  size_t size;
  ARRAY_T num;
  array_t(ARRAY_T)* res;

  pelib_init(ARRAY_T)(&num);
  h = fopen(filename, "r");
  if(h == NULL)
  {
    return NULL;
  }

  int ret = pelib_fread(size_t)(&size, h);
  if(ret != 1)
  {
    return NULL;
  }

  res = pelib_alloc_collection(array_t(ARRAY_T))(size);

  for (i = 0; i < size; i++)
    {
      pelib_fread(ARRAY_T)(&num, h);
      pelib_array_append(ARRAY_T)(res, num);
    }

  fclose(h);

  return res;
}

#define min(elem) PELIB_CONCAT_2(min_, elem)
static int
min(ARRAY_T)(int a, int b)
{
  if (a < b)
    {
      return a;
    }
  else
    {
      return b;
    }
}

array_t(ARRAY_T)*
pelib_array_loadfilenamebinary(ARRAY_T)(char *filename)
{
  return pelib_array_loadfilenamewindowbinary(ARRAY_T)(filename, 0, 0);
}

array_t(ARRAY_T)*
pelib_array_preloadfilenamebinary(ARRAY_T)(char *filename)
{
  array_t(ARRAY_T)* array;
  FILE *h;
  size_t ret;
  int numel;
  size_t size;

  h = fopen(filename, "r");
  if(h == NULL)
  {
    return NULL;
  }

  ret = fread(&numel, sizeof(int), 1, h);
  size = (size_t)numel;
  if(ret != 1)
  {
    return NULL;
  }
  fclose(h);
  if(ret != 1)
  {
    return NULL;
  }
  assert(ret == 1);

  array = malloc(sizeof(array_t(ARRAY_T)));
  array->length = size;
  array->capacity = 0;
  array->data = NULL;

  return array;
}

/// Reads length value_t elements from filename, and skip the offset first elements.
/// \param filename: File to open and read values from
/// \param offset: number of elements of value_t to skip before reading the values
/// \param length: number of elements to read. If length < 0, then reads the remaining number of elements available in the file
/// \return: an array of capacity of length, or the number or elements stored in the array file if length below 0. If not enough elements could be skipped, then returns NULL
array_t(ARRAY_T)*
pelib_array_loadfilenamewindowbinary(ARRAY_T)(char *filename, size_t offset, size_t length)
{
  array_t(ARRAY_T)* array;
  FILE * h;
  size_t ret;
  int numel;
  size_t size;

  h = fopen(filename, "r");
  if(h == NULL)
  {
    return NULL;
  }

  ret = fread(&numel, sizeof(int), 1, h);
  size = (size_t)numel;
  if(ret != 1)
  {
    return NULL;
  }

  if (ret == 1 && (ptrdiff_t)size - (ptrdiff_t)offset > 0)
    {
      fseek(h, sizeof(ARRAY_T) * (offset + 1), SEEK_SET);

      if(length > 0)
      {
        array = pelib_alloc_collection(array_t(ARRAY_T))(min(ARRAY_T)(size - offset, length));
        ret = fread(array->data, sizeof(ARRAY_T), length, h);
      }
      else
      {
        array = pelib_alloc_collection(array_t(ARRAY_T))(min(ARRAY_T)(size - offset, size - offset));
        ret = fread(array->data, sizeof(ARRAY_T), size - offset, h);
      }

      array->length = ret;
      fclose(h);

      return array;
    }
  else
    {
      return NULL;
    }
}

#define array_length_debug printf("[PELIB:%s:%s:%d] i = %d\n", __FILE__, __FUNCTION__, __LINE__, i);
#define array_length_pre_debug printf("[PELIB:%s:%s:%d] length = %d\n", __FILE__, __FUNCTION__, __LINE__, pelib_array_length(ARRAY_T)(&array));
char*
pelib_string(array_t(ARRAY_T))(array_t(ARRAY_T) array)
{
	char *str, *grow, *elem;
	size_t i;
	str = malloc(3 * sizeof(char));
	sprintf(str, "[");
	
	for(i = 0; i < pelib_array_length(ARRAY_T)(&array) - 1; i++)
	{
//		if(i % 1000 == 0 || i == pelib_array_length(ARRAY_T)(&array) - 1 - 1)
//		{
//			array_length_debug
//		}
		elem = pelib_string(ARRAY_T)(pelib_array_read(ARRAY_T)(&array, i));
		grow = malloc((strlen(str) + 1) * sizeof(char));
		sprintf(grow, "%s", str);

		free(str);
		str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
		sprintf(str, "%s%s:", grow, elem);
		free(elem);
		free(grow);
//		if(i % 1000 == 0 || i == pelib_array_length(ARRAY_T)(&array) - 1 - 1)
//		{
//			array_length_debug
//		}
	}
	for(; i < pelib_array_length(ARRAY_T)(&array); i++)
	{
//		if(i % 1000 == 0 || i == pelib_array_length(ARRAY_T)(&array) - 1)
//		{
//			array_length_debug
//		}
		elem = pelib_string(ARRAY_T)(pelib_array_read(ARRAY_T)(&array, i));
		grow = malloc((strlen(str) + 1) * sizeof(char));
		sprintf(grow, "%s", str);

		free(str);
		str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
		sprintf(str, "%s%s", grow, elem);
		free(elem);
		free(grow);
//		if(i % 1000 == 0 || i == pelib_array_length(ARRAY_T)(&array) - 1 - 1)
//		{
//			array_length_debug
//		}
	}

	grow = malloc((strlen(str) + 1) * sizeof(char));
	sprintf(grow, "%s", str);
	sprintf(str, "%s]", grow);
	free(grow);

	return str;
}

char*
pelib_string_detail(array_t(ARRAY_T))(array_t(ARRAY_T) array, int level)
{
	if(level == 0)
	{
		char *str = malloc(sizeof(char) * ((array.length > 0 ? floor(log10(array.length)) + 1 : 1) + 1));
		sprintf(str, "%zu", array.length);
		return str;
	}
	else
	{
		// Bring it on
		char *str, *grow, *elem;
		size_t i;
		str = malloc(3 * sizeof(char));
		sprintf(str, "[");
		
		for(i = 0; i < pelib_array_length(ARRAY_T)(&array) - 1; i++)
		{
			elem = pelib_string_detail(ARRAY_T)(pelib_array_read(ARRAY_T)(&array, i), level);
			grow = malloc((strlen(str) + 1) * sizeof(char));
			sprintf(grow, "%s", str);

			free(str);
			str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
			sprintf(str, "%s%s:", grow, elem);
			free(elem);
			free(grow);
		}
		for(; i < pelib_array_length(ARRAY_T)(&array); i++)
		{
			elem = pelib_string_detail(ARRAY_T)(pelib_array_read(ARRAY_T)(&array, i), level);
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
pelib_printf(array_t(ARRAY_T))(FILE* stream, array_t(ARRAY_T) array)
{
	char * str;
	str = pelib_string(array_t(ARRAY_T))(array);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

FILE*
pelib_printf_detail(array_t(ARRAY_T))(FILE* stream, array_t(ARRAY_T) array, int level)
{
	char * str;
	str = pelib_string_detail(array_t(ARRAY_T))(array, level);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

int
pelib_array_storefilename(ARRAY_T)(array_t(ARRAY_T)* array, char* filename)
{
  size_t i;
  FILE * h;

  h = fopen(filename, "w");

  fprintf(h, "%zu ", array->length);
  for (i = 0; i < array->length; i++)
    {
      fprintf(h, "%s ", pelib_string(ARRAY_T)(array->data[i]));
    }

  return -1;
}

int
pelib_array_storefilenamebinary(ARRAY_T)(array_t(ARRAY_T)* array, char* filename)
{
  FILE * h;
  size_t ret;

  h = fopen(filename, "w");

  ret = fwrite(&array->length, sizeof(int), 1, h);
  assert(ret == 1);

  ret += fwrite(array->data, sizeof(ARRAY_T), array->length, h);
  assert(ret == array->length + 1);

  fclose(h);

  return ret == array->length + 1;
}

int
pelib_array_checkascending(ARRAY_T)(array_t(ARRAY_T)* array)
{
  size_t i;

  for (i = 0; i < array->length - 1; i++)
    {
      assert(pelib_compare(ARRAY_T)(array->data[i], array->data[i + 1]) >= 0);
    }

  return array->length;
}

int
pelib_free_buffer(array_t(ARRAY_T))(array_t(ARRAY_T)* array)
{
  if(array->capacity > 0)
  {
    free(array->data);
  }
  array->data = NULL;

  return 0;
}

int
pelib_free(array_t(ARRAY_T))(array_t(ARRAY_T)* array)
{
  pelib_free_buffer(array_t(ARRAY_T))(array);
  return pelib_free_struct(array_t(ARRAY_T))(array);
}

int
pelib_free_struct(array_t(ARRAY_T))(array_t(ARRAY_T)* array)
{
  free(array);

  return 0;
}

ARRAY_T
pelib_array_read(ARRAY_T)(array_t(ARRAY_T)*array, size_t pos)
{
  ARRAY_T ret;
  pelib_init(ARRAY_T)(&ret);
//printf("[%s:%d] Hello world!\n", __FILE__, __LINE__);
  if (array->capacity > pos)
    {
//printf("[%s:%d] Hello world!\n", __FILE__, __LINE__);
      ret = array->data[pos];
//printf("[%s:%d] Hello world!\n", __FILE__, __LINE__);
	return ret;
    }
  else
    {
//printf("[%s:%d] Hello world!\n", __FILE__, __LINE__);
      return ret;
    }
}

int
pelib_array_write(ARRAY_T)(array_t(ARRAY_T)* array, size_t pos, ARRAY_T val)
{
  if (array->capacity > pos)
    {
      pelib_copy(ARRAY_T)(val, &array->data[pos]);

      return 1;
    }
  else
    {
      return 0;
    }
}

size_t
pelib_array_length(ARRAY_T)(array_t(ARRAY_T)* array)
{
  size_t l;

//fprintf(stderr, "[%s:%d] Hello world!\n", __FILE__, __LINE__);
  l = array->length;
  
//fprintf(stderr, "[%s:%d] Hello world!\n", __FILE__, __LINE__);
  return l;
}

size_t
pelib_array_capacity(ARRAY_T)(array_t(ARRAY_T)* array)
{
  return array->capacity;
}

int
pelib_array_append(ARRAY_T)(array_t(ARRAY_T)* array, ARRAY_T val)
{
  if (array->length < array->capacity)
    {
      pelib_copy(ARRAY_T)(val, &array->data[array->length]);
      array->length++;

      return 1;
    }
  else
    {
      return 0;
    }
}

int
pelib_array_compare(ARRAY_T)(array_t(ARRAY_T)* a1, array_t(ARRAY_T)* a2)
{
  if (pelib_array_length(ARRAY_T)(a1) == pelib_array_length(ARRAY_T)(a2))
    {
      size_t i;

      i = 0;
      while (pelib_compare(ARRAY_T)(pelib_array_read(ARRAY_T)(a1, i),
          pelib_array_read(ARRAY_T)(a2, i)) == 0 && i < pelib_array_length(ARRAY_T)(a1))
        {
          i++;
        }

      return pelib_compare(ARRAY_T)(pelib_array_read(ARRAY_T)(a1, i),
          pelib_array_read(ARRAY_T)(a2, i));
    }
  else
    {
      if (pelib_array_length(ARRAY_T)(a1) < pelib_array_length(ARRAY_T)(a2))
        {
          return -1;
        }
      else
        {
          return 1;
        }
    }
}
