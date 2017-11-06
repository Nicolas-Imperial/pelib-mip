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


#include <pelib/Schedule.hpp>
#include <pelib/Taskgraph.hpp>

#include <pelib/XMLScheduleParser.hpp>
#include <pelib/XMLScheduleOutput.hpp>

#ifndef PELIB_XMLSCHEDULE_H
#define PELIB_XMLSCHEDULE_H

namespace pelib
{
	/** Parser and output class for pelib::Schedule in SML format **/
	class XMLSchedule : public XMLScheduleParser, public XMLScheduleOutput
	{
		public:
			/** Destructor **/
			virtual ~XMLSchedule();
			/** Writes the content of instance of pelib::Schedule into output stream in XML format, using additional information from associated taskgraph and platform **/
			virtual	void dump(std::ostream &os, const Schedule *data) const;
			/** Writes the content of instance of pelib::Schedule into output stream in XML format, using additional information from associated taskgraph and platform **/
			virtual	void dump(std::ostream &os, const Schedule &data) const;
			/** Read text input in XML format and creates an instance of pelib::Schedule containing all data read **/
			virtual Schedule* parse(std::istream &is, const Taskgraph &tg, const Platform &pt) const;
			/** Returns a pointer to a copy of this schedule parser and output **/
			virtual XMLSchedule* clone() const;
	};
}

#endif
