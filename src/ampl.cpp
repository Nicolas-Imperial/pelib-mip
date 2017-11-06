/*
 Copyright 2015 Nicolas Melot

 This file is part of Crown.

 Crown is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Crown is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Crown. If not, see <http://www.gnu.org/licenses/>.

*/


#include <fstream>

#include <pelib/Algebra.hpp>
#include <pelib/AmplSolver.hpp>
#include <pelib/calculus.h>

#ifdef debug
#undef debug
#endif

#define debug(var) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << endl;

using namespace std;
using namespace pelib;

struct args
{
	char *model, *run, *model_file;
	bool showError, showOutput;
};
typedef struct args args_t;

static args_t
parse(char **args)
{
	args_t out;
	out.model = NULL;
	out.model_file = (char*)"model.mod";
	out.run = NULL;
	out.showError = false;
	out.showOutput = false;

	for(; args[0] != NULL; args++)
	{
		if(strcmp(args[0], "--model") == 0)
		{
			args++;
			out.model = args[0];
			continue;
		}
		if(strcmp(args[0], "--model-name") == 0)
		{
			args++;
			out.model_file = args[0];
			continue;
		}
		if(strcmp(args[0], "--run") == 0)
		{
			args++;
			out.run = args[0];
			continue;
		}
		if(strcmp(args[0], "--show-stdout") == 0)
		{
			out.showOutput = true;
			continue;
		}
		if(strcmp(args[0], "--show-stderr") == 0)
		{
			out.showError = true;
			continue;
		}
	}

	return out;
}

const Algebra*
pelib_calculus(const map<const string, const Algebra> &data, size_t argc, char**argv, map<const string, double> &statistics)
{
	args_t args = parse(argv);
	ifstream model(args.model);
	ifstream run(args.run);
	const Algebra *res = AmplSolver(model, args.model_file, run, data, args.showOutput, args.showError).solve(statistics);
	return res;
}

void
pelib_delete(Algebra *data)
{
	delete data;
}

