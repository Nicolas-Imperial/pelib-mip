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

#include <pelib/AmplInput.hpp>
#include <pelib/AmplOutput.hpp>

#include <pelib/AmplInputData.hpp>
#include <pelib/AmplInputScalar.hpp>
#include <pelib/AmplInputVector.hpp>
#include <pelib/AmplInputMatrix.hpp>

#include <pelib/AmplOutputScalar.hpp>
#include <pelib/AmplOutputVector.hpp>
#include <pelib/AmplOutputMatrix.hpp>

using namespace pelib;

Algebra
parse(AlgebraParser &parser, std::istream &input)
{
	Algebra rec;
	
	try {
		rec = parser.parse(input);
	} catch(ParseException &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return rec;
}

int
main(int argc, char **argv)
{
	AmplInput input(AmplInput::floatHandlers());
	AmplOutput output(AmplOutput::floatHandlers());
	Algebra rec;

	// We use an AMPL output, output format
	AlgebraOutput &out = input;

	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)                                                                                                        
 	<< std::setiosflags(std::ios::fixed)                                                                                                     
	<< std::setiosflags(std::ios::showpoint);

	// Open input file
	std::ifstream myfile;
	myfile.open (argv[1], std::ios::in);

	// Load input file
	rec = parse(input, myfile);

	// Close input file
	myfile.close();

	out.dump(std::cout, rec);

	// Output one raw value in parameters scalar n, in vector Wi and float matrix e
	std::cout << rec.find<Scalar<float> >("n")->getValue() << std::endl;
	std::cout << rec.find<Vector<int, float> >("Wi")->getSize() << std::endl;
	std::cout << rec.find<Matrix<int, int, float> >("e")->getRowSize() << std::endl;

	// Extract, rename and output a few parameters
	Scalar<float> nn(rec.find<Scalar<float> >("n"));
	nn.setName("nn");
	out.dump(std::cout, &nn);

	Vector<int, float> yy(rec.find<Vector<int, float> >("Wi"));
	yy.setName("yy");
	out.dump(std::cout, &yy);
	
	Matrix<int, int, float> ee(rec.find<Matrix<int, int, float> >("e"));
	ee.setName("ee");
	out.dump(std::cout, &ee);

	return EXIT_SUCCESS;
}

