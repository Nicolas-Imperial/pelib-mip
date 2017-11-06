/*
 * test_array.c
 *
 *  Created on: 13 Jan 2012
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
#include <stdio.h>

#include <pelib/unit.h>
#include <pelib/integer.h>

#define LENGTH 8
#define FILENAME_BINARY   "array_binary.bin"
#define VALUE   10
#define SKIP    6

array_t(int) *array;
int value;

void
init()
{
  unsigned int i;
  array = pelib_alloc_collection(array_t(int))(LENGTH);
  value = VALUE;

  for (i = 0; i < LENGTH; i++)
    {
      pelib_array_append(int)(array, value);
    }
}

void
cleanup()
{
  pelib_free(array_t(int))(array);
}

void setup() {};
void teardown() {};

static void
test_compare()
{
  unsigned int i;
  array_t(int)* another_array;
  another_array = pelib_alloc_collection(array_t(int))(LENGTH);

  for (i = 0; i < LENGTH; i++)
    {
      pelib_array_append(int)(another_array, value);
    }

  assert(pelib_array_compare(int)(array, another_array) == 0);
}

static void
test_store_load_binary()
{
  array_t(int)* another_array;
  pelib_array_storefilenamebinary(int)(array, FILENAME_BINARY);
  another_array = pelib_array_loadfilenamebinary(int)(FILENAME_BINARY);

  assert(pelib_array_compare(int)(array, another_array) == 0);

  pelib_free(array_t(int))(another_array);
}

static void
test_store_load_binary_skip()
{
  array_t(int)* another_array;
  pelib_array_storefilenamebinary(int)(array, FILENAME_BINARY);
  another_array = pelib_array_loadfilenamewindowbinary(int)(FILENAME_BINARY, SKIP,
      0);

  assert(pelib_array_length(int)(another_array) == LENGTH - SKIP);
  assert(pelib_array_capacity(int)(another_array) == LENGTH - SKIP);

  pelib_free(array_t(int))(another_array);
}

static void
test_store_load_binary_skip_too_much()
{
  array_t(int)* another_array;
  pelib_array_storefilenamebinary(int)(array, FILENAME_BINARY);
  another_array = pelib_array_loadfilenamewindowbinary(int)(FILENAME_BINARY, LENGTH
      + SKIP, 0);

  assert(another_array == NULL);
}

static void
test_store_load_binary_too_much()
{
  array_t(int)* another_array;
  pelib_array_storefilenamebinary(int)(array, FILENAME_BINARY);
  another_array = pelib_array_loadfilenamewindowbinary(int)(FILENAME_BINARY, LENGTH
      / 2, LENGTH);

  assert(pelib_array_length(int)(another_array) == LENGTH / 2);
  assert(pelib_array_capacity(int)(another_array) == LENGTH / 2);

  pelib_free(array_t(int))(another_array);
}

static void
test_store_load_binary_all_but_skipped_too_much()
{
  array_t(int)* another_array;
  pelib_array_storefilenamebinary(int)(array, FILENAME_BINARY);
  another_array = pelib_array_loadfilenamewindowbinary(int)(FILENAME_BINARY, LENGTH
      / 2, 0);

  assert(pelib_array_length(int)(another_array) == LENGTH / 2);
  assert(pelib_array_capacity(int)(another_array) == LENGTH / 2);

  pelib_free(array_t(int))(another_array);
}

static void
test_preload()
{
  array_t(int)* another_array;
  another_array = pelib_array_preloadfilenamebinary(int)(FILENAME_BINARY);

  assert(pelib_array_length(int)(another_array) == LENGTH);
  assert(pelib_array_capacity(int)(another_array) == 0);

  pelib_free(array_t(int))(another_array);
}

void
run()
{
  test(test_compare);
  test(test_store_load_binary);
  test(test_store_load_binary_skip);
  test(test_store_load_binary_skip_too_much);
  test(test_store_load_binary_too_much);
  test(test_store_load_binary_all_but_skipped_too_much);
  test(test_preload);
}
