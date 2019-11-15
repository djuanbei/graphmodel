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

#define STRING( s ) #s

#ifdef PRINT_STATE

#define PRINT_STATE_MACRO                                                      \
  for ( int i = 0; i < component_num; i++ ) {                                  \
    if ( state[ i + component_num ] == NO_CHANNEL )                            \
      cout << setw( LOC_OUT_WIDTH ) << state[ i ];                             \
    else {                                                                     \
      int block_source;                                                        \
      sys.tas[ i ].ta_tempate->graph.findSrc( state[ i ], block_source );      \
      cout << setw( LOC_OUT_WIDTH ) << block_source;                           \
    }                                                                          \
  }                                                                            \
  cout << endl;                                                                \
  manager.getClockManager().dump( cout, manager.getDBM( state ) ) << endl;

#else

#define PRINT_STATE_MACRO

#endif

#define LTEQ_ZERO ( (C) 1 )

#define newA( __E, __n ) (__E *) malloc( ( __n ) * sizeof( __E ) )

#define TYPE_TYPE( T )                                                         \
  T, REF_##T, CONST_##T, CONST_REF_##T, PARAMETER_##T, REF_PARAMETER_##T,      \
      ARRAY_##T

#define TYPE_STR( T )                                                          \
  case T:                                                                      \
    return STRING( T );                                                        \
  case REF_##T:                                                                \
    return STRING( REF_##T );                                                  \
  case CONST_##T:                                                              \
    return STRING( CONST_##T );                                                \
  case CONST_REF_##T:                                                          \
    return STRING( CONST_REF_##T );                                            \
  case ARRAY_##T:                                                              \
    return STRING( ARRAY_##T );                                                \
  case PARAMETER_##T:                                                          \
    return STRING( PARAMETER_##T );                                            \
  case REF_PARAMETER_##T:                                                      \
    return STRING( REF_PARAMETER_##T );

const static int TYPE_FAMILY_LEN = 7;

#define ENUM_ITEM_STR( T )                                                     \
  case T:                                                                      \
    return STRING( T );

namespace graphsat {
using std::map;
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

const static int LOC_OUT_WIDTH = 3;
const static int OP_OUT_WIDTH  = 3;

const static int VALUE_OUT_WIDTH = 5;

typedef long double DF_T;

enum Check_State { TRUE, FALSE, UNKOWN };

enum COMP_OPERATOR { EQ, LE, GE, LT, GT, NE };

enum ARGUMENT_TYPE {
  CONST_ARG,
  COUNTER_ARG,
  PARAMETER_ARG,
  REF_PARAMETER_ARG,
  EMPTY_ARG
};

struct Argument {
  ARGUMENT_TYPE type;
  int           value;
  Argument()
      : type( CONST_ARG )
      , value( 0 ) {}
  Argument( ARGUMENT_TYPE t, int v ) {
    type  = t;
    value = v;
  }
};

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
template <typename C> inline C getMAX_INT( const C c ) {
  return std::numeric_limits<C>::max() / 2 - 2;
}

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
