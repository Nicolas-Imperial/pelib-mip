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
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/PelibException.hpp>

namespace pelib
{
	PelibException::PelibException(const std::string &message): msg(message)
	{
		// Do nothing else
	}

	PelibException::PelibException(const char *message): msg(message)
	{
		// Do nothing else
	}

	PelibException::~PelibException() throw()
	{
		// Do nothing
	}

	const char*
	PelibException::what() const throw()
	{
		std::string fin = std::string("Pelib exception: ").append(this->msg).c_str();
		const char *message = fin.c_str();
		char *cpy = (char*)malloc(sizeof(char) * (strlen(message) + 1));
		strcpy(cpy, message);
		return cpy;
	}
}
