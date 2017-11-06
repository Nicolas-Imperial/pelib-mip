/*
 Copyright 2017 Nicolas Melot

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

#include <string>

#include <pelib/Record.hpp>
#include <pelib/Taskgraph.hpp>
#include <pelib/Platform.hpp>
#include <pelib/parser.h>

#ifndef SCHEDULE_PARSE
#define SCHEDULE_PARSE 1

typedef struct
{
	std::string taskgraph, platform;
} schedule_parse_args_t;

#ifdef __cplusplus
extern "C"
{
#endif
schedule_parse_args_t
parse_arguments(size_t argc, char** argv);
#ifdef __cplusplus
}
#endif

pelib::Taskgraph&
getTaskgraph(const std::string &name, const std::map<std::string, pelib::Record*> &inputs);

pelib::Platform&
getPlatform(const std::string &name, const std::map<std::string, pelib::Record*> &inputs);

#endif
