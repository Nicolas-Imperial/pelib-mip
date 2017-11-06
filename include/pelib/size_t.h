/*
 * size_t.h
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

#ifndef SIZE_T_H_
#define SIZE_T_H_

#define STRUCT_T size_t
#include <pelib/structure.h>
#define DONE_size_t

#define STACK_T size_t
#include <pelib/stack.h>
#define DONE_stack_size_t

#define CFIFO_T size_t
#include <pelib/fifo.h>
#define DONE_cfifo_size_t

#endif /* SIZE_T_H_ */
