//
//  main.cpp
//  ftma
//
//  Created by yun on 2019/3/29.
//  Copyright © 2019 yun. All rights reserved.
//
#include "linsimpcons.hpp"
#include "dbmutil.hpp"

#include "dbm.hpp"
#include <limits>
#include<iostream>

using namespace std;
using namespace ftmd;


int main(int argc, const char * argv[]) {
  
  Constraint<int, int> cons(1, 2, 2, false);
  
  cout<<cons<<endl;
  
  cout<<cons.neg()<<endl;
  // insert code here...
  dbm<int ,int, Constraint<int, int>,   dbmUTIL<int> > exampleDBM(10);
  std::cout << "Hello, World!\n";
  return 0;

}
