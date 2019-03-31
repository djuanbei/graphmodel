
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
#include<random>

namespace ftma{

using namespace std;
  
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

    
    Constraint randConst(int num, V low, V up ) const{
      std::uniform_int_distribution<V>      distribution( 0, num);
      std::default_random_engine generator;
      V xx=distribution( generator);
      V yy=distribution( generator);
      while( xx==yy){
        yy=distribution( generator);
      }
      
      std::uniform_int_distribution<C>      distribution1( low, up);
      
      C vv=distribution1( generator);
      return Constraint( xx, yy, vv);
      
    }
    
    Constraint neg( void ) const{
      Constraint re( *this );
      re.neg_impl( );
      return re;
    }

    bool isSat( const Constraint<V,C> & cons )const{
      if( (cons.x==x) && ( cons.y==y ) ){
        return true;
      }else if(  (cons.x==y) && ( cons.y==x ) ){
        Constraint<V,C> negCons=cons.neg(  );

        return negCons.matrix_value< matrix_value;
      }
      assert( false );
      return true;
    }
    
    friend  std::ostream& operator << (  std::ostream & os,  const Constraint & cons ){
      if ( isStrict<C>(cons.matrix_value) ){
        os<<"x_"<<cons.x<<  " - "<< "x_"<<cons.y<<" < "<<getRight<C>(cons.matrix_value);
      }else{
        os<<"x_"<<cons.x<<  " - "<< "x_"<<cons.y<<" <= "<<getRight<C>(cons.matrix_value);
      }
      return os;
    }
  };
}
#endif

