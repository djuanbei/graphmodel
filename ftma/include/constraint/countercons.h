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

#include "util/data.hpp"

namespace graphsat {

class CounterConstraint {

public:
  virtual bool operator()( const int *parameter_value,
                           const int *counter_value ) const = 0;
};

class DiaFreeCounterConstraint : public CounterConstraint {

public:
  virtual bool operator()( const int *parameter_value,
                           const int *counter_value ) const {
    switch ( op ) {
    case EQ:
      return counter_value[ counter_id ] == rhs;
    case LE:
      return counter_value[ counter_id ] <= rhs;
    case LT:
      return counter_value[ counter_id ] < rhs;
    case GE:
      return counter_value[ counter_id ] >= rhs;
    case GT:
      return counter_value[ counter_id ] > rhs;
    case NE:
      return counter_value[ counter_id ] != rhs;
    default:
      return false;
    }
  }

private:
  DiaFreeCounterConstraint( int ecounter_id, COMP_OPERATOR opp,
                            int right_side ) {
    counter_id = ecounter_id;
    op         = opp;
    rhs        = right_side;
  }
  ~DiaFreeCounterConstraint() {}
  int           counter_id;
  COMP_OPERATOR op;
  int           rhs;
  friend class CounterConstraintFactory;
};

class DiaFreeCounterPConstraint : public CounterConstraint {
public:
  DiaFreeCounterPConstraint( int ecounter_id, COMP_OPERATOR opp,
                             int parameter_id ) {
    counter_id = ecounter_id;
    op         = opp;
    p_id       = parameter_id;
  }
  ~DiaFreeCounterPConstraint() {}

  virtual bool operator()( const int *parameter_value,
                           const int *counter_value ) const {

    int diff = counter_value[ counter_id ];
    int rhs  = parameter_value[ p_id ];
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

  friend class CounterConstraintFactory;
};

class DiaCounterConstraint : public CounterConstraint {

public:
  virtual bool operator()( const int *parameter_value,
                           const int *counter_value ) const {
    int diff = counter_value[ counter_x ] - counter_value[ counter_y ];
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
  DiaCounterConstraint( int x, int y, COMP_OPERATOR p, int r ) {
    counter_x = x;
    counter_y = y;
    op        = p;
    rhs       = r;
  }
  ~DiaCounterConstraint() {}
  int           counter_x, counter_y;
  COMP_OPERATOR op;
  int           rhs;
  friend class CounterConstraintFactory;
};

class DefaultCounterConstraint : public CounterConstraint {
private:
  DefaultCounterConstraint( const vector<pair<int, int>> &pcons,
                            const vector<pair<int, int>> &cons, int erhs,
                            COMP_OPERATOR eop )
      : pconstraint( pcons )
      , constraint( cons )
      , rhs( erhs )
      , op( eop ) {}
  ~DefaultCounterConstraint() {}

public:
  bool operator()( const int *parameter_value,
                   const int *counter_Value ) const {
    int dummy = 0;
    for ( auto e : pconstraint ) {
      dummy += e.first * parameter_value[ e.second ];
    }
    for ( vector<pair<int, int>>::const_iterator it = constraint.begin();
          it != constraint.end(); it++ ) {
      dummy += ( it->first ) * counter_Value[ it->second ];
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
  friend class CounterConstraintFactory;
};

class CounterConstraintFactory {
  SINGLETON( CounterConstraintFactory );

public:
  DefaultCounterConstraint *
      createDefaultCounterConstraint( const vector<pair<int, int>> &pcons,
                                      const vector<pair<int, int>> &cons,
                                      int erhs, COMP_OPERATOR eop ) {
    DefaultCounterConstraint *re =
        new DefaultCounterConstraint( pcons, cons, erhs, eop );
    pdata.addPointer( STRING( DefaultCounterConstraint ), re );
    return re;
  }

  DiaFreeCounterPConstraint *
      createDiaFreeCounterPConstraint( int c, COMP_OPERATOR o, int p ) {
    DiaFreeCounterPConstraint *re = new DiaFreeCounterPConstraint( c, o, p );
    pdata.addPointer( STRING( DiaFreeCounterPConstraint ), re );
    return re;
  }

  DiaCounterConstraint *createDiaCounterConstraint( int x, int y,
                                                    COMP_OPERATOR p, int r ) {
    DiaCounterConstraint *re = new DiaCounterConstraint( x, y, p, r );
    pdata.addPointer( STRING( DiaCounterConstraint ), re );
    return re;
  }
  DiaFreeCounterConstraint *
      createDiaFreeCounterConstraint( int cid, COMP_OPERATOR p, int r ) {
    DiaFreeCounterConstraint *re = new DiaFreeCounterConstraint( cid, p, r );
    pdata.addPointer( STRING( DiaFreeCounterConstraint ), re );
    return re;
  }

  void destroy() {

    deleteType( DefaultCounterConstraint );
    deleteType( DiaFreeCounterPConstraint );
    deleteType( DiaCounterConstraint );
    pdata.clear();
  }

private:
  PointerData pdata;
};

} // namespace graphsat

#endif
