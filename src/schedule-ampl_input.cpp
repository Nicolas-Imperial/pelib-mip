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

#include <pelib/parser.h>
#include <pelib/output.h>

#include <pelib/AmplInput.hpp>
#include <pelib/Schedule.hpp>

#include "schedule-parse.hpp"

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef debug
#undef debug
#endif

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

// /!\ the content of argv is freed after this function is run
pelib::Record*
pelib_parse(std::istream& cin, size_t argc, char **argv, const map<string, Record*> &inputs)
{
	schedule_parse_args_t args = parse_arguments(argc, argv);
	Taskgraph &tg = getTaskgraph(args.taskgraph, inputs);
	Platform &pt = getPlatform(args.platform, inputs);

	Algebra al = AmplInput(AmplInput::floatHandlers()).parse(cin);
	al = al.merge(tg.buildAlgebra());
	al = al.merge(pt.buildAlgebra());

	Schedule *sched = new Schedule("Converted from Algebra", al);

	return sched;
}

void
pelib_delete(Record* rec)
{
	delete rec;
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, const std::map<string, Record*> &records, size_t argc, char **argv)
{
	Schedule *sc = (Schedule*)records.find(typeid(Schedule).name())->second;
	AmplInput(AmplInput::floatHandlers()).dump(cout, sc->buildAlgebra());
}

#ifdef __cplusplus
}
#endif

