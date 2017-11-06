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


#include <sys/types.h>
#include <sys/wait.h>
#include <ext/stdio_filebuf.h>
#include <boost/regex.hpp>

#include <pelib/PelibException.hpp>
#include <pelib/AmplSolver.hpp>
#include <pelib/AmplInput.hpp>
#include <pelib/AmplOutput.hpp>

#ifdef debug
#undef debug
#endif

#define debug(var) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << endl;

using namespace std;
using namespace boost;
using namespace pelib;

static
string
istream_to_string(istream &in)
{
	istreambuf_iterator<char> eos;
	return string(istreambuf_iterator<char>(in), eos);
}

void
AmplSolver::initModelFromStream(istream &model, const string &model_filename, istream &run, bool showOutput, bool showError)
{
	this->model = istream_to_string(model);
	this->run = istream_to_string(run);
	this->model_file = model_filename;
	this->showOutput = showOutput;
	this->showError = showError;
}

AmplSolver::AmplSolver(istream &model, const string &model_filename, istream &run, bool showOutput, bool showError)
{
	initModelFromStream(model, model_filename, run, showOutput, showError);
}

AmplSolver::AmplSolver(istream &model, const string &model_filename, istream &run, const map<const string, const Algebra> &data, bool showOutput, bool showError)
{
	initModelFromStream(model, model_filename, run, showOutput, showError);
	this->data = data;
}

const Algebra*
AmplSolver::solve() const
{
	map<const string, double> statistics;
	return new Algebra(this->solve(this->data, statistics));
}

const Algebra*
AmplSolver::solve(map<const string, double> &statistics) const
{
	return new Algebra(this->solve(this->data, statistics));
}

// From http://codereview.stackexchange.com/questions/71793/function-to-grab-stdin-stdout-stderr-of-a-child-process
static
int
opencmd(string cmd, vector<string> argv, istream &stdin, string &out, string &err)
{
	int infd[2];
	int outfd[2];
	int errfd[2];
	pid_t pid;

	argv.insert(argv.begin(), cmd);
	char **c_argv = (char**)malloc(sizeof(char*) * (argv.size() + 1));
	c_argv[argv.size()] = NULL;
	//debug("Hello world!");
	//cout << cmd << " ";
	for(vector<string>::iterator i = argv.begin(); i != argv.end(); i++)
	{
		size_t ii = distance(argv.begin(), i);
		c_argv[ii] = (char*)i->c_str();
		//cout << "'" << c_argv[ii] << "' ";
	}
	//cout << endl;
	//debug("Hello world!");

	/*
	 * Creating pipes in between ...
	 */
	if(pipe(infd) != 0)
	{
		return -1;
	}

	if(pipe(outfd) != 0)
	{
		close(infd[0]);
		close(infd[1]);

		return -1;
	}

	if(pipe(errfd))
	{
		close(outfd[0]);
		close(outfd[1]);
		close(infd[0]);
		close(infd[1]);

		return -1;
	}

	/*
	 * Starting actual processing ...
	 */
	pid = fork();
	switch (pid) {
		case -1:       /* Error */
			close(errfd[0]);
			close(errfd[1]);
			close(outfd[0]);
			close(outfd[1]);
			close(infd[0]);
			close(infd[1]);
		
			return -1;
		break;
		case 0:        /* Child */
			close(infd[1]);
			close(outfd[0]);
			close(errfd[0]);

			dup2(infd[0], 0);    /* redirect child stdin to in[0] */
			dup2(outfd[1], 1);   /* redirect child stdout to out[1] */
			dup2(errfd[1], 2);   /* redirect child stderr to err[1] */
			execvp(cmd.c_str(), c_argv); /* actual command execution */

			return -1; /* shall never be executed */
		break;
		default:       /* Parent */
			close(infd[0]);  /* no need to read its stdin */
			close(outfd[1]); /* no need to write to its stdout */
			close(errfd[1]); /* no need to write to its stderr */

			// Turn fd into string stream
			// hack seen here:
			// http://stackoverflow.com/questions/2746168/how-to-construct-a-c-fstream-from-a-posix-file-descriptor
			__gnu_cxx::stdio_filebuf<char> filein(infd[1], ios::out);
    			ostream pstdin(&filein);

			__gnu_cxx::stdio_filebuf<char> fileout(outfd[0], ios::in);
    			istream pstdout(&fileout);
			__gnu_cxx::stdio_filebuf<char> fileerr(errfd[0], ios::in);
    			istream pstderr(&fileerr);

			// Write stdin to process' stdin
			string string = istream_to_string(stdin);
			pstdin << string;

			// This will not run before the solver is done
			out = istream_to_string(pstdout);
			err = istream_to_string(pstderr);

			// Wait for child process to terminate
			int status;
			waitpid(pid, &status, WUNTRACED);

			/*
			debug(WIFSIGNALED(status));
			debug(WIFEXITED(status));
			debug(WEXITSTATUS(status));
			*/

			if(WIFEXITED(status) == 1 && WIFSIGNALED(status) == 0)
			{
				status = WEXITSTATUS(status);
				//debug(status);
			}
			else
			{
				status = 0;
				//debug(status);
			}

			// Close streams
			filein.close();
			fileout.close();
			fileerr.close();

			// Free argument list
			free(c_argv);
			return status;
		break;
	}
}

