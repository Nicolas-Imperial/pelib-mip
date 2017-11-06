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


#include <cstdlib>

#ifndef PELIB_NODECIMALFLOATEXCEPTION
#define PELIB_NODECIMALFLOATEXCEPTION

namespace pelib
{
	/** Exception fired when parsing an integer while a floating-point value was expected and strict mode parsing is enabled **/
	class NoDecimalFloatException: std::exception
	{
		public:
			/** Constructor
				@param message Human-readable message that explains the error
				@param floating point value parsed
			**/
			explicit NoDecimalFloatException(std::string message, float value);

			/** Destructor **/
			virtual
			~NoDecimalFloatException() throw();

			/** Returns a mesasge that explain why the exception was thrown **/
			virtual const char*
			what() const throw();

			/** Return the floating-point value that was parsed **/
			const float
			getValue();

		protected:
			/** Human-readable message that explains the reason why the exception was thrown **/
			std::string message;

			/** floating-point value that was parsed instead of an integer **/
			float value;
	};
}

#endif
