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


#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

#include <pelib/Record.hpp>
//#include <pelib/Vector.hpp>

#include <pelib/argument_parsing.hpp>
#include <pelib/dl.h>

#ifdef debug
#undef debug
#endif

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

using namespace std;
using namespace pelib;

struct conversion
{
	vector<pelib_argument_stream_t> inputs;
	vector<pelib_argument_stream_t> process;
	vector<pelib_argument_stream_t> outputs;
};
typedef struct conversion conversion_t;

static conversion_t
parse_args(char** argv)
{
	conversion_t conversion;
	string self(argv[0]);
	//conversion.process.library = NULL;

	for(argv++; *argv != NULL; argv++)
	{
		if(string(*argv).compare("--input") == 0)
		{
			pelib_argument_stream_t input;
			pelib_argument_stream_init(&input);
			argv++;
			size_t num = pelib_argument_stream_parse(argv, &input);
			argv = argv + num - 1;
			conversion.inputs.push_back(input);
			continue;
		}

		if(string(*argv).compare("--process") == 0)
		{
			pelib_argument_stream_t process;
			pelib_argument_stream_init(&process);
			argv++;
			size_t num = pelib_argument_stream_parse(argv, &process);
			argv = argv + num - 1;
			conversion.process.push_back(process);
			continue;
		}

		if(string(*argv).compare("--output") == 0)
		{
			pelib_argument_stream_t output;
			pelib_argument_stream_init(&output);
			argv++;
			size_t num = pelib_argument_stream_parse(argv, &output);
			argv = argv + num - 1;
			conversion.outputs.push_back(output);
			continue;
		}

		if(string(*argv).compare("--sources") == 0)
		{
			string cmd = string("ls $(dirname $(realpath $(which ").append(self).append(")))/../share/pelib/pelib-*.tar.gz | sort -rV | head -1 | xargs realpath");
			exit(system(cmd.c_str()));
		}

		//const Vector<int, float> *streaming_task = algebra.find<Vector<int, float> >("stream");
		if(string(*argv).compare("--seed") == 0)
		{
			argv++;
			size_t seed;
			if(string(*argv).compare("--random") == 0)
			{
				seed = (size_t)time(NULL);
			}
			else
			{
				stringstream str(argv[0]);
				str >> seed;
			}
			srand(seed);
		}
	}

	return conversion;
}

