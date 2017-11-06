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


#include <string>
#include <cstdlib>

#ifndef PELIB_CASTEXCEPTION
#define PELIB_CASTEXCEPTION

namespace pelib
{
	/** Exception thrown upon failed cast attempts from a superclass to a derived class **/
	class CastException: public std::exception
	{
		public:
			/** Constructor
				@param message Message to display to the screen if the exception is not caught
			**/
			explicit CastException(const std::string &message);
			/** Constructor
				@param message Message to display to the screen if the exception is not caught
			**/
			explicit CastException(const char *message);

			/** Destructor **/
			virtual
			~CastException() throw();

			/** Returns a human-readable string that explains the reason that lead to throwing this exception **/
			virtual const char*
			what() const throw();

		protected:
			/** Human-readable message to show if the exception is not caught **/
			std::string msg;
	};
}

#endif
