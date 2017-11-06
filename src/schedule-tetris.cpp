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

#include <pelib/parser.h>
#include <pelib/output.h>

#include <pelib/TetrisSchedule.hpp>
#include <pelib/Taskgraph.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef debug
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#endif

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, const std::map<string, Record*> &records, size_t argc, char **argv)
{
	Schedule *sc = (Schedule*)records.find(typeid(Schedule).name())->second;

	if(argv[0] != NULL)
	{
		float ratio = TetrisSchedule::defaultRatio();
		float stroke_size = TetrisSchedule::defaultStrokeSize();
		vector<unsigned int> colors = TetrisSchedule::defaultFrequencyColors();
		bool showFrequencies = true;
		bool useTaskName = false;
		bool showTaskId = true;
		
		for(; argv[0] != NULL; argv++)
		{
			if(strcmp(argv[0], "--ratio") == 0)
			{
				argv++;
				std::istringstream converter(argv[0]);
				converter >> ratio;
				if(converter.fail())
				{
					ratio = TetrisSchedule::defaultRatio();
				}

				continue;
			}

			if(strcmp(argv[0], "--no-legend") == 0)
			{
				showFrequencies = false;
				continue;
			}

			if(strcmp(argv[0], "--no-task-id") == 0)
			{
				showTaskId = false;
			}

			if(strcmp(argv[0], "--use-task-name") == 0)
			{
				useTaskName = true;
				continue;
			}

			if(strcmp(argv[0], "--stroke-size") == 0)
			{
				argv++;
				std::istringstream converter(argv[0]);
				converter >> stroke_size;	
				if(converter.fail())
				{
					stroke_size = TetrisSchedule::defaultStrokeSize();
				}
				continue;
			}

			if(strcmp(argv[0], "--gradient") == 0)
			{
				colors.clear();
				argv++;
				if(argv[0] != NULL)
				{
					unsigned int color;
					bool isInt = true;
					stringstream converter;
					converter << hex << argv[0];
					converter >> color;
					isInt = !converter.fail();

					// Read all integers
					while(argv[0] != NULL && isInt)
					{
						colors.push_back(color);
						argv++;
						if(argv[0] != NULL)
						{
							converter.str("");
							converter.clear();
							converter << hex << argv[0];
							converter >> color;
							isInt = !converter.fail();
						}
					}
					argv--;
				}

				if(colors.size() < 2)
				{
					colors = TetrisSchedule::defaultFrequencyColors();
				}

				continue;
			}
		}

		TetrisSchedule(ratio, showFrequencies, showTaskId, useTaskName, colors, stroke_size).dump(cout, sc);
	}
	else	
	{
		TetrisSchedule().dump(cout, sc);
	}
}

void
pelib_delete(Record *obj)
{
	delete obj;
}

#ifdef __cplusplus
}
#endif