int
main(int argc, char **argv)
{
	vector<void*> process;
	map<string, Record*> inputs;
	map<string, void (*)(Record*)> freelist;
	map<string, void*> parserList;

	conversion_t conversion = parse_args(argv);

	size_t counter = 1;
	for(vector<pelib_argument_stream_t>::const_iterator i = conversion.inputs.begin(); i != conversion.inputs.end(); i++, counter++)
	{
		/* Load functions from shared libraries */
		void *libParser;
		if(i->library != NULL)
		{
			libParser = load_lib(i->library);
		}
		else
		{
			cerr << "[WARNING] No parser library specified for input #" << counter << ". Skipping." << endl;
			continue;
		}

		/* Link function handles to function pointers */
		Record* (*parse)(istream&, size_t, char**, const map<string, Record*>&) = (Record* (*)(istream&, size_t, char**, const map<string, Record*>&))load_function(libParser, "pelib_parse");
		void (*del)(Record*) = (void (*)(Record*))load_function(libParser, "pelib_delete");

		switch(i->stream)
		{
			case STREAM_STDIN:
				{
					Record* rec = parse(cin, i->argc, i->argv, inputs);
					if(rec != NULL)
					{
						if(i->name != NULL)
						{
							inputs.insert(pair<string, Record*>(i->name, rec));
							freelist.insert(pair<const char*, void (*)(Record*)>(i->name, del));
							parserList.insert(pair<const char*, void*>(pair<const char*, void*>(i->name, libParser)));
						}
						else
						{
							inputs.insert(pair<string, Record*>(typeid(*rec).name(), rec));
							freelist.insert(pair<const char*, void (*)(Record*)>(typeid(*rec).name(), del));
							parserList.insert(pair<const char*, void*>(pair<const char*, void*>(typeid(*rec).name(), libParser)));
						}
					}
					else
					{
						cerr << "[WARNING] Parser \"" << i->library << "\" failed to parse input on stdin for input #" << counter << ". Skipping." << endl;
					}
				}
			break;
			case STREAM_STDOUT:
			case STREAM_STDERR:
				cerr << "[WARNING] Cannot read data from an output stream for input #" << counter << ". Skipping." << endl;
			break;
			case STREAM_FILE:
				{
					ifstream myfile(i->filename);
					Record *rec = parse(myfile, i->argc, i->argv, inputs);
					
					if(rec != NULL)
					{
						if(i->name != NULL)
						{
							inputs.insert(pair<string, Record*>(i->name, rec));
							freelist.insert(pair<const char*, void (*)(Record*)>(i->name, del));
						}
						else
						{
							inputs.insert(pair<string, Record*>(typeid(*rec).name(), rec));
							freelist.insert(pair<const char*, void (*)(Record*)>(typeid(*rec).name(), del));
						}
						myfile.close();
					}
					else
					{
						cerr << "[WARNING] Parser \"" << i->library << "\" failed to parse input on stdin for input #" << counter << ". Skipping." << endl;
					}
				}
			break;
			case STREAM_NOTHING:
			default:
				cerr << "[WARNING] No input stream specified for input #" << counter << ". Skipping." << endl;
			break;
		}

		// Don't destroy the libparser yet, as we need to destroy data elements parsed
		// Don't destroy the pelib_argument_stream descriptor yet as we still need input names
	}

	for(vector<pelib_argument_stream_t>::const_iterator i = conversion.process.begin(); i != conversion.process.end(); i++)
	{
		void *libProcess = load_lib(i->library);
		process.push_back(libProcess);

		/* Link function handles to function pointers */
		std::map<string, Record*> (*process)(const std::map<string, Record*> &records, size_t, char**) = (std::map<string, Record*> (*)(const std::map<string, Record*> &records, size_t, char**))load_function(libProcess, "pelib_process");
		std::map<string, Record*> transform = process(inputs, i->argc, i->argv);

		// Free data structure parsed
		for(map<string, void (*)(Record*)>::const_iterator i = freelist.begin(); i != freelist.end(); i++)
		{
			void (*del)(Record*) = i->second;
			del(inputs.find(i->first)->second);
		}

		// Replace inputs with transformation outcome
		inputs = transform;

		// Replace function pointers
		freelist.clear();
		void (*del)(Record*) = (void (*)(Record*))load_function(libProcess, "pelib_delete");
		for(map<string, Record*>::iterator i = inputs.begin(); i != inputs.end(); i++)
		{
			freelist.insert(pair<const char*, void (*)(Record*)>(i->first.c_str(), del));
		}
	}

	counter = 1;
	for(vector<pelib_argument_stream_t>::const_iterator i = conversion.outputs.begin(); i != conversion.outputs.end() && inputs.size() > 0; i++, counter++)
	{
		/* Load functions from shared libraries */
		void *libOutput;
		if(i->library != NULL)
		{
			libOutput = load_lib(i->library);
		}
		else
		{
			cerr << "[WARNING] No output library specified for output #" << counter << ". Skipping." << endl;
			continue;
		}

		/* Link function handles to function pointers */
		void (*dump)(ostream&, const std::map<string, Record*> &records, size_t, char**) = (void (*)(ostream&, const std::map<string, Record*> &records, size_t, char**))load_function(libOutput, "pelib_dump");

		switch(i->stream)
		{
			case STREAM_STDIN:
				cerr << "[WARNING] Cannot dump data to an input stream for output #" << counter << ". Skipping." << endl;
			case STREAM_STDOUT:
				dump(cout, inputs, i->argc, i->argv);
			break;
			case STREAM_STDERR:
				dump(cerr, inputs, i->argc, i->argv);
			break;
			case STREAM_FILE:
				{
					ofstream myfile(i->filename, ios::out | ios::trunc | ios::binary);
					dump(myfile, inputs, i->argc, i->argv);
					myfile.close();
				}
			break;
			case STREAM_NOTHING:
			default:
				cerr << "[WARNING] No output stream specified for output #" << counter << ". Skipping." << endl;
			break;
		}

		// Destroy dynamic libraries
		destroy_lib(libOutput);

		// Destroy the stream descriptor
		pelib_argument_stream_destroy(*i);
	}

	// Destroy input structures
	for(map<string, Record*>::iterator i = inputs.begin(); i != inputs.end(); i++)
	{
		void (*del)(Record*) = freelist.find(i->first.c_str())->second;
		del(i->second);
	}

	// Destroy input library handlers
	for(map<string, void*>::iterator i = parserList.begin(); i != parserList.end(); i++)
	{
		destroy_lib(i->second);
	}

	// Delete processing dynamic library
	for(vector<void*>::const_iterator i = process.begin(); i != process.end(); i++)
	{
		destroy_lib(*i);
	}

	// Destroy input arguments
	for(vector<pelib_argument_stream_t>::const_iterator i = conversion.inputs.begin(); i != conversion.inputs.end(); i++)
	{
		pelib_argument_stream_destroy(*i);
	}
}

