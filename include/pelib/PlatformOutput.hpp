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
#include <pelib/RecordOutput.hpp>

#ifndef PELIB_PLATFORMOUTPUT
#define PELIB_PLATFORMOUTPUT

namespace pelib
{
	/** Base class for a Platform output class **/
	class PlatformOutput : public RecordOutput
	{
		public:	
			/** Writes the platform in output stream
				@param o Output stream to write Platform instance to
				@param data Instance of pelib::Platform to be written to output stream
			**/
			virtual	void dump(std::ostream& o, const Record *data) const = 0;

			/** Returns a pointer to a copy of this Platform output class **/
			virtual PlatformOutput* clone() const = 0;

		protected:
		private:		
	};
}

#endif
