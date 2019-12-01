
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

#include "constraint/clockdiffcons.h"
#include "util/dbmutil.hpp"

namespace graphsat {
using namespace std;

#define ADD( x, y )                                                            \
  ( ( ( x ) >= MAX_INT || ( y ) >= MAX_INT )                                   \
        ? MAX_INT                                                              \
        : ( ( x ) + ( y ) - ( ( (x) &1 ) | ( (y) &1 ) ) ) )

#define LOC( row, col ) ( row ) * ( clock_num ) + ( col )


class DBMFactory {

public:
  DBMFactory( void ) {
    clock_num   = 0;
    matrix_size = 0;
    MAX_INT     = getMAX_INT( (int) 0 );

    distribution = std::uniform_int_distribution<int>( -100, 100 );
  }

  explicit DBMFactory(const int n );
  
 

  DBMFactory(const int n, const vector<int> &oclockUppuerBound,
             const vector<ClockConstraint> &odifferenceCons );
     

  ~DBMFactory() { clock_num = 0; }

  void setClockNum( int num ) {
    clock_num   = num + 1;
    matrix_size = clock_num * clock_num;
  }

  int *createDBM() const {
    int *dbm = new int[ matrix_size ];
    fill( dbm, dbm + matrix_size, LTEQ_ZERO ); // x-y<=0
    return dbm;
  }

  /**
   * Create a new matrix and initial the values with D
   *
   * @param dbm dbm matrix
   *
   * @return a copy dbm matrix
   */
  int *createDBM( const int *const dbm ) const {
    int *newD = new int[ matrix_size ];
    memcpy( newD, dbm, sizeof( int ) * matrix_size );
    return newD;
  }

  void destroyDBM( int *dbm ) const { delete[] dbm; }

  void init( int *dbm ) const {
    fill( dbm, dbm + matrix_size, LTEQ_ZERO ); // x-y<=0
  }

  int *randomDBM() {

    int *newD = new int[ matrix_size ]();
    for ( int i = 0; i < matrix_size; i++ ) {
      newD[ i ] = distribution( generator );
    }
    return newD;
  }

  int *randomFeasiableDBM();
  

  int getSize() const { return matrix_size; }

  ostream &dump( ostream &out, const int *const dbm ) const {

    for ( int i = 0; i < clock_num; i++ ) {
      out << "[ ";
      for ( int j = 0; j < clock_num; j++ ) {
        out << "(";
        int v = dbm[ LOC( i, j ) ];
        out << setw( 3 );
        if ( isStrict<int>( v ) ) {
          out << "< ";
        } else {
          out << "<=";
        }
        out << setw( 4 );
        int right = getRight( v );
        if ( right >= MAX_INT / 2 ) {
          out << std::left << ( (char) 126 );
        } else {
          out << std::left << (int) right;
        }

        out << ") ";
      }
      out << "]\n";
    }
    return out;
  }

  void dump( const int * const dbm ) const { dump( cout, dbm ); }

  uint32_t getHashValue( const int *const dbm ) const {
    return FastHash( (char *) dbm, sizeof( int ) * matrix_size );
  }

  bool equal( const int *lhs, const int *rhs ) const {
    return 0 == memcmp( lhs, rhs, matrix_size * sizeof( int ) );
  }

  /**
   * recovery memory
   *
   * @param Cvec the vector need to delete
   */
  void deleteVectorM( vector<int *> &Cvec ) const {
    for ( auto d : Cvec ) {
      delete[] d;
    }
    Cvec.clear();
  }

  /**
   * Floyds algorithm for computing all shortest path
   * TODO improve efficient
   */
  void canonicalForm( int *dbm ) const {
    for ( int k = 0; k < clock_num; k++ ) {
      for ( int i = 0; i < clock_num; i++ ) {
        int row_index = LOC( i, 0 );
        for ( int j = 0; j < clock_num; j++ ) {
          int temp = ADD( dbm[ row_index + k ], dbm[ LOC( k, j ) ] );
          dbm[ row_index + j ] =
              dbm[ row_index + j ] < temp ? dbm[ row_index + j ] : temp;
        }
      }
    }
  }

