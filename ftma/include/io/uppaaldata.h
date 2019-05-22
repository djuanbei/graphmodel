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
using std::pair;
using std::string;
using std::vector;

enum ParserType { GLOBAL_DEC, TEMPLATE_DEC };

const static string CLOCK_STR = "clock";

const static string COUNTER_STR = "counter";

const static string CHANNEL_STR = "channel";

const static string CLOCK_CS = "clock_cons";

const static string COUNTER_CS = "counter_cons";

const static string CLOCK_RESET = "clock_reset";

const static string COUNTER_UPDATE = "counter_update";

class UppaalData {
public:
  UppaalData() { init_loc = 0; }
  void clear() {
    globalIntArray.clear();

    variable.clear();
  }

  void addIntArray( string &key, vector<int> &v ) { globalIntArray[ key ] = v; }
  const vector<int> &getIntArray( string &key ) const {
    return globalIntArray.at( key );
  }

  void setInitialLoc( int id ) { init_loc = id; }

  int getInitialLoc() const { return init_loc; }

  void addValue( const string &type, const string &name, int v = 0 ) {

    for ( size_t i = 0; i < variable[ type ].size(); i++ ) {
      if ( variable[ type ][ i ].first == name ) {
        variable[ type ][ i ].second.push_back( v );
        return;
      }
    }

    vector<int> vec_v;
    vec_v.push_back( v );
    variable[ type ].push_back( make_pair( name, vec_v ) );
  }

  int getTypeNum( const string &type ) const {
    if ( variable.find( type ) == variable.end() ) {
      return 0;
    }
    return variable.at( type ).size();
  }

  /**
   *
   *
   * @param type
   * @param variable
   *
   * @return  -1 if name is not in variable
   */
  int getId( const string &type, const string &name ) const {
    if ( variable.find( type ) == variable.end() ) {
      return -1;
    }

    for ( size_t i = 0; i < variable.at( type ).size(); i++ ) {
      if ( variable.at( type )[ i ].first == name ) {
        return i;
      }
    }
    return -1;
  }

  bool hasValue( const string &type, const string &name, int id = 0 ) const {
    if ( variable.find( type ) == variable.end() ) {
      return false;
    }
    for ( size_t i = 0; i < variable.at( type ).size(); i++ ) {
      if ( variable.at( type )[ i ].first == name ) {
        return id < (int) variable.at( type )[ i ].second.size();
      }
    }
    return false;
  }

  int getValue( const string &type, const string &name, int id = 0 ) const {
    for ( size_t i = 0; i < variable.at( type ).size(); i++ ) {
      if ( variable.at( type )[ i ].first == name ) {
        return variable.at( type )[ i ].second[ id ];
      }
    }
    assert( false );
    return -1;
  }
  const pair<string, vector<int>> &getValue( const string &type,
                                             int           id ) const {
    return variable.at( type )[ id ];
  }

  void addPointer( const string &type, void *v ) {
    pointValues[ type ].push_back( v );
  }

  vector<void *> getPoints( const string &type ) const {
    if ( pointValues.find( type ) == pointValues.end() ) {
      vector<void *> temp;
      return temp;
    }
    return pointValues.at( type );
  }

  void clearPoints( const string &type ) { pointValues[ type ].clear(); }
  void clearPoints() { pointValues.clear(); }

private:
  map<string, vector<int>> globalIntArray;

  map<string, vector<pair<string, vector<int>>>> variable;

  map<string, vector<void *>> pointValues;

  int init_loc;
};

void parseProblem( const string &str, const UppaalData *, UppaalData * );
} // namespace graphsat

#endif
