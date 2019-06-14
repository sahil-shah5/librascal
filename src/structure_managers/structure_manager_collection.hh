/**
 * file   structure_manager_collection.hh
 *
 * @author Felix Musil <felix.musil@epfl.ch>
 *
 * @date   13 Jun 2019
 *
 * @brief Implementation of a container for structure managers
 *
 * Copyright 2019 Felix Musil COSMO (EPFL), LAMMM (EPFL)
 *
 * Rascal is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3, or (at
 * your option) any later version.
 *
 * Rascal is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software; see the file LICENSE. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef SRC_STRUCTURE_MANAGERS_STRUCTURE_MANAGER_COLLECTION_HH_
#define SRC_STRUCTURE_MANAGERS_STRUCTURE_MANAGER_COLLECTION_HH_

#include "structure_managers/structure_manager.hh"
#include "structure_managers/make_structure_manager.hh"
#include "structure_managers/property.hh"
#include "structure_managers/updateable_base.hh"
#include "rascal_utility.hh"
#include "atomic_structure.hh"


namespace rascal {


  template<typename Manager,
            template <class> class... AdaptorImplementationPack>
  class ManagerCollection {
   public:
    using TypeHolder_t = StructureManagerTypeHolder<Manager, AdaptorImplementationPack...>;
    using Manager_t = TypeHolder_t::type;
    using ManagerPtr_t = std::shared_ptr<Manager_t>;
    using ManagerList_t = TypeHolder_t::type_list;
    using Hypers_t = typename Manager_t::Hypers_t;
    using traits = typename Manager_t::traits;

    ManagerCollection() = default;

    explicit ManagerCollection(const Hypers_t& adaptor_inputs)
      : adaptor_inputs{adaptor_inputs} {};


    //! Copy constructor
    ManagerCollection(const ManagerCollection & other) = delete;

    //! Move constructor
    ManagerCollection(ManagerCollection && other) = default;

    //! Destructor
    ~ManagerCollection() = default;

    //! Copy assignment operator
    ManagerCollection & operator=(const ManagerCollection & other) = delete;

    //! Move assignment operator
    ManagerCollection & operator=(ManagerCollection && other) = default;

    // AtomicStructure<traits::Dim>
    void add_structure(const Hypers_t& structure, const Hypers_t& adaptor_inputs) {
      make_structure_manager_stack<Manager, AdaptorImplementationPack...>(structure, adaptor_inputs)
    }

    void add_structure(const Hypers_t& structure) {
      make_structure_manager_stack<Manager, AdaptorImplementationPack...>(structure, this->adaptor_inputs)
    }

    void add_structures(const Hypers_t& structures, const Hypers_t& adaptors_inputs) {
      if (not structures.is_array()) {
        throw std::runtime_error(R"(Provide the structures as an array
        (or list) of json dictionary defining the structure)");
      }
      if (structures.size() != adaptors_inputs.size()) {
        throw std::runtime_error(R"(There should be as many structures as
        adaptors_inputs)");
      }

      for (int i_structure{0}; i_structure < structures.size(); ++i_structure) {
        this->add_structure(structures[i_structure], adaptors_inputs[i_structure])
      }


    }



   protected:
    std::vector<ManagerPtr_t> managers{};

    Hypers_t adaptor_inputs{};
  }

}

#endif  // SRC_STRUCTURE_MANAGERS_STRUCTURE_MANAGER_COLLECTION_HH_