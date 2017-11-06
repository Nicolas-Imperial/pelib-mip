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

#include <pelib/AmplInput.hpp>
#include <pelib/AmplInputScalar.hpp>
#include <pelib/AmplInputVector.hpp>
#include <pelib/AmplInputSet.hpp>
#include <pelib/AmplInputMatrix.hpp>
#include <pelib/AmplDataParser.hpp>

#include <pelib/ParseException.hpp>
#include <pelib/CastException.hpp>
#include <pelib/PelibException.hpp>

#ifdef debug
#undef debug
#endif

#define debug(var) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << endl;

using namespace std;
using namespace boost::algorithm;

namespace pelib
{
	/*
	AmplInput::AmplInput()
	{
		// Add parsers
		addParsers();
				
		// Add interpreters
		addOutputs();
	}

	AmplInput::AmplInput(std::vector<AmplInputDataParser*> parsers,
			std::vector<AmplInputDataOutput*> outputs)
	{
		this->parsers = parsers;
		this->outputs = outputs;
	}
	*/

	AmplInput::AmplInput(std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > handlers)
	{
		this->parsers = handlers.first;
		this->outputs = handlers.second;
	}

	AmplInput::~AmplInput()
	{
		deleteParsers();
		deleteOutputs();			
	}
	
	void
	AmplInput::deleteParsers()
	{
		
		for(std::vector<AmplInputDataParser*>::iterator i = parsers.begin(); i != parsers.end(); i = parsers.erase(i))
		{
			delete *i;
		}
	}

	void
	AmplInput::deleteOutputs()
	{
		for(std::vector<AmplInputDataOutput*>::iterator i = outputs.begin(); i != outputs.end(); i = outputs.erase(i))
		{
			delete *i;
		}
	}

	AmplInput&
	AmplInput::operator=(const AmplInput &amplInput)
	{
		deleteParsers();
		deleteOutputs();

		for(std::vector<AmplInputDataParser*>::const_iterator i = amplInput.parsers.begin(); i != amplInput.parsers.end(); i++)
		{
			parsers.push_back((*i)->clone());
		}

		for(std::vector<AmplInputDataOutput*>::const_iterator i = amplInput.outputs.begin(); i != amplInput.outputs.end(); i++)
		{
			outputs.push_back((*i)->clone());
		}

		return *this;
	}

	Algebra
	AmplInput::parse(std::istream &ampl_data) const 
	{
		Algebra record;
		std::string line;
		std::string first_only = "\\1";
		std::string nothing = "";

		// Remove all comments from input before starting to parse it
		std::stringstream noComment;
		while(!getline(ampl_data, line).fail())
		{
			boost::regex comment("([^#]*)#[^$]*$");
			line = boost::regex_replace(line, comment, first_only, boost::match_default | boost::format_all);
			boost::regex surrounding_space("^[\\n\\s]*([^\\s\n]*)[\\s\n]*$");
			line = boost::regex_replace(line, surrounding_space, first_only, boost::match_default | boost::format_all);

			if(line.compare("") != 0)
			{
				noComment << line << std::endl;
			}
		}

		// Parse input
		while(!getline(noComment, line, ';').fail())
		{
			trim(line);	
			boost::regex surrounding_space("^[\\n\\s]*([^\\s\\n].*?)[\\s\\n]*$");
			line = boost::regex_replace(line, surrounding_space, first_only, boost::match_default | boost::format_all);

			if(line.length() == 0)
			{
				continue;
			}

			std::vector<AmplInputDataParser*>::const_iterator iter;
			for(iter = parsers.begin(); iter != parsers.end(); iter++)
			{
				AmplInputDataParser *parser = *iter;
				try {
					std::istringstream istr(line);
					std::istream &str = istr;
					AlgebraData *data = parser->parse(str);
					record.insert(data);

					// No need to try another parser; proceed with the next token
					break;
				} catch (ParseException &e)
				{
					//std::cerr << e.what() << "Trying next parser." << std::endl;
				}
			}

			if(iter == parsers.end())
			{
				std::vector<AmplInputDataParser*> parsers = stringParsers();
				for(iter = parsers.begin(); iter != parsers.end(); iter++)
				{
					AmplInputDataParser *parser = *iter;
					try {
						std::istringstream istr(line);
						std::istream &str = istr;
						AlgebraData *data = parser->parse(str);
						record.insert(data);

						// No need to try another parser; proceed with the next token
						break;
					} catch (ParseException &e)
					{
						//std::cerr << e.what() << "Trying next parser." << std::endl;
					}
				}

				if(iter == parsers.end())
				{
					//throw ParseException(std::string("No parser was suitable to the token \"").append(line).append("\"."));
				}
			}
		}
		
		return record;
	}

	void
	AmplInput::dump(std::ostream& o, const Algebra &record) const
	{
		const std::map<std::string, const AlgebraData * const> records = record.getAllRecords();
		for (std::map<std::string, const AlgebraData * const>::const_iterator rec = records.begin(); rec != records.end(); rec++)
		{
			dump(o, rec->second);
		}
	}

