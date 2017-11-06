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
#include <pelib/AmplInputData.hpp>

#ifndef PELIB_AMPLINPUT
#define PELIB_AMPLINPUT

namespace pelib
{
	/** Parses and output AMPL input data format (.dat) from and to AlgebraData instances, using AmplInputDataParsers and AmplInputDataOutputs **/
	class AmplInput: public AlgebraParser, public AlgebraOutput
	{
		public:
			/** Creates a new instance of AmplInput parser and output. Parser and Output attempt to use all parsers and output classes until one produces output without throwing any instance of Parse- or Cast- exceptions. **/
			AmplInput(std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > handlers);

			/** Destroys the parser and output classes and destroys this instance. **/
			virtual
			~AmplInput();

			/** Parses AMPL input text from input stream and returns an instance of Algebra containing all data structure that could be parsed successfully, using parsers in the parser collection given when the class was instanciated. Parsing is performed in strict mode (See AlgebraDataParser). Any element that could not be parsed is parsed as Scalar, Vector, Matrix or Set of strings. **/
			Algebra
			parse(std::istream &data) const;

			/** Outputs all elements in the given Algebra instance into output stream in output stream o in AmplInput format, using output classes given when the class was instanciated. Also output all string-based AlgebraData instances **/
			virtual
			void
			dump(std::ostream& o, const Algebra &record) const;

			/** Outputs an instance of AlgebraData instance into output stream in output stream o in AmplInput format, using output classes given when the class was instanciated. If no output could produce an output, uses a string-based output class **/
			virtual
			void
			dump(std::ostream& o, const AlgebraData *data) const;

			/** Outputs an instance of AlgebraData instance into output stream in output stream o in AmplInput format, using output classes given when the class was instanciated. If no output could produce an output, uses a string-based output class **/
			virtual
			void
			dump(std::ostream& o, const AlgebraData &data) const;

			/** Copy constructor. Flushes all AmplInputData Parser and Output classes and copies the ones from the instance of AmplInput given as reference */
			virtual
			AmplInput&
			operator=(const AmplInput &rhs);

			/** Default AmplInput parsers to produce float derivatives instances of AmplInputScalar, AmplInputVector, AmplInputMatrix and AmplInputSet classes. **/
			static std::vector<AmplInputDataParser*> floatParsers();
			/** Default AmplInput outputs to output float derivatives instances of AmplInputScalar, AmplInputVector, AmplInputMatrix and AmplInputSet classes. **/
			static std::vector<AmplInputDataOutput*> floatOutputs();
			/** Default AmplInput parsers and outputs to produce and output float derivatives instances of AmplInputScalar, AmplInputVector, AmplInputMatrix and AmplInputSet classes. **/
			static std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > floatHandlers();

			/** Default AmplInput parsers to produce integer or float derivatives instances of AmplInputScalar, AmplInputVector, AmplInputMatrix and AmplInputSet classes. **/
			static std::vector<AmplInputDataParser*> intFloatParsers();
			/** Default AmplInput outputs to output integer or float derivatives instances of AmplInputScalar, AmplInputVector, AmplInputMatrix and AmplInputSet classes. **/
			static std::vector<AmplInputDataOutput*> intFloatOutputs();
			/** Default AmplInput parsers and outputs to produce and output integer and float derivatives instances of AmplInputScalar, AmplInputVector, AmplInputMatrix and AmplInputSet classes. **/
			static std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > intFloatHandlers();

			/** Default AmplInput parsers to produce string derivatives instances of AmplInputScalar, AmplInputVector, AmplInputMatrix and AmplInputSet classes. Used as last-resort parsers. **/
			static std::vector<AmplInputDataParser*> stringParsers();
			/** Default AmplInput outputs to output string derivatives instances of AmplInputScalar, AmplInputVector, AmplInputMatrix and AmplInputSet classes. Used as last-resort outputs **/
			static std::vector<AmplInputDataOutput*> stringOutputs();
			/** Default AmplInput parsers and outputs to produce and output float derivatives instances of AmplInputScalar, AmplInputVector, AmplInputMatrix and AmplInputSet classes. Used at last resort in parsing and output operations. **/
			static std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > stringHandlers();

		protected:
			/** Contains parser classes **/
			std::vector<AmplInputDataParser*> parsers;
			/** Contains output classes **/
			std::vector<AmplInputDataOutput*> outputs;

			/** Flushes the parser collection **/
			void
			deleteParsers();

			/** Flushes the output collection **/
			void
			deleteOutputs();
		private:
	};
}

#endif

