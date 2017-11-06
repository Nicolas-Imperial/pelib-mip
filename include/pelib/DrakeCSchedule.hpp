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

#include <pelib/DrakeCScheduleOutput.hpp>

#ifndef PELIB_DRAKECSCHEDULE_H
#define PELIB_DRAKECSCHEDULE_H

namespace pelib
{
	/** Generate C code containing all metadata about a drake streaming application **/
	class DrakeCSchedule : public DrakeCScheduleOutput
	{
		public:
			/** Destructor **/
			virtual ~DrakeCSchedule();
			/** Generate C code to output stream, that holds all Taskgraph, Platform and Schedule information required for a drake application
				@param os Output stream where generated C code is written
				@param data Schedule of the Drake application
				@param tg Taskgraph of the application
				@param pt Platform description for which the Drake application is scheduled for
			**/
			virtual	void dump(std::ostream &os, const Schedule *data) const;
			/** Generate C code to output stream, that holds all Taskgraph, Platform and Schedule information required for a drake application
				@param os Output stream where generated C code is written
				@param data Schedule of the Drake application
				@param tg Taskgraph of the application
				@param pt Platform description for which the Drake application is scheduled for
			**/
			virtual	void dump(std::ostream &os, const Schedule &data) const;
			/** Returns a pointer to a copy of this instance **/
			virtual DrakeCSchedule* clone() const;
	};
}

#endif
