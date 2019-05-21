/**
 * @file   uppaaldata.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Tue May 21 13:44:10 2019
 *
 * @brief
 *
 *
 */

#ifndef __UPPAAL_H
#define __UPPAAL_H
#include <map>
#include <string>
#include <vector>
namespace graphsat {
using std::map;
using std::string;
using std::vector;

enum ParserType { GLOBAL_DEC, TEMPLATE_DEC };

class UppaalData {
public:
  UppaalData() { ptype = GLOBAL_DEC; }
  void clear() {
    globalIntArray.clear();
    globalCounters.clear();
    globalCounters.clear();
  }
  ParserType getType() const { return ptype; }
  void addIntArray( string &key, vector<int> &v ) { globalIntArray[ key ] = v; }
  const vector<int> &getIntArray( string &key ) const {
    return globalIntArray.at( key );
  }
  void addCounter( string &name ) { globalCounters.push_back( name ); }
  int  getCounterId( string &name ) const {
    vector<string>::const_iterator it =
        find( globalCounters.begin(), globalCounters.end(), name );
    if ( it == globalCounters.end() ) {
      return -1;
    }
    return it - globalCounters.begin();
  }
  int addLoc( string &name ) {
    int re = locations.size();
    locations.push_back( name );
    return re;
  }
  int getLoc( string &name ) const {
    vector<string>::const_iterator it =
        find( locations.begin(), locations.end(), name );
    if ( it == locations.end() ) {
      return -1;
    }
    return it - locations.begin();
  }

private:
  map<string, vector<int>> globalIntArray;
  vector<string>           globalCounters;
  vector<string>           locations;
  ParserType               ptype;
};

void parseProblem( const string &str, UppaalData * );
} // namespace graphsat

#endif
