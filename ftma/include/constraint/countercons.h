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

using std::pair;
using std::vector;

class CounterConstraint {
public:
  virtual bool operator()( const Parameter &p, const int *valuation ) const = 0;
};

class DefaultCounterConstraint : public CounterConstraint {
public:
  DefaultCounterConstraint( const vector<pair<int, int>> &pcons,
                            const vector<pair<int, int>> &cons, int erhs,
                            COMP_OPERATOR eop )
      : pconstraint( pcons )
      , constraint( cons )
      , rhs( erhs )
      , op( eop ) {}

  bool operator()( const Parameter &p, const int *valuation ) const {
    int dummy = 0;
    for ( auto e : pconstraint ) {
      dummy += e.first * p.getValue( e.second );
    }
    for ( vector<pair<int, int>>::const_iterator it = constraint.begin();
          it != constraint.end(); it++ ) {
      dummy += ( it->first ) * valuation[ it->second ];
    }
    switch ( op ) {
    case EQ:
      return dummy == rhs;
    case LE:
      return dummy <= rhs;
    case LT:
      return dummy < rhs;
    case GE:
      return dummy >= rhs;
    case GT:
      return dummy > rhs;
    case NE:
      return dummy != rhs;
    default:
      return false;
    }
  }

private:
  vector<pair<int, int>> pconstraint;
  vector<pair<int, int>> constraint;
  int                    rhs;
  COMP_OPERATOR          op;
};

} // namespace graphsat

#endif
