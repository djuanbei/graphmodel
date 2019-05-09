#include "util/dbmutil.hpp"

namespace graphsat {

std::vector<string> splitStr( const string &stringToBeSplitted,
                              const string &delimeter ) {

  vector<string> splitString;
  size_t         startIndex = 0;
  size_t         endIndex   = 0;
  size_t         found      = stringToBeSplitted.find( delimeter, startIndex );
  while ( found != string::npos ) {
    startIndex = found;
    string val = stringToBeSplitted.substr( startIndex, endIndex - startIndex );
    splitString.push_back( val );
    startIndex = endIndex + delimeter.size();
    found      = stringToBeSplitted.find( delimeter, startIndex );
  }
  if ( startIndex < stringToBeSplitted.size() ) {
    string val = stringToBeSplitted.substr( startIndex );
    splitString.push_back( val );
  }
  return splitString;
}
} // namespace graphsat
