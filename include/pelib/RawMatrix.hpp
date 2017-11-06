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


#include <pelib/RawData.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_RAWMATRIX
#define PELIB_RAWMATRIX

#ifdef debug
#undef debug
#endif

#if 0
#include <iostream>
#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"" << std::endl;
#else
#define debug(var)
#endif

using namespace std;

namespace pelib
{
	/** Parser and output class of an instance of pelib::Matrix in AMPL output format **/
	template <class Col, class Row, class Value>		
	class RawMatrix: public RawData
	{
		typedef std::map<Col, Value> RowType;
		typedef std::map<Row, RowType> MatrixType;
		
		public:
			/** Returns a pointer to a copy of this instance **/
			virtual
			RawMatrix*
			clone() const
			{
				return new RawMatrix();
			}
			
			/** Writes the content of an instance of pelib::Matrix to a stream output in AMPL output format **/
			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{
				const Matrix<Col, Row, Value> *matrix = dynamic_cast<const Matrix<Col, Row, Value>*>(data);
				if(matrix == NULL) throw CastException("parameter \"data\" was not of type \"Matrix<Col, Row, Value>\".");
				map<Col, map<Row, Value> > record = matrix->getValues();
				for(typename map<Col, map<Row, Value> >::const_iterator i = record.begin(); i != record.end(); i++)
				{
					for(typename map<Row, Value>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
					{
						o << j->second << " ";
						//cerr << i->first << " " << j->first << " " << j->second << " ";
					}
					
					o << endl;
				}
			}
	
		protected:
		private:		
	};
}

#undef debug
#endif
