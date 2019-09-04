#include "util/dbmutil.hpp"

namespace graphsat {
extern string trim( std::string s );

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
