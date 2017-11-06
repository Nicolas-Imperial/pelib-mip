/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef PELIB_DL_H
#define PELIB_DL_H

/** Dynamically loads a dynamic library through its filename
	@param library Filename of the library to be loaded
	@return Handler for the library loaded
**/
void* load_lib(const char* library);

/** Extracts a function pointer from a dynamically laoded function
	@param Handle Dynmic Library handle obtained with load_lib()
	@param Function Name of the function in the library to be run
	@return Function pointer ready to be called
**/
void* load_function(void *handle, const char *function);

/** Unload a dynamically loaded function and destroy its handler **/
void destroy_lib(void *handle);

#endif
