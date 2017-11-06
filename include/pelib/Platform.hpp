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


#include <set>

#include <pelib/Core.hpp>
#include <pelib/Algebra.hpp>

#ifndef PELIB_PLATFORM
#define PELIB_PLATFORM

#define debug(var) std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << (var) << "\"" << std::endl;

namespace pelib
{
	/** Models an execution platform **/
	class Platform: public Record
	{
		public:
			typedef std::set<const Core*, Core::LessCorePtrByCoreId> island;
			struct LessIslandByFirstCoreId 
			{
				bool operator() (const island &x, const island &y) const
				{
					for(island::const_iterator xx = x.begin(), yy = y.begin(); xx != x.end() && yy != y.end(); xx++, yy++)
					{
						if(*xx == *yy)
						{
							continue;
						}
						else
						{
							return **xx < **yy;
						}
					}

					return x.size() < y.size();
				}
			};
			typedef std::set<island, LessIslandByFirstCoreId> islands;

			/** Internal type of a platform core island **/

			/** Constructor **/
			Platform();

			/** Constructor
				@param p Number of cores in this platform. Each core is its own shared memory, main memory, private memory, voltage and frequency island
				@param ref Pointer to an instance of pelib::Core duplicated p times and stored in the Platform instance being build
			**/
			Platform(size_t p, const Core* ref);

			/** Constructor
				@param cores Collection of derivative instances of pelib::Core that model all execution cores in the platform. Each core is its own shared memory, main memory, private memory, voltage and frequency island
			**/
			Platform(const island &cores);

			/** Constructor
				@param cores Collection of derivative instances of pelib::Core that model all execution cores in the platform. Each core is its own shared memory, main memory, private memory, voltage and frequency island
				@param shared On-chip shared memory islands
				@param main Off-chip shared memory islands
				@param priv Off-chip Private memory islands
				@param voltage Voltage scaling islands
				@param freq Frequency scaling islands
			**/
			Platform(const island& cores, const islands &shared, const islands &main, const islands &priv, const islands &voltage, const islands &freq);

			/** Copy constructor **/
			Platform(const Platform *arch);

			/** Copy constructor **/
			Platform(const Platform &arch);

			/** Constructor
				@param arch Algebraic representation of a platform: Scalar p is the number of cores, set F is the set of frequency each core can run at. Each core is its own shared memory, main memory, private memory, voltage and frequency island
			**/
			Platform(const Algebra &arch);

			/** Returns a pointer to a copy of this platform **/
			virtual Platform* clone() const;

			/** Returns all cores in the platform **/
			virtual const std::set<const Core*, Core::LessCorePtrByCoreId>& getCores() const;

			/** Returns true if all cores are identical and false otherwise **/
			virtual bool isHomogeneous() const;

			/** Returns all shared memory islands **/
			virtual const islands& getSharedMemoryIslands() const;

			/** Returns the shared memory island corresponding to the core_id-th core of the platform, starting with 1 **/
			virtual const islands sharedMemoryIslands(size_t core_id) const;

			/** Returns the shared memory islands a set of cores belongs to
				@param islands Set of core id the shared memory islands' are requested
			**/
			virtual const islands sharedMemoryIslands(const std::set<int>& islands) const;
			/** Returns the core ids contained in a shared memory island
				@param islands Shared memory islands that contains the cores to be returned
			**/
			virtual const std::set<unsigned int> sharedMemoryIslands(const islands& isls) const;
			/** Returns all main memory islands **/
			virtual const islands& getMainMemoryIslands() const;
			/** Returns the main memory island corresponding to the core_id-th core of the platform, starting with 1 **/
			virtual const islands mainMemoryIslands(size_t core_id) const;
			/** Returns all private memory islands **/
			virtual const islands& getPrivateMemoryIslands() const;
			/** Returns the private memory island corresponding to the core_id-th core of the platform, starting with 1 **/
			virtual const islands privateMemoryIslands(size_t core_id) const;
			/** Returns all voltage memory islands **/
			virtual const islands& getVoltageIslands() const;
			/** Returns the voltage memory island corresponding to the core_id-th core of the platform, starting with 1 **/
			virtual const island& getVoltageIsland(size_t core_id) const;
			/** Returns all frequency memory islands **/
			virtual const islands& getFrequencyIslands() const;
			/** Returns the frequency memory island corresponding to the core_id-th core of the platform, starting with 1 **/
			virtual const island& getFrequencyIsland(size_t core_id) const;

			/** Returns a pointer to core corresponding to its number id in the platform **/
			virtual const Core& getCore(size_t id) const;

			/** Returns the core id in the platform that corresponds to a pointer to core; throws a CastException if the core doesn't belong to this plaform **/
			virtual size_t getCoreId(const Core*) const;

			/** Builds an algebraic representation of this platform: p is the number of cores and F is the set of frequency they can run at **/
			virtual Algebra buildAlgebra() const;

			/** Destructor **/
			virtual	~Platform();
			virtual Platform&
			operator=(const Platform& copy);
		protected:
			/** Collection of cores **/
			island cores;
			/** Islands **/
			islands shared, main, priv, voltage, freq;
		private:		
			void copy(const Platform *pt);
	};
}

#undef debug

#endif
