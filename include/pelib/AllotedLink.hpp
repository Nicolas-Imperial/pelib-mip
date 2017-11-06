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

#include <pelib/AbstractLink.hpp>
#include <pelib/Buffer.hpp>
#include <pelib/Memory.hpp>

#ifndef PELIB_ALLOTED_LINK
#define PELIB_ALLOTED_LINK

namespace pelib
{
	/** Model a link between tasks of a streaming application **/
	class AllotedLink
	{
		public:
			/** Constructor. Takes the producer and consumer tasks at both ends of the link **/
			AllotedLink(const AbstractLink &link, const Buffer &queue, const Memory &producer_buffer, const Memory &consumer_buffer);

			AllotedLink(const AllotedLink &link);
			virtual ~AllotedLink();

			virtual bool
			operator<(const AllotedLink &other) const;

			/** Returns true if this instance is considered equivalent to another link
				@param other Other link to be compared
			**/
	    		virtual bool
			operator==(const AllotedLink &other) const;

			const AbstractLink&
			getLink() const;

			const Buffer&
			getQueueBuffer() const;

			const Memory&
			getProducerMemory() const;

			const Memory&
			getConsumerMemory() const;

		protected:
			const AbstractLink &link;
			Buffer queueBuffer;
			Memory producerMemory, consumerMemory;

		private:		
	};
}

#endif
