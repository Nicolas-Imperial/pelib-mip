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
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/Cpp.hpp>
#include <pelib/CppScalar.hpp>
#include <pelib/CppVector.hpp>
#include <pelib/CppSet.hpp>
#include <pelib/CppMatrix.hpp>

#include <pelib/ParseException.hpp>
#include <pelib/CastException.hpp>

namespace pelib
{
	Cpp::Cpp()
	{				
		this->setName(std::string("make_algebra"));
		
		// Add outputs
		addOutputs();
	}

	Cpp::Cpp(std::vector<CppData*> outputs)
	{
		this->setName(std::string("make_algebra"));
		this->outputs = outputs;
	}

	Cpp::Cpp(const std::string &name)
	{
		this->setName(name);
		
		// Add outputs
		addOutputs();
	}

	Cpp::Cpp(const Cpp &src)
	{
		*this = src;
	}

	Cpp::~Cpp()
	{
		deleteOutputs();			
	}

	std::string
	Cpp::getName() const
	{
		return this->name;
	}

	void
	Cpp::setName(const std::string &name)
	{
		this->name = name;
	}

	void
	Cpp::deleteOutputs()
	{
		for(std::vector<CppData*>::iterator i = outputs.begin(); i != outputs.end(); i = outputs.erase(i))
		{
			delete *i;
		}
	}

	Cpp&
	Cpp::operator=(const Cpp &src)
	{
		deleteOutputs();
		this->name = src.getName();

		for(std::vector<CppData*>::const_iterator i = src.outputs.begin(); i != src.outputs.end(); i++)
		{
			outputs.push_back((*i)->clone());
		}

		return *this;
	}

	void
	Cpp::dump(std::ostream& o, const Algebra &record) const
	{
		o << "pelib::Algebra" << std::endl << this->getName() << "()" << std::endl << "{" << std::endl;
		o << "pelib::Algebra new_algebra;" << std::endl;
		
		std::map<std::string, const AlgebraData * const> records = record.getAllRecords();
		for (std::map<std::string, const AlgebraData * const>::const_iterator rec = records.begin(); rec != records.end(); rec++)
		{
			try
			{
				dump(o, rec->second);
				o << "new_algebra.insert(&" << rec->first << ");" << std::endl;
			}
			catch (CastException &e)
			{
				// Do nothing
			}
			
		}

		o << "return new_algebra;" << std::endl;
		o << "}" << std::endl;
	}

	void
	Cpp::dump(std::ostream& o, const AlgebraData *data) const
	{
		bool was_output = false;
		
		for (std::vector<CppData*>::const_iterator out = outputs.begin(); out != outputs.end(); out++)
		{	
			const CppDataOutput *output = *out;
			
			try
			{
				output->dump(o, data);
				was_output = true;
				
				break;
			} catch(CastException &e)
			{
				// No suitable element to output
				// Couldn't cast the element to record: just let that go and try again with next element
			}
		}

		if(!was_output)
		{
			throw CastException("Could not find output for structure \"" + data->getName() + "\".");
		}
	}

	void			
	Cpp::addOutputs()
	{		
		outputs.push_back(new CppScalar<int>());
		outputs.push_back(new CppScalar<float>());
		outputs.push_back(new CppVector<int, int>());
		outputs.push_back(new CppVector<int, float>());
		outputs.push_back(new CppSet<int>());
		outputs.push_back(new CppSet<float>());
		outputs.push_back(new CppMatrix<int, int, int>());
		outputs.push_back(new CppMatrix<int, int, float>());
	}
}
