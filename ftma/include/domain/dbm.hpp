
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

#define ADD( x, y )                                                            \
  ( ( x ) >= MAX_INT || ( y ) >= MAX_INT )                                     \
      ? MAX_INT                                                                \
      : ( ( x ) + ( y ) - ( ( (x) &1 ) | ( (y) &1 ) ) );

#define LOC( row, col ) ( row ) * ( clock_num ) + ( col )

/**
 *@param C the type of value of clock
 *@param add the operator of x+y
 *
 */

template <typename C> class DBMFactory {

public:
  DBMFactory( void ) {
    clock_num   = 0;
    matrix_size = 0;
    MAX_INT     = getMAX_INT( (C) 0 );

    distribution =
        std::uniform_int_distribution<int>( -MAX_INT / 4 + 1, MAX_INT / 4 );
  }

  DBMFactory( int n )
      : clock_num( n + 1 ) {
    matrix_size = clock_num * clock_num;
    MAX_INT     = getMAX_INT( (C) 0 );

    distribution =
        std::uniform_int_distribution<int>( -MAX_INT / 4 + 1, MAX_INT / 4 );
  }

  DBMFactory( int n, const vector<C> &oclockUppuerBound,
              const vector<ClockConstraint<C>> &odifferenceCons )
      : clock_num( n + 1 ) {
    matrix_size = clock_num * clock_num;

    MAX_INT = getMAX_INT( (C) 0 );

    distribution =
        std::uniform_int_distribution<int>( -MAX_INT / 4 + 1, MAX_INT / 4 );
    clock_uppuer_bound = oclockUppuerBound;
    difference_cons    = odifferenceCons;
  }

  ~DBMFactory() { clock_num = 0; }

  void setClockNum( int num ) {
    clock_num   = num + 1;
    matrix_size = clock_num * clock_num;
  }

  C *createDBM() const {
    C *dbm = new C[ matrix_size ];
    fill( dbm, dbm + matrix_size, LTEQ_ZERO ); // x-y<=0
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
    C *newD = new C[ matrix_size ];
    memcpy( newD, dbm, sizeof( C ) * matrix_size );
    return newD;
  }

  void destroyDBM( C *dbm ) const { delete[] dbm; }

  void init( C *dbm ) const {
    fill( dbm, dbm + matrix_size, LTEQ_ZERO ); // x-y<=0
  }
  C *randomDBM() {

    C *newD = new C[ matrix_size ]();
    for ( int i = 0; i < matrix_size; i++ ) {
      newD[ i ] = distribution( generator );
    }
    return newD;
  }

  C *randomFeasiableDBM() {
    int num_cons = sqrt( clock_num ) * clock_num + 1;
    C * dbm      = createDBM();
    while ( num_cons >= 0 ) {
      init( dbm ); // x-y<=0
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
    init( dbm ); // x-y<=0
    return dbm;
  }

  int getSize() const { return matrix_size; }

  std::string dump( const C *const dbm ) const {

    std::stringstream ss;

    for ( int i = 0; i < clock_num; i++ ) {
      ss << "[ ";
      for ( int j = 0; j < clock_num; j++ ) {
        ss << " (";
        C v = dbm[ LOC( i, j ) ];

        if ( isStrict<C>( v ) ) {
          ss << "<  ";
        } else {
          ss << "<= ";
        }
        ss.width( 4 );
        C right = getRight<C>( v );
        if ( right >= MAX_INT / 2 ) {
          ss << std::left << ( (char) 126 );
        } else {
          ss << std::left << (int) right;
        }

        ss << ") ";
      }
      ss << "]\n";
    }
    return ss.str();
  }

  uint32_t getHashValue( const C *const dbm ) const {
    return FastHash( (char *) dbm, sizeof( C ) * matrix_size );
  }

  bool equal( const C *lhs, const C *rhs ) const {
    return 0 == memcmp( lhs, rhs, matrix_size * sizeof( C ) );
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
        int row_index = LOC( i, 0 );
        for ( int j = 0; j < clock_num; j++ ) {
          C temp = ADD( dbm[ row_index + k ], dbm[ LOC( k, j ) ] );
          dbm[ row_index + j ] =
              dbm[ row_index + j ] < temp ? dbm[ row_index + j ] : temp;
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
  bool isConsistent( const C *const dbm ) const {
    return dbm[ 0 ] >= LTEQ_ZERO;
  }

  /**
   *
   *
   * @param C
   * @param other
   *
   * @return true if this is included by other lhs>= rhs
   */
  bool include( const C *const lhs, const C *const rhs ) const {

    for ( int i = 0; i < matrix_size; i++ ) {
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
      int k = LOC( i, 0 );
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
    return cons.matrix_value + dbm[ LOC( cons.y, cons.x ) ] > LTEQ_ZERO;
  }

  void upImpl( C *dbm ) const {
    for ( int i = 1; i < clock_num; i++ ) {
      dbm[ LOC( i, 0 ) ] = MAX_INT;
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
        int k    = LOC( j, i );
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

    if ( cons.matrix_value + newD[ LOC( cons.y, cons.x ) ] <= LTEQ_ZERO ) {
      newD[ 0 ] = getMatrixValue( -1, false );
    } else if ( cons.matrix_value < newD[ LOC( cons.x, cons.y ) ] ) {
      newD[ LOC( cons.x, cons.y ) ] = cons.matrix_value;
      for ( int i = 0; i < clock_num; i++ ) {
        for ( int j = 0; j < clock_num; j++ ) {
          C   temp  = ADD( newD[ LOC( i, cons.x ) ], newD[ LOC( cons.x, j ) ] );
          int k     = LOC( i, j );
          newD[ k ] = newD[ k ] < temp ? newD[ k ] : temp;

          C DandC   = ADD( newD[ LOC( i, cons.y ) ], newD[ LOC( cons.y, j ) ] );
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
      dbm[ LOC( x, i ) ] = MAX_INT;
      dbm[ LOC( i, x ) ] = dbm[ LOC( i, 0 ) ];
    }
    dbm[ LOC( x, x ) ] = LTEQ_ZERO;
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
    int xStart = LOC( x, 0 );

    for ( int i = 0; i < clock_num; i++ ) {
      dbm[ xStart + i ]  = ADD( postM, dbm[ i ] );
      dbm[ LOC( i, x ) ] = ADD( dbm[ LOC( i, 0 ) ], negM );
    }

    dbm[ LOC( x, x ) ] = LTEQ_ZERO;
  }

  /**
   * In forwards exploration this operation is used to set clocks to specific
   * values x:=m Can not modify value of D
   * @param x
   * @param m
   */
  C *reset( const C *const dbm, const int x, const C m ) const {
    C *newD = createDBM( dbm );
    resetImpl( newD, x, m );
    return newD;
  }

  void copyImpl( C *dbm, const int x, const int y ) const {
    for ( int i = 0; i < clock_num; i++ ) {
      dbm[ LOC( x, i ) ] = dbm[ LOC( y, i ) ];
      dbm[ LOC( i, x ) ] = dbm[ LOC( i, y ) ];
    }
    dbm[ LOC( x, x ) ] = dbm[ LOC( x, y ) ] = dbm[ LOC( y, x ) ] = LTEQ_ZERO;
  }
  /**
   * This is another operation used in forwards state space exporation.
   * Can not modify value of D
   * @param x
   * @param y
   */
  C *copy( const C *const dbm, const int x, const int y ) const {
    C *newD = createDBM( dbm );
    copyImpl( newD, x, y );
    return newD;
  }

  C *shiftImpl( C *dbm, const int x, const C m ) const {

    C postM = getMatrixValue( m, false );
    C negM  = getMatrixValue( -m, false );

    for ( int i = 0; i < clock_num; i++ ) {
      dbm[ LOC( x, i ) ] = ADD( dbm[ LOC( x, i ) ], postM );
      dbm[ LOC( i, x ) ] = ADD( dbm[ LOC( i, x ) ], negM );
    }

    dbm[ LOC( x, x ) ] = LTEQ_ZERO;
    int temp           = LOC( x, 0 );
    dbm[ temp ]        = dbm[ temp ] > LTEQ_ZERO ? dbm[ temp ] : LTEQ_ZERO;
    dbm[ x ]           = dbm[ x ] < LTEQ_ZERO ? dbm[ x ] : LTEQ_ZERO;
  }

  /**
   *@brief x-> x+m
   * The last reset operation is shifting a clock, i.e. adding or substracting
   * a clock with an integer value.
   *
   * @param x
   * @param m
   */
  C *shift( const C *const dbm, const int x, const C m ) const {
    C *newD = createDBM( dbm );
    shiftImpl( newD, x, dbm );
    return newD;
  }

  /**
   * For a timed automaton and safty prperty to be checked, that contain no
   * difference constraints. assert(maximum.matrix_size()==2*n) maximum[ i ]:=
   * <= k_i maximum[i+n]:= < -k_i
   * @param maximums maximums[i] is the maximum upper for x_i
   */
  void norm( C *dbm, const vector<C> &maximums ) const {
    bool modify = false;
    for ( int i = 0; i < clock_num; i++ ) {
      for ( int j = 0; j < clock_num; j++ ) {
        if ( dbm[ LOC( i, j ) ] < MAX_INT ) {
          if ( dbm[ LOC( i, j ) ] > maximums[ i ] ) {
            modify             = true;
            dbm[ LOC( i, j ) ] = MAX_INT;
          } else if ( dbm[ LOC( i, j ) ] < maximums[ j + clock_num ] ) {
            modify             = true;
            dbm[ LOC( i, j ) ] = maximums[ j + clock_num ];
          }
        }
      }
    }
    if ( modify ) {
      canonicalForm( dbm );
    }
  }

  /**
   * TODO: The difference bounds will adjust depend on source
   *
   * @param source
   * @param D
   * @param re
   */
  void norm( C *dbm, vector<C *> &re_vec ) const {
    if ( difference_cons.empty() ) {
      norm( dbm, clock_uppuer_bound );
      re_vec.push_back( dbm );
      return;
    }
    norm( dbm, clock_uppuer_bound, difference_cons, re_vec );
  }

  void norm( C *dbm ) const { norm( dbm, clock_uppuer_bound ); }

private:
  /**
   * number of clocks
   */
  int                                clock_num;
  int                                matrix_size; // clock_num*clock_num
  C                                  MAX_INT;
  std::default_random_engine         generator;
  std::uniform_int_distribution<int> distribution;
  vector<C>                          clock_uppuer_bound;

  vector<ClockConstraint<C>> difference_cons;

  bool contain( const vector<C *> &values, const C *const dbm ) const {

    for ( auto v : values ) {
      if ( equal( v, dbm ) ) {
        return true;
      }
    }
    return false;
  }

  /**
   * For automaton containing difference constraints in the guards, it is more
   * complicated and expensive to compute the normalized zones.
   * assert(maximums.matrix_size()==maximums*clock_num)
   * maximums[ i ]:= <= maximums_i
   * maximums[clock_num +n]:= < -maximums_i
   * @param dbm D will deen destroied before return
   * @param maximums maximums[i] is the maximum upper for x_i
   * @param G
   * @param re
   */
  void norm( C *dbm, const vector<C> &maximums,
             const vector<ClockConstraint<C>> &diff_cons,
             vector<C *> &                     re_vec ) const {

    assert( re_vec.empty() );

    vector<C *> split_domains;
    split( dbm, diff_cons, split_domains );

    for ( auto temp_dbm : split_domains ) {
      re_vec.push_back( corn_norm( temp_dbm, maximums, diff_cons ) );
    }
  }

  void split( C *dbm, const vector<ClockConstraint<C>> &diffCons,
              vector<C *> &re_vec ) const {

    assert( re_vec.empty() );
    vector<C *> wait_s;
    re_vec.push_back( dbm );

    for ( auto cs : diffCons ) {

      vector<bool> addToWaitS( re_vec.size(), false );

      for ( size_t i = 0; i < re_vec.size(); i++ ) {
        /**
         * split
         * (D and C) && (D and -C) satisfies then using C to split D
         * into two parts
         */
        if ( isSatisfied( re_vec[ i ], cs ) &&
             isSatisfied( re_vec[ i ], cs.neg() ) ) {

          C *D_and_C     = And( re_vec[ i ], cs );
          C *D_and_neg_C = And( re_vec[ i ], cs.neg() );
          if ( !contain( wait_s, D_and_C ) ) {
            wait_s.push_back( D_and_C );
          } else {
            destroyDBM( D_and_C );
          }
          if ( !contain( wait_s, D_and_neg_C ) ) {
            wait_s.push_back( D_and_neg_C );
          } else {
            destroyDBM( D_and_neg_C );
          }

        } else {
          if ( !contain( wait_s, re_vec[ i ] ) ) {
            wait_s.push_back( re_vec[ i ] );
            addToWaitS[ i ] = true;
          }
        }
      }
      re_vec.swap( wait_s );
      for ( size_t i = 0; i < wait_s.size(); i++ ) {
        if ( !addToWaitS[ i ] ) {
          destroyDBM( wait_s[ i ] );
        }
        wait_s.clear();
      }
    }
  }

  C *corn_norm( C *dbm, const vector<C> &maximums,
                const vector<ClockConstraint<C>> &difference_cons ) const {

    vector<ClockConstraint<C>> G_unsat;

    for ( size_t i = 0; i < difference_cons.size(); i++ ) {
      /**
       * If D and C does not satisiable then norm(D,k) and C does not
       * satisable
       *
       */

      if ( !isSatisfied( dbm, difference_cons[ i ] ) ) {
        G_unsat.push_back( difference_cons[ i ] );
      }
      /**
       * If D and neg(C) does not satisiable then norm(D,k) and C does not
       * satisable
       *
       */

      if ( !isSatisfied( dbm, difference_cons[ i ].neg() ) ) {
        G_unsat.push_back( difference_cons[ i ].neg() );
      }
    }

    norm( dbm, maximums );
    for ( auto cs : G_unsat ) {
      andImpl( dbm, cs.neg() );
    }
    return dbm;
  }
};

} // namespace graphsat

#endif
