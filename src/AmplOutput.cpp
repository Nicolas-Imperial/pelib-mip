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

#include <pelib/AmplOutput.hpp>
#include <pelib/AmplOutputScalar.hpp>
#include <pelib/AmplOutputVector.hpp>
#include <pelib/AmplOutputSet.hpp>
#include <pelib/AmplOutputMatrix.hpp>
#include <pelib/AmplDataParser.hpp>

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
	/*
	AmplOutput::AmplOutput()
	{
		// Add parsers
		addParsers();
				
		// Add interpreters
		addOutputs();
	}

	AmplOutput::AmplOutput(std::vector<AmplOutputDataParser*> parsers,
			std::vector<AmplOutputDataOutput*> outputs)
	{
		this->parsers = parsers;
		this->outputs = outputs;
	}
*/
	AmplOutput::AmplOutput(std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > handlers)
	{
		this->parsers = handlers.first;
		this->outputs = handlers.second;
	}

	AmplOutput::~AmplOutput()
	{
		deleteParsers();
		deleteOutputs();			
	}

	void
	AmplOutput::deleteParsers()
	{
		for(std::vector<AmplOutputDataParser*>::iterator i = parsers.begin(); i != parsers.end(); i = parsers.erase(i))
		{
			delete *i;
		}
	}

	void
	AmplOutput::deleteOutputs()
	{
		for(std::vector<AmplOutputDataOutput*>::iterator i = outputs.begin(); i != outputs.end(); i = outputs.erase(i))
		{
			delete *i;
		}
	}

	AmplOutput&
	AmplOutput::operator=(const AmplOutput &amplOutput)
	{
		deleteParsers();
		deleteOutputs();

		for(std::vector<AmplOutputDataParser*>::const_iterator i = amplOutput.parsers.begin(); i != amplOutput.parsers.end(); i++)
		{
			parsers.push_back((*i)->clone());
		}

		for(std::vector<AmplOutputDataOutput*>::const_iterator i = amplOutput.outputs.begin(); i != amplOutput.outputs.end(); i++)
		{
			outputs.push_back((*i)->clone());
		}

		return *this;
	}

	Algebra
	AmplOutput::parse(std::istream &ampl_data) const
	{
		Algebra record;
		std::string line;
		
		while(!getline(ampl_data, line, ';').fail())
		{
			// Read section into string buffer stream
			std::stringstream section;
			section << line;

			while(!getline(section, line, ';').fail())
			{			
				std::vector<AmplOutputDataParser*>::const_iterator iter;
				for(iter = parsers.begin(); iter != parsers.end(); iter++)
				{
					AmplOutputDataParser *parser = *iter;
					try {
						// If any non-blank prefix to be discarded, it must be separated by a new line
						std::string regex = std::string("(.*?\\n|)\\s*(")
							.append(parser->getGlobalPattern())
							.append(")(?:\\s*)");
						
						boost::cmatch match; // = AlgebraDataParser::match(regex, line);
						if(!boost::regex_match(line.c_str(), match, boost::regex(regex)))
						{
							throw ParseException(std::string("String \"").append(line).append("\" doesn't match regex \"").append(regex).append("\". "));
						}

						std::stringstream token;
						token.str(match[2]);

						//std::cerr << "Discarded =\"" << match[1] << "\"." << std::endl;
						//std::cerr << "Token =\"" << match[2] << "\"." << std::endl;
						AlgebraData *data = parser->parse(token);
						record.insert(data);

						// Keep only the part we have not parsed
						line = match[1];
						
						// Feed the remaining Output line to parsers Output is more useful information are available
	  					section.str(line);
						section.seekp(std::ios_base::beg);
						section.clear();

						// No need to try another parser; proceed with the next token in section
						break;
					} catch (ParseException &e)
					{
						// Try next parser
					}
				}

				// There was no parser for this section; drop it and go to next
				if(iter == parsers.end())
				{
					std::vector<AmplOutputDataParser*>::const_iterator iter;
					std::vector<AmplOutputDataParser*> parsers = stringParsers();
					for(iter = parsers.begin(); iter != parsers.end(); iter++)
					{
						AmplOutputDataParser *parser = *iter;
						try {
							// If any non-blank prefix to be discarded, it must be separated by a new line
							std::string regex = std::string("(.*?\\n|)\\s*(")
								.append(parser->getGlobalPattern())
								.append(")(?:\\s*)");
							
							boost::cmatch match; // = AlgebraDataParser::match(regex, line);
							if(!boost::regex_match(line.c_str(), match, boost::regex(regex)))
							{
								throw ParseException(std::string("String \"").append(line).append("\" doesn't match regex \"").append(regex).append("\". "));
							}
				

							std::stringstream token;
							token.str(match[2]);

							//std::cerr << "Discarded =\"" << match[1] << "\"." << std::endl;
							//std::cerr << "Token =\"" << match[2] << "\"." << std::endl;
							AlgebraData *data = parser->parse(token);
							record.insert(data);

							// Keep only the part we have not parsed
							line = match[1];
							
							// Feed the remaining Output line to parsers Output is more useful information are available
							section.str(line);
							section.seekp(std::ios_base::beg);
							section.clear();

							// No need to try another parser; proceed with the next token in section
							break;
						} catch (ParseException &e)
						{
							// Try next parser
						}
					}
				}
			}
		}
		
		return record;
	}

	void
	AmplOutput::dump(std::ostream& o, const Algebra &record) const
	{
		std::map<std::string, const AlgebraData * const> records = record.getAllRecords();
		for (std::map<std::string, const AlgebraData * const>::const_iterator rec = records.begin(); rec != records.end(); rec++)
		{
			dump(o, rec->second);
		}
	}

	void
	AmplOutput::dump(std::ostream& o, const AlgebraData *data) const
	{
		std::vector<AmplOutputDataOutput*>::const_iterator out;
		for (out = outputs.begin(); out != outputs.end(); out++)
		{
			const AmplOutputDataOutput *output = *out;
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
			vector<AmplOutputDataOutput*> string_outputs = stringOutputs();
			for (out = string_outputs.begin(); out != string_outputs.end(); out++)
			{
				const AmplOutputDataOutput *output = *out;
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
	AmplOutput::dump(std::ostream& o, const AlgebraData &data) const
	{
		dump(o, &data);
	}

	std::vector<AmplOutputDataParser*> AmplOutput::floatParsers()
	{
		std::vector<AmplOutputDataParser*> parsers;
		
		parsers.push_back(new AmplOutputScalar<float>(false));
		parsers.push_back(new AmplOutputVector<int, float>(false));
		parsers.push_back(new AmplOutputSet<float>(false));
		parsers.push_back(new AmplOutputMatrix<int, int, float>(false));

		return parsers;
	}
		
	std::vector<AmplOutputDataOutput*> AmplOutput::floatOutputs()
	{
		std::vector<AmplOutputDataOutput*> outputs;

		outputs.push_back(new AmplOutputScalar<float>(false));
		outputs.push_back(new AmplOutputVector<int, float>(false));
		outputs.push_back(new AmplOutputSet<float>(false));
		outputs.push_back(new AmplOutputMatrix<int, int, float>(false));

		return outputs;
	}

	std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > AmplOutput::floatHandlers()
	{
		return std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> >(AmplOutput::floatParsers(), AmplOutput::floatOutputs());
	}

	std::vector<AmplOutputDataParser*> AmplOutput::stringParsers()
	{
		std::vector<AmplOutputDataParser*> parsers;
		
		parsers.push_back(new AmplOutputScalar<string>(false));
		parsers.push_back(new AmplOutputVector<int, string>(false));
		parsers.push_back(new AmplOutputSet<string>(false));
		parsers.push_back(new AmplOutputMatrix<int, int, string>(false));

		return parsers;
	}
		
	std::vector<AmplOutputDataOutput*> AmplOutput::stringOutputs()
	{
		std::vector<AmplOutputDataOutput*> outputs;

		outputs.push_back(new AmplOutputScalar<string>(false));
		outputs.push_back(new AmplOutputVector<int, string>(false));
		outputs.push_back(new AmplOutputSet<string>(false));
		outputs.push_back(new AmplOutputMatrix<int, int, string>(false));

		return outputs;
	}

	std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > AmplOutput::stringHandlers()
	{
		return std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> >(AmplOutput::stringParsers(), AmplOutput::stringOutputs());
	}

	std::vector<AmplOutputDataParser*> AmplOutput::intFloatParsers()
	{
		std::vector<AmplOutputDataParser*> parsers;
		parsers.push_back(new AmplOutputScalar<int>());
		parsers.push_back(new AmplOutputScalar<float>());
		parsers.push_back(new AmplOutputVector<int, int>(true));
		parsers.push_back(new AmplOutputVector<int, float>(true));
		parsers.push_back(new AmplOutputSet<int>(true));
		parsers.push_back(new AmplOutputSet<float>(true));
		parsers.push_back(new AmplOutputMatrix<int, int, int>(true));
		parsers.push_back(new AmplOutputMatrix<int, int, float>(true));

		return parsers;
	}
		
	std::vector<AmplOutputDataOutput*> AmplOutput::intFloatOutputs()
	{
		std::vector<AmplOutputDataOutput*> outputs;

		outputs.push_back(new AmplOutputScalar<int>());
		outputs.push_back(new AmplOutputScalar<float>());
		outputs.push_back(new AmplOutputVector<int, int>(true));
		outputs.push_back(new AmplOutputVector<int, float>(true));
		outputs.push_back(new AmplOutputSet<int>(true));
		outputs.push_back(new AmplOutputSet<float>(true));
		outputs.push_back(new AmplOutputMatrix<int, int, int>(true));
		outputs.push_back(new AmplOutputMatrix<int, int, float>(true));

		return outputs;
	}

	std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > AmplOutput::intFloatHandlers()
	{
		return std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> >(AmplOutput::intFloatParsers(), AmplOutput::intFloatOutputs());
	}

	/*
	// Protected
	void
	AmplOutput::addParsers()
	{		
		parsers.push_back(new AmplOutputScalar<int>());
		parsers.push_back(new AmplOutputScalar<float>());
		parsers.push_back(new AmplOutputVector<int, int>());
		parsers.push_back(new AmplOutputVector<int, float>());
		parsers.push_back(new AmplOutputSet<int>());
		parsers.push_back(new AmplOutputSet<float>());
		parsers.push_back(new AmplOutputMatrix<int, int, int>());
		parsers.push_back(new AmplOutputMatrix<int, int, float>());
	}

	void			
	AmplOutput::addOutputs()
	{		
		outputs.push_back(new AmplOutputScalar<int>());
		outputs.push_back(new AmplOutputScalar<float>());
		outputs.push_back(new AmplOutputVector<int, int>());
		outputs.push_back(new AmplOutputVector<int, float>());
		outputs.push_back(new AmplOutputSet<int>());
		outputs.push_back(new AmplOutputSet<float>());
		outputs.push_back(new AmplOutputMatrix<int, int, int>());
		outputs.push_back(new AmplOutputMatrix<int, int, float>());
	}
	*/
}
