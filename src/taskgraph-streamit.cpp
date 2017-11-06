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

#include <pelib/AmplOutput.hpp>
#include <pelib/AmplInput.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/Task.hpp>
#include <pelib/AbstractLink.hpp>
#include <pelib/Taskgraph.hpp>

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
pelib_parse(std::istream& cin, size_t argc, char **argv, const map<string, Record*> &inputs)
{
	using namespace std;
	using namespace pelib;

	AmplOutput ampl_output(AmplOutput::intFloatHandlers());
	AmplInput ai(AmplInput::floatHandlers());

/*
	std::cout << std::setprecision(6)
		<< std::setiosflags(std::ios::fixed)
		<< std::setiosflags(std::ios::showpoint);
*/

	Algebra data = ai.parse(cin);
	const Matrix<int, int, float> *taskworkcomm = data.find<Matrix<int, int, float> >("taskworkcomm");
	set<Task> tasks;
	set<AbstractLink> links;

	for(map<int, map<int, float> >::const_iterator i = taskworkcomm->getValues().begin(); i != taskworkcomm->getValues().end(); i++)
	{
		stringstream estr;
		estr << "task_" << i->first;
		Task t(estr.str());
		stringstream efficiency;

		float work = i->second.find(1)->second;
		float comm = i->second.find(2)->second;

		t.setWorkload(work);
		efficiency << "fml: p == 1 ? 1 : p <= W ? tau / (tau + p * ";
		if(isfinite(comm))
		{
			t.setMaxWidth(std::numeric_limits<double>::infinity());
			efficiency << comm;
		}
		else
		{
			t.setMaxWidth(1);
			efficiency << std::numeric_limits<float>::max();
		}
		efficiency << ") : 1e-6";
		t.setEfficiencyString(efficiency.str());

		tasks.insert(t);
	}

	char* autname;
	if(argc > 1)
	{
		autname = argv[1];
	}
	else
	{
		autname = (char*)string("converted_from_streamit").c_str();
	}
	Taskgraph *tg = new Taskgraph(tasks, links);
	tg->setName(autname);
	tg->setDeadlineCalculator("class:synthetic");

	return tg;
}

void
pelib_delete(Record *rec)
{
	delete rec;
}

#ifdef __cplusplus
}
#endif

