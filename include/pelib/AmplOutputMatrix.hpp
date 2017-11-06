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


#include <pelib/AmplOutputData.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_AMPLOUTPUTMATRIX
#define PELIB_AMPLOUTPUTMATRIX

#ifdef debug
#undef debug
#endif

#if 0
#include <iostream>
#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"" << std::endl;
#else
#define debug(var)
#endif

namespace pelib
{
	/** Parser and output class of an instance of pelib::Matrix in AMPL output format **/
	template <class Col, class Row, class Value>		
	class AmplOutputMatrix: public AmplOutputData
	{
		typedef std::map<Col, Value> RowType;
		typedef std::map<Row, RowType> MatrixType;
		
		public:
			/** Constructor
				@param strict if 1 and parsing a matrix of floating-points values (except indexes), fails if any value parsed is an integer. An integer written as a float (e.g. 52.0) doesn't makes the parser to fail
			**/
			AmplOutputMatrix(bool strict = true)
			{
				this->strict = strict;
			}
			
			/** Returns a pointer to a copy of this instance **/
			virtual
			AmplOutputMatrix*
			clone() const
			{
				return new AmplOutputMatrix();
			}
			
			/** Returns a string boost::regex regular expression able to match a Matrix in AMPL output format and extract its name, indexes and values to an instance of pelib::Matrix **/
			virtual
			std::string
			getDetailedPattern()
			{
				return "(\\w[\\w\\d_]*)\\s*\\[\\*,\\*\\]\\s*:\\s*(.+?)\\s*:=\\s*(.+?)\\s*";
			}

			/** Returns a string boost::regex regular expression able to match a Matrix in AMPL output format **/
			virtual
			std::string
			getGlobalPattern()
			{
				return "\\w[\\w\\d_]*\\s*\\[\\*,\\*\\]\\s*:\\s*(?:[-\\w\\d\\.+]+\\s*)+\\s*:=.+";
			}

			/** Reads the content of an input stream in AMPL output text format and builds an instance of pelib::Matrix from all values read **/
			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				MatrixType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				boost::cmatch match; // = AlgebraDataParser::match(getDetailedPattern(), str);
				if(!boost::regex_match(str.c_str(), match, boost::regex(getDetailedPattern())))
				{
					throw ParseException(std::string("String \"").append(str).append("\" doesn't match regex \"").append(getDetailedPattern()).append("\". "));
				}

				std::string indexes = match[2];
				std::string remain = match[3];

				// Extract all indexes
				boost::regex isolated_value("\\s*([^\\s]+)\\s*");
				boost::sregex_token_iterator iter = make_regex_token_iterator(indexes, isolated_value, 1, boost::regex_constants::match_default);
				boost::sregex_token_iterator end;
			
				// Put them in a vector
				std::vector<Col> cols;
				for(; iter != end; ++iter)
				{
					try
					{
						cols.push_back(AlgebraDataParser::convert<Col>(*iter, strict));
					} catch(NoDecimalFloatException &e)
					{
						std::ostringstream ss;
						ss << e.getValue();
						throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));		
					}
				}
				int s = cols.size();

				// Parse the rest of the matrix; catch the row index every s reads
				iter = make_regex_token_iterator(remain, isolated_value, 1, boost::regex_constants::match_default);

				Row row;
				Col col;
				Value val;

				int integer_values = 0, total_values = 0;
				while(iter != end)
				{
					std::map<Col, Value> vector;

					try
					{
						row = AlgebraDataParser::convert<Row>(*iter, strict);
					} catch(NoDecimalFloatException &e)
					{
						std::ostringstream ss;
						ss << e.getValue();
						throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));		
					}
					iter++;
					for(int i = 0; i < s; i++)
					{
						try
						{
							val = AlgebraDataParser::convert<Value>(*iter, strict);
						} catch(NoDecimalFloatException &e)
						{
							float float_value = e.getValue();
							std::stringstream ss;
							ss << float_value;
							ss >> val;

							integer_values++;
						}
						col = cols[i];
						vector.insert(std::pair<Col, Value>(col, val));
						
						iter++;
						total_values++;
					}
					values.insert(std::pair<Row, RowType>(row, vector));
				}

				// If all values could have been parsed as integer, then this is obviously an integer vector rather to a float one
				if(integer_values == total_values)
				{
					//throw NoDecimalFloatException(std::string("Matrix only composed of integer-parsable values."), 0);
					throw ParseException(std::string("Matrix only composed of integer-parsable values."));
				}
				
				return new Matrix<Col, Row, Value>(match[1], values);
			}

			/** Writes the content of an instance of pelib::Matrix to a stream output in AMPL output format **/
			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Matrix<Col, Row, Value> *matrix = dynamic_cast<const Matrix<Col, Row, Value>*>(data);
				if(matrix == NULL) throw CastException("parameter \"data\" was not of type \"Matrix<Col, Row, Value>\".");
				
				o << matrix->getName() << " [*,*]" << std::endl << ":\t";
				typename std::map<Row, std::map<Col, Value> >::const_iterator row_iter = matrix->getValues().begin();
				if(row_iter != matrix->getValues().end())
				{
					for(typename std::map<Col, Value>::const_iterator col_iter = row_iter->second.begin(); col_iter != row_iter->second.end(); col_iter++)
					{	
						o << col_iter->first << "\t";
					}
					o << ":=" << std::endl;
				
					for(; row_iter != matrix->getValues().end(); row_iter++)
					{
						o << row_iter->first << "\t";
						for(typename std::map<Col, Value>::const_iterator col_iter = row_iter->second.begin(); col_iter != row_iter->second.end(); col_iter++)
						{	
							 o << col_iter->second << "\t";
						}
						o << std::endl;
					}
				}
				   
				o << ";" << std::endl;
			}
	
		protected:
			/** Defines if parsing operations are performed in strict mode **/
			bool strict;
		private:		
	};
}

#undef debug
#endif
