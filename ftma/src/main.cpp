
/**
 * @file   main.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Mar 30 18:10:24 2019
 *
 * @brief  a test main
 *
 *
 */
//#define CHECK_MEMORY 1
//#define PRINT_STATE 1
#include <random>

#include "action/counteraction.h"
#include "constraint/clockdiffcons.hpp"
#include "location.hpp"
#include "model/ta.hpp"
#include "problem/reachability.hpp"
#include "transition.hpp"

#include "util/dbmutil.hpp"

#include "domain/dbm.hpp"
#include "domain/dbmset.hpp"
#include "io/uppaalmodelparser.h"
#include "reachableset.hpp"

#include "discretestate.hpp"

#include "util/datacompression.h"

#include <cstdint>
#include <iostream>
#include <limits>

using std::vector;
using namespace graphsat;

typedef ReachableSet<INT_TAS_t> R_t;

void example1( void ) {
  // x:1 y:2 z:3
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;
  typename INT_TAS_t::L_t         S0( 0 );
  typename INT_TAS_t::L_t         S1( 1 );
  typename INT_TAS_t::L_t         S2( 2 );
  typename INT_TAS_t::L_t         S3( 3 );

  typename INT_TAS_t::T_t e01( 0, 1 );
  pair<int, int>          rest1( 3, 0 );
  e01.addReset( rest1 );
  // e01.reset.push_back( 3 ); // z -->0
  typename INT_TAS_t::T_t e12( 1, 2 );

  typename INT_TAS_t::CS_t cs1( 0, 2, -2, true ); // 0-y < -2
  e12 += cs1;
  pair<int, int> rest2( 2, 0 );
  e12.addReset( rest2 ); // y --> 0

  typename INT_TAS_t::T_t e23( 2, 3 );

  typename INT_TAS_t::CS_t cs2( 1, 3, 1, true ); // x-z < 1
  typename INT_TAS_t::CS_t cs3( 3, 2, 1, true ); // z-y < 1
  e23 += cs2;
  // e23.cons.push_back( cs2 );
  e23 += cs3;
  //    e23.cons.push_back( cs3 );

  ls.push_back( S0 );
  ls.push_back( S1 );
  ls.push_back( S2 );
  ls.push_back( S3 );

  es.push_back( e01 );
  es.push_back( e12 );
  es.push_back( e23 );
  typename INT_TAS_t::TA_t tma1( ls, es, 0, 3 );

  INT_TAS_t sys;
  sys += tma1;
  R_t data( sys );

  Reachability<R_t> reacher( data );
  vector<int>       loc;
  loc.push_back( 3 );
  LocReachProperty prop( loc );

  reacher.satisfy( &prop );
  // vector< dbmset<C_t, DBM > > reachSet;

  // reacher.computeAllReachableSet();
}

void example50() {
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;
  typename INT_TAS_t::L_t         L0( 0 );
  typename INT_TAS_t::L_t         L1( 1 );

  typename INT_TAS_t::T_t E00a( 0, 0 );
  pair<int, int>          reset1( 2, 0 );
  E00a.addReset( reset1 );                        // y-->0
  typename INT_TAS_t::CS_t cs1( 2, 0, 2, false ); // y<=2
  E00a += cs1;

  typename INT_TAS_t::T_t E00b( 0, 0 );
  pair<int, int>          reset2( 1, 0 );
  E00b.addReset( reset2 );                        // x-->0
  typename INT_TAS_t::CS_t cs2( 1, 0, 2, false ); // x<=2
  E00b += cs2;

  typename INT_TAS_t::T_t E01( 0, 1 );

  typename INT_TAS_t::CS_t cs3( 2, 0, 2, false );  // y<=2
  typename INT_TAS_t::CS_t cs4( 0, 1, -4, false ); // x>=4

  E01 += cs3;
  E01 += cs4;

  ls.push_back( L0 );
  ls.push_back( L1 );

  es.push_back( E00a );

  es.push_back( E00b );
  es.push_back( E01 );

  typename INT_TAS_t::TA_t tma1( ls, es, 0, 2 );
  tma1.addOnePara();
  INT_TAS_t sys;

  sys += tma1;
  R_t               data( sys );
  Reachability<R_t> reacher( data );
  vector<int>       loc;

  loc.push_back( 1 );
  LocReachProperty prop( loc );

  reacher.satisfy( &prop );
}

