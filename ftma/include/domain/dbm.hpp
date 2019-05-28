
/**
 * @file DBMFactory.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date Thu Mar 28 19:47:51 2019
 *
 * @brief difference bound D
 * ref paper Timed Automata: Semantics, Algorithms and Tools
 *
 * ftmd: fast timed automata model checker
 *
 */

#ifndef __DBM__HPP
#define __DBM__HPP
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

#include <random>
#include <utility>

#include "util/fastHash.h"

#include "constraint/clockdiffcons.hpp"
#include "util/dbmutil.hpp"

namespace graphsat {
using namespace std;

/**
 *@param C the type of value of clock
 *@param add the operator of x+y
 *
 */

template <typename C> class DBMFactory {

public:
  DBMFactory( void ) {
    clock_num = 0;
    size      = 0;
    MAX_INT   = getMAX_INT( (C) 0 );

    distribution =
        std::uniform_int_distribution<int>( -MAX_INT / 4 + 1, MAX_INT / 4 );
  }

  DBMFactory( int n )
      : clock_num( n + 1 ) {
    size    = clock_num * clock_num;
    MAX_INT = getMAX_INT( (C) 0 );

    distribution =
        std::uniform_int_distribution<int>( -MAX_INT / 4 + 1, MAX_INT / 4 );
  }

  DBMFactory( int n, const vector<C> &oclockUppuerBound,
              const vector<ClockConstraint<C>> &odifferenceCons )
      : clock_num( n + 1 ) {
    size = clock_num * clock_num;

    MAX_INT = getMAX_INT( (C) 0 );

    distribution =
        std::uniform_int_distribution<int>( -MAX_INT / 4 + 1, MAX_INT / 4 );
    clockUppuerBound = oclockUppuerBound;
    differenceCons   = odifferenceCons;
  }

  ~DBMFactory() { clock_num = 0; }

  void setClockNum( int num ) {
    clock_num = num + 1;
    size      = clock_num * clock_num;
  }

  C *createDBM() const {
    C *dbm = new C[ size ];
    fill( dbm, dbm + size, LTEQ_ZERO ); // x-x<=0
    return dbm;
  }

  /**
   * Create a new matrix and initial the values with D
   *
   * @param D
   *
   * @return
   */
  C *createDBM( const C *const dbm ) const {
    C *newD = new C[ size ];
    memcpy( newD, dbm, sizeof( C ) * size );
    return newD;
  }

  void destroyDBM( C *dbm ) const { delete[] dbm; }

  void init( C *dbm ) const {
    fill( dbm, dbm + size, LTEQ_ZERO ); // x-x<=0
  }
  C *randomDBM() {

    C *newD = new C[ size ]();
    for ( int i = 0; i < size; i++ ) {
      newD[ i ] = distribution( generator );
    }
    return newD;
  }

  C *randomFeasiableDBM() {
    int num_cons = sqrt( clock_num ) * clock_num + 1;
    C * dbm        = createDBM();
    while ( num_cons >= 0 ) {
      init( dbm);// x-y<=0
      upImpl( dbm );
      for ( int i = 0; i < sqrt( clock_num ); i++ ) {
        int x = abs( distribution( generator ) ) % ( clock_num );
        if ( x > 0 ) freeImpl( dbm, x );
      }
      for ( int i = 0; i < num_cons; i++ ) {
        int x = abs( distribution( generator ) ) % ( clock_num );
        int y = abs( distribution( generator ) ) % ( clock_num );
        if ( x != y ) {
          C                  rhs = distribution( generator );
          ClockConstraint<C> cs( x, y, rhs );
          andImpl( dbm, cs );
        }
        x = abs( distribution( generator ) ) % ( clock_num );
        if ( x > 0 ) freeImpl( dbm, x );
        freeImpl( dbm, x );
      }
      if ( isConsistent( dbm ) ) {
        return dbm;
      }
      num_cons -= 3;
    }
    init( dbm);//x-y<=0
    return dbm;
  }

  int getSize() const { return size; }

  std::string dump( const C *const dbm ) const {

    std::stringstream ss;

    for ( int i = 0; i < clock_num; i++ ) {
      ss << "[ ";
      for ( int j = 0; j < clock_num; j++ ) {
        ss << " (";
        C v =dbm[ loc( i, j ) ];

        if ( isStrict<C>( v ) ) {
          ss << "< ,";
        } else {
          ss << "<=,";
        }
        ss.width( 4 );
        if ( MAX_INT == getRight<C>( v ) ) {
          ss << std::left << ( (char) 126 );
        } else {
          ss << std::left << (int) getRight<C>( v );
        }

        ss << ") ";
      }
      ss << "]\n";
    }
    return ss.str();
  }

  uint32_t getHashValue( const C *const dbm ) const {
    return FastHash( (char *) dbm, sizeof( C ) * size );
  }

  bool MEqual( const C *lhs, const C *rhs ) const {
    return 0 == memcmp( lhs, rhs, size * sizeof( C ) );
  }

  /**
   * recovery memory
   *
   * @param Cvec
   */
  void deleteVectorM( vector<C *> &Cvec ) const {
    for ( auto d : Cvec ) {
      delete[] d;
    }
    Cvec.clear();
  }

  /**
   * Floyds algorithm for computing all shortest path
   * TODO improve efficient
   */
  void canonicalForm( C *dbm ) const {
    for ( int k = 0; k < clock_num; k++ ) {
      for ( int i = 0; i < clock_num; i++ ) {
        for ( int j = 0; j < clock_num; j++ ) {
          C temp           = add( dbm[ loc( i, k ) ], dbm[ loc( k, j ) ] );
          dbm[ loc( i, j ) ] = dbm[ loc( i, j ) ] < temp ? dbm[ loc( i, j ) ] : temp;
        }
      }
    }
  }

  /**
   *
   * precondition D is canonicalForm
   * @param D
   *
   * @return true if DBMFactory D is not empty,
   * false otherwise.
   */
  bool isConsistent( const C *const dbm ) const { return dbm[ 0 ] >= LTEQ_ZERO; }

  /**
   *
   *
   * @param C
   * @param other
   *
   * @return true if this is included by other lhs>= rhs
   */
  bool include( const C *const lhs, const C *const rhs ) const {

    for ( int i = 0; i < size; i++ ) {
      if ( lhs[ i ] < rhs[ i ] ) {
        return false;
      }
    }
    return true;
  }

  /**
   * If lhs contains  rhs, then getIncludeFeature(lhs ) >= getIncludeFeature(
   * rhs)
   *
   * @param D
   *
   * @return
   */

  DF_T getIncludeFeature( const C *const dbm ) const {
    DF_T re = 0;

    for ( int i = 0; i < clock_num; i++ ) {
      int k = loc( i, 0 );
      for ( int j = 0; j < clock_num; j++ ) {
        if ( i == j ) continue;
        re = ( 2 * re * MAX_INT + ( dbm[ k + j ] + MAX_INT ) );
      }
    }
    return re;
  }
  /**
   * D_{y,x} + cons.matrix_value ">=" 0
   } x-y < ( <= ) rhs and
   * y-x < ( <= ) d is feasiable if and only if -rhs< d

   * This is to check D and x-y < (<=) m is non-empty
   * @param cons
   *
   * @return true if there is a value in this domain which satisfies cons
   * false, otherwise.
   */
  bool isSatisfied( const C *const dbm, const ClockConstraint<C> &cons ) const {
    ClockConstraint<C> negCons = cons.neg();
    return negCons.matrix_value < dbm[ loc( cons.y, cons.x ) ];
  }

  void upImpl( C *dbm ) const {
    for ( int i = 1; i < clock_num; i++ ) {
      dbm[ loc( i, 0 ) ] = MAX_INT;
    }
  }

  /**
   * Can not modify value of D
   * compute strongest post condition
   * up(D)={u+d | u \in D, d\in R+ }
   *
   */
  C *up( const C *const dbm ) const {
    C *newD = createDBM( dbm );
    upImpl( newD );
    return newD;
  }

  void downImpl( C *dbm ) const {
    for ( int i = 1; i < clock_num; i++ ) {
      dbm[ i ] = LTEQ_ZERO;
      for ( int j = 1; j < clock_num; j++ ) {
        int k  = loc( j, i );
        dbm[ i ] = dbm[ i ] < dbm[ k ] ? dbm[ i ] : dbm[ k ];
      }
    }
  }

  /**
   * Can not modify value of D
   * compute weakest precondition
   *
   */
  C *down( const C *const dbm ) const {
    C *newD = createDBM( dbm );
    downImpl( newD );
    return newD;
  }

  void andImpl( C *newD, const ClockConstraint<C> &cons ) const {
    if ( newD[ 0 ] < LTEQ_ZERO ) {
      return;
    }

    ClockConstraint<C> negCons = cons.neg();

    if ( negCons.matrix_value >= newD[ loc( cons.y, cons.x ) ] ) {
      newD[ 0 ] = getMatrixValue( -1, false );
    } else if ( cons.matrix_value < newD[ loc( cons.x, cons.y ) ] ) {
      newD[ loc( cons.x, cons.y ) ] = cons.matrix_value;
      for ( int i = 0; i < clock_num; i++ ) {
        for ( int j = 0; j < clock_num; j++ ) {
          C   temp  = add( newD[ loc( i, cons.x ) ], newD[ loc( cons.x, j ) ] );
          int k     = loc( i, j );
          newD[ k ] = newD[ k ] < temp ? newD[ k ] : temp;

          C DandC   = add( newD[ loc( i, cons.y ) ], newD[ loc( cons.y, j ) ] );
          newD[ k ] = newD[ k ] < DandC ? newD[ k ] : DandC;
        }
      }
    }
  }

  /**
   * Can not modify value of D
   * The most used operation in state-space exploration in conjunction
   * @param cons
   */
  C *And( const C *const dbm, const ClockConstraint<C> &cons ) const {

    C *newD = createDBM( dbm );
    andImpl( newD, cons );
    return newD;
  }

  void freeImpl( C *dbm, const int x ) const {
    for ( int i = 0; i < clock_num; i++ ) {
      dbm[ loc( x, i ) ] = MAX_INT;
      dbm[ loc( i, x ) ] = dbm[ loc( i, 0 ) ];
    }
    dbm[ loc( x, x ) ] = LTEQ_ZERO;
  }

  /**
   * Can not modify value of D
   * The free operation removes all constraints on a given clock x.
   *
   * @param x
   */
  C *free( const C *const dbm, const int x ) const {
    C *newD = createDBM( dbm );
    freeImpl( newD, x );
    return newD;
  }

  void resetImpl( C *dbm, const int x, const C m ) const {
    // clock id start from 1
    assert( x > 0 );
    assert( m >= 0 );
    C   postM  = getMatrixValue( m, false );
    C   negM   = getMatrixValue( -m, false );
    int xStart = loc( x, 0 );

    for ( int i = 0; i < clock_num; i++ ) {
      dbm[ xStart + i ]  = add( postM, dbm[ i ] );
      dbm[ loc( i, x ) ] = add( dbm[ loc( i, 0 ) ], negM );
    }

    dbm[ loc( x, x ) ] = LTEQ_ZERO;
  }

  /**
   * In forwards exploration this operation is used to set clocks to specific
   * values x:=m Can not modify value of D
   * @param x
   * @param m
   */
  C *reset( const C *const D, const int x, const C m ) const {
    C *newD = createDBM( D );
    resetImpl( newD, x, m );
    return newD;
  }

  void copyImpl( C *D, const int x, const int y ) const {
    for ( int i = 0; i < clock_num; i++ ) {
      D[ loc( x, i ) ] = D[ loc( y, i ) ];
      D[ loc( i, x ) ] = D[ loc( i, y ) ];
    }
    D[ loc( x, x ) ] = D[ loc( x, y ) ] = D[ loc( y, x ) ] = LTEQ_ZERO;
  }
  /**
   * This is another operation used in forwards state space exporation.
   * Can not modify value of D
   * @param x
   * @param y
   */
  C *copy( const C *const D, const int x, const int y ) const {
    C *newD = createDBM( D );
    copyImpl( newD, x, y );
    return newD;
  }

  C *shiftImpl( C *D, const int x, const C m ) const {

    C postM = getMatrixValue( m, false );
    C negM  = getMatrixValue( -m, false );

    for ( int i = 0; i < clock_num; i++ ) {
      D[ loc( x, i ) ] = add( D[ loc( x, i ), postM ] );
      D[ loc( i, x ) ] = add( D[ loc( i, x ) ], negM );
    }

    D[ loc( x, x ) ] = LTEQ_ZERO;
    int temp         = loc( x, 0 );
    D[ temp ]        = D[ temp ] > LTEQ_ZERO ? D[ temp ] : LTEQ_ZERO;
    D[ x ]           = D[ x ] < LTEQ_ZERO ? D[ x ] : LTEQ_ZERO;
  }

  /**
   * The last reset operation is shifting a clock, i.e. adding or substracting
   * a clock with an integer value.
   *
   * @param x
   * @param m
   */
  C *shift( const C *const D, const int x, const C m ) const {
    C *newD = createDBM( D );
    shiftImpl( newD, x, m );
    return newD;
  }

  /**
   * For a timed automaton and safty prperty to be checked, that contain no
   * difference constraints. assert(k.size()==2*n) k[ i ]:= <= k_i k[i+n]:= <
   * -k_i
   * @param k k[i] is the maximum upper for x_i
   */
  void norm( C *D, const vector<C> &k ) const {

    for ( int i = 0; i < clock_num; i++ ) {
      for ( int j = 0; j < clock_num; j++ ) {
        if ( D[ loc( i, j ) ] < MAX_INT ) {
          if ( D[ loc( i, j ) ] > k[ i ] ) {
            D[ loc( i, j ) ] = MAX_INT;
          } else if ( D[ loc( i, j ) ] < k[ j + clock_num ] ) {
            D[ loc( i, j ) ] = k[ j + clock_num ];
          }
        }
      }
    }
    canonicalForm( D );
  }

  C *corn_norm( C *dbm, const vector<C> &uppers,
                const vector<ClockConstraint<C>> &differenceCons ) const {

    vector<ClockConstraint<C>> Gunsat;

    for ( size_t i = 0; i < differenceCons.size(); i++ ) {
      /**
       * If D and C does not satisiable then norm(D,k) and C does not
       * satisable
       *
       */

      if ( !isSatisfied( dbm, differenceCons[ i ] ) ) {
        Gunsat.push_back( differenceCons[ i ] );
      }
      /**
       * If D and neg(C) does not satisiable then norm(D,k) and C does not
       * satisable
       *
       */

      if ( !isSatisfied( dbm, differenceCons[ i ].neg() ) ) {
        Gunsat.push_back( differenceCons[ i ].neg() );
      }
    }

    norm( dbm, uppers );
    for ( auto cs : Gunsat ) {
      andImpl( dbm, cs.neg() );
    }
    return dbm;
  }

  void split( C *dbm, const vector<ClockConstraint<C>> &diffCons,
              vector<C *> &re_vec ) const {
    
    assert( re_vec.empty() );
    vector<C *> waitS;
    re_vec.push_back( dbm );

    for ( auto cs : diffCons ) {

      vector<bool> addToWaitS(re_vec.size(), false );

      for ( size_t i = 0; i < re_vec.size(); i++ ) {
        /**
         * split
         * (D and C) && (D and -C) satisfies then using C to split D
         * into two parts
         */
        if ( isSatisfied( re_vec[ i ], cs ) && isSatisfied( re_vec[ i ], cs.neg() ) ) {

          C *DandC    = And( re_vec[ i ], cs );
          C *DandNegC = And( re_vec[ i ], cs.neg() );
          if ( !contain( waitS, DandC ) ) {
            waitS.push_back( DandC );
          } else {
            destroyDBM( DandC );
          }
          if ( !contain( waitS, DandNegC ) ) {
            waitS.push_back( DandNegC );
          } else {
            destroyDBM( DandNegC );
          }

        } else {
          if ( !contain( waitS, re_vec[ i ] ) ) {
            waitS.push_back( re_vec[ i ] );
            addToWaitS[ i ] = true;
          }
        }
      }
      re_vec.swap( waitS );
      for ( size_t i = 0; i < waitS.size(); i++ ) {
        if ( !addToWaitS[ i ] ) {
          destroyDBM( waitS[ i ] );
        }
        waitS.clear();
      }
    }
  }
  /**
   * TODO: The difference bounds will adjust depend on source
   *
   * @param source
   * @param D
   * @param re
   */
  void norm( int source, C *dbm, vector<C *> &re_vec ) const {
    return norm( dbm, clockUppuerBound, differenceCons, re_vec );
  }
  /**
   * For automaton containing difference constraints in the guards, it is more
   * complicated and expensive to compute the normalized zones.
   * assert(k.size()==2*n)
   * k[ i ]:= <= k_i
   * k[i+n]:= < -k_i
   * @param k k[i] is the maximum upper for x_i
   * @param D D will deen destroied before return
   * @param G
   * @param re
   */
  void norm( C *dbm, const vector<C> &upper, const vector<ClockConstraint<C>> &diffCons,
             vector<C *> &re ) const {

    assert( re.empty() );

    vector<C *> splitDomains;
    split( dbm, diffCons, splitDomains );

    for ( auto d : splitDomains ) {
      re.push_back( corn_norm( d, upper, diffCons ) );
    }
  }

private:
  /**
   * number of clocks
   */
  int                                clock_num;
  int                                size; // n*n
  C                                  MAX_INT;
  std::default_random_engine         generator;
  std::uniform_int_distribution<int> distribution;
  vector<C>                          clockUppuerBound;

  vector<ClockConstraint<C>> differenceCons;

  inline int loc( const int row, const int col ) const {
    return row * clock_num + col;
  }

  bool contain( const vector<C *> &values, const C *const D ) const {

    for ( auto v : values ) {
      if ( MEqual( v, D ) ) {
        return true;
      }
    }
    return false;
  }
};

} // namespace graphsat

#endif
