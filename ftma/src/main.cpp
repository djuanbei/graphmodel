
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
#include<iostream>

using namespace std;
using namespace ftmd;


int main(int argc, const char * argv[]) {
  
  Constraint<int, int> cons(1, 2, 2, false);
  
  cout<<"constrain: "<<cons<<endl;
  
  cout<<"negation constraint: "<<cons.neg()<<endl;
  // insert code here...
  dbm<int ,int, Constraint<int, int>,   dbmUTIL<int> > exampleDBM(10);
  std::cout << "Hello, World!\n";
  return 0;

}
