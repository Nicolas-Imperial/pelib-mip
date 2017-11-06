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

#include <pelib/Set.hpp>
#include <pelib/AmplInput.hpp>
#include <pelib/Platform.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef debug
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#endif

// /!\ the content of argv is freed after this function is run
pelib::Record*
pelib_parse(std::istream& cin, size_t argc, char **argv, const map<string, Record*> &input)
{
	AmplInput reader(AmplInput::floatHandlers());
	std::string line;
	Algebra alg_arch = reader.parse(cin);

	Platform *arch = new Platform(alg_arch);

	return arch;
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, const std::map<string, Record*> &records, size_t argc, char **argv)
{
	const Platform *arch = (Platform*)records.find(typeid(Platform).name())->second;

	AmplInput output(AmplInput::intFloatHandlers());
	Algebra alg = arch->buildAlgebra();
	output.dump(cout, alg);
}

void
pelib_delete(Record *obj)
{
	delete obj;
}

#ifdef __cplusplus
}
#endif

