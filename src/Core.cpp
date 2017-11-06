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


#include <pelib/Core.hpp>

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;
using namespace std;

namespace pelib
{
	size_t Core::counter = 0;
	Core::Core()
	{
		this->id = Core::counter++;
	}	

	Core::Core(size_t id)
	{
		this->id = id;
	}

	Core::~Core()
	{
		// Do nothing
	}

	
	bool
	Core::operator<(const Core &other) const
	{
		return this->id < other.id;
	}

	bool
	Core::operator==(const Core &other) const
	{
		return this->id == other.id;
	}
}
