/*
 * complex.h
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

#ifndef COMPLEX_H_
#define COMPLEX_H_

/** Integer complex number **/
struct complex
{
		int r, im;
};
/** Spaceless alias for complex structure **/
typedef struct complex complex_t;

#define STRUCT_T complex_t
#include <pelib/structure.h>
#define DONE_complex_t

#define STACK_T complex_t
#include <pelib/stack.h>
#define DONE_stack_complex_t

#define ARRAY_T complex_t
#include <pelib/array.h>
#define DONE_array_complex_t

#define CFIFO_T complex_t
#include <pelib/fifo.h>
#define DONE_cfifo_complex_t

#endif /* COMPLEX_H_ */