  /**
   *
   * precondition D is canonicalForm
   * @param dbm a dbm matrix
   *
   * @return true if DBMFactory D is not empty,
   * false otherwise.
   */
  bool isConsistent( const int *const dbm ) const {
    return dbm[ 0 ] >= LTEQ_ZERO;
  }

  /**
   *
   *
   * @brief  check whether lhs >= rhs, in other words whether lhs contains rhs
   *

   * @return true if this is included by other lhs>= rhs
   */
  bool include( const int *const lhs, const int *const rhs ) const {

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
   */

  DF_T getIncludeFeature( const int *const dbm ) const {
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
   * @param cons the constraints vector
   *
   * @return true if there is a value in this domain which satisfies cons
   * false, otherwise.
   */
  bool isSatisfied( const int *const dbm, const ClockConstraint &cons ) const {

    return ADD( cons.matrix_value, dbm[ LOC( cons.clock_y, cons.clock_x ) ] ) >=
           LTEQ_ZERO;
  }

  void upImpl( int *dbm ) const {
    int row_index = 0;
    for ( int i = 1; i < clock_num; i++ ) {
      row_index += clock_num;
      dbm[ row_index ] = MAX_INT;
    }
  }

  /**
   * Can not modify value of D
   * compute strongest post condition
   * up(D)={u+d | u \in D, d\in R+ }
   *
   */
  int *up( const int *const dbm ) const {
    int *newD = createDBM( dbm );
    upImpl( newD );
    return newD;
  }

  void downImpl( int *dbm ) const {
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
  int *down( const int *const dbm ) const {
    int *newD = createDBM( dbm );
    downImpl( newD );
    return newD;
  }

  void andImpl( int *newD, const ClockConstraint &cons ) const {
    if ( newD[ 0 ] < LTEQ_ZERO ) {
      return;
    }

    if ( ADD( cons.matrix_value, newD[ LOC( cons.clock_y, cons.clock_x ) ] ) <
         LTEQ_ZERO ) {
      newD[ 0 ] = getMatrixValue( -1, false );
    } else if ( cons.matrix_value <
                newD[ LOC( cons.clock_x, cons.clock_y ) ] ) {
      newD[ LOC( cons.clock_x, cons.clock_y ) ] = cons.matrix_value;
      for ( int i = 0; i < clock_num; i++ ) {
        for ( int j = 0; j < clock_num; j++ ) {
          int   temp1 = ADD( newD[ LOC( i, cons.clock_x ) ],
                         newD[ LOC( cons.clock_x, j ) ] );
          int k     = LOC( i, j );
          newD[ k ] = newD[ k ] < temp1 ? newD[ k ] : temp1;

          int temp2   = ADD( newD[ LOC( i, cons.clock_y ) ],
                         newD[ LOC( cons.clock_y, j ) ] );
          newD[ k ] = newD[ k ] < temp2 ? newD[ k ] : temp2;
        }
      }
    }
  }

  /**
   * Can not modify value of D
   * The most used operation in state-space exploration in conjunction
   */
  int *And( const int *const dbm, const ClockConstraint &cons ) const {

    int *newD = createDBM( dbm );
    andImpl( newD, cons );
    return newD;
  }

  void freeImpl( int *dbm, const int x ) const {
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
   */
  int *free( const int *const dbm, const int x ) const {
    int *newD = createDBM( dbm );
    freeImpl( newD, x );
    return newD;
  }

  void resetImpl( int *dbm, const int x, const int m ) const {
    // clock id start from 1
    assert( x > 0 );
    assert( m >= 0 );
    int   postM  = getMatrixValue( m, false );
    int   negM   = getMatrixValue( -m, false );
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
   * @param x the reset clock id
   * @param m rhe reset clock value
   */
  int *reset( const int *const dbm, const int x, const int m ) const {
    int *newD = createDBM( dbm );
    resetImpl( newD, x, m );
    return newD;
  }

  void copyImpl( int *dbm, const int x, const int y ) const {
    for ( int i = 0; i < clock_num; i++ ) {
      dbm[ LOC( x, i ) ] = dbm[ LOC( y, i ) ];
      dbm[ LOC( i, x ) ] = dbm[ LOC( i, y ) ];
    }
    dbm[ LOC( x, x ) ] = dbm[ LOC( x, y ) ] = dbm[ LOC( y, x ) ] = LTEQ_ZERO;
  }
  /**
   * This is another operation used in forwards state space exporation.
   * Can not modify value of D
   * @param x source clock id
   * @param y target clock id
   */
  int *copy( const int *const dbm, const int x, const int y ) const {
    int *newD = createDBM( dbm );
    copyImpl( newD, x, y );
    return newD;
  }

  int *shiftImpl( int *dbm, const int x, const int m ) const {

    int postM = getMatrixValue( m, false );
    int negM  = getMatrixValue( -m, false );

    for ( int i = 0; i < clock_num; i++ ) {
      dbm[ LOC( x, i ) ] = ADD( dbm[ LOC( x, i ) ], postM );
      dbm[ LOC( i, x ) ] = ADD( dbm[ LOC( i, x ) ], negM );
    }

    dbm[ LOC( x, x ) ] = LTEQ_ZERO;
    int temp           = LOC( x, 0 );
    dbm[ temp ]        = dbm[ temp ] > LTEQ_ZERO ? dbm[ temp ] : LTEQ_ZERO;
    dbm[ x ]           = dbm[ x ] < LTEQ_ZERO ? dbm[ x ] : LTEQ_ZERO;

    return dbm;
  }

  /**
   *@brief x-> x+m
   * The last reset operation is shifting a clock, i.e. adding or substracting
   * a clock with an integer value.
   *
   * @param x the shift clock id
   * @param m the reset clock value
   */
  int *shift( const int *const dbm, const int x, const int m ) const {
    int *newD = createDBM( dbm );
    shiftImpl( newD, x, m );
    return newD;
  }

  /**
   * For a timed automaton and safty prperty to be checked, that contain no
   * difference constraints. assert(maximum.matrix_size()==2*n) maximum[ i ]:=
   * <= k_i maximum[i+n]:= < -k_i
   * @param maximums maximums[i] is the maximum upper for x_i
   */
  void norm( int *dbm, const vector<int> &maximums ) const {
    bool modify = false;
    for ( int i = 0; i < clock_num; i++ ) {
      int row_index = LOC( i, 0 );
      for ( int j = 0; j < clock_num; j++ ) {
        int index = row_index + j;
        if ( dbm[ index ] < MAX_INT ) {
          if ( dbm[ index ] > maximums[ i ] ) {
            modify       = true;
            dbm[ index ] = MAX_INT;
          } else if ( dbm[ index ] < maximums[ j + clock_num ] ) {
            // modify       = true;
            dbm[ index ] = maximums[ j + clock_num ];
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
   * @param dbm the dbm matrix which want to norm
   * @param re_vec the return norm dbm vector
   */
  void norm( int *dbm, vector<int *> &re_vec ) const {
    if ( difference_cons.empty() ) {
      norm( dbm, clock_upper_bounds );
      re_vec.push_back( dbm );
      return;
    }
    norm( dbm, clock_upper_bounds, difference_cons, re_vec );
  }

  void norm( int *dbm ) const { norm( dbm, clock_upper_bounds ); }
  /**
   * @brief For compress the state data, we reduce the data
   *
   * @param dbm
   */
  void encode( int *dbm ) const {

    for ( int i = 0; i < clock_num; i++ ) {
      int row_index = LOC( i, 0 );
      if ( i == 0 ) {
        for ( int j = 0; j < clock_num; j++ ) {
          if ( dbm[ row_index + j ] > LTEQ_ZERO ) {
            dbm[ row_index + j ] = LTEQ_ZERO;
          }
        }
      } else {
        for ( int j = 0; j < clock_num; j++ ) {
          if ( j == i ) {
            dbm[ row_index + j ] = LTEQ_ZERO;
          } else if ( dbm[ row_index + j ] > clock_upper_bounds[ i ] ) {
            dbm[ row_index + j ] = clock_upper_bounds[ i ] + 1;
          }
        }
      }
    }
  }
  void decode( int *dbm ) const {

    for ( int i = 0; i < clock_num; i++ ) {
      int row_index = LOC( i, 0 );
      for ( int j = 0; j < clock_num; j++ ) {
        if ( dbm[ row_index + j ] > clock_upper_bounds[ i ] ) {
          dbm[ row_index + j ] = MAX_INT;
        }
      }
    }
  }

  ClockConstraint getCons( const int *const dbm, const int i,
                              const int j ) const {
    assert( i <= clock_num );
    assert( j <= clock_num );
    if ( isStrict( dbm[ LOC( i, j ) ] ) ) {
      return ClockConstraint( i, j, LT, getRight( dbm[ LOC( i, j ) ] ) );
    } else {
      return ClockConstraint( i, j, LE, getRight( dbm[ LOC( i, j ) ] ) );
    }
  }

private:
  /**
   * number of clocks
   */
  int                        clock_num;   // contain global clock which id is 0
  int                        matrix_size; // clock_num*clock_num
  int                          MAX_INT;
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution;
  vector<int>                          clock_upper_bounds;

  vector<ClockConstraint> difference_cons;

  bool contain( const vector<int *> &values, const int *const dbm ) const {

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
   * @param diff_cons All different constraint in model like x-y < c
   *@param re_vec the return dbm vector
   */
  void norm( int *dbm, const vector<int> &maximums,
             const vector<ClockConstraint> &diff_cons,
             vector<int *> &                     re_vec ) const {

    assert( re_vec.empty() );

    vector<int *> split_domains;
    split( dbm, diff_cons, split_domains );

    for ( auto temp_dbm : split_domains ) {
      re_vec.push_back( corn_norm( temp_dbm, maximums, diff_cons ) );
    }
  }

  void split( int *dbm, const vector<ClockConstraint> &diffCons,
              vector<int *> &re_vec ) const {

    assert( re_vec.empty() );
    vector<int *> wait_s;
    re_vec.push_back( dbm );

    for ( auto cs : diffCons ) {

      vector<bool> addToWaitS( re_vec.size(), false );

      for ( size_t i = 0; i < re_vec.size(); i++ ) {
        /**
         * split
         * (D and int) && (D and -int) satisfies then using int to split D
         * into two parts
         */
        if ( isSatisfied( re_vec[ i ], cs ) &&
             isSatisfied( re_vec[ i ], cs.neg() ) ) {

          int *D_and_C     = And( re_vec[ i ], cs );
          int *D_and_neg_C = And( re_vec[ i ], cs.neg() );
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

  int *corn_norm( int *dbm, const vector<int> &maximums,
                const vector<ClockConstraint> &difference_cons ) const {

    vector<ClockConstraint> G_unsat;

    for ( size_t i = 0; i < difference_cons.size(); i++ ) {
      /**
       * If D and int does not satisiable then norm(D,k) and int does not
       * satisable
       *
       */

      if ( !isSatisfied( dbm, difference_cons[ i ] ) ) {
        G_unsat.push_back( difference_cons[ i ] );
      }
      /**
       * If D and neg(int) does not satisiable then norm(D,k) and int does not
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
