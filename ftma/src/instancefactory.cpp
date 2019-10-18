#include "util/instancefactory.h"
namespace graphsat {

regist_to_factory(DefaultCounterConstraint,(const vector<int> &pcons,
                                    const vector<int> &cons, int erhs,
                                            COMP_OPERATOR eop ),  (
                                                pcons, cons, erhs, eop ) );
// DefaultCounterConstraint *
//     createDefaultCounterConstraint( const vector<int> &pcons,
//                                     const vector<int> &cons, int erhs,
//                                     COMP_OPERATOR eop ) {

//   return InstanceFactory::getInstance().createDefaultCounterConstraint(
//       pcons, cons, erhs, eop );
// }

FreeCounterConstraint *
    createFreeCounterConstraint( int parameter_id, COMP_OPERATOR op, int rhs ) {
  return InstanceFactory::getInstance().createFreeCounterConstraint(
      parameter_id, op, rhs );
}

CounterParameterConstraint *createCounterParameterConstraint( int counter_id,
                                                              int parameter_id,
                                                              COMP_OPERATOR op,
                                                              int rhs ) {
  return InstanceFactory::getInstance().createCounterParameterConstraint(
      counter_id, parameter_id, op, rhs );
}

DiaCounterConstraint *createDiaCounterConstraint( int x, int y,
                                                  COMP_OPERATOR op, int r ) {
  return InstanceFactory::getInstance().createDiaCounterConstraint( x, y, op,
                                                                    r );
}

DiaFreeCounterConstraint *
    createDiaFreeCounterConstraint( int cid, COMP_OPERATOR op, int r ) {
  return InstanceFactory::getInstance().createDiaFreeCounterConstraint( cid, op,
                                                                        r );
}

CounterConstraint *copy( CounterConstraint *other ) {
  return InstanceFactory::getInstance().copy( other );
}

SimpleCounterAction *createSimpleCounterAction( int cid, int v ) {
  return InstanceFactory::getInstance().createSimpleCounterAction( cid, v );
}

SimpleCounterPAction *createSimpleCounterPAction( int cid, int eparameter_id ) {
  return InstanceFactory::getInstance().createSimpleCounterPAction(
      cid, eparameter_id );
}

DefaultCAction *createDefaultCAction(
    vector<pair<int, vector<pair<int, int>>>> &relations1 ) {
  return InstanceFactory::getInstance().createDefaultCAction( relations1 );
}

CounterAction *copy( const CounterAction *other ) {
  return InstanceFactory::getInstance().copy( other );
}

} // namespace graphsat