Algebra
AmplSolver::solve(const std::map<const std::string, const Algebra> &data) const
{
	map<const string, double> statistics;
	return solve(data, statistics);
}

Algebra
AmplSolver::solve(const std::map<const std::string, const Algebra> &data, map<const string, double> &statistics) const
{
	if(this->model.compare("") != 0 && this->run.compare("") != 0)
	{
		// Do solving
		stringstream in;
		string out, err;
		vector<string> args;

		size_t counter = 1;
		stringstream str;
		str << counter;
		string newrun = run;
		newrun = boost::regex_replace(newrun, regex("\\\\"), string("\\\\\\\\\\\\\\\\\\\\\\\\"));
		newrun = boost::regex_replace(newrun, regex("\\$"), string("\\\\\\\\\\\\$"));
		newrun = boost::regex_replace(newrun, regex("^\\s*model\\s+" + this->model_file + "\\s*;"), string("model \\\\\\$" + str.str() + ";"));
		counter++;

		string data_args;

		for(map<string, const Algebra>::const_iterator i = data.begin(); i != data.end(); i++)
		{
			stringstream str;
			str << counter;

			// Replace data statement in model
			newrun = boost::regex_replace(newrun, regex("^\\s*data\\s+" + i->first + "\\s*;"), string("data \\\\\\$" + str.str() + ";"));

			// Build input data sources
			stringstream data;
			AmplInput(AmplInput::intFloatHandlers()).dump(data, i->second);
			// Fixes possible quote and backslashes issues in input data file
			string data_str = boost::regex_replace(data.str(), regex("\""), string("\"'\"'\""));
			data_str = boost::regex_replace(data_str, regex("\\\\"), string("\\\\"));
			data_str = boost::regex_replace(data_str, regex("\\$"), string("\\$"));

			data_args += " <(echo '" + data_str + "')";
			counter++;
		}

		// Fix possible quotes and backslashes issues in input data file
		newrun = boost::regex_replace(newrun, regex("'"), string("\\'"));
		newrun = boost::regex_replace(newrun, regex("\""), string("\\\\\"'\\\\\"'\\\\\""));

		args.push_back("-c");
		args.push_back("bash <(echo -e \"#!/bin/bash\nampl <(echo -e \\\"" + newrun + "\\\");\") <(echo -e \"" + model + "\")" + data_args + "");
		int success = opencmd("bash", args, in, out, err);

		bool optimal = out.find("time limit") == std::string::npos && out.find("optimal solution") != std::string::npos;
		bool feasible = !((out.find("infeasible") != std::string::npos) || (out.find(" cannot hold") != std::string::npos) || (out.find("bailing out") != std::string::npos) || (err.find(" but lower bound") != std::string::npos) || (out.find("No primal or dual variables returned") != std::string::npos) || (err.find("Logical constraint is always false") != std::string::npos));
		
		statistics.insert(pair<string, double>(string("optimal"), optimal ? 1 : 0));
		statistics.insert(pair<string, double>(string("feasible"), feasible ? 1 : 0));

		if(showError)
		{
			cerr << err;
		}
		
		if(showOutput)
		{
			cout << out;
		}

		// If the solver didn't fail, but determined that their is no possible solution, then report output
		if(success == 0 || !feasible)
		{
			out = boost::regex_replace(out, regex("^\\s*absmipgap[^\\n]*\\n"), string(""));
			stringstream outstr(out);
			Algebra solution = AmplOutput(AmplOutput::floatHandlers()).parse(outstr);

			if(solution.find<Scalar<float> >("_total_solve_elapsed_time") != NULL)
			{
				Scalar<float> time = *solution.find<Scalar<float> >("_total_solve_elapsed_time");
				statistics.insert(pair<const string, double>(string("time"), time.getValue()));
			}
			return solution;
		}
		else
		{	
			stringstream ss;
			ss << success;
			throw PelibException(string("Ampl exited with value ") + ss.str() + string("."));
		}
	}
	else
	{
		throw PelibException(string("Missing model or run script to run with Ampl."));
	}
}

