/**
 * @file   dbm.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Mar 28 19:47:51 2019
 *
 * @brief  difference bound D
 * ref paper  Timed Automata: Semantics, Algorithms and Tools
 *
 * ftmd: fast timed automata model checker
 *
 */



#ifndef __DBM__HPP
#define __DBM__HPP
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <limits>
#include <set>

#include "fastHash.h"

namespace ftmd{
  using namespace std;
  
  /**
   *@param V the type of clock variable index
   *@param C the type of value of clock
   *@param add the operator of x+y
   *
   */
  
  template <typename V,  typename C, typename Cons, typename dbmUTIL >
  class dbm{
  private:
    /**
     * number of clocks
     */
    int n;
    int size; // n*n
    C MAX_INT;
    dbmUTIL add;
    inline  int loc(const int row, const int col ) const{
      return  row* n +col;
    }
    
  public:
    
    dbm( int nn ):n( nn+1 ){
      size=n*n;
      MAX_INT=numeric_limits<C>::max(  );
    }
    ~dbm(  ){
      n=0;
    }
    
    C* newMatrix(  ) const{
      C* D=new C[ size ](  );
      fill( D, D+size, add.LTEQ_ZERO ); // x-x<=0
      return D;
    }
    /**
     *  Create a new matrix and initial the values with D
     *
     * @param D
     *
     * @return
     */
    C * newMatrix(const C * D ) const{
      C* newD=new C[ size ](  );
      memcpy( newD, D, sizeof( C )*size );
      return  newD;
    }
    
    uint32_t getHashValue( const C* D ) const{
      return FastHash( ( char* )D,  sizeof( C )*size );
    }
    
    /**
     * recovery memory
     *
     * @param Cvec
     */
    void deleteVectorM( vector<C*> & Cvec  )const{
      for( typename vector<C*>::iterator it=Cvec.begin( ); it!= Cvec.end( ); it++  ){
        delete[  ] ( *it );
      }
      Cvec.clear( );
    }
    
    
    
    
    
    /**
     *  Floyds algorithm for computing all shortest path
     *
     */
    void canonicalForm( C * D ) const{
      for ( int k=0; k< n; k ++ ){
        for( int i=0; i< n; i++ ){
          for ( int j=0; j< n; j++ ){
            C temp=add( D[ loc( i,k ) ], D[ loc(k,j  ) ]   );
            D[ loc( i,j ) ]= D[ loc( i,j ) ]< temp? D[ loc( i,j ) ]: temp;
          }
        }
      }
    }
    
    /**
     *
     * precondition D is canonicalForm
     * @param D
     *
     * @return  true if dbm D is not empty,
     * false otherwise.
     */
    bool isConsistent(const  C* D ) const{
      return D[0] >= add.LTEQ_ZERO;
    }
    
    
    /**
     *
     *
     * @param V
     * @param C
     * @param other
     *
     * @return  true if this is included by other
     */
    bool isInclude (const C * lhs, const C *rhs) const{
      
      for( int i=0; i< size; i++ ){
        if( lhs[ i ]> rhs[ i ] ){
          return false;
        }
      }
      return true;
    }
    
    /**
     * D_{y,x} + cons.right ">=" 0
     }
     * This is to check D and x-y < ( <= ) m is non-empty
     * @param cons
     *
     * @return  true if  there is a  value in this domain which satisfies cons
     * false, otherwise.
     */
    bool isSatisfied(const C* D, const Cons &cons ) const{
      return add.isSat(cons.right, D[ loc( cons.y, cons.x )] );
    }
    
    /**
     * Can not modify value of D
     * compute strongest post condition
     * up( D )={u+d | u \in D, d\in R+   }
     *
     */
    C* up( const C* D ) const{
      C* newD=newMatrix( D );
      for(int i; i< n; i++ )  {
        newD[ loc( i, 0 ) ]=MAX_INT;
      }
      return newD;
    }
    
    
    /**
     * Can not modify value of D
     *  compute weakest precondition
     *
     */
    C* down(const C * D ) const{
      C *newD=newMatrix( D );
      for ( int i=1; i< n; i++ ){
        newD[i ] =add.LTEQ_ZERO;
        for( int j=1; j< n; j++ ){
          int k=loc( j,i );
          if( newD[ i ]>newD[ k ] ){
            newD[ i ]=newD[ k ];
          }
        }
      }
      return newD;
    }
    
    void andImpl( C * newD, const Cons & cons ) const{
      if (newD[loc( cons.y, cons.x )] + cons.right <0){
        newD[ 0 ]= add.getRight(-1, false  );
      }else if ( cons.right < newD[ loc(cons.x, cons.y) ] ){
        newD[ loc(cons.x, cons.y)  ]=cons.right;
        for( int i=0; i< n; i++ ){
          for( int j=0; j< n ; j++ ){
            C temp=add(newD[ loc( i, cons.x ) ], newD[ loc( cons.x, j ) ]);
            
            if (temp < newD[ loc( i, j ) ] ){
              newD[ loc( i, j ) ] = temp;
            }
            
            C temp1=add( newD[loc( i, cons.y ) ], newD[ loc( cons.y, j   ) ] );
            if( temp1  < newD[ loc( i,j ) ] ){
              newD[ loc( i,j ) ]=temp1;
            }
          }
        }
      }
      
    }
    /**
     *   Can not modify value of D
     * The most used operation in state-space exploration in conjunction
     * @param cons
     */
    C * And( const C *D,  const Cons & cons ) const{
      
      C *newD=newMatrix( D );
      andImpl( newD, cons );
      
      return newD;
    }
    
    
    
