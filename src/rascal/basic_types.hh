/**
 * @file   rascal/basic_types.hh
 *
 * @author Federico Giberti <federico.giberti@epfl.ch>
 *
 * @date   14 Mar 2018
 *
 * @brief  Implementation of base-type objects for Rascal
 *
 * Copyright  2017 Felix Musil
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

#ifndef SRC_RASCAL_BASIC_TYPES_HH_
#define SRC_RASCAL_BASIC_TYPES_HH_

namespace rascal {
  /**
   * integer scalar type to store the number of real space dimensions of the
   * system.
   */
  using Dim_t = int;

}  // namespace rascal

#endif  // SRC_RASCAL_BASIC_TYPES_HH_
