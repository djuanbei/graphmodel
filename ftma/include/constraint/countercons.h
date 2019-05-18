/**
 * @file   countercons.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu May  9 10:02:20 2019
 *
 * @brief  counter constraint like  pid==id
 *
 *
 */

#ifndef COUNTER_CONS_H
#define COUNTER_CONS_H

#include <vector>

#include "parameter.h"

#include "util/dbmutil.hpp"

namespace graphsat {

using namespace std;
class CounterConstraint {

public:
  CounterConstraint( const vector<pair<int, int>> &cons, int erhs,
                     COMP_OPERATOR eop )
      : constraint( cons )
      , rhs( erhs )
      , op( eop ) {}

  bool operator()( const Parameter &p, const int *valuation ) const {
    int dummy = 0;
    for ( vector<pair<int, int>>::const_iterator it = constraint.begin();
          it != constraint.end(); it++ ) {
      dummy += ( it->first ) * valuation[ it->second ];
    }
    if ( EQ == op ) {
      return dummy == rhs;
    } else if ( LE == op ) {
      return dummy <= rhs;
    } else if ( LT == op ) {
      return dummy < rhs;
    } else if ( GE == op ) {
      return dummy >= rhs;
    } else if ( GT == op ) {
      return dummy > rhs;
    } else if ( NE == op ) {
      return dummy != rhs;
    }
    return false;
  }

private:
  vector<pair<int, int>> constraint;
  int                    rhs;
  COMP_OPERATOR          op;
};

} // namespace graphsat

#endif
