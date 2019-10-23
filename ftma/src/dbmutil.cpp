#include "util/dbmutil.hpp"

namespace graphsat {
extern string trim( std::string s );

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
    TYPE_STR( BOOL_T );
    TYPE_STR( CHAN_T );
    TYPE_STR( URGENT_CHAN_T );
    TYPE_STR( BROADCAST_CHAN_T );

    ENUM_ITEM_STR( SYSTEM_T );

    ENUM_ITEM_STR( TEMPLATE_T );

    ENUM_ITEM_STR( LOCATION_T );

    ENUM_ITEM_STR( PARAMETER_T );

    ENUM_ITEM_STR( REF_PARAMETER_T );

    ENUM_ITEM_STR( CLOCK_CS_T );

    ENUM_ITEM_STR( INT_CS_T );

    ENUM_ITEM_STR( INT_UPDATE_T );

    ENUM_ITEM_STR( RESET_T );

    ENUM_ITEM_STR( NO_T );
  }
}

TYPE_T base_type( TYPE_T type ) { return ( TYPE_T )( type / 5 ); }

bool isRefType( const TYPE_T type ) {
  if ( type >= PARAMETER_T ) {
    return false;
  }
  return type % 2 == 1;
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
} // namespace graphsat