void example2( void ) {
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;
  typename INT_TAS_t::L_t         L0( 0 );
  typename INT_TAS_t::L_t         L1( 1 );

  typename INT_TAS_t::T_t E00a( 0, 0 );
  pair<int, int>          reset1( 2, 0 );
  E00a.addReset( reset1 );                        // y-->0
  typename INT_TAS_t::CS_t cs1( 2, 0, 2, false ); // y<=2
  E00a += cs1;

  typename INT_TAS_t::T_t E00b( 0, 0 );
  pair<int, int>          reset2( 1, 0 );
  E00b.addReset( reset2 );                        // x-->0
  typename INT_TAS_t::CS_t cs2( 1, 0, 2, false ); // x<=2
  E00b += cs2;

  typename INT_TAS_t::T_t E01( 0, 1 );

  typename INT_TAS_t::CS_t cs3( 2, 0, 2, false );  // y<=2
  typename INT_TAS_t::CS_t cs4( 0, 1, -4, false ); // x>=4

  E01 += cs3;
  E01 += cs4;

  ls.push_back( L0 );
  ls.push_back( L1 );

  es.push_back( E00a );

  es.push_back( E00b );
  es.push_back( E01 );

  typename INT_TAS_t::TA_t tma1( ls, es, 0, 2 );
  tma1.addOnePara();
  INT_TAS_t sys;

  sys += tma1;
  R_t               data( sys );
  Reachability<R_t> reacher( data );
  Property          prop;

  if ( reacher.satisfy( &prop ) ) {

    cout << "right" << endl;
  } else {
    cout << "there is something wrong" << endl;
  }
}

void example3( void ) {}

std::default_random_engine         generator;
std::uniform_int_distribution<int> distribution( 0, 1000 );

INT_TAS_t::C_t *randV( int n, int s ) {

  INT_TAS_t::C_t *v = new INT_TAS_t::C_t[ n ];

  for ( int i = 0; i < n; i++ ) {
    v[ i ] = distribution( generator );
  }

  return v;
}

void example5( void ) {
  StateSet<INT_TAS_t::C_t> sets;
  vector<INT_TAS_t::C_t *> vecs;
  int                      n   = 20;
  int                      s   = 4;
  int                      num = 1000;
  for ( int i = 0; i < num; i++ ) {
    INT_TAS_t::C_t *temp = randV( n, s );
    if ( sets.add( temp ) ) {
      vecs.push_back( temp );
    } else {
      delete temp;
    }
  }
  for ( auto e : vecs ) {
    sets.contain( e );
  }
}

void example6() {

  vector<int *>   vecs;
  int             n = 10;
  DBMFactory<int> manager( n );
  DBMset<int>     sets( manager );
  int             num = 1000;

  for ( int i = 0; i < num; i++ ) {
    int *dbm = manager.randomFeasiableDBM();
    if ( sets.add( dbm ) ) {
      vecs.push_back( dbm );
    } else {
      manager.destroyDBM( dbm );
    }
  }
  for ( auto e : vecs ) {
    sets.contain( e );
  }
}

void fisher( int n = 2 ) {
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;
  int                             k = 2;

  typename INT_TAS_t::L_t A( 0 );

  typename INT_TAS_t::L_t  req( 1 );
  typename INT_TAS_t::CS_t cs1( 1, 0, k, false ); // x <= k
  req += cs1;

  typename INT_TAS_t::L_t wait( 2 );

  typename INT_TAS_t::L_t cs( 3 );

  typename INT_TAS_t::T_t A_req( 0, 1 );

  DiaFreeCounterConstraint *ccs1 =
      CounterConstraintFactory::getInstance().createDiaFreeCounterConstraint(
          0, EQ, 0 ); // id==0

  A_req.addCounterCons( ccs1 );

  pair<int, int> reset1( 1, 0 ); // x-->0
  A_req.addReset( reset1 );

  typename INT_TAS_t::T_t  req_wait( 1, 2 );
  typename INT_TAS_t::CS_t cs2( 1, 0, k, false ); // x <= k
  req_wait += cs2;

  pair<int, int> reset2( 1, 0 ); // x-->0
  req_wait.addReset( reset2 );

  SimpleCounterPAction *caction =
      CounterActionFactory::getInstance().createSimpleCounterPAction( 0, 0 );

  req_wait.addCounterAction( caction );

  typename INT_TAS_t::T_t wait_req( 2, 1 );

  pair<int, int> reset3( 1, 0 ); // x-->0
  wait_req.addReset( reset3 );
  wait_req.addCounterCons( ccs1 ); // id==0

  typename INT_TAS_t::T_t wait_cs( 2, 3 );

  DiaFreeCounterPConstraint *ccs2 =
      CounterConstraintFactory::getInstance().createDiaFreeCounterPConstraint(
          0, EQ, 0 ); // id==pid
  wait_cs.addCounterCons( ccs2 );
  typename INT_TAS_t::CS_t cs3( 0, 1, -k, true ); // x> k
  wait_cs += cs3;

  typename INT_TAS_t::T_t cs_A( 3, 0 );

  SimpleCounterAction *caction1 =
      CounterActionFactory::getInstance().createSimpleCounterAction(
          0, 0 ); //( relations1 );

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
  typename INT_TAS_t::TA_t tma1( ls, es, 0, 1 );
  // tma1.addOnePara();
  INT_TAS_t sys;
  for ( int i = 1; i <= n; i++ ) {
    typename INT_TAS_t::TA_t tma2 = tma1;
    tma2.addOnePara( i );
    sys += tma2;
  }
  //  INT_TAS_t   sys = n * tma1;
  Counter counter( 0, 100 );
  sys += counter;

  R_t data( sys );

  Reachability<R_t> reacher( data );
  FischerMutual     prop;

  if ( reacher.satisfy( &prop ) ) {
    cout << "There is something wrong" << endl;
  } else {
    cout << "fisher mutual exclusion property check right" << endl;
  }
  // reacher.computeAllReachableSet();
  int s = data.size();

  cout << "reach data size: " << data.size() << endl;
}
void fisher1() {
  UppaalParser parser( "example/fischer.xml" );
  INT_TAS_t    sys = parser.getSYS();
  R_t          data( sys );

  Reachability<R_t> reacher( data );
  FischerMutual     prop;
  //  if ( reacher.satisfy( &prop ) ) {
  //   cout << "There is something wrong" << endl;
  // } else {

  //   cout << "fisher mutual exclusion property check right" << endl;
  // }
  reacher.computeAllReachableSet();
  cout << "reach data size: " << data.size() << endl;
  //  reacher.computeAllReachableSet();
}
void testOP() {
  for ( int i = 0; i < 10; i++ ) {

    cout << "<  " << i - 5 << " " << getMatrixValue( i - 5, true ) << endl;
    cout << "<= " << i - 5 << " " << getMatrixValue( i - 5, false ) << endl
         << endl;
    ;
  }
}

