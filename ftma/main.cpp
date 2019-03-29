//
//  main.cpp
//  ftma
//
//  Created by yun on 2019/3/29.
//  Copyright © 2019 yun. All rights reserved.
//
#include "include/linsimpcons.hpp"
#include "include/dbmutil.hpp"

#include "include/dbm.hpp"
#include <limits>
#include<iostream>

using namespace std;
using namespace ftmd;





int main(int argc, const char * argv[]) {
    // insert code here...
    dbm<int ,int, Constraint<int, int>,   dbmUTIL<int> > exampleDBM(10);
    std::cout << "Hello, World!\n";
    return 0;
}
