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


#include <pelib/AmplInputDataParser.hpp>
#include <pelib/AmplInputDataOutput.hpp>

#ifndef PELIB_AMPLINPUTDATA
#define PELIB_AMPLINPUTDATA

namespace pelib
{
	/** Base class for any class able to parser and output derivative classes of pelib::AlgebraData from input and output streams in AMPL input data format **/
	class AmplInputData: public AmplInputDataOutput, public AmplInputDataParser
	{
		public:
		protected:
		private:		
	};
}

#endif
