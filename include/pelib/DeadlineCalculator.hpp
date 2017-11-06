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


#include <pelib/Platform.hpp>
#include <pelib/Taskgraph.hpp>

#ifndef DEADLINE_CALCULATOR
#define DEADLINE_CALCULATOR

namespace pelib
{
	/** Superclass of any class that can compute a deadline for a Taskgraph given a Platform **/
	class DeadlineCalculator
	{
		public:
		/** Destructor **/
		virtual ~DeadlineCalculator(){}

		/** Computes and returns a deadline value for a taskgraph and platform **/
		virtual double calculate(const Taskgraph& tg, const Platform& arch) const = 0;

		/** Return an instance derived from DeadlineCalculator that can compute a deadline from a string formula
			@param key Formula used by a suitable derived class of DeadlineCalculator to compute a deadline
		**/
		static DeadlineCalculator* getDeadlineCalculator(const std::string key);
	};
}



#endif
