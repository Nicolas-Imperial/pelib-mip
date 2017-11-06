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


#include <set>
#include <string>

#include <pelib/Algebra.hpp>
#include <pelib/Task.hpp>
#include <pelib/AbstractLink.hpp>
#include <pelib/Platform.hpp>

extern "C"{
#include <igraph.h>
}

#ifndef PELIB_TASKGRAPH
#define PELIB_TASKGRAPH

namespace pelib
{
	using namespace std;

	/** Models a high-level description of a streaming application **/
	class Taskgraph: public Record
	{
		public:
			/** Constructor **/
			Taskgraph();

			/** Copy constructor **/
			Taskgraph(const Taskgraph *graph);
			/** Copy constructor **/
			Taskgraph(const Taskgraph &graph);
			/** Constructor
				@param tasks Set of tasks in the application
				@param links Set of links between tasks in the application
			**/
			Taskgraph(const set<Task> &tasks, const set<AbstractLink> &links);
			/** Constructor
				@param algebra Algebraic representation of a streaming application
			**/
			Taskgraph(const Algebra &algebra);

			/** Destructor **/
			virtual ~Taskgraph();

			/** Returns a pointer to a copy of this taskgraph **/
			virtual
			Taskgraph*
			clone() const;

			/** Builds an algebraic representation of this taskgraph, assuming 1 core **/
			virtual Algebra buildAlgebra() const;

			/** Builds an algebraic representation of this taskgraph, for the Platform given as parameter **/
			virtual Algebra buildAlgebra(const Platform &arch) const;

			/** Get the name of the application i.e. what it computes **/
			virtual string
			getName() const;

			/** Set the name of the application i.e. what it computes **/
			virtual void
			setName(const string name);

			/** Get the formula that computes the deadline for each pipeline stage running this application **/
			virtual string
			getDeadlineCalculator() const;

			/** Computes the deadline for this application given an execution platform **/
			virtual double
			getDeadline(const Platform &arch) const;

			/** Sets the formula that computes the deadline for this applicaion **/
			virtual void
			setDeadlineCalculator(const string deadlineCalculator);

			/** Returns the number of tasks in this application **/
			virtual const set<Task>&
			getTasks() const;

			/** Returns the number of tasks in this application **/
			virtual set<Task>&
			getTasks();

			/** Fins a task in this application using its string identifier **/
			virtual const Task&
			findTask(const string &taskId) const;

			/** Gets all links between tasks in this application **/
			virtual const set<AbstractLink>&
			getLinks() const;

			/** Gets all links between tasks in this application **/
			virtual set<AbstractLink>&
			getLinks();

			/** Copies all tasks and set links to task copies in the taskgraph instance that receives the assignment **/
			virtual Taskgraph&
			operator=(const Taskgraph& copy);

		protected:
			string name, deadlineCalculator;
			set<Task> tasks;
			set<AbstractLink> links;
			
			virtual void
			setLinks(const set<AbstractLink> &link);
		private:		
	};
}

#endif
