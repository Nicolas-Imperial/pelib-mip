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


#include <pelib/CppData.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_CPPMATRIX
#define PELIB_CPPMATRIX

namespace pelib
{
	/** Output class Matrix as C++ code **/
	template <class Col, class Row, class Value>		
	class CppMatrix: public CppData
	{
		typedef std::map<Col, Value> RowType;
		typedef std::map<Row, RowType> MatrixType;
		
		public:
			/** Constructor **/
			CppMatrix() : CppData()
			{
				// Do nothing
			}
			
			/** Returns a pointer to a copy of this class instance **/
			virtual
			CppMatrix*
			clone() const
			{
				return new CppMatrix();
			}

			/** Generates C++ code that instanciates and instance of pelib::Matrix and fills it with all values contained in the Matrix given as parameter
				@param o Output stream where C++ code is written to
				@param data Instance of pelib::Matrix that is written to output stream as C++ code
			**/
			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Matrix<Col, Row, Value> *matrix = dynamic_cast<const Matrix<Col, Row, Value>*>(data);
				if(matrix == NULL) throw CastException("parameter \"data\" was not of type \"Matrix<Col, Row, Value>\".");

				// Dummy key and value and string for their type
				Row row;
				Col col;
				Value value;
				std::string row_str = type_name(typeid(row)).c_str();
				std::string col_str = type_name(typeid(col)).c_str();
				std::string value_str = type_name(typeid(value)).c_str();

				// Declare a map to contain the whole matrix
				o << "std::map<" << row_str.c_str() << ", std::map<" << col_str.c_str() << ", " << value_str.c_str() << "> > map_" << matrix->getName() << ";" << std::endl;

				// Declare a map to contain each row
				o << "std::map<" << col_str.c_str() << ", " << value_str.c_str() << "> map_row;" << std::endl;
				for(typename std::map<Row, std::map<Col, Value> >::const_iterator row_iter = matrix->getValues().begin();
				    row_iter != matrix->getValues().end(); row_iter++)
				{
					row = row_iter->first;
					for(typename std::map<Col, Value>::const_iterator col_iter = row_iter->second.begin(); col_iter != row_iter->second.end(); col_iter++)
					{
						col = col_iter->first;
						value = col_iter->second;

						// Add a value to row
						o << "map_row.insert(std::pair<" << col_str.c_str() << ", " << value_str.c_str() << ">(" << col << ", " << value << "));" << std::endl;
					}

					// Add the row to matrix
					o << "map_" << matrix->getName() << ".insert(std::pair<" << row_str.c_str()
						<< ", std::map<" << col_str.c_str() << ", " << value_str.c_str()
						<< "> >(" << row << ", map_row));" << std::endl;

					// Reinitialize row
					o << "map_row = std::map<" << col_str.c_str() << ", " << value_str.c_str() << ">();" << std::endl;
				}

				// Add matrix to data structure
				o << "pelib::Matrix<" << row_str.c_str() << ", " << col_str.c_str() << ", " << value_str.c_str() << "> " << matrix->getName() << "(\"" << matrix->getName() << "\", map_e);" << std::endl;  
			}
	
		protected:
		private:		
	};
}

#endif
