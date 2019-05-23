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

#include "util/data.hpp"
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

const static string INT_ARRAY = "int_array";

class UppaalData {
public:
  UppaalData() { init_loc = 0; }
  void clear() {
    intValues.clear();
    pointValues.clear();
  }

  void addIntArray( string &key, vector<int> &v ) {
    for ( auto e : v ) {
      addValue( INT_ARRAY, key, e );
    }
  }
  const vector<int> &getIntArray( string &key ) const {
    int id = getId( INT_ARRAY, key );
    if ( id > -1 ) {
      const pair<string, vector<int>> &pp = getValue( INT_ARRAY, id );
      return pp.second;
    }
    static vector<int> dummy;
    return dummy;
  }

  void setInitialLoc( int id ) { init_loc = id; }

  int getInitialLoc() const { return init_loc; }

  void addValue( const string &type, const string &name, int v = 0 ) {
    intValues.addValue( type, name, v );
  }

  int getTypeNum( const string &type ) const {
    return intValues.getTypeNum( type );
  }

  /**
   *
   *
   * @param type
   * @param intValues
   *
   * @return  -1 if name is not in intValues
   */
  int getId( const string &type, const string &name ) const {
    return intValues.getId( type, name );
  }

  const pair<string, vector<int>> &getValue( const string &type,
                                             int           id ) const {
    return intValues.getValue( type, id );
  }

  bool hasValue( const string &type, const string &name, int id = 0 ) const {
    return intValues.hasValue( type, name, id );
  }

  int getValue( const string &type, const string &name, int id = 0 ) const {
    return intValues.getValue( type, name, id );
  }

  void addPointer( const string &type, void *v ) {

    pointValues.addPointer( type, v );
  }

  vector<void *> getPoints( const string &type ) const {
    return pointValues.getPoints( type );
  }

  void clearPoints( const string &type ) { pointValues.clearPoints( type ); }
  void clearPoints() { pointValues.clear(); }

private:
  ValueData<int> intValues;
  PointerData    pointValues;

  int init_loc;
};

void parseProblem( const string &str, const UppaalData *, UppaalData * );
} // namespace graphsat

#endif
