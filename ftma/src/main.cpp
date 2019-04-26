
/**
 * @file   main.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Mar 30 18:10:24 2019
 *
 * @brief  a test main
 *
 *
 */

#include "action.hpp"
#include "dbmutil.hpp"
#include "transition.hpp"
#include "linsimpcons.hpp"
#include "location.hpp"
#include "reachability.hpp"
#include "tma.hpp"

#include "dbm.hpp"
#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;
using namespace ftma;

typedef int C;

typedef Constraint<C>       CS;
typedef DBM<C, CS>          DBM_t;
typedef DBMset<C, DBM_t>      DSet;
typedef Location<C, CS>     L;
typedef Transition<C, CS, Action> E;

void example1( void ) {
    // x:1 y:2 z:3
    vector<E> es;
    vector<L> ls;
    L         S0, S1, S2, S3;
    E         e01, e12, e23;

    e01.source = 0;
    e01.target = 1;
    e01.reset.push_back( 3 ); // z -->0

    e12.source = 1;
    e12.target = 2;
    CS cs1( 0, 2, -2, true ); // 0-y < -2
    e12.cons.push_back( cs1 );
    e12.reset.push_back( 2 ); // y --> 0

    e23.source = 2;
    e23.target = 3;

    CS cs2( 1, 3, 1, true ); // x-z < 1
    CS cs3( 3, 2, 1, true ); // z-y < 1
    e23+=cs2;
    //e23.cons.push_back( cs2 );
    e23+=cs3;
    //    e23.cons.push_back( cs3 );

    ls.push_back( S0 );
    ls.push_back( S1 );
    ls.push_back( S2 );
    ls.push_back( S3 );

    es.push_back( e01 );
    es.push_back( e12 );
    es.push_back( e23 );
    TMA<L, E> tma1( ls, es, 0, 3 );

    reach<C, L, E> reacher( tma1 );

    // vector< dbmset<C, DBM > > reachSet;

    reacher.computeAllReachableSet();
}

void example2( void ) {
    vector<E> es;
    vector<L> ls;
    L         L0, L1;

    E E00a, E00b, E01;
    E00a.source = 0;
    E00a.target = 0;
    E00a.reset.push_back( 2 ); // y-->0
    CS cs1( 2, 0, 2, false );  // y<=2
    E00a.cons.push_back( cs1 );

    E00b.source = 0;
    E00b.target = 0;
    E00b.reset.push_back( 1 ); // x-->0
    CS cs2( 1, 0, 2, false );  // x<=2
    E00b+=cs2;
    //E00b.cons.push_back( cs2 );

    E01.source = 0;
    E01.target = 1;

    CS cs3( 2, 0, 2, false ); // y<=2
    CS cs4( 0, 1, -4, false ); //x>=4

    E01.cons.push_back( cs3 );
    E01.cons.push_back( cs4 );

    ls.push_back( L0 );
    ls.push_back( L1 );

    es.push_back( E00a );
    es.push_back( E00b );
    es.push_back( E01 );

    TMA<L, E> tma1( ls, es, 0, 2 );

    reach<C, L, E> reacher( tma1 );

    if(reacher.reachable( 1)){

      cout<<"right"<<endl;
    }else{
      cout<<"there is something wrong"<<endl;
    }

}

void example3( void){

  
}

int main( int argc, const char *argv[] ) {
    example1();
    example2();
  
  
    Constraint<C> cons( 1, 2, 2, false );

    cout << "constrain: " << cons << endl;

    cout << "negation constraint: " << cons.neg() << endl;
    // insert code here...
    DBM_t exampleDBM( 4 );
    C *                   D = exampleDBM.randomMatirx();
    cout << "matrix dump :\n" << exampleDBM.dump( D ) << endl;

    cout << "========================" << endl;
    exampleDBM.canonicalForm( D );
    cout << "matrix dump :\n" << exampleDBM.dump( D ) << endl;

    cout << "========================" << endl;
    exampleDBM.canonicalForm( D );
    cout << "matrix dump :\n" << exampleDBM.dump( D ) << endl;
    std::cout << "Hello, World!\n";

    C *D1 = exampleDBM.newMatrix();
    cout << "matrix dump :\n" << exampleDBM.dump( D1 ) << endl;
    C *D2 = exampleDBM.reset( D1, 1, (C) 10 );
    cout << "matrix dump :\n" << exampleDBM.dump( D2 ) << endl;

    // cout<<"constrain: "<<cons<<endl;

    // //C*  D3=exampleDBM.And( D2, cons.neg( ));
    C *D3 = exampleDBM.reset( D2, 2, (C) 10 );

    cout << "matrix dump :\n" << exampleDBM.dump( D3 ) << endl;
/*
    vector<L> locs;

    vector<E> es;

    tma<L, E> tma1( locs, es, 0, 3 );

    reach<C, L, E> RETMA( tma1 );
*/
    return 0;
}
