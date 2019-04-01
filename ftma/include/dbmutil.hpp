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

namespace ftma{
  
/**
 *  Both have compare < and <=
 *
 */
  
template<typename C>
struct dbmUTIL{
  const static C  LTEQ_ZERO=1;
  const static  C  MAX_INT=std::numeric_limits<C>::max( )/2-2;

};



template<typename C>
C add   (  const C x, const C y ) {
  if( dbmUTIL<C>::MAX_INT== x ||dbmUTIL<C>::MAX_INT==y){
    return dbmUTIL<C>::MAX_INT;
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
