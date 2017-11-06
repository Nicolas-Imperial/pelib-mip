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

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/Scalar.hpp>

#include <pelib/AmplInput.hpp>
#include <pelib/AmplOutput.hpp>

#include <pelib/DummyCore.hpp>
#include <pelib/GraphML.hpp>
#include <pelib/Platform.hpp>
#include <pelib/XMLSchedule.hpp>

#include <pelib/AmplInputData.hpp>
#include <pelib/AmplInputScalar.hpp>
#include <pelib/AmplInputVector.hpp>
#include <pelib/AmplInputMatrix.hpp>

#include <pelib/AmplOutputScalar.hpp>
#include <pelib/AmplOutputVector.hpp>
#include <pelib/AmplOutputMatrix.hpp>

#include <pelib/Scalar.hpp>
#include <pelib/Set.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Matrix.hpp>

#include <pelib/unit.h>

#ifndef debug
#if 0
#define debug(var) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"" << endl;
#else
#define debug(var)
#endif
#endif

using namespace pelib;
using namespace std;

static AmplInput input(AmplInput::intFloatHandlers());
static AmplOutput output(AmplOutput::intFloatHandlers());

// Include cpp format input
// Import the buildAlgebra() function
#include "make_algebra.cpp"

extern char _binary_fft_n15_1_graphml_start;
extern size_t _binary_fft_n15_1_graphml_size;
std::string string_taskgraph_graphml;
std::istringstream istream_taskgraph_graphml;

extern char _binary_fft_n15_1_input_dat_start;
extern size_t _binary_fft_n15_1_input_dat_size;
std::string string_taskgraph_amplinput;
std::istringstream istream_taskgraph_amplinput;

extern char _binary_fft_n15_1_output_dat_start;
extern size_t _binary_fft_n15_1_output_dat_size;
std::string string_taskgraph_amploutput;
std::istringstream istream_taskgraph_amploutput;

extern char _binary_p8_dat_start;
extern size_t _binary_p8_dat_size;
std::string string_platform;
std::istringstream istream_platform;

extern char _binary_schedule_xml_start;
extern size_t _binary_schedule_xml_size;
std::string string_schedule_xml;
std::istringstream istream_schedule_xml;

extern char _binary_crown_dat_start;
extern size_t _binary_crown_dat_size;
std::string string_schedule_crown;
std::istringstream istream_schedule_crown;

extern char _binary_schedule_dat_start;
extern size_t _binary_schedule_dat_size;
std::string string_schedule_amploutput;
std::istringstream istream_schedule_amploutput;

extern char _binary_platform_dat_start;
extern size_t _binary_platform_dat_size;
std::string string_platform_amplinput;
std::istringstream istream_platform_amplinput;

void
setup()
{
	std::cout << std::setprecision(6)
 		<< std::setiosflags(std::ios::fixed)
		<< std::setiosflags(std::ios::showpoint);
}

void
init()
{
	string_taskgraph_graphml = std::string(&_binary_fft_n15_1_graphml_start).substr(0, (size_t)(&_binary_fft_n15_1_graphml_size));
	istream_taskgraph_graphml.clear();
	istream_taskgraph_graphml.str(string_taskgraph_graphml);
	
	string_taskgraph_amplinput = std::string(&_binary_fft_n15_1_input_dat_start).substr(0, (size_t)(&_binary_fft_n15_1_input_dat_size));
	istream_taskgraph_amplinput.clear();
	istream_taskgraph_amplinput.str(string_taskgraph_amplinput);
	
	string_taskgraph_amploutput = std::string(&_binary_fft_n15_1_output_dat_start).substr(0, (size_t)(&_binary_fft_n15_1_output_dat_size));
	istream_taskgraph_amploutput.clear();
	istream_taskgraph_amploutput.str(string_taskgraph_amploutput);
	
	string_platform = std::string(&_binary_p8_dat_start).substr(0, (size_t)(&_binary_p8_dat_size));
	istream_platform.clear();
	istream_platform.str(string_platform);
	
	string_schedule_xml = std::string(&_binary_schedule_xml_start).substr(0, (size_t)(&_binary_schedule_xml_size));
	istream_schedule_xml.clear();
	istream_schedule_xml.str(string_schedule_xml);
	
	string_schedule_crown = std::string(&_binary_crown_dat_start).substr(0, (size_t)(&_binary_crown_dat_size));
	istream_schedule_crown.clear();
	istream_schedule_crown.str(string_schedule_crown);
	
	string_schedule_amploutput = std::string(&_binary_schedule_dat_start).substr(0, (size_t)(&_binary_schedule_dat_size));
	istream_schedule_amploutput.clear();
	istream_schedule_amploutput.str(string_schedule_amploutput);

	string_platform_amplinput = std::string(&_binary_platform_dat_start).substr(0, (size_t)(&_binary_platform_dat_size));
	istream_platform_amplinput.clear();
	istream_platform_amplinput.str(string_platform_amplinput);

/*
	set<float> f;
	f.insert(1);
	f.insert(2);
	f.insert(3);
	f.insert(4);
	f.insert(5);
	pt = Platform(8, new DummyCore(f));
*/
}

