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


#include <pelib/Algebra.hpp>
#include <pelib/DataOutput.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/Data.hpp>

#include <pelib/Algebra.hpp>
#include <pelib/RecordOutput.hpp>

#ifndef PELIB_ALGEBRAOUTPUT
#define PELIB_ALGEBRAOUTPUT

namespace pelib
{
	/** Base class of classes that can generate any representation out of an instance of Algebra. Derive this class before use. **/
	class AlgebraOutput : public RecordOutput
	{
		public:
			/** Generates some representation of an instance of Algebra through its reference and write it to output stream o. **/
			virtual
			void
			dump(std::ostream& o, const Algebra &record) const = 0;

			/** Generates some representation of an instance of Algebra through its pointer and write it to output stream o. **/
			virtual
			void
			dump(std::ostream& o, const AlgebraData *data) const = 0;
			
		protected:
		private:
	};
}

#endif
