/*
 Copyright 2015 Nicolas Melot, Johan Janzén

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


#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <boost/algorithm/string.hpp>

#include <signal.h>
#include <pthread.h>
extern "C"{
#include <igraph.h>
}

#include <pelib/Taskgraph.hpp>
#include <pelib/GraphML.hpp>

#include <pelib/Scalar.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/Set.hpp>
#include <pelib/Task.hpp>
#include <pelib/AbstractLink.hpp>
#include <pelib/Buffer.hpp>

#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>

#ifdef debug
#undef debug
#endif

#if defined DEBUG && DEBUG != 0
#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#endif

using namespace pelib;
using namespace std;
using namespace xmlpp;

struct reader_args
{
	FILE *instream;
	ostream *o;
};
typedef struct reader_args reader_args_t;

const string GraphML::producerName = "producer_name";
const string GraphML::consumerName = "consumer_name";
const string GraphML::type = "type";
const string GraphML::producer_rate = "producer_rate";
const string GraphML::consumer_rate = "consumer_rate";

static void*
thread_reader(void* aux)
{
	reader_args_t *args = (reader_args_t*)aux;
	
	char c;
	while ((c = fgetc (args->instream)) != EOF)
	{
		(*args->o) << c;
	}

	fclose(args->instream);
	return NULL;
}

struct writer_args
{
	int file_descr;
	istream *i;
};
typedef struct writer_args writer_args_t;

static void*
thread_writer(void* aux)
{
	writer_args_t *args = (writer_args_t*)aux;

	char c = args->i->get();
	while(!args->i->eof())
	{
		size_t ans = write(args->file_descr, &c, 1);
		
		if(ans < 0)
		{
			throw runtime_error("Error: Pipe has been possessed X-(");
		}
		
		c = args->i->get();
	}
	
	close(args->file_descr);
	return NULL;
}

GraphML::~GraphML()
{
	// Do nothing
}

void
GraphML::dump(ostream& os, const Taskgraph *data, const Platform *arch) const
{
	const Taskgraph *tg = dynamic_cast<const Taskgraph* >(data);
	if(tg == NULL) throw CastException("Parameter \"data\" was not of type \"Taskgraph*\".");

	igraph_t *graph = new igraph_t();
	igraph_i_set_attribute_table(&igraph_cattribute_table); //do this to enable attribute fetching

	/*
	igraph_vector_init(tg, 8);
  	
  	VECTOR(v1)[2]=1; VECTOR(v1)[3]=2;
  	VECTOR(v1)[4]=2; VECTOR(v1)[5]=3;
  	VECTOR(v1)[6]=2; VECTOR(v1)[7]=2;
	  */
	igraph_vector_t edges;
	igraph_vector_init(&edges, 0);
	//VECTOR(edges)[0]=0; VECTOR(edges)[1]=1;
  	igraph_create(graph, &edges, 0, true);

	// Add graph attributes
	SETGAS(graph,"name", tg->getName().c_str());
	SETGAS(graph,"deadline", tg->getDeadlineCalculator().c_str());

	// Add vertices
	int ret = igraph_add_vertices(graph, tg->getTasks().size(), 0);
	if(ret == IGRAPH_EINVAL) throw CastException("Could not add vertices to igraph.");
	if(arch != NULL && !(arch->isHomogeneous()))
	{
		throw CastException("Cannot output discretive efficiency function for a heterogeneous platform.");
	}

	size_t counter = 0;
	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++, counter++)
	{
		Task task = *i;
		
		SETVAS(graph, "name", counter, task.getName().c_str());
		SETVAS(graph, "module", counter, task.getModule().c_str());
		SETVAN(graph, "workload", counter, task.getWorkload());
		stringstream max_width;

		// If no platform is provided, just dump efficiency and max width as is.
		// Otherwise, compute and output each possible value one by one, and
		// replace any larger value of max_width than number of core, by number of cores 
		if (arch == NULL)
		{
			SETVAS(graph, "efficiency", counter, task.getEfficiencyString().c_str());
			max_width << task.getMaxWidth();
		}
		else
		{
			stringstream ss;
			for(size_t j = 1; j <= arch->getCores().size(); j++)
			{
				ss << task.getEfficiency(j) << " ";
			}

			SETVAS(graph, "efficiency", counter, ss.str().c_str());
			if(task.getMaxWidth() >= arch->getCores().size())
			{
				max_width << arch->getCores().size();
			}
			else
			{
				max_width << task.getMaxWidth();
			}
		}

		SETVAS(graph, "max_width", counter, max_width.str().c_str());
	}

	counter = 0;
	for(set<AbstractLink>::const_iterator i = tg->getLinks().begin(); i != tg->getLinks().end(); i++, counter++)
	{
		int ret = igraph_add_edge(graph, std::distance(tg->getTasks().begin(), tg->getTasks().find(*i->getProducer())), std::distance(tg->getTasks().begin(), tg->getTasks().find(*i->getConsumer())));
		if(ret == IGRAPH_EINVAL) throw CastException("Could not add vertices to igraph.");
		SETEAS(graph, producerName.c_str(), counter, i->getProducerName().c_str());
		SETEAS(graph, consumerName.c_str(), counter, i->getConsumerName().c_str());
		if(i->getDataType().compare(string()) != 0)
		{
			SETEAS(graph, type.c_str(), counter, i->getDataType().c_str());
		}
		if(i->getProducerRate() > 0)
		{
			SETEAN(graph, GraphML::producer_rate.c_str(), counter, i->getProducerRate());
		}
		if(i->getConsumerRate() > 0)
		{
			SETEAN(graph, GraphML::consumer_rate.c_str(), counter, i->getConsumerRate());
		}
	}

	// Dump data to stream os
	int p[2];
	int ans = pipe(p);
	if(ans)
	{
		throw runtime_error("Error: no pipe :/");
	}

	FILE *fake_fileptr = fdopen(p[1], "w"); 
	FILE *instream = fdopen (p[0], "r");

	// Spawn a thread that reads from the pipe as fast as it can.
	// This is required if igraph indend to output massive amounts of data
	pthread_attr_t attr;
	pthread_t thread;
	reader_args_t args;
	args.instream = instream;
	args.o = &os;

	pthread_attr_init(&attr);
	pthread_create(&thread, &attr, &thread_reader, (void*) &args);

	igraph_write_graph_graphml(graph, fake_fileptr, true); 
	fclose(fake_fileptr);

	pthread_join(thread, NULL);
	//fclose (instream);
	close(p[0]);

	igraph_destroy(graph);
	delete graph;
}

