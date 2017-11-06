/*
 * sort.c
 *
 *  Created on: 25 Jan 2012
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
#include <assert.h>

#include <pelib/sort.h>

static int
swap(array_t(int)* array, int left, int right)
{
  int pivot;
  pivot = pelib_array_read(int)(array, left);
  pelib_array_write(int)(array, left, pelib_array_read(int)(array, right));
  pelib_array_write(int)(array, right, pivot);

  return 0;
}

struct qsort_param
{
  array_t
  (int)
  *array;
  qsort_tune_t tune;
};
typedef struct qsort_param qsort_param_t;

struct qsort_next_step
{
  char done;
  qsort_param_t left, right;
};
typedef struct qsort_next_step qsort_next_step_t;

struct qsort_bound
{
  int begin, end;
};
typedef struct qsort_bound qsort_bound_t;

// Generate stack for qsort_param_t structure
#define STRUCT_T qsort_bound_t
#include <pelib/structure.h>

#define STACK_T qsort_bound_t
#include <pelib/stack.h>

#define STACK_T qsort_bound_t
#include <pelib/stack.c>

int
pelib_copy
( qsort_bound_t)(qsort_bound_t s, qsort_bound_t *d)
{
  d->begin = s.begin;
  d->end = s.end;

  return 1;
}
int
pelib_init
( qsort_bound_t)(qsort_bound_t *d)
{
  d->begin = 0;
  d->end = 0;

  return 1;
}

static void
move(array_t(int)* array, int pos, int dest)
{
  int pivot;
  int i;

  assert(pos >= dest);

  pivot = pelib_array_read(int)(array, pos);
  for (i = pos; i > dest; i--)
    {
      pelib_array_write(int)(array, i, pelib_array_read(int)(array, i - 1));
    }
  pelib_array_write(int)(array, dest, pivot);
}

void
pelib_insertsort_window(array_t(int)* array, int start, int stop)
{
  int i, j;

  assert(stop >= start);

  i = 0;
  j = 0;

  for (i = start + 1; i < stop; i++)
    {
      j = i;
      while (pelib_compare(int)(pelib_array_read(int)(array, i),
          pelib_array_read(int)(array, j - 1)) < 0 && j > start)
        {
          j--;
        }
      move(array, i, j);
    }
}

void
pelib_insertsort(array_t(int)* array)
{
  pelib_insertsort_window(array, 0, pelib_array_length(int)(array));
}

int
pelib_sample(array_t(int)* array, int ratio, int start, int stop)
{
  double pivot;
  int i;
  int read;

  // Sample input to find a good pivot
  pivot = 0;
  if (stop - start >= ratio)
    {
      array_t(int)* sample = pelib_alloc_collection(array_t(int))(ratio);
      for (i = start; i < stop; i += (int)((float) (stop - start) / ratio))
        {
	  read = pelib_array_read(int)(array, (int)i);
          pelib_array_append(int)(sample, read);
        }

      for (i = 0; i < pelib_array_length(int)(sample); i++)
        {
          pivot += (double) pelib_array_read(int)(sample, i);
        }
      pivot /= (double) pelib_array_length(int)(sample);
      pelib_free(array_t(int))(sample);
    }
  else
    {
      pivot = 0;
      for (i = start; i < stop; i++)
        {
          pivot += (double) pelib_array_read(int)(array, i);

        }
      pivot /= (double) (stop - start);
    }

  return (int) pivot;
}

static qsort_next_step_t
quicksort_tune(qsort_param_t param)
{
  qsort_next_step_t next;
  int i, j;

#if 0
  if((param.tune.end > 16777216 && param.tune.begin < 16777216 && param.tune.end - param.tune.begin < param.tune.threshold) || param.tune.begin == 0 && param.tune.end == 100000000)
  {
  char s[30];
  struct tm tim;
  time_t now;
  
  now = time(NULL);
  tim = *(localtime(&now));
  strftime(s,30,"%b %d, %Y; %H:%M:%S",&tim);
  printf("[%s, %s] %d->%d\n", s, __FUNCTION__, param.tune.begin, param.tune.end);
  }
#endif

  if (param.tune.end - param.tune.begin > param.tune.threshold) // Threshold to be decided experimentally
    {
      int pivot, pv;
      pivot = pelib_sample(param.array, param.tune.num_sample,
          param.tune.begin, param.tune.end);

      next.done = 0;
      next.left = param;
      next.right = param;

      i = param.tune.begin;
      j = param.tune.end;
      pv = j;

      int read;

      while (i < j)
        {
	  read = pelib_array_read(int)(param.array, i);
          if (read < pivot)
            {
              i++;
            }
          else
            {
              // Alloc a j
              j--;
              swap(param.array, i, j);

              if (pelib_array_read(int)(param.array, j) == pivot)
                {
                  // Alloc a pv
                  pv--;
                  swap(param.array, j, pv);
                }
            }
        }
      next.left.tune.end = i;

      for (i = pv; i < param.tune.end; i++)
        {
          swap(param.array, j, pv);
          j++;
          pv++;
        }

      next.right.tune.begin = j;
      return next;
    }
  else
    {
      pelib_insertsort_window(param.array, param.tune.begin, param.tune.end);

      next.done = 1;

      return next;
    }
}

static qsort_next_step_t
quicksort_tune_recursive(qsort_param_t param)
{
  qsort_next_step_t next;
  int i, j;

#if 0
  if((param.tune.end > 16777216 && param.tune.begin < 16777216 && param.tune.end - param.tune.begin < param.tune.threshold) || param.tune.begin == 0 && param.tune.end == 100000000)
  {
  char s[30];
  struct tm tim;
  time_t now;
  
  now = time(NULL);
  tim = *(localtime(&now));
  strftime(s,30,"%b %d, %Y; %H:%M:%S",&tim);
  printf("[%s, %s] %d->%d\n", s, __FUNCTION__, param.tune.begin, param.tune.end);
  }
#endif

  if (param.tune.end - param.tune.begin > param.tune.threshold) // Threshold to be decided experimentally
    {
      int pivot, pv;
      pivot = pelib_sample(param.array, param.tune.num_sample,
          param.tune.begin, param.tune.end);

      next.done = 0;
      next.left = param;
      next.right = param;

      i = param.tune.begin;
      j = param.tune.end;
      pv = j;

      while (i < j)
        {
          if (pelib_array_read(int)(param.array, i) < pivot)
            {
              i++;
            }
          else
            {
              // Alloc a j
              j--;
              swap(param.array, i, j);

              if (pelib_array_read(int)(param.array, j) == pivot)
                {
                  // Alloc a pv
                  pv--;
                  swap(param.array, j, pv);
                }
            }
        }
      next.left.tune.end = i;

      for (i = pv; i < param.tune.end; i++)
        {
          swap(param.array, j, pv);
          j++;
          pv++;
        }

      next.right.tune.begin = j;

      quicksort_tune_recursive(next.left);
      quicksort_tune_recursive(next.right);
      next.done = 1;

      return next;
    }
  else
    {
      pelib_insertsort_window(param.array, param.tune.begin, param.tune.end);

      next.done = 1;

      return next;
    }
}

void
pelib_quicksort_tune(array_t(int)* array, qsort_tune_t p)
{
  qsort_next_step_t res;
  qsort_param_t param;
  qsort_bound_t start;
  stack_t(qsort_bound_t) * stack;

  stack = pelib_alloc_collection(stack_t(qsort_bound_t))(0);
  pelib_init(stack_t(qsort_bound_t))(stack);

  param.array = array;
  param.tune = p;

  start.begin = p.begin;
  start.end = p.end;
  pelib_stack_push(qsort_bound_t)(stack, start);

  res.done = 1;
  while (res.done == 0 || !pelib_stack_isempty(qsort_bound_t)(stack))
    {
      if (res.done == 1)
        {
        int ret = pelib_stack_pop(qsort_bound_t)(stack, &start);
          param.tune.begin = start.begin;
          param.tune.end = start.end;

          res = quicksort_tune(param);
        }
      else
        {
          start.begin = res.right.tune.begin;
          start.end = res.right.tune.end;
          pelib_stack_push(qsort_bound_t)(stack, start);

          start.begin = res.left.tune.begin;
          start.end = res.left.tune.end;
          pelib_stack_push(qsort_bound_t)(stack, start);

          res.done = 1;
        }
    }

  pelib_free(stack_t(qsort_bound_t))(stack);
}

void
pelib_quicksort_tune_recursive(array_t(int)* array, qsort_tune_t p)
{
  qsort_param_t param;

  param.array = array;
  param.tune = p;

  quicksort_tune_recursive(param);
}

void
pelib_quicksort_window(array_t(int)* array, int start, int stop)
{
  qsort_tune_t p;

  p.begin = start;
  p.end = stop;
  p.threshold = PELIB_QUICKSORT_THRESHOLD;
  p.num_sample = PELIB_QUICKSORT_NUM_SAMPLE;

  pelib_quicksort_tune(array, p);
}

void
pelib_quicksort(array_t(int)* array)
{
  qsort_tune_t p;

  p.begin = 0;
  p.end = pelib_array_length(int)(array);
  p.threshold = PELIB_QUICKSORT_THRESHOLD;
  p.num_sample = PELIB_QUICKSORT_NUM_SAMPLE;

  pelib_quicksort_tune(array, p);
}

int
pelib_mergesort(array_t(int)* array)
{
	if(pelib_array_length(int)(array) <= 1)
	{
		return 1;
	}
	else
	{
		return pelib_array_length(int)(array) / 2;
//		left = pelib_alloc_from(
	}
}


int
pelib_is_increasing(array_t(int)* array)
{
  int val, ref;
  int i, res;
  res = 1;

  if (pelib_array_length(int)(array) > 0)
    {
      val = pelib_array_read(int)(array, 0);
      for (i = 1; i < pelib_array_length(int)(array); i++)
        {
          ref = pelib_array_read(int)(array, i);
          res = res && (pelib_compare(int)(val, ref) <= 0);
          val = ref;
        }

      return res;
    }
  else
    {
      return 1;
    }
}
