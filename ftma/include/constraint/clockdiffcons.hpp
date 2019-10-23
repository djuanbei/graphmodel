
/**
 * @file   linsimpcons.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Mar 29 10:47:56 2019
 *
 * @brief  linear simple constraint such clock_x-clock_y < ( <= ) right
 *
 *
 */
#ifndef LIN_SIMP_CONS_HPP
#define LIN_SIMP_CONS_HPP
#include "util/dbmutil.hpp"
#include <iomanip>
#include <iostream>
#include <random>

namespace graphsat {

using namespace std;

const int GLOBAL_CLOCK_ID = 0;

/**
 *  clock_x -clock_y < ( <= ) realRight
 *
 */
template <typename C> class ClockConstraint {

public:
  int           clock_x;
  int           clock_y;
  COMP_OPERATOR op;
  C             matrix_value;

  ClockConstraint( const int clock_id1, const int clock_id2, COMP_OPERATOR eop,
                   const int rhs, const int eparameter_id = -100 ) {
    if ( eparameter_id < 0 ) {
      init( clock_id1, clock_id2, eop, rhs );
      return;
    }
    clock_x      = clock_id1;
    clock_y      = clock_id2;
    op           = eop;
    parameter_id = eparameter_id;
  }

  void globalUpdate( const vector<int> &parameter_value ) {
    if ( parameter_id < 0 ) {
      return;
    }
    int rhs = parameter_value[ parameter_id ];
    int id1 = clock_x;
    int id2 = clock_y;
    init( id1, id2, op, rhs );
  }

  void clockShift( int shift ) {
    if ( clock_x > 0 ) {
      clock_x += shift;
    }
    if ( clock_y > 0 ) {
      clock_y += shift;
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

    if ( ( cons.clock_x == clock_x ) && ( cons.clock_y == clock_y ) ) {
      return true;
    } else if ( ( cons.clock_x == clock_y ) && ( cons.clock_y == clock_x ) ) {
      ClockConstraint<C> negCons = cons.neg();
      return negCons.matrix_value < matrix_value;
    }

    if ( ( clock_x > 0 && clock_y > 0 ) &&
         ( cons.clock_x > 0 && cons.clock_y > 0 ) ) {
      return true;
    }
    if ( clock_y > 0 && cons.clock_y > 0 ) {
      return true;
    }

    if ( 0 == clock_y ) {
      if ( clock_x != cons.clock_y ) {
        return true;
      }

      return matrix_value + cons.matrix_value > LTEQ_ZERO;

    } else {
      if ( clock_y != cons.clock_x ) {
        return true;
      }
      return matrix_value + cons.matrix_value > LTEQ_ZERO;
    }

    return true;
  }

  friend std::ostream &operator<<( std::ostream &         out,
                                   const ClockConstraint &cons ) {
    if ( cons.clock_x >= 0 && cons.clock_y >= 0 ) {
      if ( isStrict<C>( cons.matrix_value ) ) {
        out << "x_" << cons.clock_x << " - "
            << "x_" << cons.clock_y << setw( 2 ) << "<" << setw( 5 )
            << getRight( cons.matrix_value );
      } else {
        out << "x_" << cons.clock_x << " - "
            << "x_" << cons.clock_y << setw( 2 ) << "<=" << setw( 5 )
            << getRight( cons.matrix_value );
      }
    }
    if ( cons.clock_x < 0 ) {
      if ( isStrict<C>( cons.matrix_value ) ) {
        out << "0     - "
            << "x_" << cons.clock_y << setw( 2 ) << "<" << setw( 5 )
            << getRight( cons.matrix_value );
      } else {
        out << "0     - "
            << " - "
            << "x_" << cons.clock_y << setw( 2 ) << "<=" << setw( 5 )
            << getRight( cons.matrix_value );
      }
    } else {
      if ( isStrict<C>( cons.matrix_value ) ) {
        out << "x_" << cons.clock_x << "          <  "
            << getRight( cons.matrix_value );
      } else {
        out << "x_" << cons.clock_x << "          <= " << setw( 5 )
            << getRight( cons.matrix_value );
      }
    }
    return out;
  }

private:
  /**
   is_strict_ref  is true : <
   is_strict_ref  is true : <=
   */
  ClockConstraint( const int clock_id1, const int clock_id2, const C rhs,
                   bool is_strict_ref = true ) {
    parameter_id = -100;
    clock_x      = clock_id1;
    clock_y      = clock_id2;
    matrix_value = getMatrixValue( rhs, is_strict_ref );
  }

  void neg_impl( void ) {
    int temp     = clock_x;
    clock_x      = clock_y;
    clock_y      = temp;
    matrix_value = 1 - matrix_value;
  }
  void init( const int clock_id1, const int clock_id2, COMP_OPERATOR op,
             const C rhs ) {
    assert( op != NE );
    assert( op != EQ );
    if ( LE == op ) {
      clock_x      = clock_id1;
      clock_y      = clock_id2;
      matrix_value = getMatrixValue( rhs, false );
    } else if ( LT == op ) {
      clock_x      = clock_id1;
      clock_y      = clock_id2;
      matrix_value = getMatrixValue( rhs, true );
    } else if ( GE == op ) {
      clock_x      = clock_id2;
      clock_y      = clock_id1;
      matrix_value = getMatrixValue( -rhs, false ); // clock_y-clock_x <= -rhs
    } else if ( GT == op ) {
      clock_x      = clock_id2;
      clock_y      = clock_id1;
      matrix_value = getMatrixValue( -rhs, true ); // clock_y-clock_x < -rhs
    }
  }

  int parameter_id;
};
} // namespace graphsat
#endif
