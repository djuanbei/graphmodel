
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
typedef Transition<C, CS, Action> T;

void example1( void ) {
    // x:1 y:2 z:3
    vector<T> es;
    vector<L> ls;
    L         S0, S1, S2, S3;
    
    T e01( 0,1);
    e01+=3;
    // e01.reset.push_back( 3 ); // z -->0
    T e12( 1,2);

    CS cs1( 0, 2, -2, true ); // 0-y < -2
    e12+= cs1 ;
    e12+= 2 ; // y --> 0

    T e23( 2,3);


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
    TA<L, T> tma1( ls, es, 0, 3 );

    reach<C, L, T> reacher( tma1 );

    // vector< dbmset<C, DBM > > reachSet;

    reacher.computeAllReachableSet();
}

void example2( void ) {
    vector<T> es;
    vector<L> ls;
    L         L0, L1;

    T E00a( 0,0);

    E00a+= 2 ; // y-->0
    CS cs1( 2, 0, 2, false );  // y<=2
    E00a+= cs1 ;

    T E00b( 0,0);

    E00b+= 1; // x-->0
    CS cs2( 1, 0, 2, false );  // x<=2
    E00b+=cs2;

    T E01( 0,1);


    CS cs3( 2, 0, 2, false ); // y<=2
    CS cs4( 0, 1, -4, false ); //x>=4

    E01+= cs3 ;
    E01+= cs4 ;

    ls.push_back( L0 );
    ls.push_back( L1 );

    es.push_back( E00a );
    es.push_back( E00b );
    es.push_back( E01 );

    TA<L, T> tma1( ls, es, 0, 2 );

    reach<C, L, T> reacher( tma1 );

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

    vector<T> es;

    tma<L, T> tma1( locs, es, 0, 3 );

    reach<C, L, T> RETMA( tma1 );
*/
    return 0;
}
