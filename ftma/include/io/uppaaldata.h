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

struct FormalParameterItem {
  FormalParameterItem() { is_ref = false; }
  bool   is_ref;
  TYPE_T type;
  int    id;
  string name;
  string type_name;
};

struct RealParameterItem {
  bool is_constant;
  int  id;
  RealParameterItem()
      : is_constant( false ) {}
};

struct TaDec {
  bool                      has_parameter;
  string                    name;
  string                    tmt_name;
  vector<RealParameterItem> param_list;
  TaDec()
      : has_parameter( true ) {
    name = "";
  }
};

struct SystemDec {
  vector<TaDec *> timed_automata_list;

  ~SystemDec() {
    for ( vector<TaDec *>::iterator it = timed_automata_list.begin();
          it != timed_automata_list.end(); it++ ) {
      delete *it;
    }
  }
};

enum ParserType { GLOBAL_DEC, TEMPLATE_DEC };

const static int UN_DEFINE = 100000000;

class UppaalParser;

class UppaalData {
public:
  UppaalData();

  void clear() {
    int_values.clear();
    point_values.clear();
  }

  void setStartId( int id ) { startId = id; }

  int getGlobalId( const string &name ) const;

  void setName( const string &n ) { name = n; }

  string getName( void ) { return name; }

  int getVarNum( void ) const;

  int getParameterId( const string &name ) {
    return getId( PARAMETER_T, name );
  }

  TYPE_T getType( const string &name ) const;

  bool isConstant( const string &name ) const;
  int  getConstant( const string &name ) const;

  void addIntArray( const string &key, vector<int> &v ) {
    for ( auto e : v ) {
      addValue( SELF_DEF_T, key, e );
    }
  }
  const vector<int> &getIntArray( const string &key ) const {
    int id = getId( SELF_DEF_T, key );
    if ( id != NOT_FOUND ) {
      const pair<string, vector<int>> &pp = getValue( SELF_DEF_T, id );
      return pp.second;
    }
    static vector<int> dummy;
    return dummy;
  }

  void setInitialLoc( int id ) { init_loc = id; }

  int getInitialLoc() const { return init_loc; }

  void addValue( const TYPE_T type, const string &name, int v = UN_DEFINE ) {
    int_values.addValue( getTypeStr( type ), name, v );
  }

  void setValue( const TYPE_T type, const string &name, int v = UN_DEFINE ) {
    int_values.setValue( getTypeStr( type ), name, v );
  }

  int getTypeNum( const TYPE_T type ) const {
    if(int_values.hasValue(getTypeStr( type )) )
      return int_values.getTypeNum(getTypeStr( type ) );
    return point_values.getTypeNum(getTypeStr( type ) );
  }

  /**
   *
   * @param type  The type of element which want to find.
   * @param name The element name which want to find.
   *
   * @return  NOT_FOUND if name is not in int_values
   */
  int getId( const TYPE_T type, const string &name ) const {
    int re=int_values.getId( getTypeStr( type ), name );
    if(re>-1){
      return re;
    }
    return point_values.getId( getTypeStr( type ), name );
  }

  int getClockId( const string &name ) const {
    return getId( CLOCK_T, name ) + 1; // start with 1
  }

  const pair<string, vector<int>> &getValue( const TYPE_T type, int id ) const {
    return int_values.getValue( getTypeStr( type ), id );
  }

  bool hasValue( const TYPE_T type, const string &name ) const {
    return  int_values.hasValue( getTypeStr( type ), name );
  }

  bool hasPointer( const TYPE_T type, const string &name ) const {
    return point_values.hasValue( getTypeStr( type ), name );
  }
  

  int getValue( const TYPE_T type, const string &name, int id = 0 ) const {
    return int_values.getValue( getTypeStr( type ), name, id );
  }

  vector<pair<string, vector<int>>> getValue( const TYPE_T type ) const {
    return int_values.getValue( getTypeStr( type ) );
  }

  void addValue( const TYPE_T type, const string &name, void *v ) {
    point_values.addValue( getTypeStr( type ), name, v );
  }

  void addPointer( const TYPE_T type, void *v ) {
    point_values.addValue( getTypeStr( type ), getTypeStr( type ), v );
  }

  vector<void *> getPoints( const TYPE_T type, const string name ) const {
    int id = point_values.getId( getTypeStr( type ), name );
    if ( id != NOT_FOUND ) {
      return point_values.getValue( getTypeStr( type ), id ).second;
    } else {
      vector<void *> dummy;
      return dummy;
    }
  }
  void *getPointer( const TYPE_T type ) const {
    return point_values.getValue( getTypeStr( type ), getTypeStr( type ), 0 );
  }
  void *getPointer( const TYPE_T type, const string &name ) const {
    return point_values.getValue( getTypeStr( type ), name, 0 );
  }
  // int getPointNum( const TYPE_T type ) const {
  //   return point_values.getTypeNum( getTypeStr( type ) );
  // }

  vector<pair<string, vector<void *>>> getPoints( const TYPE_T type ) const {
    return point_values.getValue( getTypeStr( type ) );
  }

  
  void clear( const TYPE_T type ) {
    string type_str=getTypeStr( type );
    if(point_values.hasValue( type_str) ){
      point_values.clear( type_str );
    }else if( int_values.hasValue( type_str )){
      int_values.clear( type_str );
    }
  }

  void clearPoints() { point_values.clear(); }

  
  void addClockConstraint( int clock1_id, int clock2_id, COMP_OPERATOR op,
                           int rhs, int parameter_id = -10 );

private:
  string         name;
  ValueData<int> int_values;
  PointerData    point_values;

  vector<TYPE_T> base_types;

  int         init_loc;
  UppaalData *parent;

  int startId;

  typename INT_TAS_t::TAT_t tat;
  friend class UppaalParser;
};

void parseProblem( const string &str, UppaalData * );
} // namespace graphsat

#endif
