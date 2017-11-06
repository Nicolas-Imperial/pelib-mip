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

#include <pelib/NoDecimalFloatException.hpp>

namespace pelib
{
	NoDecimalFloatException::NoDecimalFloatException(std::string message, float value) : message(message) 
	{
		this->value = value;
	}

	NoDecimalFloatException::~NoDecimalFloatException() throw() {}

	const char*
	NoDecimalFloatException::what() const throw() 
	{
		std::string fin = std::string("Float parsing exception: ").append(this->message).c_str();
		const char *message = fin.c_str();
		char *cpy = (char*)malloc(sizeof(char) * (strlen(message) + 1));
		strcpy(cpy, message);
		return cpy;
	}

	const float
	NoDecimalFloatException::getValue()
	{
		return this->value;
	}
}
