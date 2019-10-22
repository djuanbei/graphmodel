#include <random>
#include <vector>

#include <gtest/gtest.h>

#include "discretestate.hpp"

#include "model/ta.hpp"
#include "problem/reachability.hpp"
#include "reachableset.hpp"

#include "domain/dbm.hpp"
#include "domain/dbmset.hpp"

using namespace graphsat;
using namespace std;

typedef ReachableSet<INT_TAS_t> R_t;

TEST( TA, UNREACH ) {
  vector<INT_TAS_t::T_t> es;
  vector<INT_TAS_t::L_t> ls;
  INT_TAS_t::L_t         L0( 0 );
  INT_TAS_t::L_t         L1( 1 );

  INT_TAS_t::T_t E00a( 0, 0 );
  pair<int, int> reset1( 2, 0 );
  E00a.addReset( reset1 );            // y-->0
  INT_TAS_t::CS_t cs1( 2, 0, LE, 2 ); // y<=2
  E00a += cs1;

  INT_TAS_t::T_t E00b( 0, 0 );
  pair<int, int> reset2( 1, 0 );
  E00b.addReset( reset2 );            // x-->0
  INT_TAS_t::CS_t cs2( 1, 0, LE, 2 ); // x<=2
  E00b += cs2;

  INT_TAS_t::T_t E01( 0, 1 );

  INT_TAS_t::CS_t cs3( 2, 0, LE, 2 ); // y<=2
  INT_TAS_t::CS_t cs4( 1, 0, GE, 4 ); // x>=4

  E01 += cs3;
  E01 += cs4;

  ls.push_back( L0 );
  ls.push_back( L1 );

  es.push_back( E00a );

  es.push_back( E00b );
  es.push_back( E01 );

  INT_TAS_t::TAT_t tmt1( ls, es, 0, 2 );
  Parameter        param;
  param.addParameterValue( 1 );
  INT_TAS_t::TA_t tma1( &tmt1, param );
  //  tma1.addOnePara();
  INT_TAS_t sys;

  sys += tma1;
  R_t               data( sys );
  Reachability<R_t> reacher( data );
  vector<int>       loc;
  loc.push_back( 1 );
  LocReachProperty prop( loc );

  EXPECT_TRUE( reacher.satisfy( &prop ) );
}
TEST( REACHSET, TERIMINAL ) {
  vector<INT_TAS_t::T_t> es;
  vector<INT_TAS_t::L_t> ls;

  INT_TAS_t::L_t start( 0 );

  INT_TAS_t::L_t  loop( 1 );
  INT_TAS_t::CS_t cs1( 1, 0, LE, 10 ); // x <= 10
  loop += cs1;

  INT_TAS_t::L_t end( 2 );

  INT_TAS_t::T_t start_loop( 0, 1 );

  pair<int, int> reset1( 1, 0 ); // x-->0
  pair<int, int> reset2( 2, 0 ); // y-->0
  start_loop.addReset( reset1 );
  start_loop.addReset( reset2 );

  INT_TAS_t::T_t  loop_loop( 1, 1 );
  INT_TAS_t::CS_t cs2( 1, 0, LE, 10 ); // x <= 10
  INT_TAS_t::CS_t cs3( 1, 0, GE, 10 ); // x>=10 === 0-x <= -10
  loop_loop += cs2;
  loop_loop += cs3;
  pair<int, int> reset3( 1, 0 ); // x-->0
  loop_loop.addReset( reset3 );

  INT_TAS_t::T_t  loop_end( 1, 2 );
  INT_TAS_t::CS_t cs4( 2, 0, GE, 20 ); // y>=20=== 0-y <= -20
  loop_end += cs4;
  loop_end.addReset( reset1 );
  loop_end.addReset( reset2 );
  ls.push_back( start );
  ls.push_back( loop );
  ls.push_back( end );

  es.push_back( start_loop );
  es.push_back( loop_loop );
  es.push_back( loop_end );
  INT_TAS_t::TAT_t tmt1( ls, es, 0, 2 );
  Parameter        param;
  INT_TAS_t::TA_t  tma1( &tmt1, param );
  INT_TAS_t        sys;
  sys += tma1;
  R_t data( sys );

  Reachability<R_t> reacher( data );

  Property prop;
  EXPECT_FALSE( reacher.satisfy( &prop ) );
}

