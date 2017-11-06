/*
 * template.h
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

#ifndef TEMPLATE_H_
#define TEMPLATE_H_

#define PELIB_GLUE(a, b) a ## b
#define PELIB_CONCAT_2(FIRST, SECOND) PELIB_GLU##E(FIRST, SECOND)
#define PELIB_CONCAT_3(FIRST, SECOND, THIRD) PELIB_CO##NCAT_2(PELIB_CONCAT_2(FIRST, SECOND), THIRD)
#define PELIB_CONCAT_4(FIRST, SECOND, THIRD, FOURTH) PELIB_CON##CAT_2(PELIB_CONCAT_3(FIRST, SECOND, THIRD), FOURTH)
#define PELIB_CONCAT_5(FIRST, SECOND, THIRD, FOURTH, FIFTH) PELIB_CON##CAT_2(PELIB_CONCAT_4(FIRST, SECOND, THIRD, FOURTH), FIFTH)

#endif /* TEMPLATE_H_ */
