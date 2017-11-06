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


#include <pelib/Platform.hpp>
#include <pelib/Core.hpp>
#include <pelib/DummyCore.hpp>

#include <pelib/Scalar.hpp>
#include <pelib/Set.hpp>
#include <pelib/ParseException.hpp>
#include <pelib/CastException.hpp>

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;

using namespace std;

namespace pelib
{
	Platform::Platform()
	{
		set<float> f;
		f.insert(1);
		Core *core = new DummyCore(f, 1);
		this->cores.insert(core);
		island isl;
		isl.insert(core);
		main.insert(island(isl));
		shared.insert(island(isl));
		priv.insert(island(isl));
		voltage.insert(island(isl));
		freq.insert(island(isl));
	}

	Platform::Platform(const island &cores)
	{
		//set<const Core*> shared;
		for(set<const Core*>::const_iterator i = cores.begin(); i != cores.end(); i++)
		{
			Core *core = (*i)->clone();
			this->cores.insert(core);
			island isl;
			isl.insert(core);
			//shared.insert(core);

			// Each core is its own Platform::island of anything
			this->shared.insert(island(isl));
			this->main.insert(island(isl));
			this->priv.insert(island(isl));
			this->voltage.insert(island(isl));
			this->freq.insert(island(isl));
		}
		//this->shared.insert(set<const Core*>(shared));
	}
	
	Platform::Platform(const island& cores, const islands &shared, const islands &main, const islands &priv, const islands &voltage, const islands &freq)
	{
		this->cores = cores;
		this->shared = shared;
		this->main = main;
		this->priv = priv;
		this->voltage = voltage;
		this->freq = freq;
	}

	Platform::Platform(size_t p, const Core* ref)
	{
		//set<const Core*> shared;
		for(size_t i = 0; i < p; i++)
		{
			Core *core = ref->clone();
			this->cores.insert(core);
			island isl;
			isl.insert(core);
			//shared.insert(core);

			// Each core is its own Platform::island of anything
			this->shared.insert(island(isl));
			this->main.insert(island(isl));
			this->priv.insert(island(isl));
			this->voltage.insert(island(isl));
			this->freq.insert(island(isl));
		}
		//this->shared.insert(shared);
	}

	void
	Platform::copy(const Platform *arch)
	{
		this->cores.clear();
		for(island::const_iterator i = arch->getCores().begin(); i != arch->getCores().end(); i++)
		{
			this->cores.insert((*i)->clone());
		}

		this->shared.clear();
		this->main.clear();
		this->priv.clear();
		this->voltage.clear();
		this->freq.clear();

		for(islands::const_iterator i = arch->getSharedMemoryIslands().begin(); i != arch->getSharedMemoryIslands().end(); i++)
		{
			island isl;
			for(island::const_iterator j = i->begin(); j != i->end(); j++)
			{
				isl.insert(&this->getCore(arch->getCoreId(*arch->getCores().find(*j))));
			}

			this->shared.insert(isl);
		}

		for(islands::const_iterator i = arch->getMainMemoryIslands().begin(); i != arch->getMainMemoryIslands().end(); i++)
		{
			island isl;
			for(island::const_iterator j = i->begin(); j != i->end(); j++)
			{
				isl.insert(&this->getCore(arch->getCoreId(*arch->getCores().find(*j))));
			}

			this->main.insert(isl);
		}

		for(islands::const_iterator i = arch->getPrivateMemoryIslands().begin(); i != arch->getPrivateMemoryIslands().end(); i++)
		{
			island isl;
			for(island::const_iterator j = i->begin(); j != i->end(); j++)
			{
				isl.insert(&this->getCore(arch->getCoreId(*arch->getCores().find(*j))));
			}

			this->priv.insert(isl);
		}

		for(islands::const_iterator i = arch->getVoltageIslands().begin(); i != arch->getVoltageIslands().end(); i++)
		{
			island isl;
			for(island::const_iterator j = i->begin(); j != i->end(); j++)
			{
				isl.insert(&this->getCore(arch->getCoreId(*arch->getCores().find(*j))));
			}

			this->voltage.insert(isl);
		}

		for(islands::const_iterator i = arch->getFrequencyIslands().begin(); i != arch->getFrequencyIslands().end(); i++)
		{
			island isl;
			for(island::const_iterator j = i->begin(); j != i->end(); j++)
			{
				isl.insert(&this->getCore(arch->getCoreId(*arch->getCores().find(*j))));
			}

			this->freq.insert(isl);
		}
	}

