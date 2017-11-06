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

#include <pelib/Task.hpp>
#include <pelib/Memory.hpp>
#include <pelib/AllotedLink.hpp>

#ifndef PELIB_EXECTASK
#define PELIB_EXECTASK

namespace pelib
{
	// Forward declaration
	/** Forward declaration of a link between tasks **/
	class AbstractLink;

	/** Models a moldable task of a streaming application **/
	class ExecTask
	{
		public:
			/** Constructor
				@param id Identifier of the task
			**/
			ExecTask(const Task& task);
			ExecTask(const Task& task, const std::set<AllotedLink> &links, double frequency, double width, double start, unsigned int instance, unsigned int master, const Memory &sync);
			ExecTask(const Task& task, const std::set<const AllotedLink*> &links, double frequency, double width, double start, unsigned int instance, unsigned int master, const Memory &sync);
			/** Copy constructor **/
			ExecTask(const ExecTask&);

			/** Destructor **/
			virtual ~ExecTask();
			
			/** Return the frequency, in KHz, the task runs at **/
			virtual double
			getFrequency() const;

			/** Sets the frequency at which the task runs **/
			virtual void
			setFrequency(double frequency);

			/** Get the number of processors that run this task **/
			virtual double
			getWidth() const;

			const Task&
			getTask() const;

			virtual double
			getStart() const;

			unsigned int
			getInstance() const;

			const std::set<const AllotedLink*>&
			getProducers() const;

			const std::set<const AllotedLink*>&
			getConsumers() const;

			/** Allows the comparison of tasks. Used with ==, allows the test of difference **/
			virtual bool
			operator<(const ExecTask &other) const;

	    		virtual bool
			operator==(const ExecTask &other) const;

			std::set<const AllotedLink*>
			allLinks() const;

			const Memory&
			getMemory() const;

			unsigned int
			getMasterCore() const;

		protected:
			void importLinks(const std::set<AllotedLink> &links);
			void importLinks(const std::set<const AllotedLink*> &links);
			const Task &task;
			/** Frequency and width allocated to this task **/
			double frequency, width, start;
			unsigned int instance, master;
			Memory sync;
			/** Sets of producers and consumers linked to this task **/
			std::set<const AllotedLink*> consumers, producers;
			
		private:
	};
}

#endif
