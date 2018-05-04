/**
 * file   species_manager.hh
 *
 * @author Till Junge <till.junge@epfl.ch>
 *
 * @date   04 May 2018
 *
 * @brief iterable proxy to a neigbourhood, filtered by species. You
 * can iterate over it by atom species, and get a subset of the
 * neighbourhood of only pairs, triplets ... for which the first atom
 * is of a given type and descend recursively
 *
 * @section LICENSE
 *
 * Copyright © 2018 Till Junge, COSMO (EPFL), LAMMM (EPFL)
 *
 * librascal is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3, or (at
 * your option) any later version.
 *
 * librascal is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Emacs; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */



#ifndef SPECIES_MANAGER_H
#define SPECIES_MANAGER_H

#include "neighbourhood_managers/neighbourhood_manager_base.hh"
namespace rascal {

  template <class NeighManager, int MaxDepth, int Depth=0>
  class SpeciesManager
  {
  public:

    using Species_t = int;
    using Dummy_t = char;
    using SubManagerType =
      std::conditional_t<(Depth != MaxDepth),
                         SpeciesManager<NeighManager, MaxDepth, Depth+1>,
                         Dummy_t>;
    //! Default constructor
    SpeciesManager() = delete;

    //! Construct from an existing Neighbourhoodmanager
    SpeciesManager(NeighManager & manager);

    //! Copy constructor
    SpeciesManager(const SpeciesManager &other) = delete;

    //! Move constructor
    SpeciesManager(SpeciesManager &&other) = default;

    //! Destructor
    virtual ~SpeciesManager()  = default;

    //! Copy assignment operator
    SpeciesManager& operator=(const SpeciesManager &other) = delete;

    //! Move assignment operator
    SpeciesManager& operator=(SpeciesManager &&other) = default;

    //! get the symmetry functions and the corresponding neighbourhoodmanager
    std::map<Species_t, SymFunManager> & get_symmetry_functions();

    //! get the next level of depth
    template <bool NotAtMaxDepth = (Depth != MaxDepth)>
    std::map<Species_t, std::enable_if_t<NotAtMaxDepth, SubManagerType>> & get_next_level();

  protected:
    std::array<Species_t, Depth> fixed_species;
    std::map<Species_t, SymFunManager>;
    std::map<Species_t, SubManagerType>;
  private:
  };

}  // rascal

#endif /* SPECIES_MANAGER_H */
