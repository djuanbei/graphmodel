
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

#include "dbm.hpp"
#include <limits>
#include <cstdint>
#include<iostream>

using namespace std;
using namespace ftmd;


typedef int C;
typedef int V;

int main(int argc, const char * argv[]) {
  
  Constraint<V, C> cons(1, 2, 2, false);
  
  cout<<"constrain: "<<cons<<endl;
  
  cout<<"negation constraint: "<<cons.neg()<<endl;
  // insert code here...
  dbm<V ,C, Constraint<V, C>,   dbmUTIL<C> > exampleDBM(4);
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
  C*  D2=exampleDBM.up( D1);
  cout<<"matrix dump :\n"<<exampleDBM.dump( D2)<<endl;


  cout<<"constrain: "<<cons<<endl;
  C*  D3=exampleDBM.And( D2, cons.neg( ));
  cout<<"matrix dump :\n"<<exampleDBM.dump( D3)<<endl;
  
  return 0;

}
