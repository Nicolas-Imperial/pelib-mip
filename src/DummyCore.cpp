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


#include <pelib/DummyCore.hpp>

using namespace std;

namespace pelib
{
	DummyCore::DummyCore(size_t id, const set<float> &f, float unit): Core(id)
	{
		this->frequencies = set<float>(f);
		this->frequencyUnit = unit;
	}
	DummyCore::DummyCore(const set<float> &f, float unit): Core()
	{
		this->frequencies = set<float>(f);
		this->frequencyUnit = unit;
	}

	DummyCore::DummyCore(const DummyCore* src): Core(src->id)
	{
		this->frequencies = set<float>(src->getFrequencies());
		this->frequencyUnit = src->getFrequencyUnit();
	}

	DummyCore::DummyCore(const DummyCore &src): Core(src.id)
	{
		this->frequencies = set<float>(src.getFrequencies());
		this->frequencyUnit = src.getFrequencyUnit();
	}

	DummyCore::~DummyCore()
	{
		// Do nothing
	}

	Core*
	DummyCore::clone() const
	{
		return new DummyCore(this);
	}

	const set<float>&
	DummyCore::getFrequencies() const
	{
		return this->frequencies;
	}

	float
	DummyCore::getFrequencyUnit() const
	{
		return this->frequencyUnit;
	}
}
