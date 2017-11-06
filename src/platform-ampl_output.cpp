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
#include <pelib/DummyCore.hpp>
#include <pelib/AmplOutput.hpp>
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
	AmplOutput reader(AmplOutput::floatHandlers());
	std::string line;
	Algebra alg_arch = reader.parse(cin);

	const Scalar<float> *scalar_p = alg_arch.find<Scalar<float> >("p");
	const Scalar<float> *f_unit = alg_arch.find<Scalar<float> >("Funit");
	const Set<float> *set_F = alg_arch.find<Set<float> >("F");

	if(scalar_p == NULL || set_F == NULL)
	{
		throw ParseException(std::string("Missing core number scalar \"p\" or frequency set \"F\" in input."));
	}

	set<const Core*, Core::LessCorePtrByCoreId> cores;
	for(size_t i = 0; i < (size_t)scalar_p->getValue(); i++)
	{
		cores.insert(new DummyCore(set_F->getValues(), f_unit->getValue()));
	}

	Platform *arch = new Platform(cores);
	
	return arch;
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, const std::map<string, Record*> &records, size_t argc, char **argv)
{
	const Platform *arch = (Platform*)records.find(typeid(Platform).name())->second;

	AmplOutput output(AmplOutput::intFloatHandlers());
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

