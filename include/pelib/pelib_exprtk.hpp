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


#include <string>
#include <set>
#include <vector>

#include <pelib/Task.hpp>
#include <pelib/Platform.hpp>

#ifndef PELIB_EXPRTK
#define PELIB_EXPRTK

namespace pelib
{
	double parseEfficiency(const std::string &formula, double W, double tau, double p);
	double parseDeadline(const std::string &deadline, const std::set<Task> &tasks, const Platform &pt, std::vector<double> n, std::vector<double> p, std::vector<double> F, std::vector<double> tau, std::vector<double> W);
}
#endif