	Platform::Platform(const Platform *arch)
	{
		copy(arch);
	}

	Platform::Platform(const Platform &pt)
	{
		copy(&pt);
	}

	Platform::Platform(const Algebra &arch)
	{
		const Scalar<float> *scalar_p = arch.find<Scalar<float> >("p");
		const Scalar<float> *scalar_fin = arch.find<Scalar<float> >("Fin");
		//const Scalar<float> *scalar_sin = arch.find<Scalar<float> >("Sin");
		const Scalar<float> *f_unit = arch.find<Scalar<float> >("Funit");
		const Set<float> *set_F = arch.find<Set<float> >("F");
		const Set<float> *set_Fi = arch.find<Set<float> >("Fi");
		const Set<float> *set_Si = arch.find<Set<float> >("Si");

		if(scalar_p == NULL || set_F == NULL || f_unit == NULL || set_Fi == NULL || scalar_fin == NULL)
		{
			throw ParseException(std::string("Missing core number scalar \"p\", frequency set \"F\", frequency unit \"Funit\" or frequency islands number \"Fin\" and details set \"Fi[1..Fin]\" in input."));
		}

		// Create empty frequency islands, if a description is given
		vector<island> frequency;
		if(set_Fi != NULL)
		{
			Set<float>::SetOfSetsType Fi = set_Fi->getSubsets();
			for(Set<float>::SetOfSetsType::iterator i = Fi.begin(); i != Fi.end(); i++)
			{
				frequency.push_back(island());
			}
		}

		// Create empty frequency islands, if a description is given
		vector<island> shared;
		if(set_Si != NULL)
		{
			Set<float>::SetOfSetsType Si = set_Si->getSubsets();
			for(Set<float>::SetOfSetsType::iterator i = Si.begin(); i != Si.end(); i++)
			{
				shared.push_back(island());
			}
		}

		for(size_t i = 0; i < scalar_p->getValue(); i++)
		{
			const Core *core = new DummyCore(i + 1, set_F->getValues(), f_unit == NULL ? 1 : f_unit->getValue());
			this->cores.insert(core);
			island isl;
			isl.insert(core);

			// Each core is its own shared mamory island
			main.insert(island(isl));

			// If frequency islands are provided, then insert core in the right island
			if(set_Fi != NULL)
			{
				size_t core_id = i + 1;
				vector<Platform::island>::iterator jj = frequency.begin();

				// Find the frequency island that holds this core
				Set<float>::SetOfSetsType Fi = set_Fi->getSubsets();
				for(Set<float>::SetOfSetsType::iterator j = Fi.begin(); j != Fi.end(); j++)
				{
					if(j->second.find(core_id) != j->second.end())
					{
						size_t island_id = std::distance(Fi.begin(), j);
						std::advance(jj, island_id);
						island isl = *jj;
						frequency[island_id].insert(core);

						break;
					}
				}
			}

			// If shared memory islands are provided, then insert core in the right island
			if(set_Si != NULL)
			{
				size_t core_id = i + 1;
				vector<Platform::island>::iterator jj = shared.begin();

				// Find the frequency island that holds this core
				Set<float>::SetOfSetsType Si = set_Si->getSubsets();
				for(Set<float>::SetOfSetsType::iterator j = Si.begin(); j != Si.end(); j++)
				{
					if(j->second.find(core_id) != j->second.end())
					{
						size_t island_id = std::distance(Si.begin(), j);
						std::advance(jj, island_id);
						island isl = *jj;
						shared[island_id].insert(core);

						break;
					}
				}
			}

			this->priv = main;

			// If voltage/frequency islands are provided, then copy the
			// frequency island set computed above to voltage and frequency
			// islands
			if(set_Fi == NULL)
			{
				this->voltage = main;
				this->freq = main;
			}
			else
			{
				islands isls;
				for(vector<Platform::island>::iterator i = frequency.begin(); i != frequency.end(); i++)
				{
					isls.insert(*i);
				}
				this->voltage = isls;
				this->freq = isls;
			}

			// If ishared memory islands are provided, then copy the
			// frequency island set computed above to voltage and frequency
			// islands
			if(set_Si == NULL)
			{
				this->shared = main;
			}
			else
			{
				islands isls;
				for(vector<Platform::island>::iterator i = shared.begin(); i != shared.end(); i++)
				{
					isls.insert(*i);
				}
				this->shared = isls;
			}
		}
	}

