/*
 * sort.h
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

#ifndef SORT_H_
#define SORT_H_

#include <pelib/integer.h>

#define PELIB_QUICKSORT_THRESHOLD 15
#define PELIB_QUICKSORT_NUM_SAMPLE 10

/** Tuning knobs for mixed sorting algorithms **/
struct qsort_tune
{
	int begin; /** Beginning of the sub-array to be sorted **/
	int end; /** End of the subarray to be sorted **/
	int threshold; /** Sub-array size under which insert sort should be used **/
	int num_sample; /** Number of samples to pick to compute a good pivot for quicksort **/
};
/** Spaceless alias for struct_qsort **/
typedef struct qsort_tune qsort_tune_t;

/** Computes a pivot from a sample of an array
	@param a Array to compute a pivot from
	@param size Number of elements fo pick to compute a pivot
	@param start Minimal index from which to pick samples
	@param stop Maximal index from which to pick samples
**/ 
int
pelib_sample(array_t(int)* a, int size, int start, int stop);

/** Insert sort in a subset of an array
	@param a Array to sort
	@param begin Index of the first element of the subarray to run insert sort on
	@param end Index of the last element of the subarray to run insert sort on
**/
void
pelib_insertsort_window(array_t(int)* a, int begin, int end);
/** Run insert sort on a complete array **/
void
pelib_insertsort(array_t(int)*);
/** Run quicksort with tuning parameters **/
void
pelib_quicksort_tune(array_t(int)* array, qsort_tune_t p);
/** Run recursive version of quicksort **/
void
pelib_quicksort_tune_recursive(array_t(int)* array, qsort_tune_t p);
/** Run quicksort on a subset of an array **/
void
pelib_quicksort_window(array_t(int)* array, int begin, int end);
/** Run quicksort on a complete array **/
void
pelib_quicksort(array_t(int)*);
/** Returns 0 if an array is not in non-decreasing order **/
int
pelib_is_increasing(array_t(int)*);

#endif /* SORT_H_ */
