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
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/CastException.hpp>
#include <pelib/CppData.hpp>
#include <pelib/Data.hpp>

namespace pelib
{
	const std::string
	CppData::type_name(const std::type_info &type) const
	{
		int int_var;
		float float_var;
		double double_var;

		if(type == typeid(int_var))
		{
			return std::string("int").c_str();
		}
		else if(type == typeid(float_var))
		{
			return std::string("float").c_str();
		}
		else if(type == typeid(double_var))
		{
			return std::string("double").c_str();
		}
		else
		{
			throw CastException("Type is not int, float or double");
		}
	}
}
