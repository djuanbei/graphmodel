#include "util/dbmutil.hpp"

namespace graphsat {
extern string trim( std::string s );

std::vector<string> splitStr( const string &stringToBeSplitted,
                              const string &delimeter ) {
  vector<string> splitString;
  size_t         startIndex = 0;
  size_t         found      = stringToBeSplitted.find( delimeter, startIndex );
  while ( found != string::npos ) {
    string val =
        trim( stringToBeSplitted.substr( startIndex, found - startIndex ) );
    if ( val != "" ) {
      splitString.push_back( val );
    }
    startIndex = found + delimeter.size();
    found      = stringToBeSplitted.find( delimeter, startIndex );
  }
  if ( startIndex < stringToBeSplitted.size() ) {
    string val = trim( stringToBeSplitted.substr( startIndex ) );
    if ( val != "" ) {
      splitString.push_back( val );
    }
  }
  return splitString;
}
} // namespace graphsat
