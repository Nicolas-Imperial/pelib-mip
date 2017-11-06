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
#include <stddef.h>
#include <set>

#ifndef PELIB_CORE
#define PELIB_CORE

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;
namespace pelib
{
	/** Models a processor Core **/
	class Core
	{
		public:
			struct LessCorePtrByCoreId 
			{
				bool operator() (const Core *x, const Core *y) const
				{
					return x->id < y->id;
				}
			};

			/** Constructor **/
			Core();
			Core(size_t id);
			/** Returns a pointer to a copy of this Core **/
			virtual Core* clone() const = 0;
			/** Destructor **/
			virtual ~Core();
			/** Returns the set of frequencies admissible by this core **/
			virtual const std::set<float>& getFrequencies() const = 0;
			/** Returns the frequency multiplier of the frequency set to obtain frequencies in Hertz **/
			virtual float getFrequencyUnit() const = 0;
			virtual bool
			operator<(const Core &other) const;
	    		virtual bool
			operator==(const Core &other) const;
			int id;
		protected:
			static size_t counter;
			float frequencyUnit;
		private:
	};
}
#undef debug
#endif
