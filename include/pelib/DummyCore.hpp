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


#include <pelib/Core.hpp>

#ifndef PELIB_DUMMY_CORE
#define PELIB_DUMMY_CORE

namespace pelib
{
	/** Simple core model **/
	class DummyCore: public Core
	{
		public:
			/** Constructor
				@param f Set of frequency that this core can run at
			**/
			DummyCore(const std::set<float> &f, float unit);
			DummyCore(size_t id, const std::set<float> &f, float unit);
			/** Copy constructor **/
			DummyCore(const DummyCore*);
			/** Copy constructor **/
			DummyCore(const DummyCore&);
			/** Destructor **/
			virtual ~DummyCore();
			/** Returns a pointer to a copy of this class instance **/
			virtual Core* clone() const;
			/** Returns the set of frequencies this core can run at **/
			virtual const std::set<float>& getFrequencies() const;
			/** Returns the frequency multiplier of the frequency set to obtain frequencies in Hertz **/
			virtual float getFrequencyUnit() const;
		protected:
			/** Admissible set of frequencies for this core **/
			std::set<float> frequencies;
		private:
	};
}

#endif
