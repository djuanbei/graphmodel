#include "util/dbmutil.hpp"
#include <cassert>
namespace graphsat {
extern string trim( std::string s );

int getValue(const RealArgument & arg,  const int * counter_value  ){
  switch(arg.type ){
    case CONST_ARG:
      return arg.value;
    case  TEMPLATE_VAR_ARG:
      break;
    case SYSTEM_VAR_ARG:
      break;
    case PARAMETER_ARG:
      return arg.value;
    case REF_PARAMETER_ARG:
      break;
    case FUN_POINTER_ARG:
      return (( ConstraintFun_t)arg.value)( counter_value);
    case SELECT_VAR_ARG:
      return arg.value;
    case EMPTY_ARG:
      return 0;
  }
  if( nullptr!= arg.index){
    int shift=getValue(arg.index, counter_value );
    return counter_value[ arg.value+shift ];
  }

  return counter_value[ arg.value ];
}

int getValue(const shared_ptr<RealArgument> & arg,  const int * counter_value  ){

    switch(arg->type ){
    case CONST_ARG:
      return arg->value;
    case  TEMPLATE_VAR_ARG:
      break;
    case PARAMETER_ARG:
      return arg->value;
    case REF_PARAMETER_ARG:
      break;

    case FUN_POINTER_ARG:
      return (( ConstraintFun_t)arg->value)( counter_value);
    case SELECT_VAR_ARG:
      return arg->value;
    case EMPTY_ARG:
      return 0;
  }
  if( nullptr!= arg->index){
    int shift=getValue(arg->index, counter_value );
    return counter_value[ arg->value+shift ];
  }

  return counter_value[ arg->value ];
}


int_fast64_t getMapValue( const Argument & arg, const vector<int> &id_map,
                          const vector<int> &parameter_value ){
  switch(arg.type ){
    case CONST_ARG:
      return arg.value;
    case TEMPLATE_VAR_ARG:
      return arg.value;
    case PARAMETER_ARG:
      return parameter_value[arg.value ];
    case REF_PARAMETER_ARG:
      return parameter_value[arg.value ];
    case FUN_POINTER_ARG:
      return arg.value;
    case SELECT_VAR_ARG:
      return parameter_value.back( );
    case EMPTY_ARG:
      return 0;
  }
  
}

string getOpStr( COMP_OPERATOR op ) {
  switch ( op ) {
    ENUM_ITEM_STR( EQ );
    ENUM_ITEM_STR( LE );
    ENUM_ITEM_STR( GE );
    ENUM_ITEM_STR( LT );
    ENUM_ITEM_STR( GT );
    ENUM_ITEM_STR( NE );
  }
}

COMP_OPERATOR negation( COMP_OPERATOR op ) {
  switch ( op ) {
  case EQ: {
    return NE;
  }
  case LE: {
    return GT;
  }
  case GE: {
    return LT;
  }
  case LT: {
    return GE;
  }
  case GT: {
    return LE;
  }
  case NE: {
    return EQ;
  }
  }
}

string getTypeStr( TYPE_T type ) {
  switch ( type ) {
    TYPE_STR( INT_T );
    TYPE_STR( CLOCK_T );
    TYPE_STR( CHAN_T );
    TYPE_STR( URGENT_CHAN_T );
    TYPE_STR( BROADCAST_CHAN_T );

    ENUM_ITEM_STR( SYSTEM_T );

    ENUM_ITEM_STR( TEMPLATE_T );

    ENUM_ITEM_STR( AUTOMATA_T );

    ENUM_ITEM_STR( LOCATION_T );

    ENUM_ITEM_STR( FORMAL_PARAMETER_T );

    ENUM_ITEM_STR( CLOCK_CS_T );

    ENUM_ITEM_STR( INT_CS_T );

    ENUM_ITEM_STR( INT_UPDATE_T );

    ENUM_ITEM_STR( RESET_T );

    ENUM_ITEM_STR( SELF_DEF_T );

    ENUM_ITEM_STR( CHAN_ACTION_T );

    ENUM_ITEM_STR( REF_CHAN_ACTION_T );

    ENUM_ITEM_STR( NO_T );
  }
}

TYPE_T baseType( TYPE_T type ) {
  return ( TYPE_T )( ( type / TYPE_FAMILY_LEN ) * TYPE_FAMILY_LEN );
}

bool isRefChan( TYPE_T type ) {
  TYPE_T base_t = baseType( type );
  if ( base_t != CHAN_T && base_t != URGENT_CHAN_T &&
       base_t != BROADCAST_CHAN_T ) {
    return false;
  }
  return isRefType( type );
}
TYPE_T get_formal_paramter_type( TYPE_T type ) {
  if ( INT_T == type || CONST_INT_T == type ) {
    return PARAMETER_INT_T;
  }
  if ( REF_INT_T == type || CONST_REF_INT_T == type ) {
    return REF_PARAMETER_INT_T;
  }

  if ( CLOCK_T == type || CONST_CLOCK_T == type ) {
    return PARAMETER_CLOCK_T;
  }
  if ( REF_CLOCK_T == type || CONST_REF_CLOCK_T == type ) {
    return REF_PARAMETER_CLOCK_T;
  }

  if ( CHAN_T == type || CONST_CHAN_T == type ) {
    return PARAMETER_CHAN_T;
  }

  if ( REF_CHAN_T == type || CONST_REF_CHAN_T == type ) {
    return REF_PARAMETER_CHAN_T;
  }

  if ( URGENT_CHAN_T == type || CONST_URGENT_CHAN_T == type ) {
    return PARAMETER_CHAN_T;
  }

  if ( REF_URGENT_CHAN_T == type || CONST_REF_URGENT_CHAN_T == type ) {
    return REF_PARAMETER_CHAN_T;
  }

  if ( BROADCAST_CHAN_T == type || CONST_BROADCAST_CHAN_T == type ) {
    return PARAMETER_CHAN_T;
  }

  if ( REF_BROADCAST_CHAN_T == type || CONST_BROADCAST_CHAN_T == type ) {
    return REF_PARAMETER_CHAN_T;
  }

  if ( SELF_DEF_T == type ) {
    return PARAMETER_INT_T;
  }

  assert( false );
  return NO_T;
}

bool isRefType( const TYPE_T type ) {
  if ( type >= SYSTEM_T ) {
    return false;
  }
  int m = type % TYPE_FAMILY_LEN;
  return m == 1 || m == 3 || m == 5;
}

std::vector<string> splitStr( const string &string_to_splitted,
                              const string &delimeter ) {
  vector<string> split_string;
  size_t         start_index = 0;
  size_t         found = string_to_splitted.find( delimeter, start_index );
  while ( found != string::npos ) {
    string val =
        trim( string_to_splitted.substr( start_index, found - start_index ) );
    if ( val != "" ) {
      split_string.push_back( val );
    }
    start_index = found + delimeter.size();
    found       = string_to_splitted.find( delimeter, start_index );
  }
  if ( start_index < string_to_splitted.size() ) {
    string val = trim( string_to_splitted.substr( start_index ) );
    if ( val != "" ) {
      split_string.push_back( val );
    }
  }
  return split_string;
}
string deleteChar( const string &value, const size_t start, const char ch ) {
  char * buf = new char[ value.length() + 1 ];
  size_t i   = 0;
  size_t k   = 0;
  for ( ; i < start; i++ ) {
    buf[ i ] = value[ i ];
  }
  k = i;
  for ( ; i < value.length(); i++ ) {
    if ( value[ i ] != ch ) {
      buf[ k++ ] = value[ i ];
    }
  }
  buf[ k ] = '\0';
  return string( buf );
}

int fromPidToChanId( int id ) { return id + 1; }

int chanIdToFromPid( int id ) { return id - 1; }
} // namespace graphsat
