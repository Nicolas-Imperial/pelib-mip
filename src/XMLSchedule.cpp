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


#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>

extern "C"{
#include <igraph.h>
}

#include <pelib/Schedule.hpp>
#include <pelib/XMLSchedule.hpp>

#include <pelib/Scalar.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/Set.hpp>
#include <pelib/Task.hpp>
#include <pelib/Taskgraph.hpp>

#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>

#include <pelib/AmplInput.hpp>
#include <pelib/Algebra.hpp>

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

using namespace pelib;
using namespace std;
using namespace xmlpp;

XMLSchedule::~XMLSchedule()
{
	// Do nothing
}

void
XMLSchedule::dump(ostream& os, const Schedule &data) const
{
	dump(os, &data);
}

void
XMLSchedule::dump(ostream& os, const Schedule *sched) const
{
	Schedule::Table schedule = sched->getSchedule();
	const set<AllotedLink> &links = sched->getLinks();
	set<string> task_ids;
	
	for(Schedule::Table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(set<ExecTask>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			task_ids.insert(j->getTask().getName());
		}
	}

	os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
		<< "<schedule scheduler=\"" << sched->getName() << "\" application=\"" << sched->getAppName() << "\""
		<< ">" << endl;

	// Finds and set the precision required for this schedule
	std::streamsize old_precision = os.precision();
	std::streamsize max_work_precision = 0;
	std::streamsize max_freq_precision = 0;
	std::streamsize max_start_precision = 0;
	for(Schedule::Table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		float last_start = 0;
		float last_time = 0;
		for(set<ExecTask>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			const ExecTask &t = *j;
			float start_time = j->getStart();
			if(start_time > 0)
			{
				float delta = start_time - last_start;
				delta = delta - floor(delta);
				std::streamsize precision = (std::streamsize)(ceil(-log10(delta)) + 2);
				if(precision > max_start_precision)
				{
					max_start_precision = precision;
				}
				last_start = start_time;

				delta = abs(start_time - (last_start + last_time));
				delta = delta - floor(delta);
				precision = (std::streamsize)(ceil(-log10(delta)) + 1);
				if(precision > max_start_precision)
				{
					max_start_precision = precision;
				}
				last_time = t.getTask().runtime(t.getWidth(), t.getFrequency());

				double work = t.getTask().getWorkload() - floor(t.getTask().getWorkload());
				precision = (std::streamsize)(ceil(-log10(work)) + 1);
				if(precision > max_work_precision)
				{
					max_work_precision = precision;
				}

				double freq = t.getFrequency() - floor(t.getFrequency());
				precision = (std::streamsize)(ceil(-log10(freq)) + 1);
				if(precision > max_freq_precision)
				{
					max_freq_precision = precision;
				}
			}
		}
	}

	for(Schedule::Table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		int p = i->first;
		os << " <core id=\"" << p + 1 << "\">" << endl;
		set<ExecTask> core_schedule = i->second;
		int order = 0;

		for(set<ExecTask>::const_iterator j = i->second.begin(); j != i->second.end(); j++, order++)
		{
			const ExecTask &t = *j;
			float start = t.getStart();
			string taskid = t.getTask().getName();
			
			os << "  <task name=\"" << taskid << "\" ";
			os << setprecision(max_start_precision);
			os << "instance=\"" << std::fixed << t.getInstance() << "\" ";
			os << "start=\"" << std::fixed << start << "\" ";
			os << setprecision(old_precision);
			os << "frequency=\"" << (float)t.getFrequency() << "\"";
			if(i->first == j->getMasterCore() && j->getWidth() > 1)
			{
				os << " role=\"master\"";
			}
			os << "/>" << endl;

			start += t.getTask().runtime(t.getWidth(), t.getFrequency());
		}
		os << " </core>" << endl;
	}

	for(set<AllotedLink>::iterator i = links.begin(); i != links.end(); i++)
	{
		const AllotedLink &link = *i;

		os << " <link>" << endl;
		os << "  <producer ";
		os << "name=\"" << link.getLink().getProducerName() << "\" ";
		os << "task=\"" << link.getLink().getProducer()->getName() << "\"";
		if(((int)link.getProducerMemory().getFeature() & 3 & (int)Memory::Feature::distributed) == (int)Memory::Feature::distributed)
		{
			os << " core=\"" << link.getProducerMemory().getCore() + 1 << "\" ";
			os << "memory=\"" << Memory::featureToString(link.getProducerMemory().getFeature()) << "\" ";
			os << "level=\"" << link.getProducerMemory().getLevel() << "\"";
		}
		os << "/>" << endl;

		os << "  <consumer ";
		os << "name=\"" << link.getLink().getConsumerName() << "\" ";
		os << "task=\"" << link.getLink().getConsumer()->getName() << "\"";
		if(((int)link.getConsumerMemory().getFeature() & 3 & (int)Memory::Feature::distributed) == (int)Memory::Feature::distributed)
		{
			os << " core=\"" << link.getConsumerMemory().getCore() + 1 << "\" ";
			os << "memory=\"" << Memory::featureToString(link.getConsumerMemory().getFeature()) << "\" ";
			os << "level=\"" << link.getConsumerMemory().getLevel() << "\"";
		}
		os << "/>" << endl;

		os << "  <buffer type=\"" << link.getQueueBuffer().getType() << "\" ";
		if(link.getQueueBuffer().getHeader().compare(string()) != 0)
		{
			os << "header=\"" << link.getQueueBuffer().getHeader() << "\" ";
		}
		os << "size=\"" << link.getQueueBuffer().getSize() << "\">" << endl;
		os << "   <memory core=\"" << link.getQueueBuffer().getMemory().getCore() + 1 << "\" ";
		os << "feature=\"" << Memory::featureToString(link.getQueueBuffer().getMemory().getFeature()) << "\" ";
		os << "level=\"" << link.getQueueBuffer().getMemory().getLevel() << "\"";
		os << "/>" << endl;
		os << "  </buffer>" << endl;
		os << " </link>" << endl;
	}

	set<ExecTask> allExecTasks;
	for(Schedule::Table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(set<ExecTask>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			allExecTasks.insert(*j);
		}
	}

	for(set<ExecTask>::const_iterator i = allExecTasks.begin(); i != allExecTasks.end(); i++)
	{
		if(i->getMemory().getFeature() != Memory::Feature::undefined)
		{
			os << " <sync " << i->getTask().getName();
			os << "\" id" << i->getInstance();
			os << "  <memory core=\"" << i->getMemory().getCore() + 1 << "\" ";
			os << "feature=\"" << Memory::featureToString(i->getMemory().getFeature()) << "\" ";
			os << "level=\"" << i->getMemory().getLevel() << "\" ";
			os << "instance=\"" << i->getInstance() << "\"";
			os << "/>" << endl;
			os << " </sync>" << endl;
		}
	}
		
	os << setprecision(old_precision);

	os << "</schedule>" << endl;
}

