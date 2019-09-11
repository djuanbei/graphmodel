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

enum ParaType {
  PARAM_BOOL_T,
  PARAM_BOOL_REF_T,
  PARAM_CHANNEL_T,
  PARAM_CHANNEL_REF_T,
  PARAM_URGENT_CHANNEL_T,
  PARAM_URGENT_CHANNEL_REF_T,
  PARAM_INT_T,
  PARAM_INT_REF_T,
  PARAM_SCALAR_T
};
struct ParaElement {
  ParaElement( ){
    is_ref=false;
  }
  bool is_ref;
  ParaType type;
  int      id;
  string   type_name;
  string   name;
};

struct TaDec {
  bool        no_parameter;
  string      name;
  vector<int> param_list;
  TaDec()
      : no_parameter( false ) {
    name = "";
  }
};

struct SystemDec {
  vector<TaDec *> ta_list;

  ~SystemDec() {
    for ( vector<TaDec *>::iterator it = ta_list.begin(); it != ta_list.end();
          it++ ) {
      delete *it;
    }
  }
};

enum ParserType { GLOBAL_DEC, TEMPLATE_DEC };




const static string CLOCK_CS = "clock_cons";


const static string INT_CS = "counter_cons";

const static string CLOCK_RESET = "clock_reset";

const static string INT_UPDATE = "counter_update";

const static string INT_ARRAY = "int_array";

//const static string USING_GLOBAL = STRING( USING_GLOBAL );

const static int  UN_DEFINE=100000000;



class UppaalParser;

class UppaalTemplateData {
public:
  UppaalTemplateData() { init_loc = 0; name=""; global_var_num=0; }
  void clear() {
    int_values.clear();
    point_values.clear();
  }

  void setName(const string &n ){
    name=n;
  }
  
  string getName( void){
    return name;
  }
  void setGlobalVarNum( int n){
    global_var_num=n;
  }
  
  int getGlobalVarNum( void) const{
    return global_var_num;
  }

  string getVarFullName(const string &var_name) const{
    if(""!=name ){
      return name+"#"+var_name;
    }
    return var_name;
  }



  void addIntArray(const string &key, vector<int> &v ) {
    for ( auto e : v ) {
      addValue( INT_ARRAY, key, e );
    }
  }
  const vector<int> &getIntArray(const string &key ) const {
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

  void addValue( const string &type, const string &name, int v = UN_DEFINE ) {
    int_values.addValue( type, name, v );
  }

  void setValue( const string & type, const string & name, int v= UN_DEFINE){
    int_values.setValue( type, name, v );
  }
  
  
  int getTypeNum( const string &type ) const {
    return int_values.getTypeNum( type );
  }

  /**
   *
   * @param type  The type of element which want to find.
   * @param name The element name which want to find.
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

  bool hasValue( const string &type, const string &name ) const {
    
    return int_values.hasValue( type, name );
    
  }

  int getValue( const string &type, const string &name, int id = 0 ) const {
    return int_values.getValue( type, name, id );
  }

  
  vector<pair<string, vector<int>>> getValue( const string &type ) const {
    return int_values.getValue( type );
  }

  void addPointer( const string &type, const string &name, void *v ) {
    point_values.addValue( type, name, v );
  }

  bool hasPointer( const string &type, const string name ) const {
    return point_values.hasValue( type, name );
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
  void *getPointer( const string &type, const string &name ) const {
    return point_values.getValue( type, name, 0 );
  }

  int getPointNum( const string &type ) const {
    return point_values.getTypeNum( type );
  }

  vector<pair<string, vector<void *>>> getPoints( const string &type ) const {
    return point_values.getValue( type );
  }

  void clearPoints( const string &type ) { point_values.clear( type ); }

  void clearPoints() { point_values.clear(); }

private:
  string         name;
  int global_var_num;
  ValueData<int> int_values;
  PointerData    point_values;

  int init_loc;

  typename INT_TAS_t::TAT_t tat;
  friend class UppaalParser;
};

void parseProblem( const string &str, UppaalTemplateData *,
                   UppaalTemplateData * );
} // namespace graphsat

#endif
