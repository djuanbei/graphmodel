
#include "util/instancefactory.h"
#include <cassert>

namespace graphsat {

CounterConstraint *copy( CounterConstraint *other ) {
  return InstanceFactory::getInstance().copy( other );
}

CounterAction *copy( const CounterAction *other ) {
  return InstanceFactory::getInstance().copy( other );
}

void *createConstraint( Argument first, Argument second, COMP_OPERATOR op,
                        Argument rhs ) {

  if ( first.type == TEMPLATE_VAR_ARG && second.type == TEMPLATE_VAR_ARG &&
       rhs.type == CONST_ARG ) {
    return InstanceFactory::getInstance().createTwoCounterConstraint(
        first.value, second.value, op, rhs.value, 0 );
  }

  if ( first.type == PARAMETER_ARG && second.type == PARAMETER_ARG &&
       rhs.type == CONST_ARG ) {
    return InstanceFactory::getInstance().createTwoParameterConstraint(
        first.value, second.value, op, rhs.value );
  }

  if ( first.type == TEMPLATE_VAR_ARG && second.type == PARAMETER_ARG &&
       rhs.type == CONST_ARG ) {
    return InstanceFactory::getInstance().createCounterParameterConstraint(
        first.value, second.value, op, rhs.value );
  }
  if ( first.type == PARAMETER_ARG && second.type == TEMPLATE_VAR_ARG &&
       rhs.type == CONST_ARG ) {
    COMP_OPERATOR nop = negation( op );
    return InstanceFactory::getInstance().createCounterParameterConstraint(
        second.value, first.value, nop, rhs.value * -1 );
  }
  if ( first.type == TEMPLATE_VAR_ARG && second.type == EMPTY_ARG &&
       rhs.type == CONST_ARG ) {
    return InstanceFactory::getInstance().createOneCounterConstraint(
        first.value, op, rhs.value );
  }

  if ( second.type == TEMPLATE_VAR_ARG && first.type == EMPTY_ARG &&
       rhs.type == CONST_ARG ) {
    COMP_OPERATOR nop = negation( op );
    return InstanceFactory::getInstance().createOneCounterConstraint(
        second.value, nop, -1 * rhs.value );
  }
  if ( first.type == REF_PARAMETER_ARG && second.type == EMPTY_ARG &&
       rhs.type == CONST_ARG ) {
    return InstanceFactory::getInstance().createOneRefCounterConstraint(
        first.value, op, rhs.value );
  }
  return nullptr;

  //  assert( false );
}

} // namespace graphsat
