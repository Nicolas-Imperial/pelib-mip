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

#include <sstream>

#include <pelib/exprtk.hpp>
#include <pelib/pelib_exprtk.hpp>

using namespace std;

namespace pelib
{
	struct print : public exprtk::ifunction<double>
	{
		public:
			print(ostream &stream)
				: exprtk::ifunction<double>(1), stream(stream)
			{}  

			inline double operator()(const double &value) 
			{   
				stream << value << endl;
				return value;
			}   

		private:
			ostream &stream;
	};

	struct e: public exprtk::ifunction<double>
	{
		public:
			e(const set<Task> &tasks, const Platform &arch)
				: exprtk::ifunction<double>(2), tasks(tasks), arch(arch)
			{}  

			inline double operator()(const double &id, const double &p) 
			{
				set<Task>::const_iterator begin = tasks.begin();
				std::advance(begin, id - 1);
				double val = begin->getEfficiency((int)p);
				return val;
			}   

		private:
			const set<Task> &tasks;
			const Platform &arch;
	};

	double
	parseEfficiency(const string &formula, double W, double tau, double p)
	{
		print print_cout(cout);
		print print_cerr(cerr);

		typedef exprtk::expression<double> expression_t;
		typedef exprtk::parser<double> parser_t;

		expression_t expression;

		exprtk::symbol_table<double> symbol_table;
		symbol_table.add_variable("W", W);
		symbol_table.add_variable("p", p);
		symbol_table.add_variable("tau", tau);
		symbol_table.add_function("cout", print_cout);
		symbol_table.add_function("cerr", print_cerr);

		expression.register_symbol_table(symbol_table);

		parser_t parser;

		parser.compile(formula, expression);	
		return expression.value();
	}

	double
	parseDeadline(const string &formula, const set<Task> &tasks, const Platform &pt, vector<double> n, vector<double> p, vector<double> F, vector<double> tau, vector<double> W)
	{
		e matrix_e(tasks, pt);

		print print_cout(cout);
		print print_cerr(cerr);

		typedef exprtk::expression<double> expression_t;
		typedef exprtk::parser<double> parser_t;
		expression_t expression;

		exprtk::symbol_table<double> symbol_table;
		symbol_table.add_vector("n", n);
		symbol_table.add_vector("p", p);
		symbol_table.add_vector("F", F);
		symbol_table.add_vector("tau", tau);
		symbol_table.add_vector("W", W);
		symbol_table.add_function("e", matrix_e);
		symbol_table.add_function("cout", print_cout);
		symbol_table.add_function("cerr", print_cerr);

		expression.register_symbol_table(symbol_table);

		parser_t parser;

		parser.compile(formula, expression);	
		return expression.value();
	}
}
