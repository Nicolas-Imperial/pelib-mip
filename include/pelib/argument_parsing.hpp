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


#ifndef PELIB_ARGUMENT_PARSING_H
#define PELIB_ARGUMENT_PARSING_H

enum stream {STREAM_NOTHING, STREAM_STDIN, STREAM_STDOUT, STREAM_STDERR, STREAM_FILE};

/** Input or output argument model for pelib-convert **/
struct pelib_argument_stream
{
	char *library; /** Filename of the library that holds the code able to perform the input or output operation. If a path is not provided, checks in pelib-convert built-in search path (defined as the LD_RUN_PATH environmnet variable at the time pelib-convert was built) or LD_LIBRARY_PATH environment variable, if defined (see document of ld for more information). If not a filename, look for the dynamic library "libpelib-" + library + ".so" in search path as described above **/
	char **argv; /** List of arguments to prived to the library to perform its work **/
	size_t argc; /** Number of arguments passed to the library **/
	char *filename; /** Filename to read input from or write output to by the library **/
	char *name; /** If reading input, associates a custom name to the content parse, instead of the class name of the content parsed. **/
	enum stream stream; /** Type of stream the library needs to manipulate: standard input, standard output, standard error or a file. No stream by default **/
};
/** Spaceless type alias for struct pelib_argument_stream **/
typedef struct pelib_argument_stream pelib_argument_stream_t;

/** Give defaults values to an instance of pelib argument data structure **/
void pelib_argument_stream_init(pelib_argument_stream_t *pelib_argument_stream);

/** Frees all memory that pointers in an instance of pelib argument stram structure points to **/
void pelib_argument_stream_destroy(const pelib_argument_stream_t pelib_argument_stream);

/** Performs parsing operations stores data gather in a pelib_argument stream instance
	@param argv Array of strings containing all data to be parsed. Its last element must be a NULL pointer.
	@param stream Instance of pelib argument_stream that holds the data being parsed
**/
unsigned int
pelib_argument_stream_parse(char **argv, pelib_argument_stream_t* stream);

#endif