	Platform::~Platform()
	{
		for(island::const_iterator i = this->getCores().begin(); i != this->getCores().end(); i++)
		{
			delete *i;
		}
	}
	
	Platform*
	Platform::clone() const
	{
		Platform* res = new Platform(this);
		return res;
	}

	const Platform::island&
	Platform::getCores() const
	{
		return this->cores;
	}
		
	bool
	Platform::isHomogeneous() const
	{
		const char* type = NULL;
		
		for(island::const_iterator i = this->getCores().begin(); i != this->getCores().end(); i++)
		{
			if(type != NULL)
			{
				if(string(type).compare(typeid(**i).name()) != 0)
				{
					return false;
				}
			}

			type = typeid(**i).name();
		}

		// If we reach this point having read at least one string, then this is homogeneous
		return type != NULL;
	}

	Algebra
	Platform::buildAlgebra() const
	{
		Algebra record;

		if(!this->isHomogeneous())
		{
			throw CastException(std::string("Missing core number scalar \"p\" or frequency set \"F\" in input."));
		}

		Scalar<float> scalar_p("p", this->getCores().size());
		Scalar<float> f_unit("Funit", (*this->getCores().begin())->getFrequencyUnit());
		Set<float> set_F("F", (*this->getCores().begin())->getFrequencies());

		Set<float>::SetOfSetsType Fi;
		for(set<island>::iterator i = this->freq.begin(); i != this->freq.end(); i++)
		{
			Set<float>::SetType isl;
			for(island::const_iterator j = i->begin(); j != i->end(); j++)
			{
				island::iterator core_iter = cores.find(*j);
				isl.insert(std::distance(cores.begin(), core_iter) + 1);
			}
			Fi.insert(pair<size_t, Set<float>::SetType>((size_t)(std::distance(this->freq.begin(), i) + 1), isl));
		}
		Set<float> set_Fi("Fi", Fi);

		record.insert(&scalar_p);
		record.insert(&f_unit);
		record.insert(&set_F);
		record.insert(&set_Fi);
		Scalar<float> scalar_Fin("Fin", Fi.size());
		record.insert(&scalar_Fin);

		return record;
	}

	const Core&
	Platform::getCore(size_t id) const
	{
		if(id > this->getCores().size())
		{
			throw CastException("Requesting core beyond the number of core in the platform.");
		}
		island::iterator it = this->cores.begin();
		std::advance(it, id - 1);
		return **it;
	}

	size_t
	Platform::getCoreId(const Core* core) const
	{
		if(this->getCores().find(core) == this->getCores().end())
		{
			throw CastException("Core is not part of the platform.");
		}

		return std::distance(this->cores.begin(), this->cores.find(core)) + 1;
	}

	const Platform::islands&
	Platform::getSharedMemoryIslands() const
	{
		return this->shared;
	}

