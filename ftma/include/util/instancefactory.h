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

#define regist_to_factory( T, V, V1 )                                          \
  T *create##T V { return InstanceFactory::getInstance().create##T V1; }

#define regist_factory_method( T, V, V1, C )                                   \
  T *create##T V {                                                             \
    T *re = new T V1;                                                          \
    pdata.addValue( STRING( C ), STRING( T ), re );                            \
    return re;                                                                 \
  }

namespace graphsat {

const int DUMMY_ID = -1;

class InstanceFactory {
  SINGLETON( InstanceFactory );

public:
  ~InstanceFactory() { destroy(); }

private:
  regist_factory_method( DefaultCounterConstraint,
                         ( const vector<int> &pcons, const vector<int> &cons,
                           int erhs, COMP_OPERATOR eop ),
                         ( pcons, cons, erhs, eop ), CounterConstraint );

  regist_factory_method( OneParameterConstraint,
                         ( int parameter_id, COMP_OPERATOR op, int rhs ),
                         ( parameter_id, op, rhs ), CounterConstraint );

  regist_factory_method( CounterParameterConstraint,
                         ( int counter_id, int parameter_id, COMP_OPERATOR op,
                           int rhs ),
                         ( counter_id, parameter_id, op, rhs ),
                         CounterConstraint );

  regist_factory_method( TwoParameterConstraint,
                         ( int efirst_parameter_id, int esecond_parameter_id,
                           COMP_OPERATOR eop, int erhs ),
                         ( efirst_parameter_id, esecond_parameter_id, eop,
                           erhs ),
                         CounterConstraint );

  regist_factory_method( TwoCounterConstraint,
                         ( int counter_x, int counter_y, COMP_OPERATOR op,
                           int rhs, int pid ),
                         ( counter_x, counter_y, op, rhs, pid ),
                         CounterConstraint );

  regist_factory_method( OneCounterConstraint,
                         ( int counter_id, COMP_OPERATOR op, int rhs ),
                         ( counter_id, op, rhs ), CounterConstraint );

  // regist_factory_method( SimpleCounterAction, ( int counter_id, int value ),
  //                        ( counter_id, value ), CounterAction );

  // regist_factory_method( SimpleCounterPAction,
  //                        ( int counter_id, int eparameter_id ),
  //                        ( counter_id, eparameter_id ), CounterAction );

  // regist_factory_method( DefaultCAction,
  //                        ( vector<pair<int, vector<pair<int, int>>>> &
  //                          relations1 ),
  //                        ( relations1 ), CounterAction );

private:
  CounterConstraint *copy( CounterConstraint *other ) {
    CounterConstraint *re = other->copy();
    pdata.addValue( STRING( CounterConstraint ), STRING( CounterConstraint ),
                    re );
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

  friend CounterConstraint *copy( CounterConstraint *other );

  friend CounterAction *copy( const CounterAction *other );

  friend CounterParameterConstraint *
      createCounterParameterConstraint( int counter_id, int parameter_id,
                                        COMP_OPERATOR op, int rhs );

  friend void *createParameterConstraint( const int     parameter_id1,
                                          const int     parameter_id2,
                                          COMP_OPERATOR op, const int rhs );

  friend void *createCounterConstraint( const int counter_id1,
                                        const int counter_id2, COMP_OPERATOR op,
                                        const int rhs, int pid );

  // friend SimpleCounterAction *createSimpleCounterAction( int counter_id,
  //                                                        int value );

  // friend SimpleCounterPAction *createSimpleCounterPAction( int counter_id,
  //                                                          int eparameter_id );

  // friend DefaultCAction *createDefaultCAction(
  //     vector<pair<int, vector<pair<int, int>>>> &relations1 );

private:
  PointerData pdata;
};

CounterParameterConstraint *createCounterParameterConstraint( int counter_id,
                                                              int parameter_id,
                                                              COMP_OPERATOR op,
                                                              int rhs );

CounterConstraint *copy( CounterConstraint *other );

// SimpleCounterAction *createSimpleCounterAction( int counter_id, int value );

// SimpleCounterPAction *createSimpleCounterPAction( int counter_id,
//                                                   int eparameter_id );

// DefaultCAction *createDefaultCAction(
//     vector<pair<int, vector<pair<int, int>>>> &relations1 );

// CounterAction *copy( const CounterAction *other );

CounterParameterConstraint *negCounterParameterConstraint( int parameter_id,
                                                           int counter_id,
                                                           COMP_OPERATOR op,
                                                           int           rhs );

/**
 * @brief parameter_id1 - parameter_id2 op rhs
 *
 * @param parameter_id1
 * @param parameter_id2
 * @param rhs
 *
 * @return
 */
void *createParameterConstraint( const int parameter_id1,
                                 const int parameter_id2, COMP_OPERATOR op,
                                 const int rhs );

/**
 * @brief counter_id1 - counter_id2 op rhs
 *
 * @param counter_id1
 * @param counter_id2
 * @param op
 * @param rhs
 *
 * @return
 */
void *createCounterConstraint( const int counter_id1, const int counter_id2,
                               COMP_OPERATOR op, const int rhs, int pid = -10 );

} // namespace graphsat

#endif
