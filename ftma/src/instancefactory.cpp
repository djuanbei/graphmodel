
#include "util/instancefactory.h"
namespace graphsat {

regist_to_factory( CounterParameterConstraint,
                   ( int counter_id, int parameter_id, COMP_OPERATOR op,
                     int rhs ),
                   ( counter_id, parameter_id, op, rhs ) );

CounterConstraint *copy( CounterConstraint *other ) {
  return InstanceFactory::getInstance().copy( other );
}

CounterAction *copy( const CounterAction *other ) {
  return InstanceFactory::getInstance().copy( other );
}

CounterParameterConstraint *negCounterParameterConstraint( int parameter_id,
                                                           int counter_id,
                                                           COMP_OPERATOR op,
                                                           int           rhs ) {
  COMP_OPERATOR nop = negation( op );
  return createCounterParameterConstraint( counter_id, parameter_id, nop,
                                           rhs * -1 );
}

void *createParameterConstraint( const int parameter_id1,
                                 const int parameter_id2, COMP_OPERATOR op,
                                 const int rhs ) {

  if ( parameter_id1 > -1 && parameter_id2 ) {
    return InstanceFactory::getInstance().createTwoParameterConstraint(
        parameter_id1, parameter_id2, op, rhs );
  }

  if ( parameter_id1 > -1 ) {
    return InstanceFactory::getInstance().createOneParameterConstraint(
        parameter_id1, op, rhs );
  }
  if ( parameter_id2 > -1 ) {
    COMP_OPERATOR nop = negation( op );
    return InstanceFactory::getInstance().createOneParameterConstraint(
        parameter_id2, nop, rhs * -1 );
  }
  assert( false );
  return NULL;
}

void *createCounterConstraint( const int counter_id1, const int counter_id2,
                               COMP_OPERATOR op, const int rhs, int pid ) {
  if ( counter_id1 > -1 && counter_id2 > -1 ) {
    return InstanceFactory::getInstance().createTwoCounterConstraint(
        counter_id1, counter_id2, op, rhs, pid );
  }

  if ( counter_id1 > -1 ) {
    return InstanceFactory::getInstance().createOneCounterConstraint(
        counter_id1, op, rhs );
  }
  if ( counter_id2 > -1 ) {
    COMP_OPERATOR nop = negation( op );
    return InstanceFactory::getInstance().createOneCounterConstraint(
        counter_id2, nop, rhs * -1 );
  }

  assert( false );
  return NULL;
}

} // namespace graphsat
