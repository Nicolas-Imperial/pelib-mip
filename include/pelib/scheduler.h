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

#include <pelib/Taskgraph.hpp>
#include <pelib/Platform.hpp>
#include <pelib/Schedule.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CROWN_H
#define CROWN_H
	const pelib::Schedule* pelib_schedule(const pelib::Taskgraph &tg, const pelib::Platform &pt, size_t argc, char **argv, std::map<const std::string, double> &statistics);
	std::string pelib_description(size_t argc, char **argv);
	void pelib_delete(pelib::Schedule *sched);
#endif

#ifdef __cplusplus
}
#endif
