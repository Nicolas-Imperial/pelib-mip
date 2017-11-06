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


#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <pelib/DeadlineCalculator.hpp>
#include <pelib/DeadlineConstant.hpp>
#include <pelib/DeadlineFormula.hpp>
#include <pelib/ParseException.hpp>

using namespace boost::algorithm;

namespace pelib
{
	DeadlineCalculator*
	DeadlineCalculator::getDeadlineCalculator(const std::string str)
	{
		string key(str);
		trim(key);

		try{
			if(key.find("exprtk") == 0)
			{
				return new DeadlineFormula(key.substr(string("exprtk").size() + 1));
			}
			else
			{
				//Maybe it is just a number, already calculated. Fine
				return new DeadlineConstant(boost::lexical_cast<float>(key));
			}
		}
		catch(const boost::bad_lexical_cast &)
		{
			throw ParseException("Deadline type not recognized: " + key);
		}
	}
}
