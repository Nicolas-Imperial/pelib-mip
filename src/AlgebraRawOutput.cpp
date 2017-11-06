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
#include <boost/algorithm/string.hpp>
#include <iomanip>

#include <pelib/AlgebraRawOutput.hpp>
#include <pelib/RawScalar.hpp>
#include <pelib/RawVector.hpp>
#include <pelib/RawSet.hpp>
#include <pelib/RawMatrix.hpp>

#include <pelib/ParseException.hpp>
#include <pelib/CastException.hpp>
#include <pelib/PelibException.hpp>

#ifdef debug
#undef debug
#endif

#define debug(var) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"" << endl;

using namespace std;
using namespace boost::algorithm;

namespace pelib
{
	AlgebraRawOutput::AlgebraRawOutput(std::vector<RawDataOutput*> handlers)
	{
		this->outputs = handlers;
	}

	AlgebraRawOutput::AlgebraRawOutput(std::vector<RawDataOutput*> handlers, const std::vector<std::string> &list)
	{
		this->outputs = handlers;
		this->output_list = list;
	}

	AlgebraRawOutput::~AlgebraRawOutput()
	{
		deleteOutputs();			
	}

	void
	AlgebraRawOutput::deleteOutputs()
	{
		for(std::vector<RawDataOutput*>::iterator i = outputs.begin(); i != outputs.end(); i = outputs.erase(i))
		{
			delete *i;
		}
	}

	AlgebraRawOutput&
	AlgebraRawOutput::operator=(const AlgebraRawOutput &amplOutput)
	{
		deleteOutputs();

		for(std::vector<RawDataOutput*>::const_iterator i = amplOutput.outputs.begin(); i != amplOutput.outputs.end(); i++)
		{
			outputs.push_back((*i)->clone());
		}

		return *this;
	}

	void
	AlgebraRawOutput::dump(std::ostream& o, const Algebra &record) const
	{
		std::map<std::string, const AlgebraData * const> records = record.getAllRecords();
		if(output_list.size() > 0)
		{
			for(std::vector<std::string>::const_iterator i = output_list.begin(); i != output_list.end(); i++)
			{
				std::map<std::string, const AlgebraData * const>::const_iterator rec = records.find(*i);
				if(rec != records.end())
				{
					dump(o, rec->second);
				}
			}
		}
		else
		{
			for (std::map<std::string, const AlgebraData * const>::const_iterator rec = records.begin(); rec != records.end(); rec++)
			{
				dump(o, rec->second);
			}
		}
	}

	void
	AlgebraRawOutput::dump(std::ostream& o, const AlgebraData *data) const
	{
		std::vector<RawDataOutput*>::const_iterator out;
		for (out = outputs.begin(); out != outputs.end(); out++)
		{
			const RawDataOutput *output = *out;
			try
			{
				output->dump(o, data);
				break;
			} catch(CastException &e)
			{
				// No suitable element to output
				// Couldn't cast the element to record: just let that go and try again with next element
			}
		}

		if(out == outputs.end())
		{
			vector<RawDataOutput*> string_outputs = stringOutputs();
			for (out = string_outputs.begin(); out != string_outputs.end(); out++)
			{
				const RawDataOutput *output = *out;
				try
				{
					output->dump(o, data);
					break;
				} catch(CastException &e)
				{
					// No suitable element to output
					// Couldn't cast the element to record: just let that go and try again with next element
				}
			}

			if(out == string_outputs.end())
			{
				throw PelibException("Could not find a suitable output format for data record of name \"" + string(data->getName()) + "\".");
			}
		}
	}

	void
	AlgebraRawOutput::dump(std::ostream& o, const AlgebraData &data) const
	{
		dump(o, &data);
	}

	std::vector<RawDataOutput*> AlgebraRawOutput::floatOutputs()
	{
		std::vector<RawDataOutput*> outputs;

		outputs.push_back(new RawScalar<float>());
		outputs.push_back(new RawVector<int, float>());
		outputs.push_back(new RawSet<float>());
		outputs.push_back(new RawMatrix<int, int, float>());

		return outputs;
	}

	std::vector<RawDataOutput*> AlgebraRawOutput::stringOutputs()
	{
		std::vector<RawDataOutput*> outputs;

		outputs.push_back(new RawScalar<string>());
		outputs.push_back(new RawVector<int, string>());
		outputs.push_back(new RawSet<string>());
		outputs.push_back(new RawMatrix<int, int, string>());

		return outputs;
	}

	std::vector<RawDataOutput*> AlgebraRawOutput::intFloatOutputs()
	{
		std::vector<RawDataOutput*> outputs;

		outputs.push_back(new RawScalar<int>());
		outputs.push_back(new RawScalar<float>());
		outputs.push_back(new RawVector<int, int>());
		outputs.push_back(new RawVector<int, float>());
		outputs.push_back(new RawSet<int>());
		outputs.push_back(new RawSet<float>());
		outputs.push_back(new RawMatrix<int, int, int>());
		outputs.push_back(new RawMatrix<int, int, float>());

		return outputs;
	}
}
