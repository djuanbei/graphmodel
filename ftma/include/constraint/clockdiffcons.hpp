
/**
 * @file   linsimpcons.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Mar 29 10:47:56 2019
 *
 * @brief  linear simple constraint such x-y < ( <= ) right
 *
 *
 */
#ifndef LIN_SIMP_CONS_HPP
#define LIN_SIMP_CONS_HPP
#include "util/dbmutil.hpp"
#include <iostream>
#include <random>

namespace graphsat {

using namespace std;

/**
 *  x -y < ( <= ) realRight
 *
 */
template <typename C> class ClockConstraint {
private:
  void neg_impl( void ) {
    int temp     = x;
    x            = y;
    y            = temp;
    matrix_value = 1 - matrix_value;
  }

public:
  int x;
  int y;

  C matrix_value;

  ClockConstraint( const int i, const int j, const C r, bool isStrctRef = true ) {
    x            = i;
    y            = j;
    matrix_value = r * 2;
    if ( !isStrctRef ) {
      matrix_value = matrix_value | 1;
    }
  }

  ClockConstraint randConst( int num, int low, int up ) const {
    std::uniform_int_distribution<int> distribution( 0, num );
    std::default_random_engine         generator;
    int                                xx = distribution( generator );
    int                                yy = distribution( generator );
    while ( xx == yy ) {
      yy = distribution( generator );
    }

    std::uniform_int_distribution<C> distribution1( low, up );

    C vv = distribution1( generator );
    return ClockConstraint( xx, yy, vv );
  }

  ClockConstraint neg( void ) const {
    ClockConstraint re( *this );
    re.neg_impl();
    return re;
  }

  bool isSat( const ClockConstraint<C> &cons ) const {

    if ( ( cons.x == x ) && ( cons.y == y ) ) {
      return true;
    } else if ( ( cons.x == y ) && ( cons.y == x ) ) {
      ClockConstraint<C> negCons = cons.neg();
      return negCons.matrix_value < matrix_value;
    }

    if ( ( x > 0 && y > 0 ) && ( cons.x > 0 && cons.y > 0 ) ) {
      return true;
    }
    if ( y > 0 && cons.y > 0 ) {
      return true;
    }

    if ( 0 == y ) {
      if ( x != cons.y ) {
        return true;
      }

      return matrix_value + cons.matrix_value > LTEQ_ZERO;

    } else {
      if ( y != cons.x ) {
        return true;
      }
      return matrix_value + cons.matrix_value > LTEQ_ZERO;
    }

    return true;
  }

  friend std::ostream &operator<<( std::ostream &os, const ClockConstraint &cons ) {
    if ( cons.x >= 0 && cons.y >= 0 ) {
      if ( isStrict<C>( cons.matrix_value ) ) {
        os << "x_" << cons.x << " - "
           << "x_" << cons.y << " < " << getRight( cons.matrix_value );
      } else {
        os << "x_" << cons.x << " - "
           << "x_" << cons.y << " <= " << getRight( cons.matrix_value );
      }
    }
    if ( cons.x < 0 ) {
      if ( isStrict<C>( cons.matrix_value ) ) {
        os << "0     - "
           << "x_" << cons.y << " < " << getRight( cons.matrix_value );
      } else {
        os << "0     - "
           << " - "
           << "x_" << cons.y << " <= " << getRight( cons.matrix_value );
      }
    } else {
      if ( isStrict<C>( cons.matrix_value ) ) {
        os << "x_" << cons.x << "          <  "
           << getRight( cons.matrix_value );
      } else {
        os << "x_" << cons.x
           << "          <= " << getRight( cons.matrix_value );
      }
    }
    return os;
  }
};
} // namespace graphsat
#endif
