
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
#include "dbmutil.hpp"

namespace ftmd{
  
  /**
   *  x -y < ( <= ) realRight
   *
   */
  template <typename C , typename V>
  class Constraint{
  private:
    
    void neg_impl( void ){
      C temp=x;
      x=y;
      y=temp;
      matrix_value= 1-matrix_value;
    }
    
  public:
    V x;
    V y;
    
    C matrix_value;
    
    Constraint(const V i, const V j, const C r, bool isStrctRef=true ){
      x=i;
      y=j;
      matrix_value=r<<1;
      if(!isStrctRef){
        matrix_value=matrix_value|1;
      }
    }
    
    Constraint neg( void ) const{
      Constraint re( *this );
      re.neg_impl( );
      return re;
    }
    
    
    friend  std::ostream& operator << (  std::ostream & os,  const Constraint & cons ){
      if ( isStrict<C>(cons.matrix_value) ){
        os<<"x_"<<cons.x<<  " - "<< "y_"<<cons.y<<" < "<<getRight<C>(cons.matrix_value);
      }else{
        os<<"x_"<<cons.x<<  " - "<< "y_"<<cons.y<<" <= "<<getRight<C>(cons.matrix_value);
      }
      return os;
    }
  };
}
#endif

