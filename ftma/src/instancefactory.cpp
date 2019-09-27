#include "util/instancefactory.h"
namespace graphsat {


DefaultCounterConstraint *
createDefaultCounterConstraint( const vector<int> &pcons,
                                const vector<int> &cons, int erhs,
                                COMP_OPERATOR eop ){
  
  return InstanceFactory::getInstance().createDefaultCounterConstraint(pcons, cons , erhs,  eop );
  
}
FreeCounterConstraint *
createFreeCounterConstraint( int p, COMP_OPERATOR op, int rhs ){
  return InstanceFactory::getInstance().createFreeCounterConstraint(  p,  op, rhs );
}


DiaFreeCounterPConstraint *
createDiaFreeCounterPConstraint( int c, COMP_OPERATOR o, int p ){
  return InstanceFactory::getInstance().createDiaFreeCounterPConstraint( c,  o, p );
}


DiaCounterConstraint *createDiaCounterConstraint( int x, int y,
                                                  COMP_OPERATOR op, int r ){
  return InstanceFactory::getInstance().createDiaCounterConstraint(  x,  y,
                                                                     op,  r );
}

DiaFreeCounterConstraint *
createDiaFreeCounterConstraint( int cid, COMP_OPERATOR op, int r ){
  return InstanceFactory::getInstance().createDiaFreeCounterConstraint( cid,  op, r );
}


CounterConstraint *copy( CounterConstraint *other ){
  return InstanceFactory::getInstance().copy(other );
}

SimpleCounterAction *createSimpleCounterAction( int cid, int v ){
  return InstanceFactory::getInstance().createSimpleCounterAction( cid,  v );
}


SimpleCounterPAction *createSimpleCounterPAction( int cid,
                                                  int eparameter_id ){
  return InstanceFactory::getInstance().createSimpleCounterPAction(  cid,
                                                                     eparameter_id );
}

DefaultCAction *createDefaultCAction(
    vector<pair<int, vector<pair<int, int>>>> &relations1 ){
  return InstanceFactory::getInstance().createDefaultCAction(
      relations1 );
}


CounterAction *copy( const CounterAction *other ){
  return InstanceFactory::getInstance().copy(other );
}

}
