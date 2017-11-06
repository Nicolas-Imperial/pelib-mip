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

#include <pelib/PelibException.hpp>
#include "schedule-parse.hpp"

using namespace std;
using namespace pelib;

#ifndef debug
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#endif

#ifdef __cplusplus
extern "C"
{
#endif
schedule_parse_args_t
parse_arguments(size_t argc, char** argv)
{
	schedule_parse_args_t args;
	args.taskgraph = string(typeid(Taskgraph).name());
	args.platform = string(typeid(Platform).name());

	for(; *argv != NULL; argv++)
	{
		if(string(*argv).compare("--taskgraph") == 0)
		{
			argv++;
			args.taskgraph = string(*argv);
			continue;
		}

		if(string(*argv).compare("--platform") == 0)
		{
			argv++;
			args.platform = string(*argv);
			continue;
		}
	}

	return args; 
}
#ifdef __cplusplus
}
#endif

Taskgraph&
getTaskgraph(const string &name, const map<string, Record*> &inputs)
{
	Taskgraph *tg = NULL;
	map<string, Record*>::const_iterator i;
	i = inputs.find(name);
	if(i != inputs.end())
	{
		tg = (Taskgraph*)i->second;
	}
	else
	{
		throw PelibException("Could not find corresponding taskgraph in record");
	}

	return *tg;
}

Platform&
getPlatform(const string &name, const map<string, Record*> &inputs)
{
	Platform *pt = NULL;
	map<string, Record*>::const_iterator i;
	i = inputs.find(name);
	if(i != inputs.end())
	{
		pt = (Platform*)i->second;
	}
	else
	{
		throw PelibException("Could not find corresponding platform in record");
	}

	return *pt;
}

