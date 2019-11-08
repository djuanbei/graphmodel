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
    pdata.addValue( 0, STRING( T ), re );                                      \
    return re;                                                                 \
  }

namespace graphsat {

const int        DUMMY_ID   = -1;
const static int CLASS_TYPE = 0;

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
  regist_factory_method( OneRefCounterConstraint,
                         ( int out_ref_id, COMP_OPERATOR opp, int right_side ),
                         ( out_ref_id, opp, right_side ), CounterConstraint );

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

private:
  CounterConstraint *copy( CounterConstraint *other ) {
    CounterConstraint *re = other->copy();
    pdata.addValue( CLASS_TYPE, STRING( CounterConstraint ), re );
    return re;
  }

  CounterAction *copy( const CounterAction *other ) {
    CounterAction *re = other->copy();
    pdata.addValue( CLASS_TYPE, STRING( CounterAction ), re );
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

  friend void *createConstraint( Argument first, Argument second,
                                 COMP_OPERATOR op, Argument rhs );

private:
  PointerData pdata;
};

CounterConstraint *copy( CounterConstraint *other );

CounterParameterConstraint *negCounterParameterConstraint( int parameter_id,
                                                           int counter_id,
                                                           COMP_OPERATOR op,
                                                           int           rhs );

/**
 * @brief first -second op rhs
 *
 * @param first
 * @param second
 * @param op
 * @param rhs
 *
 * @return
 */
void *createConstraint( Argument first, Argument second, COMP_OPERATOR op,
                        Argument rhs );

} // namespace graphsat

#endif
