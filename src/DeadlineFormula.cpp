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


#include <cmath>

#include <pelib/DeadlineFormula.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/Set.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/Vector.hpp>
#include <pelib/ParseException.hpp>
#include <pelib/CastException.hpp>

#include <pelib/pelib_exprtk.hpp>

#ifdef debug
#undef debug
#endif

#define debug(var) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"" << endl;

using namespace pelib;
using namespace std;

DeadlineFormula::DeadlineFormula(string formula) : formula(formula) {}

double
DeadlineFormula::calculate(const Taskgraph &tg, const Platform &arch) const
{
	if(!arch.isHomogeneous())
	{
		throw CastException("Cannot compute a deadline for a taskgraph with a heterogeneous platform.");
	}

	// Vector of tasks ids, vector of task workloads tau, vector of task max load
	vector<double> n, p, F, tau, W;
	for(set<Task>::const_iterator iter = tg.getTasks().begin(); iter != tg.getTasks().end(); iter++)
	{
		n.push_back((double)std::distance(tg.getTasks().begin(), iter) + 1);
		tau.push_back((double)iter->getWorkload());
		W.push_back((double)iter->getMaxWidth());
	}

	for(double i = 1; i <= (double)arch.getCores().size(); i++)
	{
		p.push_back(i);
	}

	for(set<float>::const_iterator i = (*(arch.getCores().begin()))->getFrequencies().begin(); i != (*(arch.getCores().begin()))->getFrequencies().end(); i++)
	{
		F.push_back(*i);
	}

	double output = parseDeadline(formula, tg.getTasks(), arch, n, p, F, tau, W);
	if(std::isnan(output))
	{
		throw ParseException("Error while parsing formula: " + formula);
	}	

	return output;
}
