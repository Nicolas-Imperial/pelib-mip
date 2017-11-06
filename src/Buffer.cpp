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

#include <pelib/Buffer.hpp>
#include <pelib/Memory.hpp>
#include <pelib/PelibException.hpp>

#ifdef debug
#undef debug
#endif

#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

using namespace std;

namespace pelib
{
	Buffer::Buffer(size_t size, const string &type, const string &header, const Memory &memory)
	{
		this->type = string(type);
		this->size = size;
		this->header = header;
		this->memory = memory;
	}

	Buffer::Buffer()
	{
		this->type = string();
		this->size = 0;
		this->header = string();
		this->memory = Memory::nullMemory();
	}

	size_t
	Buffer::getSize() const
	{
		return this->size;
	}

	const Memory&
	Buffer::getMemory() const
	{
		return this->memory;
	}

	const std::string&
	Buffer::getHeader() const
	{
		return this->header;
	}

	const std::string&
	Buffer::getType() const
	{
		return this->type;
	}

	Buffer
	Buffer::nullBuffer()
	{
		return Buffer(0, "", "", Memory::nullMemory()); 
	}

	/*
	bool
	operator<(const Buffer &buffer) const
	{
		
	}
	*/

	bool
	Buffer::operator==(const Buffer &buffer) const
	{
		return this->type.compare(buffer.getType()) == 0 &&
			this->size == buffer.getSize() &&
			this->header.compare(buffer.getHeader()) == 0 &&
			this->memory == buffer.getMemory();
	}
}