Schedule*
XMLSchedule::parse(istream &is, const Taskgraph &tg, const Platform &pt) const
{
	DomParser* theSchedule = new DomParser();
	
	try
	{
		theSchedule->parse_stream(is);
	} catch(xmlpp::parse_error &e)
	{
		throw ParseException(std::string("xmlpp::parse_error"));
	}
	
	Element *root = theSchedule->get_document()->get_root_node();
	xmlpp::Node::NodeList processors = root->get_children();

	try
	{
		std::string name = root->get_attribute_value("scheduler");
		std::string aut_name = root->get_attribute_value("application");

		Schedule::Table schedule;
		map<pair<Task, unsigned int>, unsigned int> task_width, task_sync;
		set<AllotedLink> links;
		map<unsigned int, Memory> sync;
		map<pair<Task, unsigned int>, unsigned int> master;
		for(xmlpp::Node::NodeList::iterator piter = processors.begin(); piter != processors.end(); ++piter) //for each core
		{
			if((*piter)->get_name().compare("core") == 0) //skip indentation characters et cetera
			{
				unsigned int core_id = atoi(dynamic_cast<xmlpp::Element*>(*piter)->get_attribute_value("id").c_str()) - 1;
				set<ExecTask> core_schedule_map;

				std::list<xmlpp::Node*> tasks = (*piter)->get_children();
				for(std::list<xmlpp::Node*>::iterator titer = tasks.begin(); titer != tasks.end(); ++titer)
				{
					if((*titer)->get_name().compare("task") != 0) //skip indentation characters et cetera
					{
						continue;
					}

					Element *task = dynamic_cast<Element*>(*titer);

					Task search(task->get_attribute_value("name"));
					string sync(task->get_attribute_value("sync"));
					set<Task>::const_iterator task_search = tg.getTasks().find(search);
					if(task_search == tg.getTasks().end())
					{
						throw PelibException("Found task in schedule that does not exist in taskgraph");
					}

					string role = task->get_attribute_value("role");
					unsigned int instance = atoi(task->get_attribute_value("instance").c_str());
					if(role.compare(string("master")) == 0)
					{
						debug(core_id);
						master.insert(pair<pair<Task, unsigned int>, unsigned int>(pair<Task, unsigned int>(search, instance), core_id));
					}

					ExecTask etask(*task_search, 
						set<AllotedLink>(), // First consider there are no links, will add them later
						atof(task->get_attribute_value("frequency").c_str()),
						0,
						atof(task->get_attribute_value("start").c_str()),
						atoi(task->get_attribute_value("instance").c_str()),
						0,
						Memory()
					);

					map<pair<Task, unsigned int>, unsigned int>::iterator i = task_width.find(pair<Task, unsigned int>(search, etask.getInstance()));
					if(i == task_width.end())
					{
						i = task_width.insert(pair<pair<Task, unsigned int>, unsigned int>(pair<Task, unsigned int>(search, etask.getInstance()), 0)).first;
					}
					i->second++;

					if(sync.compare(string()) != 0)
					{
						map<pair<Task, unsigned int>, unsigned int>::iterator j = task_sync.find(pair<Task, unsigned int>(search, etask.getInstance()));
						if(j != task_sync.end() && j->second != (unsigned int)atoi(sync.c_str()))
						{
							if(j->second != (unsigned int)atoi(sync.c_str()))
							{
								stringstream ss;
								ss << "Parallel task \"" << search.getName() << "\" uses inconsistent synchronization buffers" << endl;
								throw PelibException(ss.str());
							}
						}
						else
						{
							task_sync.insert(pair<pair<Task, unsigned int>, unsigned int>(pair<Task, unsigned int>(search, etask.getInstance()), atoi(sync.c_str())));
						}
					}
					core_schedule_map.insert(etask);
				}

				schedule.insert(std::pair<unsigned int, set<ExecTask>>(core_id, core_schedule_map));
			}
			else if((*piter)->get_name().compare("link") == 0) //skip indentation characters et cetera
			{
				string queueType, queueHeader;
				size_t queueSize = 0;
				unsigned int queueCore = 0;
				Memory::Feature queueMemory = Memory::nullMemory().getFeature();
				unsigned int queueLevel = 0;
				string producer_name = string(), consumer_name = string();
				unsigned int producer_level = 0, consumer_level = 0;
				unsigned int producer_core = 0, consumer_core = 0;
				Memory::Feature producer_type = Memory::nullMemory().getFeature(), consumer_type = Memory::nullMemory().getFeature();
				const Task *producer = NULL, *consumer = NULL;

				std::list<xmlpp::Node*> allocs = (*piter)->get_children();
				for(std::list<xmlpp::Node*>::iterator aiter = allocs.begin(); aiter != allocs.end(); ++aiter)
				{
					if((*aiter)->get_name().compare("producer") == 0)
					{
						Element *alloc = dynamic_cast<Element*>(*aiter);
						producer_name = alloc->get_attribute_value("name");
						string task_name = alloc->get_attribute_value("task");
						producer_level = atoi(alloc->get_attribute_value("level").c_str());
						producer_core = atoi(alloc->get_attribute_value("core").c_str()) - 1;
						producer_type = Memory::stringToFeature(alloc->get_attribute_value("feature").c_str());

						set<Task>::iterator i = tg.getTasks().find(Task(task_name));
						if(i == tg.getTasks().end())
						{
							stringstream ss;
							ss << "Could not find producer task \"" << task_name << "\" in task set" << endl;
							throw PelibException(ss.str());
						}
						producer = &*i;
						const Task &myTaskRef = *i;
						Task myTask = myTaskRef;
					}
					else if((*aiter)->get_name().compare("consumer") == 0)
					{
						Element *alloc = dynamic_cast<Element*>(*aiter);
						consumer_name = alloc->get_attribute_value("name");
						string task_name = alloc->get_attribute_value("task");
						consumer_level = atoi(alloc->get_attribute_value("level").c_str());
						consumer_core = atoi(alloc->get_attribute_value("core").c_str()) - 1;
						consumer_type = Memory::stringToFeature(alloc->get_attribute_value("memory").c_str());

						set<Task>::iterator i = tg.getTasks().find(Task(task_name));
						if(i == tg.getTasks().end())
						{
							stringstream ss;
							ss << "Could not find consumer task \"" << task_name << "\" in task set" << endl;
							throw PelibException(ss.str());
						}
						consumer = &*i;
					}
					else if((*aiter)->get_name().compare("buffer") == 0)
					{
						Element *alloc = dynamic_cast<Element*>(*aiter);
						queueType = alloc->get_attribute_value("type").c_str();
						queueHeader = alloc->get_attribute_value("header").c_str();
						queueSize = atoi(alloc->get_attribute_value("size").c_str());

						std::list<xmlpp::Node*> mems = (*aiter)->get_children();
						for(std::list<xmlpp::Node*>::iterator miter = mems.begin(); miter != mems.end(); ++miter)
						{
							if((*miter)->get_name().compare("memory") == 0)
							{
								Element *memory = dynamic_cast<Element*>(*miter);
								queueCore = atoi(memory->get_attribute_value("core").c_str()) - 1;
								string memoryStr = memory->get_attribute_value("feature").c_str();
								queueMemory = Memory::stringToFeature(memoryStr.c_str());
								queueLevel = atoi(memory->get_attribute_value("level").c_str());
							}
						}
					}
				}

				if(producer == NULL || consumer == NULL)
				{
					throw PelibException("Missing either reference to producer or consumer task in link");
				}

				AbstractLink linkKey(*producer, *consumer, producer_name, consumer_name);
				const set<AbstractLink> &allLinks = tg.getLinks();
				set<AbstractLink>::const_iterator linkIter = allLinks.find(linkKey);
				if(linkIter == allLinks.end())
				{
					stringstream ss;
					ss << "Link from " << producer->getName() << " (\"" << producer_name << "\") to " << consumer->getName() << " (\"" << consumer_name << "\") does not exist in taskgraph" << endl;
					throw PelibException(ss.str());
				}
				const AbstractLink &actualLink = *linkIter;
				AllotedLink link(actualLink, Buffer(queueSize, queueType, queueHeader, Memory(queueMemory, queueLevel, queueCore)), Memory(producer_type, producer_level, producer_core), Memory(consumer_type, consumer_level, consumer_core));
				links.insert(link);
			}
			else if((*piter)->get_name().compare("sync") == 0) //skip indentation characters et cetera
			{
				Element *alloc = dynamic_cast<Element*>(*piter);
				unsigned int instance = atoi(alloc->get_attribute_value("id").c_str());
				Memory::Feature feature = Memory::Feature::undefined;
				unsigned int core = 0;
				unsigned int level = 0;

				std::list<xmlpp::Node*> allocs = (*piter)->get_children();
				for(std::list<xmlpp::Node*>::iterator aiter = allocs.begin(); aiter != allocs.end(); ++aiter)
				{
					if((*aiter)->get_name().compare("memory") == 0)
					{
						Element *memory = dynamic_cast<Element*>(*aiter);
						core = atoi(memory->get_attribute_value("core").c_str()) - 1;
						string memoryStr = memory->get_attribute_value("feature").c_str();
						feature = Memory::stringToFeature(memoryStr.c_str());
						level = atoi(memory->get_attribute_value("level").c_str());
					}
				}

				sync.insert(pair<unsigned int, Memory>(instance, Memory(feature, level, core, instance)));
			}
		}
	
		// Now rebuild task set with the actual link set
		Schedule::Table finalSchedule;
		for(Schedule::Table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			set<ExecTask> core_schedule;
			for(set<ExecTask>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				unsigned int master_core;
				map<pair<Task, unsigned int>, unsigned int>::const_iterator master_search = master.find(pair<Task, unsigned int>(j->getTask(), j->getInstance()));
				if(master_search == master.end())
				{
					master_core = i->first;
					master.insert(pair<pair<Task, unsigned int>, unsigned int>(pair<Task, unsigned int>(Task(j->getTask().getName()), j->getInstance()), master_core));
				}
				else
				{
					master_core = master_search->second;
				}
	
				Memory mem_sync;
				map<pair<Task, unsigned int>, unsigned int>::const_iterator task_sync_search = task_sync.find(pair<Task, unsigned int>(j->getTask(), j->getInstance()));
				if(task_sync_search == task_sync.end() && j->getWidth() > 1)
				{
					stringstream ss;
					ss << "Parallel task \"" << j->getTask().getName() << "\" instance " << j->getInstance() << " refers to no synchronization data structure on core " << i->first << endl;
					throw PelibException(ss.str());
				}
				
				map<unsigned int, Memory>::const_iterator sync_search = sync.find(task_sync_search->second);

				if(sync_search == sync.end())
				{
					if(j->getWidth() <= 1)
					{
						mem_sync = Memory();
					}
					else
					{
						stringstream ss;
						ss << "Parallel task \"" << j->getTask().getName() << "\" instance " << j->getInstance() << " refers to inexistant synchronization data structure" << task_sync_search->second << " on core " << i->first << endl;
						throw PelibException(ss.str());
					}
				}
				else
				{
					mem_sync = sync_search->second;
				}
				
				ExecTask task(j->getTask(), links, j->getFrequency(), task_width.find(pair<Task, unsigned int>(j->getTask(), j->getInstance()))->second, j->getStart(), j->getInstance(), master_core, mem_sync);
				core_schedule.insert(task);
			}
			finalSchedule.insert(pair<unsigned int, set<ExecTask>>(i->first, core_schedule));
		}
	
		Schedule *sched = new Schedule(name, aut_name, finalSchedule, links, tg, pt);

		return sched;
	}
	catch(std::invalid_argument &e)
	{
		throw ParseException(std::string("std::invalid_argument"));
	}
}

XMLSchedule*
XMLSchedule::clone() const
{
	return new XMLSchedule();
}

