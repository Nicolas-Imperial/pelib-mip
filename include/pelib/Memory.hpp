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

#ifndef PELIB_MEMORY
#define PELIB_MEMORY

namespace pelib
{
	/** Model a link between tasks of a streaming application **/
	class Memory
	{
		public:
			enum class Feature: int { undefined = 0, exclusive = 1, shared = 2, distributed = 3, smallAndCheap = 4, largeAndCostly = 8 };
			static const std::string undefined, exclusive, shared, distributed, smallAndCheap, largeAndCostly;
			static const unsigned int memoryAccessMask;
			static const unsigned int memoryCostMask;
			/** Constructor. Takes the producer and consumer tasks at both ends of the link **/
			Memory();
			Memory(Feature feature, unsigned int level, const unsigned int core, const unsigned int instance = 0);

			unsigned int
			getCore() const;

			Feature
			getFeature() const;

			std::string
			getFeatureAsString() const;

			unsigned int
			getLevel() const;

			static std::string
			featureToString(Feature memory);

			static Feature
			stringToFeature(const std::string &str);

			static Memory
			nullMemory();

			bool
			operator==(const Memory &mem) const;

			unsigned int
			getInstance() const;

		protected:
			/** Producer and consumer task pointers **/
			Feature feature;
			unsigned int core, level;
			unsigned int instance;

		private:
			void
			setAsNull();
	};
}

#endif