void
teardown()
{
  // Do not forget to free your stacks after each test
  // to avoid memory leaks as now
}

void
cleanup()
{
  // Destroy properly your test batch
}

void
parse_and_convert_graphml()
{
	Taskgraph tg_graphml = GraphML().parse(istream_taskgraph_graphml);
	Algebra ampl_arch = AmplInput(AmplInput::floatHandlers()).parse(istream_platform);
	Platform arch(ampl_arch);
	
	Algebra tg_graphml_algebra = tg_graphml.buildAlgebra(arch);
	Taskgraph tg_from_algebra(tg_graphml_algebra);

	string efficiency = tg_graphml.getTasks().begin()->getEfficiencyString();
	tg_from_algebra.setDeadlineCalculator("exprtk:var minF := 0; for(var j := 2; j <= n[]; j += 1) { minF += tau[j - 1] / (2 * p[] * min(F)); }; var maxF := 0; for(var j := 2; j <= n[]; j += 1) { maxF += tau[j - 1] / (2 * p[] * max(F)); }; minF + maxF");
	for(set<Task>::iterator i = (set<Task>::iterator)tg_from_algebra.getTasks().begin(); i !=  (set<Task>::iterator)tg_from_algebra.getTasks().end(); i++)
	{
		Task &t = (Task&)*i;
		t.setEfficiencyString(efficiency);
	}
	
	stringstream reference;
	GraphML().dump(reference, tg_from_algebra);

	if(reference.str().compare(string_taskgraph_graphml) != 0)
	{
		cout << "Expected: " << string_taskgraph_graphml << endl;
		cout << "Obtained: " << reference.str() << endl;
	}
	
	assert(reference.str().compare(string_taskgraph_graphml) == 0);

//	Algebra taskgraph_algebra = AmplInput(AmplInput::floatHandlers()).parse(istream_taskgraph_amplinput);

#if 0
	cout << string_taskgraph_graphml << endl;
	cout << "===============" << endl;
	cout << string_taskgraph_amplinput << endl;
	cout << "===============" << endl;
	cout << string_taskgraph_amploutput << endl;
	cout << "===============" << endl;
	cout << string_platform << endl;
	cout << "===============" << endl;
	cout << string_schedule_xml << endl;
	cout << "===============" << endl;
	cout << string_schedule_crown << endl;
	cout << "===============" << endl;
	cout << string_schedule_amplinput << endl;
	cout << "===============" << endl;
#endif
}

void
parse_and_convert_schedule()
{
	Taskgraph tg_graphml = GraphML().parse(istream_taskgraph_graphml);
	Algebra ampl_schedule = AmplOutput(AmplOutput::floatHandlers()).parse(istream_schedule_amploutput);
	Algebra alg_arch = AmplInput(AmplInput::floatHandlers()).parse(istream_platform_amplinput);
	Platform pt(alg_arch);

	Schedule schedule("Converted from AMPL", ampl_schedule);

	stringstream reference;
	XMLSchedule().dump(reference, schedule, tg_graphml, pt);

	if(reference.str().compare(string_schedule_xml) != 0)
	{
		cout << "Expected: " << string_schedule_xml << endl;
		cout << "Obtained: " << reference.str() << endl;
	}
	
	assert(reference.str().compare(string_schedule_xml) == 0);
}

void
run()
{
	test(parse_and_convert_graphml);
	test(parse_and_convert_schedule);
}

