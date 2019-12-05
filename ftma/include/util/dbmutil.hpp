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
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "typedef.h"

#include "macrodef.h"

namespace graphsat {
using std::map;
using std::string;
using std::vector;

template <typename T>
bool executeOp( const T &lhs, COMP_OPERATOR op, const T &rhs ) {

  switch ( op ) {
  case EQ:
    return lhs == rhs;
  case LE:
    return lhs <= rhs;
  case LT:
    return lhs < rhs;
  case GE:
    return lhs >= rhs;
  case GT:
    return lhs > rhs;
  case NE:
    return lhs != rhs;
  default:
    return false;
  }
}
string getOpStr( COMP_OPERATOR op );

COMP_OPERATOR negation( COMP_OPERATOR op );

enum TYPE_T {
  TYPE_TYPE( INT_T ),
  TYPE_TYPE( CLOCK_T ),

  TYPE_TYPE( CHAN_T ),
  TYPE_TYPE( URGENT_CHAN_T ),
  TYPE_TYPE( BROADCAST_CHAN_T ),

  SYSTEM_T,
  TEMPLATE_T,
  AUTOMATA_T,
  LOCATION_T,
  FORMAL_PARAMETER_T,

  CLOCK_CS_T,
  INT_CS_T,
  INT_UPDATE_T,
  RESET_T,
  SELF_DEF_T,
  CHAN_ACTION_T,
  REF_CHAN_ACTION_T,
  NO_T

};

string getTypeStr( TYPE_T type );

TYPE_T baseType( TYPE_T type );

// TYPE_T getRef( TYPE_T type );

int fromPidToChanId( int id );

int chanIdToFromPid( int id );

bool isRefChan( TYPE_T type );

TYPE_T get_formal_paramter_type( TYPE_T type );

bool isRefType( const TYPE_T type );

/**
 *  Both have compare < and <=
 *
 */
template <typename C> inline C getMAX_INT() {
  return std::numeric_limits<C>::max() / 2 - 2;
}

/**
 * <  c then the MatrixValue is 2*c+1
 * <= c then the MatrixValue is 2*c
 *
 */

template <typename C>
inline C getMatrixValue( C realRight, bool isStrct = true ) {
  C right = realRight * 2;
  if ( !isStrct ) {
    right = right | 1;
  }
  return right;
}

template <typename C> inline bool isStrict( const C c ) {
  return ( c & 1 ) == 0;
}

template <typename C> std::string getComp( const C x ) {
  if ( x & 1 ) {
    return "<=";
  } else {
    return "< ";
  }
}

template <typename C> inline C getRight( const C c ) { return c >> 1; }

template <typename C> inline C negMatrixValue( C matrix_value ) {
  bool strict = isStrict( matrix_value );

  return getMatrixValue( -getRight( matrix_value ), strict );
}

template <class T> inline bool CAS( T *ptr, T oldv, T newv ) {
  if ( sizeof( T ) == 1 ) {
    return __sync_bool_compare_and_swap( (bool *) ptr, *( (bool *) &oldv ),
                                         *( (bool *) &newv ) );
  } else if ( sizeof( T ) == 4 ) {
    return __sync_bool_compare_and_swap( (int *) ptr, *( (int *) &oldv ),
                                         *( (int *) &newv ) );
  } else if ( sizeof( T ) == 8 ) {
    return __sync_bool_compare_and_swap( (long *) ptr, *( (long *) &oldv ),
                                         *( (long *) &newv ) );
  } else {
    std::cout << "CAS bad length : " << sizeof( T ) << std::endl;
    abort();
  }
}

template <class T> inline bool writeMin( T *a, T b ) {
  T    c;
  bool r = 0;
  do
    c = *a;
  while ( c > b && !( r = CAS( a, c, b ) ) );
  return r;
}

template <class T> inline void writeAdd( T *a, T b ) {
  volatile T newV, oldV;
  do {
    oldV = *a;
    newV = oldV + b;
  } while ( !CAS( a, oldV, newV ) );
}

vector<string> splitStr( const string &stringToBeSplitted,
                         const string &delimeter );

string deleteChar( const string &value, const size_t start, const char ch );

} // namespace graphsat

#endif
