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
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/Algebra.hpp>
#include <pelib/AmplDataParser.hpp>
#include <pelib/AlgebraData.hpp>

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;

using namespace std;

namespace pelib
{	
	Algebra::Algebra() { /* Do nothing */ }
	
	Algebra::Algebra(const map<string, const AlgebraData* const> &records)
	{
		this->records = records;
	}

	Algebra::~Algebra()
	{
		deleteRecords();
	}

	Algebra::Algebra(const Algebra &rhs)
	{
		*this = rhs;
	}

	Algebra::Algebra(const Algebra *rhs)
	{
		*this = *rhs;
	}

	Algebra*
	Algebra::clone() const
	{
		return new Algebra(this);
	}

	Algebra
	Algebra::merge(const Algebra& record) const
	{
		// Copy all AlgebraData
		map<string, const AlgebraData * const> rec;

		// Copy all records stored in this instance
		for(map<string, const AlgebraData * const>::const_iterator iter = records.begin(); iter != records.end(); iter++)
		{
			rec.insert(pair<string, pelib::AlgebraData*>(iter->second->getName(), iter->second->clone()));
		}

		// Copy all records from the foreign instance
		for(map<string, const AlgebraData * const>::const_iterator iter = record.getAllRecords().begin(); iter != record.getAllRecords().end(); iter++)
		{
			rec.insert(pair<string, pelib::AlgebraData*>(iter->second->getName(), iter->second->clone()));
		}

		// Build a new algebra with all records
		return Algebra(rec);
	}

	const map<string, const AlgebraData * const>&
	Algebra::getAllRecords() const
	{
		return records;
	}

	void
	Algebra::insert(const pelib::AlgebraData *data)
	{
		map<string, const AlgebraData * const>::iterator iter = records.find(data->getName());
		const AlgebraData *new_elem;
		if(iter != records.end())
		{
			const AlgebraData *ptr = iter->second;
			AlgebraData *dat = ptr->clone();
			delete ptr;
			dat->merge(data);
			new_elem = dat;
				records.erase(iter);
		}
		else
		{
			new_elem = data->clone();
		}
		records.insert(pair<string, const AlgebraData*>(data->getName(), new_elem));
	}

	void
	Algebra::remove(const string name)
	{
		map<string, const AlgebraData * const>::iterator ptr = records.find(name);

		if(ptr != records.end())
		{
			delete ptr->second;
			records.erase(name);
		}
	}

	Algebra&
	Algebra::operator=(const Algebra &rhs)
	{
		if(this != &rhs)
		{
			// Free all records
			deleteRecords();
			
			// Clone every objects of the source's collection
			map<string, const AlgebraData * const> rhrec = rhs.records; 
			for (map<string, const AlgebraData * const>::iterator i = rhrec.begin(); i != rhrec.end(); i++)
			{
				AlgebraData *copy = i->second->clone();
				this->records.insert(pair<string, const AlgebraData * const>(i->first, copy));
			}
		}
		
		return *this;
	}

	void
	Algebra::deleteRecords()
	{
		for (map<string, const AlgebraData * const>::iterator i = records.begin(); i != records.end();)
		{
			delete i->second;
			map<string, const AlgebraData * const>::iterator erase = i;
			i++;
			records.erase(erase);
		}
	}

	Algebra
	Algebra::filter(const set<string> &list) const
	{
		Algebra copy;
		for(std::map<std::string, const AlgebraData * const>::const_iterator i = this->records.begin(); i != this->records.end(); i++)
		{
			if(list.find(i->first) != list.end())
			{
				copy.insert(i->second->clone());
			}
		}

		return copy;
	}
}
