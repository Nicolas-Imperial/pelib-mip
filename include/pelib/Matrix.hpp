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
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/AlgebraData.hpp>
#include <pelib/PelibException.hpp>

#ifndef PELIB_MATRIX
#define PELIB_MATRIX

namespace pelib
{
	/** Simple Matrix implementation for linear algebra **/
	template <class Col, class Row, class Value>
	class Matrix: public AlgebraData
	{
		/** Type of a matrix row **/
		typedef std::map<Col, Value> RowType;
		/** Type of the matrix internal container **/
		typedef std::map<Row, RowType> MatrixType;
		
		public:
			/** Constructor: builds a matrix with the name and values given as parameters **/
			Matrix(const std::string name, const MatrixType values) : AlgebraData(name)
			{
				this->values = values;
			}

			/** Copy constructor **/
			Matrix(const Matrix<Col, Row, Value>* matrix): AlgebraData(matrix->getName()), values(matrix->getValues())
			{
				// Do nothing
			}

			/** Returns a pointer to a copy this matrix **/
			virtual
			Matrix*
			clone() const
			{
				return new Matrix<Col, Row, Value>(name, values);
			}

			/** Returns all values in the matrix **/
			virtual
			const MatrixType&
			getValues() const
			{
				return values;
			}

			/** Returns a value identified by its row and column **/
			virtual
			const Value&
			find(Col col, Row row) const
			{
				return values.find(row)->second.find(col)->second;
			}

			/** Returns the number of rows in the matrix **/
			virtual
			size_t
			getColSize() const // /!\ This is the size of a column, i.e. the number of rows
			{
				return values.size();
			}

			/** Returns the number of columns in the matrix **/
			virtual
			size_t
			getRowSize() const // /!\ This is the size of a row, i.e. the number of columns
			{
				return values.begin()->second.size();
			}

			/** Returns a new matrix instance that is the transposed of this matrix **/
			virtual
			Matrix<Row, Col, Value>
			transpose() const
			{
				std::map<Col, std::map<Row, Value> > new_values;
				for(typename MatrixType::const_iterator i = values.begin(); i != values.end(); i++)
				{
					for(typename RowType::const_iterator j = i->second.begin(); j != i->second.end(); j++)
					{
						Value val = j->second;
						new_values[j->first][i->first] = val;
					}
				}

				return Matrix<Row, Col, Value>(this->getName(), new_values);
			}
			
			void
			merge(const AlgebraData* ptr)
			{
				// Only allow merging if both this and ptr are of same name, type and row size
				if(ptr->getName().compare(this->getName()) == 0 &&
					std::string(typeid(*ptr).name()).compare(typeid(Matrix<Col, Row, Value>).name()) == 0)
				{
					Matrix<Col, Row, Value> *matrix = (Matrix<Col, Row, Value>*)ptr;
					// Do some complex merge work if the matrix to merge actually has data to merge
					if(matrix->getValues().size() > 0 && matrix->getValues().begin()->second.size() > 0)
					{
						// Only allow merge if this matrix is empty, or if not, 
						// if both matrices are of same row size (same number of columns)
						if (this->getValues().begin() == this->getValues().end() ||
							this->getValues().begin()->second.size() == matrix->getValues().begin()->second.size())
						{
							// Take all rows of the new matrix and merge it with the rows of this matrix
							for(typename MatrixType::iterator i = matrix->values.begin(); i != matrix->values.end(); i++)
							{
								// If this row's key already exists in this matrix, then delete the existing row
								if(this->getValues().find(i->first) != this->getValues().end())
								{
									this->values.erase(this->values.find(i->first));
								}

								// Add the new row in this matrix
								this->values.insert(std::pair<Row, std::map<Col, Value> >(i->first, i->second));
							}
						}
						else
						{
							throw PelibException(std::string("Matrices \"") + ptr->getName() + "\" and " + this->getName() + "\" are of incompatible size to merge.");
						}
					}
					else
					{
						// Nothing to merge, do nothing and return quietly
					}
				}
				else
				{
					throw PelibException(std::string("Cannot merge data \"") + ptr->getName() + "\" with " + typeid(Value).name() + " matrix of name \"" + this->getName() + "\".");
				}
			}
		protected:
			/** Internal container of the matrix values **/
			MatrixType values;
		private:	
	};
}

#endif
