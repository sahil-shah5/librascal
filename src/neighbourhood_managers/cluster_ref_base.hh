/**
 * file   cluster_ref_base.hh
 *
 * @author Till Junge <till.junge@epfl.ch>
 *
 * @date   21 Jun 2018
 *
 * @brief  a base class for getting access to clusters
 *
 * Copyright © 2018 Till Junge, Markus Stricker, COSMO (EPFL), LAMMM (EPFL)
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

#ifndef CLUSTERREFBASE_H
#define CLUSTERREFBASE_H

#include <tuple>
#include <array>

#include <Eigen/Dense>

namespace rascal {

  /**
   * Depth calculations and manipulations
   */
  /**
   * Computes depth by cluster dimension for new adaptor layer
   */
  template <size_t MaxLevel, class T>
  struct DepthIncreaser{};

  template <size_t MaxLevel, size_t... Ints>
  struct DepthIncreaser<MaxLevel,
                        std::integer_sequence<size_t, Ints...>>{
    using type = std::integer_sequence<size_t, (Ints+1)...>;
  };

  template <size_t MaxLevel, size_t... Ints>
  using DepthIncreaser_t = typename DepthIncreaser<MaxLevel,
                                                   std::integer_sequence<size_t, Ints...>>::type;

  /**
   * Extends depth by cluster for an additional cluster dimension
   */
  template <size_t MaxLevel, class T>
  struct DepthExtender{};

  template <size_t MaxLevel, size_t... Ints>
  struct DepthExtender<MaxLevel,
                       std::integer_sequence<size_t, Ints...>>{
    using type = std::integer_sequence<size_t, Ints..., 0>;
  };

  template <size_t MaxLevel, size_t... Ints>
  using DepthExtender_t = typename DepthExtender<MaxLevel,
                                                 std::integer_sequence<size_t, Ints...>>::type;

  /**
   * Dynamic access to all depths by cluster dimension (probably not
   * necessary)
   */
  template <size_t MaxLevel, size_t... Ints>
  constexpr std::array<size_t, MaxLevel>
  get_depths(std::integer_sequence<size_t, Ints...>) {
    return std::array<size_t, MaxLevel>{Ints...};
  }

  namespace internal {
    template <size_t head, size_t... tail>
    struct Min {
      constexpr static size_t value{ head < Min<tail...>::value ? head : Min<tail...>::value};
    };

    template <size_t head>
    struct Min<head> {
      constexpr static size_t value{head};
    };

    template <class Sequence>
    struct MinExtractor {};

    template <size_t... Ints>
    struct MinExtractor<std::integer_sequence<size_t, Ints...>> {
      constexpr static size_t value {Min<Ints...>::value};
    };

    template <size_t Level, class Sequence, size_t... Ints>
    struct HeadExtractor {};

    template <size_t... seq>
    struct HeadExtractorTail {
      using type = std::integer_sequence<size_t, seq...>;
    };
      

    template <size_t Level, size_t head, size_t... tail, size_t... seq>
    struct HeadExtractor<Level, std::integer_sequence<size_t, seq...>, head, tail...> {
      using Extractor_t = std::conditional_t
        <(Level > 1),
        HeadExtractor<Level-1,
                      std::integer_sequence<size_t, seq..., head>,
                      tail...>,
        HeadExtractorTail<seq..., head>>;
      using type = typename Extractor_t::type; 
    };


  }  // internal

  template <size_t Level, size_t... Ints>
  constexpr size_t compute_cluster_depth(const std::integer_sequence<size_t, Ints...> &) {
    using ActiveDimensions = typename internal::HeadExtractor
      <Level, std::integer_sequence<size_t>, Ints...>::type;
    return internal::MinExtractor<ActiveDimensions>::value;
  }

  /**
   * Dynamic access to depth by cluster dimension (possibly not
   * necessary)
   */
  template <size_t MaxLevel, size_t... Ints>
  constexpr size_t
  get_depth(size_t index, std::integer_sequence<size_t, Ints...>) {
    constexpr size_t arr[] {Ints...};
    return arr [index];
  }


  /**
   * Static access to depth by cluster dimension (e.g., for defining
   * template parameter `NbRow` for a property
   */
  template <size_t index, size_t... Ints>
  constexpr size_t get(std::integer_sequence<size_t, Ints...>) {
    return get<index>(std::make_tuple(Ints...));
  }

  
  namespace internal {
    template <size_t Depth, size_t HiDepth,typename T, size_t... Ints>
    std::array<T, Depth>
    head_helper(const std::array<T, HiDepth> & arr,
                std::index_sequence<Ints...>) {
      return std::array<T, Depth> {arr[Ints]...};
    }

    template <size_t Depth, size_t HiDepth,typename T>
    std::array<T, Depth> head(const std::array<T, HiDepth> & arr) {
      return head_helper(arr, std::make_index_sequence<Depth>{});
    }

  }  // internal

  template<size_t Level, size_t Depth>
  class ClusterRefBase
  {
  public:
    //! Default constructor
    ClusterRefBase() = delete;

    //! direct constructor
    ClusterRefBase(std::array<size_t, Level> atom_indices,
                   std::array<size_t, Depth> cluster_indeces):
      atom_indices{atom_indices}, cluster_indices(cluster_indices) {}

    // //! constructor from higher depth
    // template<size_t HiDepth>
    // ClusterRefBase(const ClusterRefBase<Level, HiDepth> & other):
    //   atom_indices{other.atom_indices},
    //   cluster_indices{internal::head<Depth>(other.cluster_indices)} {
    //     static_assert(HiDepth >= Depth,
    //                   "You are trying to access a property that "
    //                   "does not exist at this low a level in the "
    //                   "adaptor stack.");
    //   }

    //! Copy constructor
    ClusterRefBase(const ClusterRefBase &other) = default; //

    //! Move constructor
    ClusterRefBase(ClusterRefBase &&other) = default;

    //! Destructor
    virtual ~ClusterRefBase() = default;

    //! Copy assignment operator
    ClusterRefBase& operator=(const ClusterRefBase &other) = delete;

    //! Move assignment operator
    ClusterRefBase& operator=(ClusterRefBase &&other) = default;

    const std::array<size_t, Level> & get_atom_indices() const {
      return this->indices;
    }

    const size_t & front() const{return this->atom_indices.front();}
    const size_t & back() const{return this->atom_indices.back();}

    inline size_t get_cluster_index(size_t depth) const {
      return this->cluster_indices(depth);
    }

  protected:
    std::array<size_t, Level> atom_indices;
    /**
     * cluster indices by depth level, highest depth, means last
     * adaptor, and mean last entry (.back())
     */
    // Eigen::Map
    Eigen::Map<Eigen::Matrix<size_t, Depth, 1>> cluster_indices;

  private:
  };

} // rascal

#endif /* CLUSTERREFBASE_H */
