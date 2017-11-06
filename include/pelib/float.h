/*
 * float.h
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

#ifndef FLOAT_H_
#define FLOAT_H_

#define STRUCT_T float
#include <pelib/structure.h>
#define DONE_float

#define ARRAY_T float
#include <pelib/array.h>
#define DONE_array_float

#define CFIFO_T float
#include <pelib/fifo.h>
#define DONE_cfifo_float

#define CFIFO_ARRAY_T float
#include <pelib/fifo_array.h>
#define DONE_cfifo_array_float

#endif /* INTEGER_H_ */