	void
	AmplInput::dump(std::ostream& o, const AlgebraData *data) const
	{
		std::vector<AmplInputDataOutput*>::const_iterator out;
		for (out = outputs.begin(); out != outputs.end(); out++)
		{
			const AmplInputDataOutput *output = *out;
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
			vector<AmplInputDataOutput*> string_outputs = stringOutputs();
			for (out = string_outputs.begin(); out != string_outputs.end(); out++)
			{
				const AmplInputDataOutput *output = *out;
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
	AmplInput::dump(std::ostream& o, const AlgebraData &data) const
	{
		dump(o, &data);
	}

	std::vector<AmplInputDataParser*> AmplInput::floatParsers()
	{
		std::vector<AmplInputDataParser*> parsers;
		
		parsers.push_back(new AmplInputScalar<float>(false));
		parsers.push_back(new AmplInputVector<int, float>(false));
		parsers.push_back(new AmplInputSet<float>(false));
		parsers.push_back(new AmplInputMatrix<int, int, float>(false));

		return parsers;
	}
		
	std::vector<AmplInputDataOutput*> AmplInput::floatOutputs()
	{
		std::vector<AmplInputDataOutput*> outputs;

		outputs.push_back(new AmplInputScalar<float>(false));
		outputs.push_back(new AmplInputVector<int, float>(false));
		outputs.push_back(new AmplInputSet<float>(false));
		outputs.push_back(new AmplInputMatrix<int, int, float>(false));

		return outputs;
	}

	std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > AmplInput::floatHandlers()
	{
		return std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> >(AmplInput::floatParsers(), AmplInput::floatOutputs());
	}

	std::vector<AmplInputDataParser*> AmplInput::stringParsers()
	{
		std::vector<AmplInputDataParser*> parsers;
		
		parsers.push_back(new AmplInputScalar<string>(false));
		parsers.push_back(new AmplInputVector<int, string>(false));
		parsers.push_back(new AmplInputSet<string>(false));
		parsers.push_back(new AmplInputMatrix<int, int, string>(false));

		return parsers;
	}
		
	std::vector<AmplInputDataOutput*> AmplInput::stringOutputs()
	{
		std::vector<AmplInputDataOutput*> outputs;

		outputs.push_back(new AmplInputScalar<string>(false));
		outputs.push_back(new AmplInputVector<int, string>(false));
		outputs.push_back(new AmplInputSet<string>(false));
		outputs.push_back(new AmplInputMatrix<int, int, string>(false));

		return outputs;
	}

	std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > AmplInput::stringHandlers()
	{
		return std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> >(AmplInput::stringParsers(), AmplInput::stringOutputs());
	}

	std::vector<AmplInputDataParser*> AmplInput::intFloatParsers()
	{
		std::vector<AmplInputDataParser*> parsers;
		parsers.push_back(new AmplInputScalar<int>());
		parsers.push_back(new AmplInputScalar<float>());
		parsers.push_back(new AmplInputVector<int, int>(true));
		parsers.push_back(new AmplInputVector<int, float>(true));
		parsers.push_back(new AmplInputSet<int>(true));
		parsers.push_back(new AmplInputSet<float>(true));
		parsers.push_back(new AmplInputMatrix<int, int, int>(true));
		parsers.push_back(new AmplInputMatrix<int, int, float>(true));

		return parsers;
	}
		
	std::vector<AmplInputDataOutput*> AmplInput::intFloatOutputs()
	{
		std::vector<AmplInputDataOutput*> outputs;

		outputs.push_back(new AmplInputScalar<int>());
		outputs.push_back(new AmplInputScalar<float>());
		outputs.push_back(new AmplInputVector<int, int>(true));
		outputs.push_back(new AmplInputVector<int, float>(true));
		outputs.push_back(new AmplInputSet<int>(true));
		outputs.push_back(new AmplInputSet<float>(true));
		outputs.push_back(new AmplInputMatrix<int, int, int>(true));
		outputs.push_back(new AmplInputMatrix<int, int, float>(true));

		return outputs;
	}

	std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > AmplInput::intFloatHandlers()
	{
		return std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> >(AmplInput::intFloatParsers(), AmplInput::intFloatOutputs());
	}

	/*
	// Protected
	void
	AmplInput::addParsers()
	{	
		parsers.push_back(new AmplInputScalar<int>());
		parsers.push_back(new AmplInputScalar<float>());
		parsers.push_back(new AmplInputVector<int, int>(true));
		parsers.push_back(new AmplInputVector<int, float>(true));
		parsers.push_back(new AmplInputSet<int>(true));
		parsers.push_back(new AmplInputSet<float>(true));
		parsers.push_back(new AmplInputMatrix<int, int, int>(true));
		parsers.push_back(new AmplInputMatrix<int, int, float>(true));
	}

	void			
	AmplInput::addOutputs()
	{
		outputs.push_back(new AmplInputScalar<int>());
		outputs.push_back(new AmplInputScalar<float>());
		outputs.push_back(new AmplInputVector<int, int>(true));
		outputs.push_back(new AmplInputVector<int, float>(true));
		outputs.push_back(new AmplInputSet<int>(true));
		outputs.push_back(new AmplInputSet<float>(true));
		outputs.push_back(new AmplInputMatrix<int, int, int>(true));
		outputs.push_back(new AmplInputMatrix<int, int, float>(true));
	}
*/
}
