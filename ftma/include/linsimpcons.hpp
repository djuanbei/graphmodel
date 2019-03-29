
/**
 * @file   linsimpcons.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Mar 29 10:47:56 2019
 * 
 * @brief  linear simple constraint such x-y <(  <= ) right
 * 
 * 
 */
#ifndef LIN_SIMP_CONS_HPP
#define LIN_SIMP_CONS_HPP
#include<iostream>

namespace ftmd{

/** 
 *  x -y < ( <= ) realRight
 * 
 */
template <typename C , typename V>
struct Constraint{
  V x;
  V y;
  bool isStrct;
  C right;
  C rhs;
  Constraint(const  V i, const V j, const C r, const C rh, bool isStrctRef=true ){
    x=i;
    y=j;
    isStrct=isStrctRef;
    right=r;
    rhs=rh;
  }

  friend  std::ostream& operator << (  std::ostream & os,  const Constraint & cons ){
    if ( cons.isStrct ){
      os<<"x_"<<cons.x<<  " - "<< "y_"<<cons.y<<" < "<<cons.right;
    }else{
      os<<"x_"<<cons.x<<  " - "<< "y_"<<cons.y<<" <= "<<cons.right;
    }
    return os;
  }
};
}
#endif

