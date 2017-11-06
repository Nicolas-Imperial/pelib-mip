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

#include <pelib/AlgebraData.hpp>
#include <pelib/DataOutput.hpp>

#ifndef PELIB_ALGEBRADATAOUTPUT
#define PELIB_ALGEBRADATAOUTPUT

namespace pelib
{
	/** Base class of all classes able to output either element in a Algebra collection. This class must be derived to be used. **/
	class AlgebraDataOutput : public DataOutput
	{
		public:
			/** writes in o a text representation of data **/
			virtual
			void
			dump(std::ostream& o, const AlgebraData *data) const = 0;

		protected:
		private:		
	};
}

#endif
