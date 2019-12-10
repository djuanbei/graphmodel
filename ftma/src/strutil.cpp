#include"util/strutil.h"

namespace graphsat{
string getFunArg(const string &fun ){
  string::size_type start=fun.find( "(");
  if(start== string::npos ){
    return "";
  }
  string::size_type end=fun.find( ")");
  if( end== string::npos){
    return "";
  }
  if( end>start){
    return fun.substr(start, end-start );
  }
  return "";
  
}

std::string trim( std::string  s ) {
  if ( s.empty() ) {
    return s;
  }

  s.erase( 0, s.find_first_not_of( " " ) );
  s.erase( s.find_last_not_of( " " ) + 1 );
  return s;
}

}
