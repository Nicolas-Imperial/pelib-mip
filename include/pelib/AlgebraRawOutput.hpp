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

#include <pelib/AlgebraOutput.hpp>
#include <pelib/RawDataOutput.hpp>

#ifndef PELIB_ALGEBRARAWOUTPUT
#define PELIB_ALGEBRARAWOUTPUT

namespace pelib
{
	/** Parser and output class of a collection of algebraic data structures, in AMPL output format **/
	class AlgebraRawOutput: public AlgebraOutput
	{
		public:
			/** Constructor
				@param handlers A pair of collections of Algebraic Data parsers and Output. May include parsers and output for scalars, vector, set or matrices for int, float, double or any other basic type.
			**/
			AlgebraRawOutput(std::vector<RawDataOutput*> handlers);
			/** Constructor
				@param handlers A pair of collections of Algebraic Data parsers and Output. May include parsers and output for scalars, vector, set or matrices for int, float, double or any other basic type.
				@param list List of element is algebra container to produce in output
			**/
			AlgebraRawOutput(std::vector<RawDataOutput*> handlers, const std::vector<std::string> &list);

			/** Destructor **/
			virtual
			~AlgebraRawOutput();
			
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
			AlgebraRawOutput&
			operator=(const AlgebraRawOutput &rhs);
			/** Returns output specialized into floating-point content **/
			static std::vector<RawDataOutput*> floatOutputs();
			/** Returns output specialized into integer and floating-point content **/
			static std::vector<RawDataOutput*> intFloatOutputs();
			/** Returns outputs specialized into string content **/
			static std::vector<RawDataOutput*> stringOutputs();
		protected:
			std::vector<RawDataOutput*> outputs;

			/** Flushes all outputs in this instance. used by assignement operator **/
			void
			deleteOutputs();
			std::vector<std::string> output_list;
		private:
	};
}

#endif
