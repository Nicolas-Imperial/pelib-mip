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


#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

#include <pelib/dl.h>

void*
load_lib(const char* library)
{
        void *handle;
        char *error;

        handle = dlopen(library, RTLD_LAZY);
        if (!handle) {
                fprintf(stderr, "[%s:%s:%d] Cannot open shared library: %s\n", __FILE__, __FUNCTION__, __LINE__, dlerror());
                exit(EXIT_FAILURE);
        }

        if ((error = dlerror()) != NULL)  {
                fprintf(stderr, "[%s:%s:%d] Error while opening shared library: %s\n", __FILE__, __FUNCTION__, __LINE__, error);
                exit(EXIT_FAILURE);
        }   

        return handle;
}

void*
load_function(void *handle, const char *function)
{
        dlerror();    /* Clear any existing error */

        void *func = (void*)dlsym(handle, function);
        char *error;

        if ((error = dlerror()) != NULL)  {
                fprintf(stderr, "[%s:%s:%d] Error while loading function: %s\n", __FILE__, __FUNCTION__, __LINE__, error);
                exit(EXIT_FAILURE);
        }   

        return func;
}

void
destroy_lib(void *handle)
{
        dlclose(handle);
}

