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

#include <pelib/AlgebraRawOutput.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef debug
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#endif

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& out, const std::map<string, Record*> &records, size_t argc, char **argv)
{
	//out.precision(6);
	//out << std::ios::fixed;

	out.setf(ios::fixed);
	out << setprecision(6);
	vector<string> list;
	for(size_t i = 0; i < argc; i++)
	{
		list.push_back(argv[i]);
	}
	Algebra al = *(Algebra*)(records.find(typeid(Algebra).name())->second);
	AlgebraRawOutput(AlgebraRawOutput::intFloatOutputs(), list).dump(out, al);
}

void
pelib_delete(Record* rec)
{
	delete rec;
}

#ifdef __cplusplus
}
#endif

