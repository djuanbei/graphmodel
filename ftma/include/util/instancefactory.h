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

namespace graphsat {
class InstanceFactory {
  SINGLETON( InstanceFactory );

public:
  ~InstanceFactory() { destroy(); }

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


  FreeCounterConstraint *
      createFreeCounterConstraint( int p, COMP_OPERATOR o, int rhs ) {
    FreeCounterConstraint *re = new FreeCounterConstraint( p, o, rhs );
    pdata.addValue( STRING( CounterConstraint ),
                    STRING( FreeCounterConstraint ), re );
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

  CounterConstraint *copy( CounterConstraint *other ) {
    CounterConstraint *re = other->copy();
    pdata.addValue( STRING( CounterConstraint ), STRING( CounterConstraint ),
                    re );
    return re;
  }

  SimpleCounterAction *createSimpleCounterAction( int cid, int v ) {
    SimpleCounterAction *re = new SimpleCounterAction( cid, v );
    pdata.addValue( STRING( CounterAction ), STRING( SimpleCounterAction ),
                    re );
    return re;
  }

  SimpleCounterPAction *createSimpleCounterPAction( int cid,
                                                    int eparameter_id ) {
    SimpleCounterPAction *re = new SimpleCounterPAction( cid, eparameter_id );
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

  void destroy() {

    deleteType( pdata, CounterConstraint );

    deleteType( pdata, CounterAction );

    pdata.clear();
  }

private:
  PointerData pdata;
};

} // namespace graphsat

#endif
