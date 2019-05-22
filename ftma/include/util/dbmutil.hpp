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
#include <string>
#include <vector>

#define LTEQ_ZERO ( (C) 1 )

#define newA( __E, __n ) (__E *) malloc( ( __n ) * sizeof( __E ) )

namespace graphsat {
using std::string;
using std::vector;
const static int zero_clock_index = 0;

const static string NTA_STR = "nta";

const static string DECLARATION_STR = "declaration";

const static string TEMPLATE_STR = "template";

const static string NAME_STR = "name";

const static string PARAMETER_STR = "parameter";

const static string LOCATION_STR = "location";

const static string INIT_STR = "init";

const static string TRANSITION_STR = "transition";

const static string SOURCE_STR = "source";

const static string TARGET_STR = "target";

const static string ID_STR = "id";

const static string REF_STR = "ref";

const static string LABEL_STR = "label";

const static string KIND_STR = "kind";

const static string NAIL_STR = "nail";

const static string INVARIANT_STR = "invariant";

const static string GUARD_STR = "guard";

const static string ASSIGNMENT_STR = "assignment";

const static string SYNCHRONISATION_STR = "synchronisation";

const static string SYSTEM_STR = "system";

const static string QUERIES_STR = "queries";

const static string QUERY_STR = "query";

const static string FORMULA_STR = "formula";

const static string COMMENT_STR = "comment";

const static string RESET_STR = "reset";

typedef long double DF_T;

enum Check_State { TRUE, FALSE, UNKOWN };

enum COMP_OPERATOR { EQ, LE, GE, LT, GT, NE };
enum TYPE_T { CLOCK_T, COUNTER_T, PARAMETER_T, NO_T };

/**
 *  Both have compare < and <=
 *
 */

template <typename C> C getMAX_INT( const C c ) {
  return std::numeric_limits<C>::max() / 2 - 2;
}

template <typename C> C add( const C x, const C y ) {
  static C MAX_INT = getMAX_INT( x );
  if ( x >= MAX_INT || y >= MAX_INT ) {
    return MAX_INT;
  }
  return x + y - ( ( x & 1 ) | ( y & 1 ) );
}

template <typename C> C getMatrixValue( C realRight, bool isStrct = true ) {
  C right = realRight * 2;
  if ( !isStrct ) {
    right = right | 1;
  }
  return right;
}
template <typename C> bool isStrict( const C c ) { return ( c & 1 ) == 0; }

template <typename C> std::string getComp( const C x ) {
  if ( x & 1 ) {
    return "<=";
  } else {
    return "< ";
  }
}

template <typename C> C getRight( const C c ) { return c >> 1; }

template <typename C> C negMatrixValue( C matrix_value ) {
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

std::vector<string> splitStr( const string &stringToBeSplitted,
                              const string &delimeter );

} // namespace graphsat

#endif