    /**
     * Can not modify value of D
     *  The free operation removes all constraints on a given clock x.
     *
     * @param x
     */
    C* free(const C * D, const V x ) const{
      C *newD=newMatrix( D );
      for( int i=0; i<n; i++ ){
        newD [ loc( x, i ) ]=MAX_INT;
        newD[ loc( i, x ) ]=newD[ loc( i,0 ) ];
      }
      newD[ loc( x, x ) ]=add.LTEQ_ZERO;
      return newD;
    }
    
    
    
    /**
     *  In forwards exploration this operation is used to set clocks to specific values x:=m
     *  Can not modify value of D
     * @param x
     * @param m
     */
    C* reset (const C* D, const V x,const  C m )  const {
      C *newD=newMatrix( D );
      C pm=add.getRight( m, false );
      C nm=add.getRight( -m, false );
      
      for( int i=0; i< n ;i++ ){
        newD[ loc( x, i ) ] =add(  pm, newD[i ] );
        
        newD[ loc( i,x ) ]=add(newD[ loc( i, 0 ) ], nm   );
        
      }
      return newD;
    }
    
    
    
    /**
     *  This is another operation used in forwards state space exporation.
     *  Can not modify value of D
     * @param x
     * @param y
     */
    C* copy(const C * D,  const V x, const V y ) const{
      C *newD=newMatrix( D );
      for( int i=0; i< n; i++ ){
        newD[ loc( x,i ) ]=newD[ loc(  y,i ) ];
        newD[ loc(  i,x ) ]=newD[ loc( i,y) ];
      }
      
      newD[ loc( x,x ) ]=add.LTEQ_ZERO;
      newD[ loc( x,y ) ]=add.LTEQ_ZERO;
      newD[ loc( y,x ) ]=add.LTEQ_ZERO;
      return newD;
    }
    
    
    
    /**
     *  The last reset operation is shifting a clock, i.e. adding or substracting a clock with an integer value.
     *
     * @param x
     * @param m
     */
    C* shift(const C * D, const V x, const C m ) const{
      C *newD=newMatrix( D );
      C pm=add.getRight( m, false );
      C nm=add.getRight( -m, false );
      
      
      for ( int i=0; i<  n; i++ ){
        newD[ loc( x,i ) ]=add( newD[ loc( x,i ), pm ] );
        newD[ loc( i, x ) ]=add( newD[ loc(  i, x ) ], nm );
      }
      
      newD[ loc( x,x ) ]=add.LTEQ_ZERO;
      int temp= loc( x,0 );
      newD[ temp ]=newD[ temp ]> add.LTEQ_ZERO? newD[ temp ]: add.LTEQ_ZERO;
      
      newD[ x ]=newD[ x ]< add.LTEQ_ZERO> newD[ x ]? : add.LTEQ_ZERO;
      
      return newD;
      
    }
    
    
    /**
     *  For a timed automaton and safty prperty to be  checked, that contain no difference constraints.
     * assert( k.size( )==2*n )
     * @param k  k[ i ] is the maximum upper for x_i
     */
    void norm(C * D, const C* k ) const {
      
      
      for ( int i=0; i< n; i++ ){
        for( int j=0; j< n; j++ ){
          if( D[ loc( i,j ) ]< MAX_INT){
            if(D[ loc( i,j ) ]> k[ i ] ){
              D[ loc( i,j ) ]=MAX_INT;
            }
            else if(  D[ loc( i, j ) ]< k[ j+n ] ){
              D[ loc( i, j ) ]=k[ j+n ];
            }
          }
        }
      }
      canonicalForm( D );
    }
    
    
    
    C*  corn_norm( C* D, const C* k, vector<Cons> & Gd ) const{
      
      vector<Cons> Gunsat;
      for( size_t i=0; i<  Gd.size( ); i++ ){
        if(! isSatisfied( D, Gd[ i ] ) ){
          Gunsat.push_back(Gd[ i ]  );
        }
        if( ! isSatisfied( D, Gd[ i ].neg(  ) ) ) {
          Gunsat.push_back(Gd[ i ].neg(  )  );;
        }
      }
      
      norm( D, k );
      for( typename vector<Cons>::iterator it= Gunsat.begin(  );  it!= Gunsat.end(  );  it++ ){
        andImpl( D, it->neg(  ) );
      }
      return D;
    }
   
    void split(const C* D,  const vector<Cons> &Gd,   vector<C*> &re  ){
      deleteVectorM( re );
      set<uint32_t> passed;
      
      vector<C*> waitS;
      re.push_back( newMatrix(D) );
      
      for( typename vector<Cons>::const_iterator cit=Gd.begin(  ); cit!=Gd.end( ); cit++ ){
        
        for ( typename vector<C*>::iterator dit =re.begin(  ); dit!= re.end(  ); dit++ ){
          
          if( isSatisfied(*dit, *cit ) && isSatisfied( *dit, (*cit).neg(  ) )  ){
            
            waitS.push_back(Add( *dit, *cit )  );
            waitS.push_back( Add( *dit, ( *cit ).neg(  ) ) );
            
          }else{
            waitS.push_back( *dit );
          }
        }
        re.swap( waitS );
        waitS.clear(  );
      }
    }
    
    
    /**
     *  For automaton containing difference constraints in the guards, it is more
     * complicated and expensive to compute the normalized zones.
     *
     * @param D
     * @param k
     * @param G
     * @param re
     */
    void norm(C * D,  const C* k,  const vector<Cons> &Gd, vector<C*> &re )const{
      deleteVectorM( re );
      
      vector<C*> temp;
      split( D, Gd, temp );
      
      for( typename vector<C*> ::iterator it=temp.begin( );  it!=temp.end(); it++ ){
        re.push_back( corn_norm( *it, k, Gd ) );
      }
      
    }
    
  };
  
  
}



#endif 
