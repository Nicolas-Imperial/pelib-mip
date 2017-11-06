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

#include <pelib/Memory.hpp>

#ifndef PELIB_BUFFER
#define PELIB_BUFFER

namespace pelib
{
	/** Model a link between tasks of a streaming application **/
	class Buffer
	{
		public:
			/** Constructor. Takes the producer and consumer tasks at both ends of the link **/
			Buffer(size_t size, const std::string &type, const std::string &header, const Memory &memory);
			Buffer();

			size_t
			getSize() const;

			const Memory&
			getMemory() const;

			const std::string&
			getHeader() const;
		
			const std::string&
			getType() const;

			static Buffer
			nullBuffer();

			//virtual bool
			//operator<(const Buffer &buffer) const;

			virtual bool
			operator==(const Buffer &buffer) const;
		protected:
			/** Producer and consumer task pointers **/
			std::string type;
			size_t size;
			std::string header;
			Memory memory;

		private:		
	};
}

#endif
