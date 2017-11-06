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
#include <iostream>
#include <sstream>
#include <string.h>

#include <pelib/Memory.hpp>
#include <pelib/PelibException.hpp>

#ifdef debug
#undef debug
#endif

#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

using namespace std;

namespace pelib
{
	Memory::Memory()
	{
		this->setAsNull();
	}

	Memory::Memory(Feature feature, unsigned int level, const unsigned int core, const unsigned int instance)
	{
		this->core = core;
		this->feature = feature;
		this->level = level;
		this->instance = instance;
	}

	unsigned int
	Memory::getCore() const
	{
		return this->core;
	}

	Memory::Feature
	Memory::getFeature() const
	{
		return this->feature;
	}

	std::string
	Memory::getFeatureAsString() const
	{
		return Memory::featureToString(this->getFeature());
	}

	unsigned int
	Memory::getLevel() const
	{
		return this->level;
	}

	const string Memory::undefined = "DRAKE_MEMORY_UNDEFINED";
	const string Memory::exclusive = "DRAKE_MEMORY_PRIVATE";
	const string Memory::shared = "DRAKE_MEMORY_SHARED";
	const string Memory::distributed = "DRAKE_MEMORY_DISTRIBUTED";
	const string Memory::smallAndCheap = "DRAKE_MEMORY_SMALL_CHEAP";
	const string Memory::largeAndCostly = "DRAKE_MEMORY_LARGE_COSTLY";
	const unsigned int Memory::memoryAccessMask = 3;
	const unsigned int Memory::memoryCostMask = 12;
	string
	Memory::featureToString(Feature feature)
	{
		if(feature == Feature::undefined)
		{
			return Memory::undefined;
		}

		Feature access = (Feature)((int)feature & 3); // Keep only 2 weakest bits
		Feature type = (Feature)((int)feature & 12); // Keep the 2 next weakest bits

		string str;
		switch(access)
		{
			case Memory::Feature::exclusive:
				str = Memory::exclusive;
			break;
			case Memory::Feature::shared:
				str = Memory::shared;
			break;
			case Memory::Feature::distributed:
				str = Memory::distributed;
			break;
			default:
				stringstream ss;
				ss << (int)access;
				throw PelibException(string("Unknown memory access type: ") + ss.str());
			break;
			break;
		}

		switch(type)
		{
			case Memory::Feature::smallAndCheap:
				str += string(" | ") + Memory::smallAndCheap;
			break;
			case Memory::Feature::largeAndCostly:
				str += string(" | ") + Memory::largeAndCostly;
			break;
			default:
				stringstream ss;
				ss << (int)type;
				throw PelibException(string("Unknown memory cost type: ") + ss.str());
			break;
		}

		return str;
	}

	void
	Memory::setAsNull()
	{
		this->feature = Feature::undefined;
		this->core = 0;
		this->level = 0;
	}

	Memory
	Memory::nullMemory()
	{
		return Memory(); 
	}

	unsigned int
	Memory::getInstance() const
	{
		return this->instance;
	}

	Memory::Feature
	Memory::stringToFeature(const std::string &mem)
	{
		char *str = (char*)malloc(sizeof(char) * mem.length() + 1);
		strcpy(str, mem.c_str());
  		char *pch;
  		pch = strtok (str, "| \n\t\r");
		int memory = 0;
		while(pch != NULL)
		{
			string token = string(pch);

			if(token.compare(Memory::undefined) == 0)
			{
				// This is a neutral element, do nothing
				memory = memory | (int)Memory::Feature::undefined;
			}
			else if(token.compare(Memory::exclusive) == 0)
			{
				memory = memory | (int)Memory::Feature::exclusive;
			}
			else if(token.compare(Memory::shared) == 0)
			{
				memory = memory | (int)Memory::Feature::shared;
			}
			else if(token.compare(Memory::distributed) == 0)
			{
				memory = memory | (int)Memory::Feature::distributed;
			}
			else if(token.compare(Memory::smallAndCheap) == 0)
			{
				memory = memory | (int)Memory::Feature::smallAndCheap;
			}
			else if(token.compare(Memory::largeAndCostly) == 0)
			{
				memory = memory | (int)Memory::Feature::largeAndCostly;
			}
			else
			{
				throw PelibException(string("Invalid memory feature: \"") + token + string("\""));
			}

    			pch = strtok (NULL, "| \n\r\t");
		}
	
		free(str);
		return (Memory::Feature)memory;
	}

	bool
	Memory::operator==(const Memory &mem) const
	{
		return this->feature == mem.getFeature() && this->core == mem.getCore() && this->level == mem.getLevel();
	}
}