TEST( REACHSET, FISHER ) {
  int                    n = 4;
  vector<INT_TAS_t::T_t> es;
  vector<INT_TAS_t::L_t> ls;
  int                    k = 2;

  INT_TAS_t::L_t A( 0 );

  INT_TAS_t::L_t  req( 1 );
  INT_TAS_t::CS_t cs1( 1, 0, LE, k ); // x <= k
  req += cs1;

  INT_TAS_t::L_t wait( 2 );

  INT_TAS_t::L_t cs( 3 );

  INT_TAS_t::T_t A_req( 0, 1 );

  pair<int, int>         p( 1, 0 ); // 1*id
  vector<pair<int, int>> ps1, ps2;
  ps2.push_back( p );

  void *ccs1 = createCounterConstraint( 0, DUMMY_ID, EQ, 0 );
  // CounterConstraintFactory::getInstance().createDefaultCounterConstraint(
  //     ps1, ps2, 0, EQ ); // 1*id==0
  A_req.addCounterCons( ccs1 );

  pair<int, int> reset1( 1, 0 ); // x-->0
  A_req.addReset( reset1 );

  INT_TAS_t::T_t  req_wait( 1, 2 );
  INT_TAS_t::CS_t cs2( 1, 0, LE, k ); // x <= k
  req_wait += cs2;

  pair<int, int> reset2( 1, 0 ); // x-->0
  req_wait.addReset( reset2 );

  vector<pair<int, vector<pair<int, int>>>> relations;
  pair<int, int>                            passin( 1, 0 ); // 1*pid
  vector<pair<int, int>>                    passins;
  passins.push_back( passin );
  pair<int, vector<pair<int, int>>> oneUpdate( 0, passins ); // id=1*pid
  relations.push_back( oneUpdate );
  CounterAction *caction =
      new CounterAction( ASSIGNMENT_ACTION, RHS_PARAMETER_T, 0, 0 );
  //      CounterActionFactory::getInstance().createDefaultCAction( relations );

  req_wait.addCounterAction( caction );

  INT_TAS_t::T_t wait_req( 2, 1 );

  pair<int, int> reset3( 1, 0 ); // x-->0
  wait_req.addReset( reset3 );
  wait_req.addCounterCons( ccs1 ); // id==0

  INT_TAS_t::T_t wait_cs( 2, 3 );

  vector<pair<int, int>> ps3, ps4;
  pair<int, int>         id_p( -1, 0 ); //-1*pid
  pair<int, int>         id_id( 1, 0 ); // 1*id
  ps3.push_back( id_p );
  ps4.push_back( id_id );
  void *ccs2 = createCounterConstraint( 0, DUMMY_ID, EQ, 0, 0 ); //  id==pid
  // DefaultCounterConstraint *ccs2 =
  //     CounterConstraintFactory::getInstance().createDefaultCounterConstraint(
  //         ps3, ps4, 0, EQ ); // id==pid
  wait_cs.addCounterCons( ccs2 );
  INT_TAS_t::CS_t cs3( 1, 0, GT, k ); // x> k
  wait_cs += cs3;

  INT_TAS_t::T_t cs_A( 3, 0 );

  vector<pair<int, vector<pair<int, int>>>> relations1;

  vector<pair<int, int>> passins1;

  pair<int, vector<pair<int, int>>> oneUpdate1( 0, passins1 ); // id=0
  relations1.push_back( oneUpdate1 );
  CounterAction *caction1 =
      new CounterAction( ASSIGNMENT_ACTION, RHS_CONSTANT_T, 0, 0 );
  // DefaultCAction *caction1 =
  //     CounterActionFactory::getInstance().createDefaultCAction(
  //         relations1 ); //( relations1 );
  cs_A.addCounterAction( caction1 );

  ls.push_back( A );
  ls.push_back( req );
  ls.push_back( wait );
  ls.push_back( cs );

  es.push_back( A_req );
  es.push_back( req_wait );
  es.push_back( wait_req );
  es.push_back( wait_cs );
  es.push_back( cs_A );
  typename INT_TAS_t::TAT_t tmt1( ls, es, 0, 1 );
  //  INT_TAS_t::TA_t tma1( ls, es, 0, 1 );
  //  tma1.addOnePara();
  INT_TAS_t sys;
  for ( int i = 1; i <= n; i++ ) {
    Parameter param;
    param.setCounterMap( 0, 0 ); // add relation between local id and global id
    param.addParameterValue( i );

    typename INT_TAS_t::TA_t tma1( &tmt1, param );
    sys += tma1;
    INT_TAS_t::TA_t tma2 = tma1;
    // tma2.addOnePara( i );
    // sys += tma2;
  }
  // INT_TAS_t   sys = n * tma1;
  Counter counter( 0, n - 1 );
  sys += counter;

  R_t data( sys );

  Reachability<R_t> reacher( data );
  FischerMutual     prop;

  EXPECT_FALSE( reacher.satisfy( &prop ) );
}

int main( int argc, char *argv[] ) {
  testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