	const Platform::islands
	Platform::sharedMemoryIslands(size_t id) const
	{
		if(id > this->getCores().size())
		{
			throw CastException("Requesting shared memory island beyond the number of islands in the platform.");
		}
		islands isls;
		const Core &core = this->getCore(id);
		for(islands::const_iterator i = this->getSharedMemoryIslands().begin(); i != this->getSharedMemoryIslands().end(); i++)
		{
			if(i->find(&core) != i->end())
			{
				isls.insert(*i);
			}
		}

		return isls;
	}

	const std::set<unsigned int>
	Platform::sharedMemoryIslands(const islands& isls) const
	{
		set<unsigned int> indexes;
		for(Platform::islands::const_iterator i = isls.begin(); i != isls.end(); i++)
		{
			if(this->getSharedMemoryIslands().find(*i) != this->getSharedMemoryIslands().end())
			{
				indexes.insert((unsigned int)std::distance(this->getSharedMemoryIslands().begin(), this->getSharedMemoryIslands().find(*i)) + 1);
			}
			else
			{
				throw CastException("Could not find island in platform.");
			}
		}

		return indexes;
	}

	const Platform::islands
	Platform::sharedMemoryIslands(const std::set<int>& isls) const
	{
		Platform::islands out;
		for(std::set<int>::const_iterator i = isls.begin(); i != isls.end(); i++)
		{
			set<island>::const_iterator ii = this->getSharedMemoryIslands().begin();
			if(*i > (ptrdiff_t)this->getSharedMemoryIslands().size())
			{
				throw CastException("Trying to get shared memory island beyond the capacity of the platform.");
			}
			std::advance(ii, *i);
			out.insert(*ii);
		} 

		return out;
	}

	const Platform::islands&
	Platform::getMainMemoryIslands() const
	{
		return this->main;
	}

	const Platform::islands
	Platform::mainMemoryIslands(size_t id) const
	{
		islands isls;
		const Core &core = this->getCore(id);
		for(Platform::islands::const_iterator i = this->getMainMemoryIslands().begin(); i != this->getMainMemoryIslands().end(); i++)
		{
			if(i->find(&core) != i->end())
			{
				isls.insert(*i);
			}
		}

		return isls;
	}

	const Platform::islands&
	Platform::getPrivateMemoryIslands() const
	{
		return this->priv;
	}

	const Platform::islands
	Platform::privateMemoryIslands(size_t id) const
	{
		Platform::islands isls;
		const Core &core = this->getCore(id);
		for(Platform::islands::const_iterator i = this->getPrivateMemoryIslands().begin(); i != this->getPrivateMemoryIslands().end(); i++)
		{
			if(i->find(&core) != i->end())
			{
				isls.insert(*i);
			}
		}

		return isls;
	}

	const Platform::islands&
	Platform::getVoltageIslands() const
	{
		return this->voltage;
	}

	const Platform::island&
	Platform::getVoltageIsland(size_t id) const
	{
		const Core &core = this->getCore(id);
		for(Platform::islands::const_iterator i = this->getVoltageIslands().begin(); i != this->getVoltageIslands().end(); i++)
		{
			if(i->find(&core) != i->end())
			{
				return *i;
			}
		}

		throw CastException("Cannot find core in any Platform::island of the platform.");
	}

	const Platform::islands&
	Platform::getFrequencyIslands() const
	{
		return this->freq;
	}

	const Platform::island&
	Platform::getFrequencyIsland(size_t id) const
	{
		const Core &core = this->getCore(id);
		for(Platform::islands::const_iterator i = this->getFrequencyIslands().begin(); i != this->getFrequencyIslands().end(); i++)
		{
			if(i->find(&core) != i->end())
			{
				return *i;
			}
		}

		throw CastException("Cannot find core in any Platform::island of the platform.");
	}

	Platform&
	Platform::operator=(const Platform& cpy)
	{
		for(island::const_iterator i = this->getCores().begin(); i != this->getCores().end(); i++)
		{
			delete *i;
		}

		copy(&cpy);
		return *this;
	}
}
