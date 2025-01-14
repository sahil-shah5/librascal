/**
 * @file   rascal/math/utils.hh
 *
 * @author  Felix Musil <felix.musil@epfl.ch>
 * @author  Max Veit <max.veit@epfl.ch>
 *
 * @date   14 October 2018
 *
 * @brief contains the implementation of miscellaneous math functions
 *
 * Copyright  2018  Felix Musil, Max Veit, COSMO (EPFL), LAMMM (EPFL)
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

#ifndef SRC_RASCAL_MATH_UTILS_HH_
#define SRC_RASCAL_MATH_UTILS_HH_

#include <Eigen/Dense>

#include <cmath>
#include <cstdint>
#include <limits>

namespace rascal {
  /**
   * User defined literal operator to initialize size_t literals
   *
   * linter wants std::uint64_t to avoid the C style type but litterals are
   * of type unsigned long long while std::uint64_t on 64bits machines stands
   * for unsigned long. Both have equivalent underlying storage but not the
   * same type...
   */
  constexpr std::size_t
  operator"" _size_t(unsigned long long int n) {  // NOLINT
    return n;
  }

  namespace math {
    /// π to more digits than anyone could possibly need
    constexpr double PI = 3.14159265358979323846264338327950288;
    /// sqrt(π)
    constexpr double SQRT_PI = 1.772453850905516027298167483341145182;
    /// sqrt(2)
    constexpr double SQRT_TWO = 1.41421356237309504880168872420969808;
    /// 1 / sqrt(2)
    constexpr double INV_SQRT_TWO = 0.707106781186547524400844362104849039;
    /// sqrt(3)
    constexpr double SQRT_THREE = 1.7320508075688772935274463415058723;

    /// How small a number must be to be considered effectively zero
    constexpr double DBL_FTOL = 100.0 * std::numeric_limits<double>::epsilon();
    /// How large a number must be to be considered infinity
    constexpr double DOVERFLOW = std::numeric_limits<double>::infinity();

    // define some usefull matrix type
    using Matrix_t =
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
    using MatrixX2_t = Eigen::Matrix<double, Eigen::Dynamic, 2>;
    using Vector_t = Eigen::Matrix<double, 1, Eigen::Dynamic, Eigen::RowMajor>;

    using Matrix_Ref = typename Eigen::Ref<const Matrix_t>;
    using MatrixX2_Ref = typename Eigen::Ref<const MatrixX2_t>;
    using Vector_Ref = typename Eigen::Ref<const Vector_t>;

    /**
     * Define integer powers and wrap the different cases under the same name
     */
    namespace details {
      //! unsingned integer power
      inline double pow_u(double x, size_t n) {
        double value{1};

        /* repeated squaring method
         * returns 0.0^0 = 1.0, so continuous in x
         * (from GSL)
         */
        do {
          if (n & 1)
            value *= x; /* for n odd */
          n >>= 1;
          x *= x;
        } while (n);

        return value;
      }

      //! integer power
      inline double pow_i(double x, int n) {
        size_t un{0};
        double value{static_cast<double>(x)};

        if (n < 0) {
          value = 1.0 / x;
          un = static_cast<size_t>(-n);
        } else {
          un = static_cast<size_t>(n);
        }

        return pow_u(value, un);
      }
    }  // namespace details

    //! integer power
    inline double pow(double x, int n) { return details::pow_i(x, n); }

    //! integer power
    inline double pow(int x, int n) { return details::pow_i(x, n); }

    //! integer power
    inline double pow(size_t x, int n) { return details::pow_i(x, n); }

    //! unsigned integer power
    inline double pow(double x, size_t n) { return details::pow_u(x, n); }

    //! unsigned integer power
    inline int pow(int x, size_t n) { return details::pow_u(x, n); }

    //! unsigned integer power
    inline size_t pow(size_t x, size_t n) { return details::pow_u(x, n); }

    //! general power
    inline double pow(double x, double n) { return std::pow(x, n); }
  }  // namespace math
}  // namespace rascal

#endif  // SRC_RASCAL_MATH_UTILS_HH_
