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
#include <math.h>

#include <pelib/AlgebraData.hpp>

#if 01
#define debug(var) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"" << endl;
#else
#define debug(var)
#endif

using namespace std;

namespace pelib
{
	AlgebraData::AlgebraData(const std::string name, precision prec)
	{
		this->name = name;
		this->prec = prec;
	}

	AlgebraData::AlgebraData(const AlgebraData& data, precision prec)
	{
		this->name = data.getName();
		this->prec = prec;
	}
	
	void
	AlgebraData::setName(const std::string name)
	{
		this->name = name;
	}
	
	const std::string&
	AlgebraData::getName() const
	{
		return this->name;
	}

	AlgebraData::precision
	AlgebraData::getPrecision() const
	{
		return this->prec;
	}

	template<>
	float
	AlgebraData::fixPrecision(const float &data, std::streamsize precision)
	{
		float val = data;
		// Get the decimal part of our value
		float integ = floorf(data);
		float visible_dec = (data - integ) * powf(10, precision);

		// If there is more digits after the part that will actually be visible, then
		// we increment this digit
		if(visible_dec != floorf(visible_dec))
		{
			visible_dec = 1 / powf(10, precision);
			val += visible_dec;
		}

		return val;
	}

	template<>
	double
	AlgebraData::fixPrecision<double>(const double &data, std::streamsize precision)
	{
		double val = data;
		// Get the decimal part of our value
		double integ = floorf(data);
		double visible_dec = (data - integ) * powf(10, precision);

		// If there is more digits after the part that will actually be visible, then
		// we increment this digit
		if(visible_dec != floor(visible_dec))
		{
			visible_dec = 1 / pow(10, precision);
			val += visible_dec;
		}

		return data;
	}

	template<>
	long double
	AlgebraData::fixPrecision<long double>(const long double &data, std::streamsize precision)
	{
		long double val = data;
		// Get the decimal part of our value
		long double integ = floorf(data);
		long double visible_dec = (data - integ) * powf(10, precision);

		// If there is more digits after the part that will actually be visible, then
		// we increment this digit
		if(visible_dec != floorl(visible_dec))
		{
			visible_dec = 1 / powl(10, precision);
			val += visible_dec;
		}

		return data;
	}
}
