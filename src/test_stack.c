/*
 * stack_test.c
 *
 *  Created on: 18 Oct 2011
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
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

#include <pelib/unit.h>
#include <pelib/complex.h>

#define MAX_PUSH_POP 10
#define NB_THREADS 2

#define test_run(test)\
  printf("[%s:%s:%i] Running test '%s'... ", __FILE__, __FUNCTION__, __LINE__, #test);\
  test_setup();\
  if(test())\
  {\
    printf("passed\n");\
  }\
  else\
  {\
    printf("failed\n");\
  }\
  test_teardown();

struct thread_parallel_pushpop_arg
{
  complex_t value;
  pthread_mutex_t *lock;
  int *counter;
};
typedef struct thread_parallel_pushpop_arg thread_parallel_pushpop_arg_t;

struct thread_aba_arg
{
  int id;
  int* aba_detected;
  pthread_barrier_t step1, step2, step3, step4;
  stack_complex_t_t *pool;
  stackelem_complex_t_t *elem;
};
typedef struct thread_aba_arg thread_aba_arg_t;
stack_t(complex_t) *stack, *pool;
complex_t data;

#define DATA_REAL 10
#define DATA_IM 12
#define DATA_SIZE sizeof(complex_t)

void
setup()
{

}

void
init()
{
  stack = pelib_alloc_collection(stack_t(complex_t))(0);
  pelib_init(stack_t(complex_t))(stack);
  assert(pelib_stack_check(complex_t)(stack) == 1);

  pool = pelib_alloc_collection(stack_t(complex_t))(0);
  pelib_init(stack_t(complex_t))(pool);
  assert(pelib_stack_check(complex_t)(pool) == 1);

  data.im = DATA_IM;
  data.r = DATA_REAL;
}

void
cleanup()
{
  pelib_free
  (
  stack_t( complex_t))( stack);
  pelib_free
  (
  stack_t( complex_t))( pool);
}

void
teardown()
{

}

void
test_push()
{
  int res;

  // Check precondition
  res = (pelib_stack_check(complex_t)(stack) == 1);
  assert(res);

  // Test case
  pelib_stack_push
  ( complex_t)( stack, data);

  // Checking
  res &= (pelib_stack_check(complex_t)(stack) == 1);
  assert(res);

  res &= (memcmp(&stack->top->buffer, &data, DATA_SIZE) == 0);
  assert(res);

  res &= stack->top != NULL;
  assert(res);
}

void
test_peek()
{
  int res;
  complex_t out;

  // Test case
  pelib_stack_peek(complex_t)(stack, &out);

  // Checking
  res = (pelib_stack_check(complex_t)(stack) == 1);
  assert(res);

  res &= !(memcmp(&data, &out, DATA_SIZE) == 0);
  assert(res);
}

void
test_pop()
{
  int res;
  complex_t out;

  // Test case
  pelib_stack_pop(complex_t)(stack, &out);

  // Checking
  res = (pelib_stack_check(complex_t)(stack) == 1);
  assert(res);

  res = (stack->top == NULL);
  assert(res);

  res = !(memcmp(&data, &out, DATA_SIZE) == 0);
  assert(res);
}

void
test_push_safe()
{
  int res;

  // Check precondition
  res = (pelib_stack_check(complex_t)(stack) == 1);
  assert(res);

  // Test case
  pelib_stack_push_safe
  ( complex_t)( stack, data);

  // Checking
  res &= (pelib_stack_check(complex_t)(stack) == 1);
  assert(res);

  res &= (memcmp(&stack->top->buffer, &data, DATA_SIZE) == 0);
  assert(res);

  res &= stack->top != NULL;
  assert(res);
}

void
test_pop_safe()
{
  int res;
  complex_t out;

  // Test case
  pelib_stack_pop_safe(complex_t)(stack, &out);

  // Checking
  res = (pelib_stack_check(complex_t)(stack) == 1);
  assert(res);

  res &= (stack->top == NULL);
  assert(res);

  res &= !(memcmp(&data, &out, DATA_SIZE) == 0);
  assert(res);
}

#define debug_str(var) printf("[%s:%s:%d] %s = \"%s\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var);
#define debug_int(var) printf("[%s:%s:%d] %s = %d\n", __FILE__, __FUNCTION__, __LINE__, #var, var);
#define debug_size_t(var) printf("[%s:%s:%d] %s = %zu\n", __FILE__, __FUNCTION__, __LINE__, #var, var);

void
test_pushpop_safe_managed()
{
  int res;
  stackelem_complex_t_t elem;

  pelib_init(stackelem_t(complex_t))(&elem);

  // Check precondition
  res = (pelib_stack_check(complex_t)(stack) == 1);
  assert(res);

  res &= (pelib_stack_check(complex_t)(pool) == 1);
  assert(res);

  // Checking
  res &= pelib_stack_isempty(complex_t)(pool);
  assert(res);

  // Test case
  pelib_stack_push_safe_managed(complex_t)(stack, pool, data);
  pelib_stack_pop_safe_managed(complex_t)(stack, pool, &data);

  // Checking
  res &= !pelib_stack_isempty(complex_t)(pool);
  assert(res);

  res &= pool->top != &elem;
  assert(res);

  // Test case
  pelib_stack_push_elem(complex_t)(pool, &elem);

  // Checking
  res &= !pelib_stack_isempty(complex_t)(pool);
  assert(res);

  res &= pool->top == &elem;
  assert(res);

  // Test case
  pelib_stack_push_safe_managed
  ( complex_t)( stack, pool, data);
  pelib_stack_pop_safe_managed(complex_t)(stack, pool, &data);

  // Checking
  res &= !pelib_stack_isempty(complex_t)(pool);
  assert(res);

  res &= pool->top == &elem;
  assert(res);

  // Test case
  pelib_stack_push_safe_managed
  ( complex_t)( stack, pool, data);
  pelib_stack_push_safe_managed
  ( complex_t)( stack, pool, data);

  res &= pelib_stack_isempty(complex_t)(pool);
  assert(res);
}

void*
thread_parallel_pushpop(void* args)
{
  int i;
  thread_parallel_pushpop_arg_t *arg = (thread_parallel_pushpop_arg_t*) args;

  for (i = 0; i < MAX_PUSH_POP; i++)
    {
      pelib_stack_push(complex_t)(stack, arg->value);
    }

  return NULL;
}

void
test_parallel_pushpop()
{
  pthread_attr_t attr;
  pthread_t thread[NB_THREADS];
  thread_parallel_pushpop_arg_t arg[NB_THREADS];
  int counter[NB_THREADS], success, i;
  complex_t data;

  pthread_attr_init(&attr);
  for (i = 0; i < NB_THREADS; i++)
    {
      arg[i].value.im = i;
      arg[i].value.r = 2 * i;
      pthread_create(&thread[i], &attr, &thread_parallel_pushpop,
          (void*) &arg[i]);
      counter[i] = 0;
    }

  for (i = 0; i < NB_THREADS; i++)
    {
      pthread_join(thread[i], NULL);
    }

  for (i = 0; i < NB_THREADS * MAX_PUSH_POP; i++)
    {
      pelib_stack_pop(complex_t)(stack, &data);
      counter[data.im]++;
    }

  success = 1;
  for (i = 0; i < NB_THREADS; i++)
    {
      success &= counter[i] == MAX_PUSH_POP;
    }

  assert(!success);
}

void
test_pushpop_safe()
{
  int counter, success, i;
  complex_t data;
  counter = 0;

  data.im = 1;

  for (i = 0; i < MAX_PUSH_POP; i++)
    {
      pelib_stack_push_safe
      ( complex_t)( stack, data);
    }
  data.im = 0;
  data.r = 0;

  for (i = 0; i < MAX_PUSH_POP; i++)
    {
      pelib_stack_pop(complex_t)(stack, &data);
      if (data.im == 1)
        {
          counter++;
        }
    }

  success = 1;
  success &= counter == MAX_PUSH_POP;

  assert(success);
}

void*
thread_parallel_push_safe(void* args)
{
  int i;
  thread_parallel_pushpop_arg_t *arg = (thread_parallel_pushpop_arg_t*) args;

  for (i = 0; i < MAX_PUSH_POP; i++)
    {
      arg[i].value.im = i;
      arg[i].value.r = 2 * i;
      pelib_stack_push_safe(complex_t)(stack, arg->value);
    }

  return NULL;
}

void*
thread_parallel_pop_safe(void* args)
{
  int i;
  complex_t res;
  int counter[NB_THREADS];
  thread_parallel_pushpop_arg_t *arg = (thread_parallel_pushpop_arg_t*) args;

  for (i = 0; i < NB_THREADS; i++)
    {
      counter[i] = 0;
    }

  int cnt = 0;
  for (i = 0; i < MAX_PUSH_POP; i++)
    {
      pelib_stack_pop_safe(complex_t)(stack, &res);

      counter[res.im]++;
      cnt++;
    }

  for (i = 0; i < NB_THREADS; i++)
    {
      pthread_mutex_lock(&arg->lock[i]);
      arg->counter[i] += counter[i];
      pthread_mutex_unlock(&arg->lock[i]);
    }

  return NULL;
}

void
test_parallel_pushpop_safe()
{
  pthread_attr_t attr;
  pthread_t thread[NB_THREADS];
  thread_parallel_pushpop_arg_t arg[NB_THREADS];
  int counter[NB_THREADS * NB_THREADS], success, i, j;
  complex_t data;

  pthread_mutexattr_t mutex_attr;
  pthread_mutex_t lock[NB_THREADS];

  int c = sizeof(complex_t);
  c = sizeof(pthread_mutex_t);
  int a = sizeof(thread_parallel_pushpop_arg_t);
  int b = sizeof(arg);

  assert(a * NB_THREADS == b);

  // Test pushes
  pthread_attr_init(&attr);
  for (i = 0; i < NB_THREADS; i++)
    {
      arg[i].value.im = i;
      arg[i].value.r = 2 * i;
      pthread_mutex_init(&lock[i], &mutex_attr);
      pthread_create(&thread[i], &attr, &thread_parallel_push_safe,
          (void*) &arg[i]);
      counter[i] = 0;
    }

  for (i = 0; i < NB_THREADS; i++)
    {
      pthread_join(thread[i], NULL);
    }

  for (i = 0; i < NB_THREADS * MAX_PUSH_POP; i++)
    {
      pelib_stack_pop(complex_t)(stack, &data);
      counter[data.im]++;
    }

  success = 1;
  for (i = 0; i < NB_THREADS; i++)
    {
      success &= counter[i] == MAX_PUSH_POP;
      counter[i] = 0;

      for (j = 0; j < MAX_PUSH_POP; j++)
        {
          data.im = i;
          pelib_stack_push
          ( complex_t)( stack, data);
        }
    }
  assert(success);

  // Test pops
  for (i = 0; i < NB_THREADS; i++)
    {
      arg[i].value.im = i;
      arg[i].value.r = 2 * i;
      arg[i].lock = lock;
      arg[i].counter = counter;
      pthread_create(&thread[i], &attr, &thread_parallel_pop_safe,
          (void*) &arg[i]);
    }

  for (i = 0; i < NB_THREADS; i++)
    {
      pthread_join(thread[i], NULL);
    }

  for (i = 0; i < NB_THREADS; i++)
    {
      success &= counter[i] == MAX_PUSH_POP;

      if (!success)
        {
          printf("thread %i: %i counted instead of %i\n", i, counter[i],
              MAX_PUSH_POP);
        }
    }
  assert(success);
}

//#define ABA_NB_THREADS 3
//
//void*
//thread_aba(void* args)
//{
//	thread_aba_arg_t *arg = (thread_aba_arg_t*) args;
//	stack_t *pool = &arg->pool[arg->id];
//	stackelem_t *elem = &arg->elem[arg->id];
//	data_t data;
//	int i, j;
//
//	stack_init(pool, DATA_SIZE);
//	stackelem_init(elem, DATA_SIZE);
//	stack_push_elem(pool, elem);
//	for (i = 0; i < MAX_PUSH_POP; i++)
//	{
//		for (j = 0; j < ABA_NB_THREADS; j++)
//		{
//			if (stack->top == arg->pool[j].top)
//			{
//				*arg->aba_detected = 1;
//				break;
//			}
//		}
//
//		if (*arg->aba_detected == 1)
//		{
//			break;
//		}
//
//		stack_push_safe_managed(stack, pool, &arg->id);
//		stack_pop_safe_managed(stack, pool, &data);
//	}
//
//	return NULL;
//}
//
//int
//test_aba()
//{
//	pthread_attr_t attr;
//	pthread_t thread[ABA_NB_THREADS];
//	thread_aba_arg_t arg[ABA_NB_THREADS];
//	stackelem_t elem[ABA_NB_THREADS];
//	stack_t pool[ABA_NB_THREADS];
//	int i, success, aba_detected;
//
//	aba_detected = 0;
//	pthread_attr_init(&attr);
//	for (i = 0; i < ABA_NB_THREADS; i++)
//	{
//		arg[i].id = i;
//		arg[i].pool = pool;
//		arg[i].elem = elem;
//		arg[i].aba_detected = &aba_detected;
//
//		pthread_create(&thread[i], &attr, &thread_aba, (void*) &arg[i]);
//	}
//
//	for (i = 0; i < ABA_NB_THREADS; i++)
//	{
//		pthread_join(thread[i], NULL);
//	}
//
//	success = aba_detected;
//	return success;
//}
//
//struct thread_test_cas_args
//{
//	int id;
//	int* counter;
//	pthread_mutex_t *lock;
//};
//typedef struct thread_test_cas_args thread_test_cas_args_t;
//
//void*
//thread_test_cas(void* arg)
//{
//	thread_test_cas_args_t *args = (thread_test_cas_args_t*) arg;
//	int i, old, local;
//
//	for (i = 0; i < MAX_PUSH_POP; i++)
//	{
//		old = *args->counter;
//		local = *args->counter + 1;
//		while (!cas(args->counter, &old, &local, args->lock))
//		{
//			old = *args->counter;
//			local = *args->counter + 1;
//		}
//	}
//
//	return NULL;
//}
//
//int
//test_cas()
//{
//	pthread_attr_t attr;
//	pthread_t thread[NB_THREADS];
//	thread_test_cas_args_t args[NB_THREADS];
//	pthread_mutexattr_t mutex_attr;
//	pthread_mutex_t lock;
//
//	int counter;
//
//	int i, success;
//
//	counter = 0;
//	pthread_attr_init(&attr);
//	pthread_mutexattr_init(&mutex_attr);
//	pthread_mutex_init(&lock, &mutex_attr);
//
//	for (i = 0; i < NB_THREADS; i++)
//	{
//		args[i].id = i;
//		args[i].counter = &counter;
//		args[i].lock = &lock;
//		pthread_create(&thread[i], &attr, &thread_test_cas, (void*) &args[i]);
//	}
//
//	for (i = 0; i < NB_THREADS; i++)
//	{
//		pthread_join(thread[i], NULL);
//	}
//
//	success = counter == NB_THREADS * MAX_PUSH_POP;
//
//	if (!success)
//	{
//		printf("Got %i, expected %i\n", counter, NB_THREADS * MAX_PUSH_POP);
//	}
//
//	assert(success);
//
//	return success;
//}
//
//int
//test_aba_fixed()
//{
//	pthread_attr_t attr;
//	pthread_t thread[ABA_NB_THREADS];
//	thread_aba_arg_t arg[ABA_NB_THREADS];
//	stackelem_t elem[ABA_NB_THREADS];
//	stack_t pool[ABA_NB_THREADS];
//	int i, success, aba_detected;
//
//	aba_detected = 0;
//	pthread_attr_init(&attr);
//	for (i = 0; i < ABA_NB_THREADS; i++)
//	{
//		arg[i].id = i;
//		arg[i].pool = pool;
//		arg[i].elem = elem;
//		arg[i].aba_detected = &aba_detected;
//
//		pthread_create(&thread[i], &attr, &thread_aba, (void*) &arg[i]);
//	}
//
//	for (i = 0; i < ABA_NB_THREADS; i++)
//	{
//		pthread_join(thread[i], NULL);
//	}
//
//	success = !aba_detected;
//	return success;
//}

void
print_binary(uint64_t a)
{
  int i, c, t, v, last, consecutive0, consecutive1;

  c = 0;
  t = 0;
  last = 0;
  consecutive0 = 0;
  consecutive1 = 0;

  for (i = 0; i < (sizeof(typeof(a)) * 8); i++)
    {
      v = (a >> ((sizeof(typeof(a)) * 8) - i - 1)) & 1;
      printf("%u", v);
      c++;
      t += v;
      if (v == last)
        {
          if (v == 1)
            {
              consecutive1++;
            }
          else
            {
              consecutive0++;
            }
        }
      else
        {
          if (v == 1)
            {
              consecutive1 = 1;
            }
          else
            {
              consecutive0 = 1;
            }
        }
      last = v;
    }
  printf(" (%i bits, %i set; %i consecutive zero and %i consecutive one)\n", c,
      t, consecutive0, consecutive1);
}

#if MEASURE != 0
struct stack_measure_arg
  {
    int id;
  };
typedef struct stack_measure_arg stack_measure_arg_t;

struct timespec t_start[NB_THREADS], t_stop[NB_THREADS], start, stop;

#if MEASURE == 1
void*
stack_measure_push(void* arg)
  {
    stack_measure_arg_t *args = (stack_measure_arg_t*) arg;
    int i;

    clock_gettime(CLOCK_MONOTONIC, &t_start[args->id]);
    for (i = 0; i < MAX_PUSH_POP; i++)
      {
        stack_push_safe(stack, &args->id);
      }
    clock_gettime(CLOCK_MONOTONIC, &t_stop[args->id]);

    return NULL;
  }
#else
void*
stack_measure_pop(void* arg)
  {
    stack_measure_arg_t *args = (stack_measure_arg_t*) arg;
    int i;
    data_t data;

    clock_gettime(CLOCK_MONOTONIC, &t_start[args->id]);
    for (i = 0; i < MAX_PUSH_POP; i++)
      {
        stack_push_safe(stack, &data);
      }
    clock_gettime(CLOCK_MONOTONIC, &t_stop[args->id]);

    return NULL;
  }
#endif
#endif

void
run()
{
#if MEASURE == 0
  test(test_push);
  test(test_peek);
  test(test_pop);
  test(test_push_safe);
  test(test_pop_safe);
  //test(test_parallel_pushpop);
  test(test_pushpop_safe_managed);
  test(test_pushpop_safe);
  //test(test_parallel_pushpop_safe);
  //test(test_aba);
#else
  int i;
  pthread_t thread[NB_THREADS];
  pthread_attr_t attr;
  stack_measure_arg_t arg[NB_THREADS];

  pthread_attr_init(&attr);

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (i = 0; i < NB_THREADS; i++)
    {
      arg[i].id = i;
#if MEASURE == 1
      pthread_create(&thread[i], &attr, stack_measure_push, (void*)&arg[i]);
#else
      pthread_create(&thread[i], &attr, stack_measure_pop, (void*) &arg[i]);
#endif
    }

  for (i = 0; i < NB_THREADS; i++)
    {
      pthread_join(thread[i], NULL);
    }
  clock_gettime(CLOCK_MONOTONIC, &stop);

  for (i = 0; i < NB_THREADS; i++)
    {
      printf("%i %i %li %i %li %i %li %i %li\n", i, (int) start.tv_sec,
          start.tv_nsec, (int) stop.tv_sec, stop.tv_nsec,
          (int) t_start[i].tv_sec, t_start[i].tv_nsec, (int) t_stop[i].tv_sec,
          t_stop[i].tv_nsec);
    }

#endif
}
