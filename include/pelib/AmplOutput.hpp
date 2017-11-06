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

#include <pelib/AlgebraParser.hpp>
#include <pelib/AlgebraOutput.hpp>
#include <pelib/AmplDataParser.hpp>
#include <pelib/AmplDataOutput.hpp>
#include <pelib/AmplOutputData.hpp>

#ifndef PELIB_AMPLOUTPUT
#define PELIB_AMPLOUTPUT

namespace pelib
{
	/** Parser and output class of a collection of algebraic data structures, in AMPL output format **/
	class AmplOutput: public AlgebraParser, public AlgebraOutput
	{
		public:
			/** Constructor
				@param handlers A pair of collections of Algebraic Data parsers and Output. May include parsers and output for scalars, vector, set or matrices for int, float, double or any other basic type.
			**/
			AmplOutput(std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > handlers);

			/** Destructor **/
			virtual
			~AmplOutput();
			
			/** Reads input stream and produces a collection of pelib::AlgebraData derived class instances such as Scalar, Vector, Set and Matrix. Warning, this can be a ver slow process with large inputs **/
			Algebra
			parse(std::istream &data) const;

			/** Writes all derived instance of pelib::AlgebraData in the container to output stream in AMPL output text format **/
			virtual
			void
			dump(std::ostream& o, const Algebra &record) const;

			/** Writes a single derived instance of pelib::Algebradata to output stream in AMPL output format **/
			virtual
			void
			dump(std::ostream& o, const AlgebraData *data) const;

			/** Writes a single derived instance of pelib::Algebradata to output stream in AMPL output format **/
			virtual
			void
			dump(std::ostream& o, const AlgebraData &data) const;

			/** Assignment operator. Copies all parsers and output handlers to the recipient instance **/
			virtual
			AmplOutput&
			operator=(const AmplOutput &rhs);

			/** Returns parsers specialized into floating-point content **/
			static std::vector<AmplOutputDataParser*> floatParsers();
			/** Returns output specialized into floating-point content **/
			static std::vector<AmplOutputDataOutput*> floatOutputs();
			/** Returns parsers and output specialized into floating-point content **/
			static std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > floatHandlers();

			/** Returns parsers specialized into integer and floating-point content **/
			static std::vector<AmplOutputDataParser*> intFloatParsers();
			/** Returns output specialized into integer and floating-point content **/
			static std::vector<AmplOutputDataOutput*> intFloatOutputs();
			/** Returns parsers and output specialized into integer and floating-point content **/
			static std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > intFloatHandlers();

			/** Returns parsers specialized into string content. Used when no other parser can parse some content **/
			static std::vector<AmplOutputDataParser*> stringParsers();
			/** Returns outputs specialized into string content **/
			static std::vector<AmplOutputDataOutput*> stringOutputs();
			/** Returns parsers and outputs specialized into string content **/
			static std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > stringHandlers();
		protected:
			std::vector<AmplOutputDataParser*> parsers;
			std::vector<AmplOutputDataOutput*> outputs;

			/** Flushes all parsers in this instance. used by assignement operator **/
			void
			deleteParsers();

			/** Flushes all outputs in this instance. used by assignement operator **/
			void
			deleteOutputs();
		private:
	};
}

#endif
