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


#include <cstdlib>
#include <string>
#include <iostream>

#include <string.h>

#include <pelib/argument_parsing.hpp>

#ifdef debug
#undef debug
#endif

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

using namespace std;

void
pelib_argument_stream_init(pelib_argument_stream_t *pelib_argument_stream)
{
	pelib_argument_stream->library = NULL;
	pelib_argument_stream->argv = (char**)malloc(sizeof(char*) * 1);
	pelib_argument_stream->argv[0] = NULL;
	pelib_argument_stream->argc = 0;
	pelib_argument_stream->filename = NULL;
	pelib_argument_stream->name = NULL;
	pelib_argument_stream->stream = STREAM_NOTHING;
}

void
pelib_argument_stream_destroy(const pelib_argument_stream_t pelib_argument_stream)
{
	free(pelib_argument_stream.argv);

	if(pelib_argument_stream.library != NULL)
	{
		free(pelib_argument_stream.library);
	}

	if(pelib_argument_stream.filename != NULL)
	{
		free(pelib_argument_stream.filename);
	}

	if(pelib_argument_stream.name != NULL)
	{
		free(pelib_argument_stream.name);
	}
}

static bool
is_reserved(char* str)
{
	// Anything starting with a dash (-) is an option name and invalid as an option argument
	if(str == NULL || string(str).compare("-") == 0)
	{
		return true;
	}
	
	// Nothing wrong was found with this option argument
	return false;
}

static void
add_argument(pelib_argument_stream *stream, char *argv)
{
			// Make room for another argument
			char **stuff = (char**)malloc(sizeof(char*) * (stream->argc + 2));
			memcpy(stuff, stream->argv, (stream->argc + 1) * sizeof(char*));
			free(stream->argv);
			stream->argv = stuff;

			stream->argv[stream->argc] = (char*)malloc(sizeof(char) * (strlen(argv) + 1));
			strcpy(stream->argv[stream->argc], argv);
			stream->argc++;
			stream->argv[stream->argc] = NULL;
}

unsigned int
pelib_argument_stream_parse(char **argv, pelib_argument_stream_t* stream)
{
	unsigned int parsed = 0;
	while((void*)argv[0] != NULL && string(argv[0]).compare("--") != 0)
	{
		if(string(*argv).compare("--library") == 0 || string(*argv).compare("-l") == 0)
		{
			argv++;
			parsed++;
			
			if(is_reserved(*argv))
			{
				return parsed;
			}
			else
			{
				stream->library = (char*)malloc(sizeof(char) * (strlen(*argv) + 1));
				strcpy(stream->library, *argv);
				argv++;
				parsed++;
			}
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--lib") == 0 || string(*argv).compare("-m") == 0)
		{
			argv++;
			parsed++;
			
			if(is_reserved(*argv))
			{
				return parsed;
			}
			else
			{
				#define LIBPELIB "libpelib-"
				#define SO ".so"
				stream->library = (char*)malloc(sizeof(char) * (strlen(LIBPELIB) + strlen(*argv) + strlen(SO) + 1));
				strcpy(stream->library, LIBPELIB);
				strcpy(stream->library + strlen(LIBPELIB), *argv);
				strcpy(stream->library + strlen(LIBPELIB) + strlen(*argv), SO);

				argv++;
				parsed++;
			}
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--name") == 0 || string(*argv).compare("-n") == 0)
		{
			argv++;
			parsed++;
			
			if(is_reserved(*argv))
			{
				return parsed;
			}
			else
			{
				stream->name = (char*)malloc(sizeof(char) * (strlen(*argv) + 1));
				strcpy(stream->name, *argv);

				argv++;
				parsed++;
			}
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--file") == 0 || string(*argv).compare("-f") == 0)
		{
			argv++;
			parsed++;
			
			if(is_reserved(*argv))
			{
				return parsed;
			}
			else
			{
				stream->stream = STREAM_FILE;
				stream->filename = (char*)malloc(sizeof(char) * (strlen(*argv) + 1));
				strcpy(stream->filename, *argv);
				argv++;
				parsed++;
			}
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--stdin") == 0)
		{
			argv++;
			parsed++;
			stream->stream = STREAM_STDIN;
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--stdout") == 0)
		{
			argv++;
			parsed++;
			stream->stream = STREAM_STDOUT;
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--stderr") == 0)
		{
			argv++;
			parsed++;
			stream->stream = STREAM_STDERR;
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--args") == 0)
		{
			argv++;
			parsed++;
	
			size_t args_counter = 1;
			while(*argv != NULL)
			{
 				if(string(*argv).compare("--args") == 0)
				{
					args_counter++;
				}	
				
 				if(string(*argv).compare("--") == 0)
				{
					args_counter--;
					if(args_counter == 0)
					{
						break;
					}
				}

				add_argument(stream, *argv);
				argv++;
				parsed++;
			}
			
			// Skip the last "--"
			if(*argv != NULL)
			{
				if(string(*argv).compare("--") == 0)
				{
					argv++;
					parsed++;	
				}
			}

			continue;
		}

		if(string(*argv).compare("-o") == 0)
		{
			argv++;
			parsed++;

			add_argument(stream, *argv);
			argv++;
			parsed++;

			continue;
		}
		
		// This is not a valid option; exit now
		return parsed;
	}

	// Not parsed anything
	return parsed;
}

