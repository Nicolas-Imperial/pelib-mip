/*
 Copyright 2017 Nicolas Melot

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

#include <pelib/RecordParser.hpp>
#include <pelib/Algebra.hpp>

#ifndef PELIB_MIXEDINTEGERMODELPARSER
#define PELIB_MIXEDINTEGERMODELPARSER

namespace pelib
{
	/** Base class of any class that takes data from an input stream and procudes an instance of Algebra. Derive this class before use. **/
	class MixedIntegerModelParser : public ModelParser
	{
		public:
			/** Produces an instance of Algebra from data found in input stream data. **/
			virtual
			MixedIntegerModel
			parse(std::istream &data) const = 0;
	};
}

#endif

