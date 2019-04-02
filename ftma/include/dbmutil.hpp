/**
 * @file   dbmutil.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Mar 29 10:54:12 2019
 *
 * @brief   dbm util class
 *
 *
 */

#ifndef DBM_UTIL_HPP
#define DBM_UTIL_HPP
#include<limits>
#define LTEQ_ZERO 1
namespace ftma{


/**
 *  Both have compare < and <=
 *
 */


template<typename C>
C getMAX_INT(const C c){
  return std::numeric_limits<C>::max( )/2-2;
}


template<typename C>
C add   (  const C x, const C y ) {
  static C MAX_INT=getMAX_INT(x);
  if( x>=MAX_INT || y>=MAX_INT){
    return MAX_INT;
  }
  return x+y-( ( x&1 )|( y&1 ));
}



template<typename C>
C getMatrixValue( C realRight, bool isStrct=true ){
    C right=realRight<<1;
    if( !isStrct ){
      right=right |1;
    }
    return right;
}

template<typename C>
std::string getComp(const C x ) {
  if( x&1 ){
    return "<=";
  }else{
    return "< ";
  }
      
}

template<typename C>
bool isStrict( const C  c){
  return  (c&1) == 0;
}
  
template<typename C>
C getRight( const C c ){
  return c>>1;
}
  
}

#endif