void testcompression( ){
  const  int len=10;
  Compression<int> data( len);
  int low=-6;
  int up=10;
  for( int i=0; i< len; i++){
    data.setBound( i, low, up);
  }
  UINT cd[ len];
  int dd[ len];
  std::default_random_engine         generator;
  std::uniform_int_distribution<int> distribution( low, up-1 );

  int d[ len];
  for( int i=0; i< 10000; i++){
    for( int j=0; j< len; j++){
      d[ j]=distribution( generator );
    }
    data.encode( d, cd);
    data.decode( cd, dd  );
    for( int j=0; j< len; j++){
      assert( dd[ j]==d[ j]);
    }
  }
  
}

int main( int argc, const char *argv[] ) {
  //  testcompression( );
  //  return 0;
  //  testOP( );
  //  return 9;
  //  example2( );
  //  return 0;

  fisher( 2 );
  return 0;
  //  example5();
  //  return 0;
  //  State<int> s;
  //  cout<<"xml"<<endl;
  fisher1();

  return 0;

  example1();
  example2();
  return 0;
  typename INT_TAS_t::CS_t cons( 1, 2, 2, false );

  cout << "constrain: " << cons << endl;

  cout << "negation constraint: " << cons.neg() << endl;
  // insert code here...
  typename INT_TAS_t::DBMManager_t exampleDBM( 4 );
  typename INT_TAS_t::C_t *        D = exampleDBM.randomDBM();
  cout << "matrix dump :\n" << exampleDBM.dump( D ) << endl;

  cout << "========================" << endl;
  exampleDBM.canonicalForm( D );
  cout << "matrix dump :\n" << exampleDBM.dump( D ) << endl;

  cout << "========================" << endl;
  exampleDBM.canonicalForm( D );
  cout << "matrix dump :\n" << exampleDBM.dump( D ) << endl;
  std::cout << "Hello, World!\n";

  typename INT_TAS_t::C_t *D1 = exampleDBM.createDBM();
  cout << "matrix dump :\n" << exampleDBM.dump( D1 ) << endl;
  typename INT_TAS_t::C_t *D2 =
      exampleDBM.reset( D1, 1, (typename INT_TAS_t::C_t) 10 );
  cout << "matrix dump :\n" << exampleDBM.dump( D2 ) << endl;

  // cout<<"constrain: "<<cons<<endl;

  // //C_t*  D3=exampleDBM.And( D2, cons.neg( ));
  typename INT_TAS_t::C_t *D3 =
      exampleDBM.reset( D2, 2, (typename INT_TAS_t::C_t) 10 );

  cout << "matrix dump :\n" << exampleDBM.dump( D3 ) << endl;
  /*
    vector<L_t> locs;

    vector<T_t> es;

    tma<L_t, T_t> tma1( locs, es, 0, 3 );

    reach<C_t, L_t, T_t> RETMA( tma1 );
  */
  return 0;
}
