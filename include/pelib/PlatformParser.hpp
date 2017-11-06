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


#include <pelib/Platform.hpp>
#include <pelib/DataParser.hpp>

#ifndef PELIB_PLATFORMPARSER
#define PELIB_PLATFORMPARSER

namespace pelib
{
	/** Base class of any parser class that produces instances of pelib::Platform from an input stream **/
	class PlatformParser : public RecordParser
	{
		public:
			/** Reads input stream and produces and instance of pelib::Platform **/
			virtual
			Platform*
			parse(std::istream &in) const = 0;

			/** Returns a pointer to a copy of this class **/
			virtual PlatformParser* clone() const = 0;

		protected:
		private:
	};
}

#endif
