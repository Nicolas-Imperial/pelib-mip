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

#include <pelib/TetrisScheduleOutput.hpp>

#ifndef PELIB_TETRISSCHEDULE_H
#define PELIB_TETRISSCHEDULE_H

namespace pelib
{
	/** Parser and output class for pelib::Schedule in Tetris format **/
	class TetrisSchedule : public TetrisScheduleOutput
	{
		public:
			TetrisSchedule();
			TetrisSchedule(float ratio, bool showFrequencies, bool showTaskId, bool useTaskName, vector<unsigned int> colors, float strokeSize);
			/** Destructor **/
			virtual ~TetrisSchedule();
			/** Writes the content of instance of pelib::Schedule into output stream in Tetris format, using additional information from associated taskgraph and platform **/
			virtual	void dump(std::ostream &os, const Schedule *data) const;
			/** Writes the content of instance of pelib::Schedule into output stream in Tetris format, using additional information from associated taskgraph and platform **/
			virtual	void dump(std::ostream &os, const Schedule &data) const;
			/** Read text input in Tetris format and creates an instance of pelib::Schedule containing all data read **/
			virtual TetrisSchedule* clone() const;
			static float defaultRatio();
			static float defaultStrokeSize();
			static bool defaultFrequencyLegend();
			static bool defaultTaskLabel();
			static bool defaultTaskId();
			static vector<uint32_t> defaultFrequencyColors();
		protected:
			float ratio, strokeSize;
			bool showFrequencies;
			bool useTaskName;
			bool showTaskId;
			vector<uint32_t> colors;
	};
}

#endif
