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

#include <pelib/Buffer.hpp>
#include <pelib/Memory.hpp>
#include <pelib/Task.hpp>

#ifndef PELIB_ABSTRACT_LINK
#define PELIB_ABSTRACT_LINK

namespace pelib
{
	/** Model a link between tasks of a streaming application **/
	class AbstractLink
	{
		public:
			/** Constructor. Takes the producer and consumer tasks at both ends of the link **/
			AbstractLink(const Task &producer, const Task &consumer, const std::string &producerName, const std::string &consumerName, size_t producer_rate = 0, size_t consumer_rate = 0);
			AbstractLink(const AbstractLink& link);
			virtual ~AbstractLink();

			/** Returns a pointer to the producer task **/
			virtual const Task* getProducer() const;

			/** Returns a pointer to the consumer task **/
			virtual const Task* getConsumer() const;

			/** Returns a pointer to the producer task **/
			virtual void setProducer(Task*);

			/** Returns a pointer to the consumer task **/
			virtual void setConsumer(Task*);

			/** Check if this link is striclty inferior to another link given 
				@param other Other link to compare this instance to
			**/
			virtual bool
			operator<(const AbstractLink &other) const;

			/** Returns true if this instance is considered equivalent to another link
				@param other Other link to be compared
			**/
	    		virtual bool
			operator==(const AbstractLink &other) const;

			size_t
			getProducerRate() const;

			size_t
			getConsumerRate() const;

			const std::string&
			getDataType() const;

			const std::string&
			getHeader() const;

			const std::string&
			getProducerName() const;

			const std::string&
			getConsumerName() const;


			const AbstractLink&
			getLink() const
			{
				return *this;
			}

			const Buffer&
			getQueueBuffer() const
			{
				Buffer *buffer = new Buffer();
				return *buffer;
			}

			const Memory&
			getProducerMemory() const
			{
				Memory *mem = new Memory();
				return *mem;
			}
			const Memory&
			getConsumerMemory() const
			{
				Memory *mem = new Memory();
				return *mem;
			}	
		protected:
			/** Producer and consumer task pointers **/
			const Task *producer, *consumer;
			std::string producerName, consumerName;
			size_t producer_rate, consumer_rate;
			std::string dataType, header;
		private:		
	};
}

#endif
