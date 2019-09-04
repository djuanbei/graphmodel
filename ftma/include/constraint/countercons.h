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
protected:
  bool is_update;

public:
  virtual bool operator()( const int *parameter_value,
                           const int *counter_value ) const = 0;
  /**
   *Deley set the counter id
   */
  virtual void counterIpMap( const map<int, int> &id_map ) = 0;

  virtual CounterConstraint *copy() const = 0;
};

class DiaFreeCounterConstraint : public CounterConstraint {

public:
  bool operator()( const int *parameter_value,
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
  void counterIpMap( const map<int, int> &id_map ) {
    if ( !is_update ) {
      is_update  = true;
      counter_id = id_map.at( counter_id );
    }
  }

  CounterConstraint *copy() const {
    assert( !is_update );
    return new DiaFreeCounterConstraint( counter_id, op, rhs );
  }

private:
  DiaFreeCounterConstraint( int ecounter_id, COMP_OPERATOR opp,
                            int right_side ) {
    is_update  = false;
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
  bool operator()( const int *parameter_value,
                   const int *counter_value ) const {

    int diff = counter_value[ counter_id ];
    int rhs  = parameter_value[ parameter_id ];
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

  void counterIpMap( const map<int, int> &id_map ) {
    if ( !is_update ) {
      is_update  = true;
      counter_id = id_map.at( counter_id );
    }
  }
  CounterConstraint *copy() const {
    assert( !is_update );
    return new DiaFreeCounterPConstraint( counter_id, op, parameter_id );
  }

private:
  int           counter_id;
  COMP_OPERATOR op;
  int           parameter_id;

  DiaFreeCounterPConstraint( int ecounter_id, COMP_OPERATOR opp,
                             int eparameter_id ) {
    is_update    = false;
    counter_id   = ecounter_id;
    op           = opp;
    parameter_id = eparameter_id;
  }
  ~DiaFreeCounterPConstraint() {}

  friend class CounterConstraintFactory;
};

class DiaCounterConstraint : public CounterConstraint {

public:
  bool operator()( const int *parameter_value,
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

  void counterIpMap( const map<int, int> &id_map ) {
    if ( !is_update ) {
      is_update = true;
      counter_x = id_map.at( counter_x );
      counter_y = id_map.at( counter_y );
    }
  }

  CounterConstraint *copy() const {
    assert( !is_update );
    return new DiaCounterConstraint( counter_x, counter_y, op, rhs );
  }

private:
  DiaCounterConstraint( int x, int y, COMP_OPERATOR p, int r ) {
    is_update = false;
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

public:
  bool operator()( const int *parameter_value,
                   const int *counter_value ) const {
    int dummy = 0;
    for ( size_t i = 0; i < pconstraint.size(); i += 2 ) {
      dummy += pconstraint[ i ] * parameter_value[ pconstraint[ i + 1 ] ];
    }
    for ( size_t i = 0; i < constraint.size(); i += 2 ) {
      dummy += constraint[ i ] * counter_value[ constraint[ i + 1 ] ];
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

  void counterIpMap( const map<int, int> &id_map ) {
    if ( !is_update ) {
      is_update = true;
      for ( size_t i = 1; i < constraint.size(); i += 2 ) {
        constraint[ i ] = id_map.at( constraint[ i ] );
      }
    }
  }

  CounterConstraint *copy() const {
    assert( !is_update );
    return new DefaultCounterConstraint( pconstraint, constraint, rhs, op );
  }

private:
  DefaultCounterConstraint( const vector<int> &pcons, const vector<int> &cons,
                            int erhs, COMP_OPERATOR eop )
      : pconstraint( pcons )
      , constraint( cons )
      , rhs( erhs )
      , op( eop ) {
    is_update = false;
  }
  ~DefaultCounterConstraint() {}

private:
  // odd* value[even]
  vector<int>   pconstraint;
  vector<int>   constraint;
  int           rhs;
  COMP_OPERATOR op;
  friend class CounterConstraintFactory;
};

class CounterConstraintFactory {
  SINGLETON( CounterConstraintFactory );

public:
  DefaultCounterConstraint *
      createDefaultCounterConstraint( const vector<int> &pcons,
                                      const vector<int> &cons, int erhs,
                                      COMP_OPERATOR eop ) {
    DefaultCounterConstraint *re =
        new DefaultCounterConstraint( pcons, cons, erhs, eop );
    pdata.addValue( STRING( CounterConstraint ),
                    STRING( DefaultCounterConstraint ), re );
    return re;
  }

  DiaFreeCounterPConstraint *
      createDiaFreeCounterPConstraint( int c, COMP_OPERATOR o, int p ) {
    DiaFreeCounterPConstraint *re = new DiaFreeCounterPConstraint( c, o, p );
    pdata.addValue( STRING( CounterConstraint ),
                    STRING( DiaFreeCounterPConstraint ), re );
    return re;
  }

  DiaCounterConstraint *createDiaCounterConstraint( int x, int y,
                                                    COMP_OPERATOR p, int r ) {
    DiaCounterConstraint *re = new DiaCounterConstraint( x, y, p, r );
    pdata.addValue( STRING( CounterConstraint ), STRING( DiaCounterConstraint ),
                    re );
    return re;
  }
  DiaFreeCounterConstraint *
      createDiaFreeCounterConstraint( int cid, COMP_OPERATOR p, int r ) {
    DiaFreeCounterConstraint *re = new DiaFreeCounterConstraint( cid, p, r );
    pdata.addValue( STRING( CounterConstraint ),
                    STRING( DiaFreeCounterConstraint ), re );
    return re;
  }

  void destroy() {

    deleteType( pdata, CounterConstraint, DefaultCounterConstraint,
                DefaultCounterConstraint );
    deleteType( pdata, CounterConstraint, DiaFreeCounterPConstraint,
                DiaFreeCounterPConstraint );
    deleteType( pdata, CounterConstraint, DiaCounterConstraint,
                DiaCounterConstraint );
    pdata.clear();
  }

private:
  PointerData pdata;
};

} // namespace graphsat

#endif