void
GraphML::dump(ostream& os, const Taskgraph *data) const
{
	dump(os, data, NULL);
}

void
GraphML::dump(ostream& os, const Taskgraph &data) const
{
	dump(os, &data, NULL);
}

void
GraphML::dump(ostream& os, const Taskgraph &data, const Platform &arch) const
{
	dump(os, &data, &arch);
}

Taskgraph*
GraphML::parse(istream &is) const
{
	// Open the file and get an igraph record
	// Initialize igraph and build a new object
	igraph_i_set_attribute_table(&igraph_cattribute_table); //do this to enable attribute fetching
	igraph_t *the_graph = new igraph_t();

	// Create a FILE* from istream by building a posix pipe. This wont work in windows...
	int p[2];
	int ans = pipe(p);
	if(ans)
	{
		throw runtime_error("Error: no pipe :/");
	}

	FILE *fake_fileptr = fdopen(p[0], "r");   

	pthread_attr_t attr;
	pthread_t thread;
	writer_args_t args;
	args.file_descr = p[1];
	args.i = &is;

	pthread_attr_init(&attr);
	pthread_create(&thread, &attr, &thread_writer, (void*) &args);

//	char ch;
//	while( ( ch = fgetc(fake_fileptr) ) != EOF )
//      		printf("%c",ch);
	// Parse input file
	igraph_read_graph_graphml(the_graph, fake_fileptr, 0);

	// Clone the file and wait for the pipe to finish
	close(p[0]);
	pthread_join(thread, NULL);

	set<Task> tasks;
	for(int id = 0; id < igraph_vcount(the_graph); id++)
	{
		stringstream estr;
		estr << "task_" << id;
		Task task(strcmp(VAS(the_graph, "name", id), "") != 0 ? VAS(the_graph, "name", id) : estr.str());
		task.setModule(strcmp(VAS(the_graph, "module", id),"") != 0 ? VAS(the_graph, "module", id) : "dummy");
		task.setWorkload(!isnan((float)VAN(the_graph, "workload", id)) ? VAN(the_graph, "workload", id): 1.0);

		const char *str = VAS(the_graph, "max_width", id);
		string max_width_str(str);
		boost::algorithm::to_lower(max_width_str);
		boost::algorithm::trim(max_width_str);
		float max_width;

		if(max_width_str.compare("inf") == 0 || max_width_str.compare("+inf") == 0)
		{
			max_width = std::numeric_limits<float>::infinity();
		}
		else
		{
			if(max_width_str.compare("-inf") == 0)
			{
				max_width = 1;
			}
			else
			{
				std::istringstream converter(max_width_str);
				converter >> max_width;

				if(converter.fail())
				{
					throw ParseException(std::string("Couln't convert literal \"").append(max_width_str).append("\" into type \"").append(typeid(max_width).name()).append("\"."));
				}
			}
		}
		task.setMaxWidth(max_width);
		//task.setMaxWidth((int)VAN(the_graph, "max_width", id) != INT_MIN ? VAN(the_graph, "max_width", id) : 1);

		if (strcmp(VAS(the_graph, "efficiency", id), "") != 0)
		{
			task.setEfficiencyString(VAS(the_graph, "efficiency", id));
		}
		else
		{
			stringstream ss;
			ss << task.getMaxWidth();
			task.setEfficiencyString(string("exprtk:p <= ") + ss.str() + "? 1 : 1e-6");
		}

		tasks.insert(Task(task));
	}

	// Add edges between tasks
	set<AbstractLink> links;
	for(int i = 0; i < igraph_ecount(the_graph); i++)
	{
		//printf("[%s:%s:%d] Edge number %d.\n", __FILE__, __FUNCTION__, __LINE__, i);
		int producer_id, consumer_id;

		igraph_edge(the_graph, i, &producer_id, &consumer_id);
		Task producer(VAS(the_graph, "name", producer_id));
		Task consumer(VAS(the_graph, "name", consumer_id));
		string producerName = string(EAS(the_graph, GraphML::producerName.c_str(), i));
		string consumerName = string(EAS(the_graph, GraphML::consumerName.c_str(), i));
		string type("");
		if(igraph_cattribute_has_attr(the_graph, IGRAPH_ATTRIBUTE_EDGE, GraphML::type.c_str()))
		{
			type = string(EAS(the_graph, GraphML::type.c_str(), i));
		}
		size_t producer_rate = 0, consumer_rate = 0;
		if(igraph_cattribute_has_attr(the_graph, IGRAPH_ATTRIBUTE_EDGE, GraphML::producer_rate.c_str()))
		{
			producer_rate = EAN(the_graph, GraphML::producer_rate.c_str(), i);
		}
		if(igraph_cattribute_has_attr(the_graph, IGRAPH_ATTRIBUTE_EDGE, GraphML::consumer_rate.c_str()))
		{
			consumer_rate = EAN(the_graph, GraphML::consumer_rate.c_str(), i);
		}

		Buffer nullBuffer = Buffer::nullBuffer();
		AbstractLink link(*tasks.find(producer), *tasks.find(consumer), producerName, consumerName, producer_rate, consumer_rate);
		links.insert(link);

		const AbstractLink &link_ref = *links.find(link);

		// Add the link as producer and consumer links of both endpoint tasks
		Task &producer_ref = (Task&)*tasks.find(producer);
		producer_ref.getConsumers().insert(&link_ref);
		Task &consumer_ref = (Task&)*tasks.find(consumer);
		consumer_ref.getProducers().insert(&link_ref);

		/*
		trace(producer_ref.getName());
		trace(consumer_ref.getName());
		trace(producer_ref.getConsumers().size());
		trace(consumer_ref.getConsumers().size());
		trace(producer_ref.getProducers().size());
		trace(consumer_ref.getProducers().size());
		*/
	}

	// Finally build the taskgraph
	Taskgraph *tg = new Taskgraph(tasks, links);
	tg->setName(GAS(the_graph, "name"));
	tg->setDeadlineCalculator(GAS(the_graph, "deadline"));

	// Cleanup
	igraph_destroy(the_graph);
	delete the_graph;

	return tg;
}

GraphML*
GraphML::clone() const
{
	return new GraphML();
}
