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
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with pelib. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <pthread.h>

#include <pelib/template.h>

#ifndef STACK_T
#error Using generic stack without a type
#endif

#if PELIB_CONCAT_2(DONE_stack_, STACK_T) == 0

#define stackelem(elem) PELIB_CONCAT_2(stackelem_, elem)
#define stackelem_t(elem) PELIB_CONCAT_2(stackelem(elem), _t)

#define pelib_stackelem_write(elem) PELIB_CONCAT_3(pelib_, stackelem(elem), _write)
#define pelib_stackelem_read(elem) PELIB_CONCAT_3(pelib_, stackelem(elem), _read)
#define pelib_stackelem_check(elem) PELIB_CONCAT_3(pelib_, stackelem(elem), _check)

#define stack(elem) PELIB_CONCAT_2(stack_, elem)
#define stack_t(elem) PELIB_CONCAT_2(stack(elem), _t)

#define pelib_stack_check(elem) PELIB_CONCAT_3(pelib_, stack(elem), _check)
#define pelib_stack_isempty(elem) PELIB_CONCAT_3(pelib_, stack(elem), _isempty)
#define pelib_stack_push(elem) PELIB_CONCAT_3(pelib_, stack(elem), _push)
#define pelib_stack_pop(elem) PELIB_CONCAT_3(pelib_, stack(elem), _pop)
#define pelib_stack_peek(elem) PELIB_CONCAT_3(pelib_, stack(elem), _peek)
#define pelib_stack_push_safe(elem) PELIB_CONCAT_3(pelib_, stack(elem), _push_safe)
#define pelib_stack_pop_safe(elem) PELIB_CONCAT_3(pelib_, stack(elem), _pop_safe)
#define pelib_stack_push_elem(elem) PELIB_CONCAT_3(pelib_, stack(elem), _push_elem)
#define pelib_stack_pop_elem(elem) PELIB_CONCAT_3(pelib_, stack(elem), _pop_elem)
#define pelib_stack_push_elem_safe(elem) PELIB_CONCAT_3(pelib_, stack(elem), _push_elem_safe)
#define pelib_stack_pop_elem_safe(elem) PELIB_CONCAT_3(pelib_, stack(elem), _pop_elem_safe)
#define pelib_stack_push_safe_managed(elem) PELIB_CONCAT_3(pelib_, stack(elem), _push_safe_managed)
#define pelib_stack_pop_safe_managed(elem) PELIB_CONCAT_3(pelib_, stack(elem), _pop_safe_managed)

/** Generic element of a stack that holds data pushed and pointer to the next element **/
struct
stackelem(STACK_T)
  {
    STACK_T buffer;
    struct stackelem(STACK_T) *next;
  };
/** Space-less type alias for struct stackelem **/
typedef struct stackelem(STACK_T) stackelem_t(STACK_T);

#define STRUCT_T stackelem_t(STACK_T)
#include <pelib/structure.h>

/** Writes data into an existing stack element **/
int
pelib_stackelem_write(STACK_T)(stackelem_t(STACK_T)*, STACK_T);

/** Reads data from a stack element **/
int
pelib_stackelem_read(STACK_T)(stackelem_t(STACK_T)*, STACK_T*);

/** Checks if a stack element is valid **/
int
pelib_stackelem_check(STACK_T)(stackelem_t(STACK_T)*);

/** Generic chained stack **/
struct stack(STACK_T)
  {
    stackelem_t(STACK_T) * top;
    pthread_mutex_t lock;
  };
/** Space-less type alias for struct stack **/
typedef struct stack(STACK_T) stack_t(STACK_T);

#define STRUCT_T stack_t(STACK_T)
#include <pelib/structure.h>

/** Returns 0 if the stack is not in a consistent state **/
int
pelib_stack_check(STACK_T)(stack_t(STACK_T) *);

/** Returns 0 is the stack holds at least one element **/
int
pelib_stack_isempty(STACK_T)(stack_t(STACK_T) *);

/** Allocates a new stack element, write the element in it and push it to the stack **/
int
pelib_stack_push(STACK_T)(stack_t(STACK_T) *stack, STACK_T data);

/** Read value from the head stack element, removes the head stack element and free the stack element **/
int
pelib_stack_pop(STACK_T)(stack_t(STACK_T) *, STACK_T*);

/** Read the head value from the stack but doesn't remove the head element **/
int
pelib_stack_peek(STACK_T)(stack_t(STACK_T) *, STACK_T*);

/** Pushes an element to the stack in a thread-safe manner **/
int
pelib_stack_push_safe(STACK_T)(stack_t(STACK_T) *, STACK_T);

/** Pops an element to the stack in a thread-safe manner **/
int
pelib_stack_pop_safe(STACK_T)(stack_t(STACK_T) *, STACK_T*);

/** Pushes an existing stack element to the stack **/
int
pelib_stack_push_elem(STACK_T)(stack_t(STACK_T) *, stackelem_t(STACK_T)*);

/** Pops the head element of the stack without reading its value or freeing it **/
int
pelib_stack_pop_elem(STACK_T)(stack_t(STACK_T) *, stackelem_t(STACK_T)**);

/** Push a stack element in a thread-safe manner **/
int
pelib_stack_push_elem_safe(STACK_T)(stack_t(STACK_T) *, stackelem_t(STACK_T) *);

/** Pops a stack element in thread-safe manner **/
int
pelib_stack_pop_elem_safe(STACK_T)(stack_t(STACK_T) *, stackelem_t(STACK_T)**);

/** Pushes a new value in the stack, reusing a stack element from a stack element recycling bin. Allocates a new element if the recycle bin is empty
	@param stack Stack that receives the new element
	@param bin Recycle bin (stack) that contains stack element to be reused
	@param value Value to be pushed in the stack
	@return 0 If the push operation could not be performed
**/
int
pelib_stack_push_safe_managed(STACK_T)(stack_t(STACK_T) *stack, stack_t(STACK_T) *bin, STACK_T value);

/** Pops the head value of a stack, saves the stack element popped in a recycling bin and writes the value read in ptr
	@param stack Stack toread head element from
	@param bin Stack element recycle bin
	@param ptr Memory address where the value popped is written
**/
int
pelib_stack_pop_safe_managed(STACK_T)(stack_t(STACK_T) *stack, stack_t(STACK_T) *bin, STACK_T *ptr);

#endif

// Make sure the symbol does not propagate any further
#undef STACK_T
