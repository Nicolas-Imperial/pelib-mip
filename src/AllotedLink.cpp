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

#include <pelib/AllotedLink.hpp>
#include <pelib/PelibException.hpp>

#ifdef debug
#undef debug
#endif

#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

using namespace std;

namespace pelib
{
	AllotedLink::AllotedLink(const AbstractLink &link, const Buffer &queue, const Memory &producer, const Memory &consumer) : link(link), queueBuffer(queue), producerMemory(producer), consumerMemory(consumer)
	{
		// Do nothing else
	}

	AllotedLink::AllotedLink(const AllotedLink &link) : link(link.getLink())
	{
		this->queueBuffer = link.getQueueBuffer();
		this->producerMemory = link.getProducerMemory();
		this->consumerMemory = link.getConsumerMemory();
	}

	AllotedLink::~AllotedLink()
	{
		// Do nothing
	}

	const AbstractLink&
	AllotedLink::getLink() const
	{
		return this->link;
	}

	const Buffer&
	AllotedLink::getQueueBuffer() const
	{
		return this->queueBuffer;
	}

	const Memory&
	AllotedLink::getProducerMemory() const
	{
		return this->producerMemory;
	}

	const Memory&
	AllotedLink::getConsumerMemory() const
	{
		return this->consumerMemory;
	}

	bool
	AllotedLink::operator<(const AllotedLink &other) const
	{
		return this->getLink() < other.getLink();
	}

	bool
	AllotedLink::operator==(const AllotedLink &other) const
	{	
		return this->getLink() == other.getLink() && this->getQueueBuffer() == other.getQueueBuffer() && this->getProducerMemory() == other.getProducerMemory() && this->getConsumerMemory() == other.getConsumerMemory();
	}
}
