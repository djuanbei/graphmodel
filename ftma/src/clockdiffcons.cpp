#include "constraint/clockdiffcons.h"
#include <cassert>

namespace graphsat {

ClockConstraint::ClockConstraint( const int clock_id1, const int clock_id2,
                                  COMP_OPERATOR eop, const int rhs,
                                  const int eparameter_id ) {

  if ( eparameter_id < 0 ) {
    init( clock_id1, clock_id2, eop, rhs );
    return;
  }
  clock_x      = clock_id1;
  clock_y      = clock_id2;
  op           = eop;
  parameter_id = eparameter_id;
}

void ClockConstraint::globalUpdate( const vector<int> &parameter_value ) {
  if ( parameter_id < 0 ) {
    return;
  }
  int rhs = parameter_value[ parameter_id ];
  int id1 = clock_x;
  int id2 = clock_y;
  init( id1, id2, op, rhs );
}

void ClockConstraint::clockShift( int shift ) {
  if ( clock_x > 0 ) {
    clock_x += shift;
  }
  if ( clock_y > 0 ) {
    clock_y += shift;
  }
}

ClockConstraint ClockConstraint::neg() const {
  ClockConstraint re( *this );
  re.neg_impl();
  return re;
}

bool ClockConstraint::isSat( const ClockConstraint &cons ) const {
  if ( ( cons.clock_x == clock_x ) && ( cons.clock_y == clock_y ) ) {
    return true;
  } else if ( ( cons.clock_x == clock_y ) && ( cons.clock_y == clock_x ) ) {
    ClockConstraint negCons = cons.neg();
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

std::ostream &operator<<( std::ostream &out, const ClockConstraint &cons ) {
  if ( cons.clock_x >= 0 && cons.clock_y >= 0 ) {
    if ( isStrict<int>( cons.matrix_value ) ) {
      out << "x_" << cons.clock_x << " - "
          << "x_" << cons.clock_y << setw( 2 ) << "<" << setw( 5 )
          << getRight( cons.matrix_value );
    } else {
      out << "x_" << cons.clock_x << " - "
          << "x_" << cons.clock_y << setw( 2 ) << "<=" << setw( 5 )
          << getRight( cons.matrix_value );
    }
  } else if ( cons.clock_x < 0 ) {
    if ( isStrict<int>( cons.matrix_value ) ) {
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
    if ( isStrict<int>( cons.matrix_value ) ) {
      out << "x_" << cons.clock_x << "          <  "
          << getRight( cons.matrix_value );
    } else {
      out << "x_" << cons.clock_x << "          <= " << setw( 5 )
          << getRight( cons.matrix_value );
    }
  }
  return out;
}

ClockConstraint::ClockConstraint( const int clock_id1, const int clock_id2,
                                  const int rhs, bool is_strict_ref ) {
  parameter_id = -100;
  clock_x      = clock_id1;
  clock_y      = clock_id2;
  matrix_value = getMatrixValue( rhs, is_strict_ref );
}

void ClockConstraint::neg_impl( void ) {
  int temp     = clock_x;
  clock_x      = clock_y;
  clock_y      = temp;
  matrix_value = 1 - matrix_value;
}

void ClockConstraint::init( const int clock_id1, const int clock_id2,
                            COMP_OPERATOR eop, const int rhs ) {
  op = eop;
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

ClockConstraint randConst( int num, int low, int up ) {
  std::uniform_int_distribution<int> distribution( 0, num );
  std::default_random_engine         generator;
  int                                xx = distribution( generator );
  int                                yy = distribution( generator );
  while ( xx == yy ) {
    yy = distribution( generator );
  }

  std::uniform_int_distribution<int> distribution1( low, up );

  int vv = distribution1( generator );
  if ( distribution1( generator ) % 2 ) {
    return ClockConstraint( xx, yy, LT, vv );
  } else {
    return ClockConstraint( xx, yy, LE, vv );
  }
}
} // namespace graphsat
