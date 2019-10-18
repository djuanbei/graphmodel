#include "util/instancefactory.h"
namespace graphsat {

regist_to_factory( DefaultCounterConstraint,
                   ( const vector<int> &pcons, const vector<int> &cons,
                     int erhs, COMP_OPERATOR eop ),
                   ( pcons, cons, erhs, eop ) );

regist_to_factory( OneParamaterConstraint,
                   ( int parameter_id, COMP_OPERATOR op, int rhs ),
                   ( parameter_id, op, rhs ) );

regist_to_factory( CounterParameterConstraint,
                   ( int counter_id, int parameter_id, COMP_OPERATOR op,
                     int rhs ),
                   ( counter_id, parameter_id, op, rhs ) );

regist_to_factory( TwoCounterConstraint,
                   ( int counter_x, int counter_y, COMP_OPERATOR op, int rhs ),
                   ( counter_x, counter_y, op, rhs ) );

regist_to_factory( OneCounterConstraint,
                   ( int counter_id, COMP_OPERATOR op, int rhs ),
                   ( counter_id, op, rhs ) );

CounterConstraint *copy( CounterConstraint *other ) {
  return InstanceFactory::getInstance().copy( other );
}

regist_to_factory( SimpleCounterAction, ( int counter_id, int value ),
                   ( counter_id, value ) );

regist_to_factory( SimpleCounterPAction, ( int counter_id, int eparameter_id ),
                   ( counter_id, eparameter_id ) );

regist_to_factory( DefaultCAction,
                   ( vector<pair<int, vector<pair<int, int>>>> & relations1 ),
                   ( relations1 ) );

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

} // namespace graphsat
