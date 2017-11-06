/*
 * stack.h
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
 *     but WITHOUT ANY WARRANTY without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with pelib. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int
pelib_init(stackelem_t(STACK_T))(stackelem_t(STACK_T) *elem)
  {
    assert(elem != NULL);
    elem->next = NULL;

    return 1;
  }

int
pelib_stackelem_write(STACK_T)(stackelem_t(STACK_T) *elem, STACK_T buffer)
  {
    assert(pelib_stackelem_check(STACK_T)(elem) == 0);

    return pelib_copy(STACK_T)(buffer, &elem->buffer);
  }

int
pelib_stackelem_read(STACK_T)(stackelem_t(STACK_T) *elem, STACK_T* buffer)
  {
    assert(buffer != NULL);
    assert(pelib_stackelem_check(STACK_T)(elem) == 0);

    return pelib_copy(STACK_T)(elem->buffer, buffer);
  }

int
pelib_stackelem_check(STACK_T)(stackelem_t(STACK_T) *elem)
  {
    assert(elem != NULL);

    return 1;
  }

stack_t(STACK_T) *
pelib_alloc_collection(stack_t(STACK_T))(size_t aux)
  {
    stack_t(STACK_T) *res;

    res = malloc(sizeof(stack_t(STACK_T)));
    assert(res != NULL);

    if (res == NULL)
    return NULL;

    res->top = NULL;

    return res;
  }

int
pelib_init(stack_t(STACK_T))(stack_t(STACK_T) *s)
  {
    pthread_mutexattr_t attr;

    assert(s != NULL);

    pthread_mutexattr_init(&attr);
    pthread_mutex_init(&s->lock, &attr);
    s->top = NULL;

    return 1;
  }

int
pelib_free(stack_t(STACK_T))(stack_t(STACK_T) *s)
  {
    pthread_mutex_destroy(&s->lock);
    free(s);

    return 1;
  }

int
pelib_stack_check(STACK_T)(stack_t(STACK_T) *s)
  {
    assert(s != NULL);

    return 1;
  }

int
pelib_stack_isempty(STACK_T)(stack_t(STACK_T) *s)
  {
    return s->top == NULL;
  }

int
pelib_stack_push(STACK_T)(stack_t(STACK_T) *s, STACK_T buffer)
  {
    stackelem_t(STACK_T) * elem;

    elem = malloc(sizeof(stackelem_t(STACK_T)));
    assert(elem != NULL);
    if (elem == NULL)
    return 0;

    pelib_init(stackelem_t(STACK_T))(elem);
    pelib_stackelem_write(STACK_T)(elem, buffer);

    pelib_stack_push_elem(STACK_T)(s, elem);

    return 1;
  }

int
pelib_stack_pop(STACK_T)(stack_t(STACK_T) *s, STACK_T* buffer)
  {
    stackelem_t(STACK_T) * elem;

    if (!pelib_stack_peek(STACK_T)(s, buffer))
      {
        return 0;
      }

    pelib_stack_pop_elem(STACK_T)(s, &elem);
    free(elem);

    return 1;
  }

int
pelib_stack_peek(STACK_T)(stack_t(STACK_T) *s, STACK_T* buffer)
  {
    if (s->top == NULL)
      {
        return 0;
      }

    pelib_stackelem_read(STACK_T)(s->top, buffer);

    return 1;
  }

int
pelib_stack_push_safe(STACK_T)(stack_t(STACK_T) *s, STACK_T buffer)
  {
    pthread_mutex_lock(&s->lock);
    pelib_stack_push(STACK_T)(s, buffer);
    pthread_mutex_unlock(&s->lock);

    return 1;
  }

int
pelib_stack_pop_safe(STACK_T)(stack_t(STACK_T) *s, STACK_T* buffer)
  {
    pthread_mutex_lock(&s->lock);
    pelib_stack_pop(STACK_T)(s, buffer);
    pthread_mutex_unlock(&s->lock);

    return 1;
  }

int
pelib_stack_push_elem(STACK_T)(stack_t(STACK_T) *s, stackelem_t(STACK_T) *elem)
  {
    assert(pelib_stack_check(STACK_T)(s) == 0);
    assert(s->top != elem);

    // Here comes the swap
    elem->next = s->top;
    s->top = elem;

    return 1;
  }

int
pelib_stack_pop_elem(STACK_T)(stack_t(STACK_T) *s, stackelem_t(STACK_T) **elem)
  {
    assert(pelib_stack_check(STACK_T)(s) == 0);

    if (s->top == NULL)
      {
        return 0;
      }

    // Here comes the swap
    *elem = s->top;
    s->top = (*elem)->next;

    (*elem)->next = NULL;

    return 1;
  }

int
pelib_stack_push_elem_safe(STACK_T)(stack_t(STACK_T) *s, stackelem_t(STACK_T) *elem)
  {
    assert(s->top != elem);

    pthread_mutex_lock(&s->lock);
    pelib_stack_push_elem(STACK_T)(s, elem);
    pthread_mutex_unlock(&s->lock);

    return 1;
  }

int
pelib_stack_pop_elem_safe(STACK_T)(stack_t(STACK_T) *s, stackelem_t(STACK_T) **elem)
  {
    pthread_mutex_lock(&s->lock);
    pelib_stack_pop_elem(STACK_T)(s, elem);
    pthread_mutex_unlock(&s->lock);

    return 1;
  }

int
pelib_stack_push_safe_managed(STACK_T)(stack_t(STACK_T) *s, stack_t(STACK_T) *pool, STACK_T buffer)
  {
    stackelem_t(STACK_T) *elem = NULL;

    assert(pelib_stack_check(STACK_T)(pool) == 0);

    if (pelib_stack_pop_elem(STACK_T)(pool, &elem) != 1)
      {
        elem = malloc(sizeof(stackelem_t(STACK_T)));
        pelib_init(stackelem_t(STACK_T))(elem);
      }

    pelib_stackelem_write(STACK_T)(elem, buffer);

    pelib_stack_push_elem_safe(STACK_T)(s, elem);

    return 1;
  }

int
pelib_stack_pop_safe_managed(STACK_T)(stack_t(STACK_T) *s, stack_t(STACK_T) *pool, STACK_T* buffer)
  {
    stackelem_t(STACK_T) *elem = NULL;

    if (pelib_stack_pop_elem_safe(STACK_T)(s, &elem) != 1)
      {
        return 0;
      }
    pelib_stackelem_read(STACK_T)(elem, buffer);

    pelib_stack_push_elem(STACK_T)(pool, elem);

    return 1;
  }

#undef STACK_T
