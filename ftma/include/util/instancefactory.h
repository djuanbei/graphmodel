/**
 * @file   instancefactory.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Sep  6 08:37:27 2019
 *
 * @brief  Singleton as a factory role to manager object which create in parser
 * process
 *
 *
 */

#ifndef INSTANCE_FACTORY
#define INSTANCE_FACTORY
#include "constraint/countercons.h"

#include "action/counteraction.h"

#define regist_to_factory( T, V, V1 ) T *create##T V  {\
  return InstanceFactory::getInstance().create##T V1 ;   \
  }

namespace graphsat {
class InstanceFactory {
  SINGLETON( InstanceFactory );

public:
  ~InstanceFactory() { destroy(); }

private:
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

  FreeCounterConstraint *createFreeCounterConstraint( int parameter_id,
                                                      COMP_OPERATOR op,
                                                      int           rhs ) {
    FreeCounterConstraint *re =
        new FreeCounterConstraint( parameter_id, op, rhs );
    pdata.addValue( STRING( CounterConstraint ),
                    STRING( FreeCounterConstraint ), re );
    return re;
  }

  CounterParameterConstraint *
      createCounterParameterConstraint( int counter_id, int parameter_id,
                                        COMP_OPERATOR op, int rhs ) {
    CounterParameterConstraint *re =
        new CounterParameterConstraint( counter_id, parameter_id, op, rhs );
    pdata.addValue( STRING( CounterConstraint ),
                    STRING( CounterParameterConstraint ), re );
    return re;
  }

  DiaParameterConstraint *
      createDiaParameterConstraint( int efirst_parameter_id,
                                    int esecond_parameter_id, COMP_OPERATOR eop,
                                    int erhs ) {
    DiaParameterConstraint *re = new DiaParameterConstraint(
        efirst_parameter_id, esecond_parameter_id, eop, erhs );
    pdata.addValue( STRING( CounterConstraint ),
                    STRING( DiaParameterConstraint ), re );
    return re;
  }

  DiaCounterConstraint *createDiaCounterConstraint( int           counter_x,
                                                    int           counter_y,
                                                    COMP_OPERATOR op,
                                                    int           rhs ) {
    DiaCounterConstraint *re =
        new DiaCounterConstraint( counter_x, counter_y, op, rhs );
    pdata.addValue( STRING( CounterConstraint ), STRING( DiaCounterConstraint ),
                    re );
    return re;
  }

  DiaFreeCounterConstraint *createDiaFreeCounterConstraint( int counter_id,
                                                            COMP_OPERATOR op,
                                                            int rhs ) {
    DiaFreeCounterConstraint *re =
        new DiaFreeCounterConstraint( counter_id, op, rhs );
    pdata.addValue( STRING( CounterConstraint ),
                    STRING( DiaFreeCounterConstraint ), re );
    return re;
  }

  CounterConstraint *copy( CounterConstraint *other ) {
    CounterConstraint *re = other->copy();
    pdata.addValue( STRING( CounterConstraint ), STRING( CounterConstraint ),
                    re );
    return re;
  }

  SimpleCounterAction *createSimpleCounterAction( int counter_id, int value ) {
    SimpleCounterAction *re = new SimpleCounterAction( counter_id, value );
    pdata.addValue( STRING( CounterAction ), STRING( SimpleCounterAction ),
                    re );
    return re;
  }

  SimpleCounterPAction *createSimpleCounterPAction( int counter_id,
                                                    int eparameter_id ) {
    SimpleCounterPAction *re =
        new SimpleCounterPAction( counter_id, eparameter_id );
    pdata.addValue( STRING( CounterAction ), STRING( SimpleCounterPAction ),
                    re );
    return re;
  }

  DefaultCAction *createDefaultCAction(
      vector<pair<int, vector<pair<int, int>>>> &relations1 ) {
    DefaultCAction *re = new DefaultCAction( relations1 );
    pdata.addValue( STRING( CounterAction ), STRING( DefaultCAction ), re );
    return re;
  }

  CounterAction *copy( const CounterAction *other ) {
    CounterAction *re = other->copy();
    pdata.addValue( STRING( CounterAction ), STRING( CounterAction ), re );
    return re;
  }

public:
  void destroy() {

    deleteType( pdata, CounterConstraint );

    deleteType( pdata, CounterAction );

    pdata.clear();
  }

  friend DefaultCounterConstraint *
                                createDefaultCounterConstraint( const vector<int> &pcons,
                                                                const vector<int> &cons, int erhs,
                                                                COMP_OPERATOR eop );
  friend FreeCounterConstraint *createFreeCounterConstraint( int parameter_id,
                                                             COMP_OPERATOR op,
                                                             int rhs );

  friend CounterParameterConstraint *
      createCounterParameterConstraint( int counter_id, int parameter_id,
                                        COMP_OPERATOR op, int rhs );

  friend DiaParameterConstraint *
      createDiaParameterConstraint( int efirst_parameter_id,
                                    int esecond_parameter_id, COMP_OPERATOR eop,
                                    int erhs );

  friend DiaCounterConstraint *createDiaCounterConstraint( int counter_x,
                                                           int counter_y,
                                                           COMP_OPERATOR op,
                                                           int           rhs );

  friend DiaFreeCounterConstraint *
      createDiaFreeCounterConstraint( int counter_id, COMP_OPERATOR op,
                                      int rhs );

  friend CounterConstraint *copy( CounterConstraint *other );

  friend SimpleCounterAction *createSimpleCounterAction( int counter_id,
                                                         int value );

  friend SimpleCounterPAction *createSimpleCounterPAction( int counter_id,
                                                           int eparameter_id );

  friend DefaultCAction *createDefaultCAction(
      vector<pair<int, vector<pair<int, int>>>> &relations1 );

  friend CounterAction *copy( const CounterAction *other );

private:
  PointerData pdata;
};

DefaultCounterConstraint *
                       createDefaultCounterConstraint( const vector<int> &pcons,
                                                       const vector<int> &cons, int erhs,
                                                       COMP_OPERATOR eop );
FreeCounterConstraint *createFreeCounterConstraint( int           parameter_id,
                                                    COMP_OPERATOR op, int rhs );

CounterParameterConstraint *createCounterParameterConstraint( int counter_id,
                                                              int parameter_id,
                                                              COMP_OPERATOR op,
                                                              int rhs );

DiaParameterConstraint *createDiaParameterConstraint( int efirst_parameter_id,
                                                      int esecond_parameter_id,
                                                      COMP_OPERATOR eop,
                                                      int           erhs );

DiaCounterConstraint *createDiaCounterConstraint( int counter_x, int counter_y,
                                                  COMP_OPERATOR op, int rhs );

DiaFreeCounterConstraint *
    createDiaFreeCounterConstraint( int counter_id, COMP_OPERATOR op, int rhs );

CounterConstraint *copy( CounterConstraint *other );

SimpleCounterAction *createSimpleCounterAction( int counter_id, int value );

SimpleCounterPAction *createSimpleCounterPAction( int counter_id,
                                                  int eparameter_id );

DefaultCAction *createDefaultCAction(
    vector<pair<int, vector<pair<int, int>>>> &relations1 );

CounterAction *copy( const CounterAction *other );

CounterParameterConstraint *neg( int parameter_id, int counter_id,
                                 COMP_OPERATOR op, int rhs ) {
  COMP_OPERATOR nop = negation( op );
  return createCounterParameterConstraint( counter_id, parameter_id, nop,
                                           rhs * -1 );
}

} // namespace graphsat

#endif
