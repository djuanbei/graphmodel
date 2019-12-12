/**
 * @file   function.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Dec 12 21:06:18 2019
 * 
 * @brief  The function data structure in model
 * 
 * 
 */
#ifndef __FUNCTION_H
#define __FUNCTION_H

#include <vector>
#include <string>

namespace graphsat{

using namespace std;

class Function{
  
 public:
  virtual  int operator( ) (int * , ...)=0;
  virtual vector<string> getKeys( )const;
  virtual int &operator[ ] ( const string & key );
};

}


#endif

