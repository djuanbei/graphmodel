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
  virtual bool operator()( const Parameter &p,
                           const int *      counterValuation ) const = 0;
};

class DiaFreeCounterConstraint : public CounterConstraint {
public:
  DiaFreeCounterConstraint( int cid, COMP_OPERATOR p, int r ) {
    counter_id = cid;
    op         = p;
    rhs        = r;
  }
  virtual bool operator()( const Parameter &p,
                           const int *      counterValuation ) const {
    switch ( op ) {
    case EQ:
      return counterValuation[ counter_id ] == rhs;
    case LE:
      return counterValuation[ counter_id ] <= rhs;
    case LT:
      return counterValuation[ counter_id ] < rhs;
    case GE:
      return counterValuation[ counter_id ] >= rhs;
    case GT:
      return counterValuation[ counter_id ] > rhs;
    case NE:
      return counterValuation[ counter_id ] != rhs;
    default:
      return false;
    }
  }

private:
  int           counter_id;
  COMP_OPERATOR op;
  int           rhs;
};

class DiaFreeCounterPConstraint : public CounterConstraint {
public:
  DiaFreeCounterPConstraint( int c, COMP_OPERATOR o, int p ) {
    counter_id = c;
    op         = o;
    p_id       = p;
  }
  virtual bool operator()( const Parameter &p,
                           const int *      counterValuation ) const {

    int diff = counterValuation[ counter_id ];
    int rhs  = p.getValue( p_id );
    switch ( op ) {
    case EQ:
      return diff == rhs;
    case LE:
      return diff <= rhs;
    case LT:
      return diff < rhs;
    case GE:
      return diff >= rhs;
    case GT:
      return diff > rhs;
    case NE:
      return diff != rhs;
    default:
      return false;
    }
  }

private:
  int           counter_id;
  COMP_OPERATOR op;
  int           p_id;
};

class DiaCounterConstraint : public CounterConstraint {
public:
  DiaCounterConstraint( int x, int y, COMP_OPERATOR p, int r ) {
    counter_x = x;
    counter_y = y;
    op        = p;
    rhs       = r;
  }
  virtual bool operator()( const Parameter &p,
                           const int *      counterValuation ) const {
    int diff = counterValuation[ counter_x ] - counterValuation[ counter_y ];
    switch ( op ) {
    case EQ:
      return diff == rhs;
    case LE:
      return diff <= rhs;
    case LT:
      return diff < rhs;
    case GE:
      return diff >= rhs;
    case GT:
      return diff > rhs;
    case NE:
      return diff != rhs;
    default:
      return false;
    }
  }

private:
  int           counter_x, counter_y;
  COMP_OPERATOR op;
  int           rhs;
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

  bool operator()( const Parameter &p, const int *counterValuation ) const {
    int dummy = 0;
    for ( auto e : pconstraint ) {
      dummy += e.first * p.getValue( e.second );
    }
    for ( vector<pair<int, int>>::const_iterator it = constraint.begin();
          it != constraint.end(); it++ ) {
      dummy += ( it->first ) * counterValuation[ it->second ];
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
