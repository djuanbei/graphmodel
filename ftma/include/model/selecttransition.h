/**
 * @file   selecttransition.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec  6 11:27:50 2019
 * 
 * @brief  select transion
 * 
 * 
 */
#ifndef SELECT_TRANISION_H
#define SELECT_TRANISION_H

#include "model/transition.h"
#include<string>
namespace graphsat{
using std::string;

class SelectTransition:public Transition{

  void setSelectVar(const string &n ){
    select_var=n;
  }
  void setSelectCollect( const string &c){
    select_collect=c;
  }
 private:
  string select_var;
  string select_collect;
  
};

}

#endif
