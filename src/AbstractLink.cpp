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
#include <iostream>
#include <sstream>
#include <string.h>

#include <pelib/AbstractLink.hpp>
#include <pelib/PelibException.hpp>

#ifdef debug
#undef debug
#endif

#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

using namespace std;

namespace pelib
{
	AbstractLink::AbstractLink(const Task &producer, const Task &consumer, const std::string &producerName, const std::string &consumerName, size_t producer_rate, size_t consumer_rate) 
	{
		this->producerName = producerName;
		this->consumerName = consumerName;
		this->producer = (Task*)&producer;
		this->consumer = (Task*)&consumer;
		this->producer_rate = producer_rate;
		this->consumer_rate = consumer_rate;
	}

	AbstractLink::AbstractLink(const AbstractLink& link)
	{
		this->producer = link.getProducer();
		this->consumer = link.getConsumer();
		this->producerName = link.getProducerName();
		this->consumerName = link.getConsumerName();
		this->producer_rate = link.getProducerRate();
		this->consumer_rate = link.getConsumerRate();
		this->dataType = link.getDataType();
		this->header = link.getHeader();
	}

	AbstractLink::~AbstractLink()
	{
		// Do nothing
	}

	const Task*
	AbstractLink::getProducer() const
	{
		return this->producer;
	}

	const Task*
	AbstractLink::getConsumer() const
	{
		return this->consumer;
	}

	const std::string&
	AbstractLink::getDataType() const
	{
		return this->dataType;
	}

	void
	AbstractLink::setProducer(Task* producer)
	{
		this->producer = producer;
	}

	void
	AbstractLink::setConsumer(Task* consumer)
	{
		this->consumer = consumer;
	}

	size_t
	AbstractLink::getProducerRate() const
	{
		return this->producer_rate;
	}

	size_t
	AbstractLink::getConsumerRate() const
	{
		return this->consumer_rate;
	}

	const std::string&
	AbstractLink::getHeader() const
	{
		return this->header;
	}

	const std::string&
	AbstractLink::getProducerName() const
	{
		return this->producerName;
	}

	const std::string&
	AbstractLink::getConsumerName() const
	{
		return this->consumerName;
	}

	bool
	AbstractLink::operator<(const AbstractLink &other) const
	{
		Task thisProducer = *this->getProducer();
		Task otherProducer = *other.getProducer();
		Task thisConsumer = *this->getConsumer();
		Task otherConsumer = *other.getConsumer();

		if(thisProducer == otherProducer && thisConsumer == otherConsumer)
		{
			if(this->getProducerName().compare(other.getProducerName()) == 0)
			{
				return this->getConsumerName() < other.getConsumerName();
			}
			else
			{
				return this->getProducerName() < other.getProducerName();
			}
		}

		if(thisProducer == otherProducer)
		{
			return thisConsumer < otherConsumer;
		}
		else
		{
			return thisProducer < otherProducer;
		}	
	}

	bool
	AbstractLink::operator==(const AbstractLink &other) const
	{	
		bool res = (*this->getProducer() == *other.getProducer()) && (*this->getConsumer() == *other.getConsumer()) && (this->getProducerName().compare(other.getProducerName()) == 0) && (this->getConsumerName().compare(other.getConsumerName()) == 0);
		return res;
	}
}
