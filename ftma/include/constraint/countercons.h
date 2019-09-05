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

//#include "parameter.h"

#include "util/dbmutil.hpp"

#include "util/data.hpp"

namespace graphsat {

class CounterConstraint {

public:
  virtual bool operator()( const int *counter_value ) const = 0;
  /**
   *Deley set the counter id
   */
  virtual void globalUpdate( const map<int, int> &id_map,
                             const vector<int> &  parameter_value ) = 0;

  virtual CounterConstraint *copy() const = 0;
};

class DiaFreeCounterConstraint : public CounterConstraint {

public:
  bool operator()( const int *counter_value ) const {
    switch ( op ) {
    case EQ:
      return counter_value[ global_counter_id ] == rhs;
    case LE:
      return counter_value[ global_counter_id ] <= rhs;
    case LT:
      return counter_value[ global_counter_id ] < rhs;
    case GE:
      return counter_value[ global_counter_id ] >= rhs;
    case GT:
      return counter_value[ global_counter_id ] > rhs;
    case NE:
      return counter_value[ global_counter_id ] != rhs;
    default:
      return false;
    }
  }
  void globalUpdate( const map<int, int> &id_map,
                     const vector<int> &  parameter_value ) {

    global_counter_id = id_map.at( local_counter_id );
  }

  CounterConstraint *copy() const {

    return new DiaFreeCounterConstraint( local_counter_id, op, rhs );
  }

private:
  DiaFreeCounterConstraint( int ecounter_id, COMP_OPERATOR opp,
                            int right_side ) {

    local_counter_id  = ecounter_id;
    global_counter_id = 0;
    op                = opp;
    rhs               = right_side;
  }
  ~DiaFreeCounterConstraint() {}
  int           local_counter_id;
  int           global_counter_id;
  COMP_OPERATOR op;
  int           rhs;
  friend class CounterConstraintFactory;
};

class DiaFreeCounterPConstraint : public CounterConstraint {

public:
  bool operator()( const int *counter_value ) const {

    int diff = counter_value[ global_counter_id ];
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

  void globalUpdate( const map<int, int> &id_map,
                     const vector<int> &  parameter_value ) {

    global_counter_id = id_map.at( local_counter_id );
    rhs               = parameter_value[ parameter_id ];
  }

  CounterConstraint *copy() const {

    return new DiaFreeCounterPConstraint( local_counter_id, op, parameter_id );
  }

private:
  int           local_counter_id;
  int           global_counter_id;
  COMP_OPERATOR op;
  int           parameter_id;
  int           rhs;

  DiaFreeCounterPConstraint( int ecounter_id, COMP_OPERATOR opp,
                             int eparameter_id ) {

    local_counter_id = ecounter_id;
    op               = opp;
    parameter_id     = eparameter_id;
    rhs              = 0;
  }
  ~DiaFreeCounterPConstraint() {}

  friend class CounterConstraintFactory;
};

class DiaCounterConstraint : public CounterConstraint {

public:
  bool operator()( const int *counter_value ) const {
    int diff =
        counter_value[ global_counter_x ] - counter_value[ global_counter_y ];
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

  void globalUpdate( const map<int, int> &id_map,
                     const vector<int> &  parameter_value ) {

    global_counter_x = id_map.at( local_counter_x );
    global_counter_y = id_map.at( local_counter_y );
  }

  CounterConstraint *copy() const {

    return new DiaCounterConstraint( local_counter_x, local_counter_y, op,
                                     rhs );
  }

private:
  DiaCounterConstraint( int x, int y, COMP_OPERATOR p, int r ) {

    local_counter_x = x;
    local_counter_y = y;
    op              = p;
    rhs             = r;
  }
  ~DiaCounterConstraint() {}

  int local_counter_x, local_counter_y, global_counter_x, global_counter_y;
  COMP_OPERATOR op;
  int           rhs;
  friend class CounterConstraintFactory;
};

class DefaultCounterConstraint : public CounterConstraint {

public:
  bool operator()( const int *counter_value ) const {
    int dummy = parameter_part;

    for ( size_t i = 0; i < local_constraint.size(); i += 2 ) {
      dummy +=
          local_constraint[ i ] * counter_value[ local_constraint[ i + 1 ] ];
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

  void globalUpdate( const map<int, int> &id_map,
                     const vector<int> &  parameter_value ) {

    for ( size_t i = 1; i < local_constraint.size(); i += 2 ) {
      global_constraint[ i ] = id_map.at( local_constraint[ i ] );
    }

    parameter_part = 0;
    for ( size_t i = 0; i < pconstraint.size(); i += 2 ) {
      parameter_part +=
          pconstraint[ i ] * parameter_value[ pconstraint[ i + 1 ] ];
    }
  }

  CounterConstraint *copy() const {

    return new DefaultCounterConstraint( pconstraint, local_constraint, rhs,
                                         op );
  }

private:
  DefaultCounterConstraint( const vector<int> &pcons, const vector<int> &cons,
                            int erhs, COMP_OPERATOR eop )
      : pconstraint( pcons )
      , local_constraint( cons )
      , global_constraint( cons )
      , rhs( erhs )
      , op( eop ) {}
  ~DefaultCounterConstraint() {}

private:
  // odd* value[even]
  vector<int>   pconstraint;
  vector<int>   local_constraint;
  vector<int>   global_constraint;
  int           rhs;
  COMP_OPERATOR op;
  int           parameter_part;
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
