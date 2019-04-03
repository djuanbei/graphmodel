
/**
 * @file   main.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Mar 30 18:10:24 2019
 * 
 * @brief  a test main 
 * 
 * 
 */

#include "linsimpcons.hpp"
#include "dbmutil.hpp"
#include "action.hpp"
#include "location.hpp"
#include "edge.hpp"
#include "tma.hpp"
#include "reachset.hpp"

#include "dbm.hpp"
#include <limits>
#include <cstdint>
#include<iostream>


using namespace std;
using namespace ftma;
    

typedef int C;

typedef Constraint<C> CS;
typedef dbm<C, CS > DBM;
typedef dbmset< C, DBM > DSet;
typedef location<C , CS > L;
typedef edge<C, CS, action> E;


void example1( void){
  // x:1 y:2 z:3
  vector<E> es;
  vector<L> ls;
  L S0, S1, S2, S3;
  E e01, e12, e23;
  
  e01.source=0;
  e01.target=1;
  e01.reset.push_back( 3);//z -->0

  e12.source=1;
  e12.target=2;
  CS cs1(0,2, -2, true ); // 0-y < -2
  e12.cons.push_back( cs1); 
  e12.reset.push_back( 2); //y --> 0

  e23.source=2;
  e23.target=3;

  CS cs2(1, 3, 1, true ); // x-z < 1
  CS cs3(3, 2, 1, true ); // z-y < 1

  e23.cons.push_back( cs2);
  e23.cons.push_back( cs3);

  ls.push_back( S0);
  ls.push_back( S1);
  ls.push_back( S2);
  ls.push_back( S3);

  es.push_back( e01);
  es.push_back( e12);
  es.push_back( e23);
  tma< L, E > TMA1( ls, es, 0, 3);

  reach<C,L,E> reacher( TMA1 );

  //vector< dbmset<C, DBM > > reachSet;
      
  reacher.reachableSet( );
  
  
}


int main(int argc, const char * argv[]) {
  example1( )  ;
  
  Constraint< C> cons(1, 2, 2, false);
  
  cout<<"constrain: "<<cons<<endl;
  
  cout<<"negation constraint: "<<cons.neg()<<endl;
  // insert code here...
  dbm<C, Constraint< C> > exampleDBM(4);
  C * D=exampleDBM.randomMatirx( );
  cout<<"matrix dump :\n"<<exampleDBM.dump( D)<<endl;

  cout<<"========================"<<endl;
  exampleDBM.canonicalForm( D);
  cout<<"matrix dump :\n"<<exampleDBM.dump( D)<<endl;


  cout<<"========================"<<endl;
  exampleDBM.canonicalForm( D);
  cout<<"matrix dump :\n"<<exampleDBM.dump( D)<<endl;
  std::cout << "Hello, World!\n";

  C * D1=exampleDBM.newMatrix( );
  cout<<"matrix dump :\n"<<exampleDBM.dump( D1)<<endl;
  C*  D2=exampleDBM.reset( D1, 1, ( C)10);
  cout<<"matrix dump :\n"<<exampleDBM.dump( D2)<<endl;


  // cout<<"constrain: "<<cons<<endl;
  
  // //C*  D3=exampleDBM.And( D2, cons.neg( ));
  C*  D3=exampleDBM.reset( D2, 2, (C)10);
    
  cout<<"matrix dump :\n"<<exampleDBM.dump( D3)<<endl;

  vector<L > locs;
  
  vector<E > es;
  
  tma< L, E > TMA1( locs, es, 0, 3);
  
  reach<C, L, E > RETMA(TMA1);
  
  
  return 0;

}
