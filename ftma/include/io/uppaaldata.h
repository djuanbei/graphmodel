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
#include "model/ta.hpp"
#include "util/data.hpp"
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

const static string INT_ARRAY = "int_array";

class UppaalParser;

class UppaalTemplateData {
public:
  UppaalTemplateData() { init_loc = 0; }
  void clear() {
    int_values.clear();
    point_values.clear();
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
    int_values.addValue( type, name, v );
  }

  int getTypeNum( const string &type ) const {
    return int_values.getTypeNum( type );
  }

  /**
   *
   *
   * @param type
   * @param name
   *
   * @return  -1 if name is not in int_values
   */
  int getId( const string &type, const string &name ) const {
    return int_values.getId( type, name );
  }

  const pair<string, vector<int>> &getValue( const string &type,
                                             int           id ) const {
    return int_values.getValue( type, id );
  }

  bool hasValue( const string &type, const string &name, int id = 0 ) const {
    return int_values.hasValue( type, name, id );
  }

  int getValue( const string &type, const string &name, int id = 0 ) const {
    return int_values.getValue( type, name, id );
  }

  void addPointer( const string &type, const string &name, void *v ) {
    point_values.addValue( type, name, v );
  }

  bool hasPointer( const string &type, const string name ) const {
    return point_values.hasValue( type, name, 0 );
  }

  int getPointerId( const string &type, const string &name ) const {
    return point_values.getId( type, name );
  }

  vector<void *> getPoints( const string &type, const string name ) const {
    int id = point_values.getId( type, name );
    if ( id > -1 ) {
      return point_values.getValue( type, id ).second;
    } else {
      vector<void *> dummy;
      return dummy;
    }
  }

  void clearPoints( const string &type ) { point_values.clear( type ); }

  void clearPoints() { point_values.clear(); }

  vector<ParaElement> para_list;

private:
  string         name;
  ValueData<int> int_values;
  PointerData    point_values;

  int init_loc;

  typename INT_TAS_t::TA_t ta;
  friend class UppaalParser;
};

void parseProblem( const string &str, const UppaalTemplateData *,
                   UppaalTemplateData * );
} // namespace graphsat

#endif
