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


#include <iostream>
#include <sstream>
#include <string>

#include <pelib/pelib_exprtk.hpp>

#include <pelib/ExecTask.hpp>
#include <pelib/ParseException.hpp>

#ifdef debug
#undef debug
#endif

#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

using namespace std;

namespace pelib
{
	ExecTask::ExecTask(const Task& task) : task(task)
	{
		this->frequency = 1;
		this->width = 1;
		this->start = 0;
		this->instance = 0;
		this->master = 0;

		this->producers.clear();
		this->consumers.clear();
	}

	ExecTask::ExecTask(const Task& task, const std::set<AllotedLink> &links, double frequency, double width, double start, unsigned int instance, unsigned int master, const Memory &sync) : task(task), sync(sync)
	{
		this->frequency = frequency;
		this->width = width;
		this->start = start;
		this->instance = instance;
		this->master = master;

		this->producers.clear();
		this->consumers.clear();
		this->importLinks(links);
	}

	ExecTask::ExecTask(const Task& task, const std::set<const AllotedLink*> &links, double frequency, double width, double start, unsigned int instance, unsigned int master, const Memory &sync) : task(task), sync(sync)
	{
		this->frequency = frequency;
		this->width = width;
		this->start = start;
		this->instance = instance;
		this->master = master;

		this->producers.clear();
		this->consumers.clear();
		this->importLinks(links);
	}

	ExecTask::ExecTask(const ExecTask &task) : task(task.getTask()), sync(task.getMemory())
	{
		this->frequency = task.getFrequency();
		this->width = task.getWidth();
		this->start = task.getStart();
		this->instance = task.getInstance();
		this->master = task.getMasterCore();

		this->producers.clear();
		this->consumers.clear();
		this->importLinks(task.getProducers());
		this->importLinks(task.getConsumers());
	}

	ExecTask::~ExecTask()
	{
		/* Do nothing */
	}
	
	double
	ExecTask::getFrequency() const
	{
		return frequency;
	}

	void
	ExecTask::setFrequency(double frequency)
	{
		this->frequency = frequency; 
	}

	double
	ExecTask::getWidth() const
	{
		return width;
	}

	double
	ExecTask::getStart() const
	{
		return this->start; 
	}

	unsigned int
	ExecTask::getInstance() const
	{
		return this->instance;
	}

	const Task&
	ExecTask::getTask() const
	{
		return this->task;
	}

	void
	ExecTask::importLinks(const set<const AllotedLink*> &links)
	{
		for(set<const AllotedLink*>::const_iterator i = links.begin(); i != links.end(); i++)
		{
			if(*(*i)->getLink().getConsumer() == this->getTask())
			{
				this->producers.insert(*i);
			}
			if(*(*i)->getLink().getProducer() == this->getTask())
			{
				this->consumers.insert(*i);
			}
		}
	}

	void
	ExecTask::importLinks(const set<AllotedLink> &links)
	{
		for(set<AllotedLink>::const_iterator i = links.begin(); i != links.end(); i++)
		{
			if(*i->getLink().getConsumer() == this->getTask())
			{
				this->producers.insert(&*i);
			}
			if(*i->getLink().getProducer() == this->getTask())
			{
				this->consumers.insert(&*i);
			}
		}
	}

	const std::set<const AllotedLink*>&
	ExecTask::getProducers() const
	{
		return this->producers;
	}

	const std::set<const AllotedLink*>&
	ExecTask::getConsumers() const
	{
		return this->consumers;
	}

	bool
	ExecTask::operator<(const ExecTask &other) const
	{
		if(this->getStart() == other.getStart())
		{
			if(this->getInstance() == other.getInstance())
			{
				return this->getTask() < other.getTask();
			}
			else
			{
				return this->getInstance() < other.getInstance();
			}
		}
		else
		{
			return this->getStart() < other.getStart();
		}
	}
    
	bool
	ExecTask::operator==(const ExecTask &other) const
	{
		return this->getStart() == other.getStart() && this->getTask() == other.getTask() && this->getInstance() == other.getInstance();
	}

	set<const AllotedLink*>
	ExecTask::allLinks() const
	{
		std::set<const AllotedLink*> output;
		std::set_union(this->getProducers().cbegin(), this->getProducers().cend(), this->getConsumers().begin(), this->getConsumers().cend(), std::inserter(output, output.begin()));
//<std::set<const AllotedLink*>::const_iterator, std::set<const AllotedLink*>::const_iterator, std::set<const AllotedLink*>::iterator>
		return output;
	}

	const Memory&
	ExecTask::getMemory() const
	{
		return this->sync;
	}

	unsigned int
	ExecTask::getMasterCore() const
	{
		return this->master;
	}
}

