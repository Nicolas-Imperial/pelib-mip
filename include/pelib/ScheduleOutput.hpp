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
#include <pelib/RecordOutput.hpp>

#ifndef PELIB_SCHEDULEOUTPUT
#define PELIB_SCHEDULEOUTPUT

namespace pelib
{
	/** Base class for an output class that outputs instances of pelib::Schedule to output streams **/
	class ScheduleOutput : public RecordOutput
	{
		public:	
			/** Returns a pointer to a copy of this class instance **/
			virtual ScheduleOutput* clone() const = 0;

			/** Writes a schedule into output stream with support from associated taskgraph and execution platform, depending on details required
				@param os Output stream where the schedule is written
				@param data Instance of pelib::Schedule to be written in output stream
				@param tg Additional data from the taskgraph associated to this schedule
				@parame pt Additional data from the platform associated to this schedule
			**/
			virtual	void dump(std::ostream &os, const Schedule *data) const = 0;

			/** Writes a schedule into output stream with support from associated taskgraph and execution platform, depending on details required
				@param os Output stream where the schedule is written
				@param data Instance of pelib::Schedule to be written in output stream
				@param tg Additional data from the taskgraph associated to this schedule
				@parame pt Additional data from the platform associated to this schedule
			**/
			virtual	void dump(std::ostream &os, const Schedule &data) const = 0;

		protected:
		private:		
	};
}

#endif
