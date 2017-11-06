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

#ifndef PELIB_TASK
#define PELIB_TASK

namespace pelib
{
	// Forward declaration
	/** Forward declaration of a link between tasks **/
	class AbstractLink;

	/** Models a moldable task of a streaming application **/
	class Task
	{
		public:
			Task();
			/** Constructor
				@param id Identifier of the task
			**/
			Task(const std::string &id);
			/** Copy constructor **/
			Task(const Task&);

			/** Destructor **/
			virtual ~Task();
			
			/** Gets the name of the module containing the code run by this task **/
			virtual const std::string&
			getModule() const;

			/** Sets the name of the module containing the code run by this task **/
			virtual void
			setModule(const std::string &name);

			/** Gets the name (identifier string) of the task **/
			virtual const std::string&
			getName() const;

			/** Gets the formula that defines the overhead of the parallelization of this task **/
			virtual const std::string&
			getEfficiencyString() const;

			/** Sets the formula that defines the overhead of the parallelization of this task **/
			virtual void
			setEfficiencyString(const std::string &efficiencyString);

			/** Computes the efficiency of the task, given its efficiency formula
				@param p Get efficiency of the task for p cores
				@param def Default efficiency if p is higher than the maximal width of the task
			**/
			virtual double
			getEfficiency(int p, double def = minEfficiency) const;
			
			/** Gets the workload, in number of instructions, of the task **/
			virtual double
			getWorkload() const;

			/** Sets the workload, in number of instructions, of the task **/
			virtual void
			setWorkload(double workload);
			
			/** Gets the maximum number of cores that can run this task in parallel in an efficient manner **/
			virtual double
			getMaxWidth() const;

			/** Sets the maximum number of cores that can run this task in parallel in an efficient manner **/
			virtual void
			setMaxWidth(double maxWidth);

			/** Computes and returns the global run time of this task assuming a given width and frequency
				@param width Assume the task run on that many cores
				@param frequency Assumes the task runs at this frequency
			**/
			virtual double
			runtime(double width = 1, double frequency = 1) const;

			/** Allows the comparison of tasks. Used with ==, allows the test of difference **/
			virtual bool
			operator<(const Task &other) const;

			/** Get the set of task that produce inputs for this task **/
			virtual const std::set<const AbstractLink*>&
			getProducers() const;

			/** Get the set of task that consume outputs from this task **/
			virtual const std::set<const AbstractLink*>&
			getConsumers() const;

			/** Get the set of task that produce inputs for this task **/
			virtual std::set<const AbstractLink*>&
			getProducers();

			/** Get the set of task that consume outputs from this task **/
			virtual std::set<const AbstractLink*>&
			getConsumers();

	const Task&
	getTask() const
	{
		return *this;
	}

	double
	getWidth() const
	{
		return 1;
	}

	double
	getFrequency() const
	{
		return 1;
	}
	
	double
	getStart() const
	{
		return 0;
	}

	unsigned int
	getInstance() const
	{
		return 0;
	}

			/** Allows the comparison of tasks. Used with <, allows the test of difference **/
	    		virtual bool
			operator==(const Task &other) const;

		protected:
			/** Workload in number of instructions and maximum number of cores able to run this task **/
			double workload, maxWidth;
			/** Name, source code module name and efficiency formula of the task **/
			std::string name, module, efficiencyString;
			/** Sets of producers and consumers linked to this task **/
			std::set<const AbstractLink*> consumers, producers;

			/** Default efficiency value when computing efficiency for more cores that this task can support **/			
			static const float minEfficiency;
			
		private:
	};
}

#endif
