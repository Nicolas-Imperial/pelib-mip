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

#include <pelib/AlgebraDataParser.hpp>

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;

namespace pelib
{
	// This is causing too much problem, probably due to a bug in boost::regex, even when inlining
	// Inline manually instead
	#if 0
	boost::cmatch
	AlgebraDataParser::match(std::string regex, std::string str)
	{
		boost::cmatch match;
		boost::regex param_scalar(regex);
		//std::cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
		if(boost::regex_match(str.c_str(), match, param_scalar))
		{
			//std::cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
			return match;
		}
		else
		{
			//std::cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
			throw ParseException(std::string("String \"").append(str).append("\" doesn't match regex \"").append(regex).append("\". "));
		}
	}
	#endif
}
