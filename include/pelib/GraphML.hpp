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


#include <pelib/Taskgraph.hpp>

#include <pelib/GraphMLParser.hpp>
#include <pelib/GraphMLOutput.hpp>
#include <pelib/Platform.hpp>

#ifndef PELIB_GRAPHML_H
#define PELIB_GRAPHML_H

namespace pelib
{
	/** Parser and output class of taskgraph in GraphML format **/
	class GraphML : public GraphMLParser, public GraphMLOutput
	{
		public:
			/** Destructor **/
			virtual ~GraphML();
			/** Returns a pointer to a copy of this instance of GraphML **/
			virtual GraphML* clone() const;

			/** Produces and instance of Taskgraph by parsing text input stream in GraphML format
				@return is Input text stream in GraphML format
			**/
			virtual Taskgraph* parse(std::istream &is) const;
			/** Output an instance of pelib::Taskgraph to text output stream in GraphML format
				@param str output stream to write GraphML taskgraph to
				@param data Instance of pelib::Taskgraph to be written in output stream
			**/
			virtual void dump(std::ostream &str, const Taskgraph *data) const;
			/** Output an instance of pelib::Taskgraph to text output stream in GraphML format
				@param str output stream to write GraphML taskgraph to
				@param data Instance of pelib::Taskgraph to be written in output stream
			**/
			virtual void dump(std::ostream &str, const Taskgraph &data) const;
			/** Output an instance of pelib::Taskgraph to text output stream in GraphML format with formula interpreted to fit an execution platform
				@param str output stream to write GraphML taskgraph to
				@param data Instance of pelib::Taskgraph to be written in output stream
				@param arch Instance of pelib::Platform to parse deadline and efficiency formulas in Taskgraph description
			**/
			virtual	void dump(std::ostream &os, const Taskgraph &data, const Platform &arch) const;
			/** Output an instance of pelib::Taskgraph to text output stream in GraphML format with formula interpreted to fit an execution platform
				@param str output stream to write GraphML taskgraph to
				@param data Instance of pelib::Taskgraph to be written in output stream
				@param arch Instance of pelib::Platform to parse deadline and efficiency formulas in Taskgraph description
			**/
			virtual void dump(std::ostream &os, const Taskgraph *data, const Platform *arch) const;

			static const std::string producerName, consumerName;
			static const std::string type;
			static const std::string producer_rate;
			static const std::string consumer_rate;
		protected:
	};
}

#endif
