
/**
 * @file   main.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Mar 30 18:10:24 2019
 *
 * @brief  a test main
 *
 *
 */

#include <random>

#include "constraint/clockdiffcons.hpp"
#include "counteraction.h"
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

#include <cstdint>
#include <iostream>
#include <limits>

using std::vector;
using namespace graphsat;

void example1( void ) {
  // x:1 y:2 z:3
  vector<T_t> es;
  vector<L_t> ls;
  L_t         S0( 0 );
  L_t         S1( 1 );
  L_t         S2( 2 );
  L_t         S3( 3 );

  T_t            e01( 0, 1 );
  pair<int, int> rest1( 3, 0 );
  e01.addReset( rest1 );
  // e01.reset.push_back( 3 ); // z -->0
  T_t e12( 1, 2 );

  CS_t cs1( 0, 2, -2, true ); // 0-y < -2
  e12 += cs1;
  pair<int, int> rest2( 2, 0 );
  e12.addReset( rest2 ); // y --> 0

  T_t e23( 2, 3 );

  CS_t cs2( 1, 3, 1, true ); // x-z < 1
  CS_t cs3( 3, 2, 1, true ); // z-y < 1
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
  TA_t tma1( ls, es, 0, 3 );

  TAS_t sys;
  sys += tma1;
  R_t data( sys );

  Reachability<R_t> reacher( data );

  // vector< dbmset<C_t, DBM > > reachSet;

  reacher.computeAllReachableSet();
}

void example2( void ) {
  vector<T_t> es;
  vector<L_t> ls;
  L_t         L0( 0 );
  L_t         L1( 1 );

  T_t            E00a( 0, 0 );
  pair<int, int> reset1( 2, 0 );
  E00a.addReset( reset1 );    // y-->0
  CS_t cs1( 2, 0, 2, false ); // y<=2
  E00a += cs1;

  T_t            E00b( 0, 0 );
  pair<int, int> reset2( 1, 0 );
  E00b.addReset( reset2 );    // x-->0
  CS_t cs2( 1, 0, 2, false ); // x<=2
  E00b += cs2;

  T_t E01( 0, 1 );

  CS_t cs3( 2, 0, 2, false );  // y<=2
  CS_t cs4( 0, 1, -4, false ); // x>=4

  E01 += cs3;
  E01 += cs4;

  ls.push_back( L0 );
  ls.push_back( L1 );

  es.push_back( E00a );
  es.push_back( E00b );
  es.push_back( E01 );

  TA_t tma1( ls, es, 0, 2 );

  TAS_t sys;
  sys += tma1;
  R_t               data( sys );
  Reachability<R_t> reacher( data );
  vector<int>       loc;
  loc.push_back( 1 );
  if ( reacher.reachable( loc ) ) {

    cout << "right" << endl;
  } else {
    cout << "there is something wrong" << endl;
  }
}

void example3( void ) {}

std::default_random_engine         generator;
std::uniform_int_distribution<int> distribution( 0, 1000 );

NIntState *randV( int n, int s ) {
  NIntState *re = new NIntState( n, s );
  int *      v  = new int[ n ];

  for ( int i = 0; i < n; i++ ) {
    v[ i ] = distribution( generator );
  }
  re->setValue( v );
  delete[] v;
  return re;
}

void example5( void ) {
  StateSet<NIntState> sets;
  vector<NIntState *> vecs;
  int                 n   = 20;
  int                 s   = 4;
  int                 num = 1000;
  for ( int i = 0; i < num; i++ ) {
    NIntState *temp = randV( n, s );
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

int main( int argc, const char *argv[] ) {
  example5();
  //  return 0;
  //  State<int> s;

  //  UppaalParser parser( argv[ 1 ] );
  //  return 0;

  example1();
  example2();
  return 0;
  CS_t cons( 1, 2, 2, false );

  cout << "constrain: " << cons << endl;

  cout << "negation constraint: " << cons.neg() << endl;
  // insert code here...
  DBMManager_t exampleDBM( 4 );
  C_t *        D = exampleDBM.randomDBM();
  cout << "matrix dump :\n" << exampleDBM.dump( D ) << endl;

  cout << "========================" << endl;
  exampleDBM.canonicalForm( D );
  cout << "matrix dump :\n" << exampleDBM.dump( D ) << endl;

  cout << "========================" << endl;
  exampleDBM.canonicalForm( D );
  cout << "matrix dump :\n" << exampleDBM.dump( D ) << endl;
  std::cout << "Hello, World!\n";

  C_t *D1 = exampleDBM.createDBM();
  cout << "matrix dump :\n" << exampleDBM.dump( D1 ) << endl;
  C_t *D2 = exampleDBM.reset( D1, 1, (C_t) 10 );
  cout << "matrix dump :\n" << exampleDBM.dump( D2 ) << endl;

  // cout<<"constrain: "<<cons<<endl;

  // //C_t*  D3=exampleDBM.And( D2, cons.neg( ));
  C_t *D3 = exampleDBM.reset( D2, 2, (C_t) 10 );

  cout << "matrix dump :\n" << exampleDBM.dump( D3 ) << endl;
  /*
      vector<L_t> locs;

      vector<T_t> es;

      tma<L_t, T_t> tma1( locs, es, 0, 3 );

      reach<C_t, L_t, T_t> RETMA( tma1 );
  */
  return 0;
}
