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
#include <map>
#include <set>

#include <pelib/AlgebraData.hpp>
#include <pelib/Record.hpp>
#include <pelib/constraints.hpp>

#ifndef PELIB_ALGEBRA
#define PELIB_ALGEBRA
namespace pelib
{
	/**
	  * Container of AMPL data structures such as Scalars, Vectors, Matrices and Sets.
	**/
	class Algebra : public Record
	{
		public:
			/** Creates an empty AMPL data container **/
			Algebra();

			/** Creates a AMPL data container from Algebra elements found in the map collect given as argument. Note that the string element in each pair element in the collection must match the name of the corresponding AlgebraData data structure. **/
			Algebra(const std::map<std::string, const AlgebraData * const> &elements);

			/** Copy constructor **/
			Algebra(const Algebra &rhs);
			Algebra(const Algebra *rhs);

			/** Destroys all elements in the collection and destroys the collection itself **/
			virtual	~Algebra();

			/** Returns all elements of any type contained in this collection instance **/
			const std::map<std::string, const AlgebraData * const>&
			getAllRecords() const;

			/** Builds a new collection from all elements in this collection and in the collection given as argument **/
			virtual Algebra
			merge(const Algebra&) const; 


			/** Returns all elements in the collection that match type T **/
			template<class T>
			std::map<std::string, const T* const>
			getRecords() const
			{
				// Make sure T is derived from AlgebraData
				Derived_from<T, AlgebraData>();

				// Build a new map containing all elements from records that could be casted to Algebra
				std::map<std::string, const T * const> record;

				for (std::map<std::string, const AlgebraData * const>::const_iterator i = records.begin(); i != records.end(); i++)
				{
					// Try to cast this type


					const AlgebraData *ptr = i->second;

					T* elem = dynamic_cast<T*>(ptr);
					
					if(elem != NULL)
					{
						record.insert(std::pair<std::string, const T * const>(i->first, elem));
					}
				}

				return record;
			}

			/** Returns the element of type T whose name matches the key argument **/
			template<class T>
			const T * const
			find(std::string key) const
			{
				// Make sure T is derived from AlgebraData
				Derived_from<T, AlgebraData>();

				typename std::map<std::string, const AlgebraData * const>::const_iterator iter = records.find(key);
				if(iter != records.end())
				{
					const T * const elem = dynamic_cast<const T * const >(iter->second);

					if(elem != NULL)
					{
						return elem;
					}
					else
					{
						return NULL;
					}
				}
				else
				{
					return NULL;
				}
			}

			/** Removes all records whose name does not appear in the list **/
			Algebra filter(const std::set<std::string> &list) const;

			/** Does nothing at all **/
			void insert(const Data *data) {}

			/** Copies and insert a AlgebraData element in the collection **/
			virtual void insert(const pelib::AlgebraData *data);

			/** Finds and remove a AlgebraData element from this collection, based on its name **/
			virtual void
			remove(const std::string name);

			/** Flushes all elements in this collection, performs a copy of all elements found in the collection given as argument and place the copies in this collection **/
			virtual
			Algebra&
			operator=(const Algebra &rhs);

			virtual Algebra* clone() const;
			
		protected:
			/** Map container of all elements in the collection **/
			std::map<std::string, const AlgebraData * const> records;

			/** Flushes all elements in this collection **/
			void
			deleteRecords();	
		private:
	};
}

#endif
