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
#include <pelib/RecordParser.hpp>

#ifndef PELIB_SCHEDULEPARSER
#define PELIB_SCHEDULEPARSER

namespace pelib
{
	/** Superclass of any parser able to read an input stream and produce an instance of pelib::Schedule from the data read **/
	class ScheduleParser : public RecordParser
	{
		public:
			/** Read the content of input stream and produces an instance of pelib::Schedule **/
			virtual
			Schedule*
			parse(std::istream &in, const Taskgraph &tg, const Platform &pt) const = 0;

			/** Returns a pointer to a copy of this Schedule parser **/
			virtual ScheduleParser* clone() const = 0;

		protected:
		private:
	};
}

#endif
