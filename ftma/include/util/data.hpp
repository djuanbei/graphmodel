/**
 * @file   data.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu May 23 10:21:13 2019
 *
 * @brief
 *
 *
 */

#ifndef __DATA_H
#define __DATA_H
#include <map>
#include <string>
#include <vector>

namespace graphsat {
using std::map;
using std::pair;
using std::string;
using std::vector;

#define STRING( s ) #s

#define SINGLETON( T )                                                         \
private:                                                                       \
  T() {}                                                                       \
  T( const T &other ) { assert( false ); }                                     \
  T &operator=( const T &other ) {                                             \
    assert( false );                                                           \
    return *this;                                                              \
  }                                                                            \
                                                                               \
public:                                                                        \
  static T &getInstance() {                                                    \
    static T instance;                                                         \
    return instance;                                                           \
  }

#define deleteType( T )                                                        \
  {                                                                            \
    vector<void *> dummy = pdata.getPoints( #T );                              \
    for ( auto e : dummy ) {                                                   \
      delete (T *) e;                                                          \
    }                                                                          \
  }

template <typename T> class ValueData {
public:
  void clear() { values.clear(); }
  void clear( const string &type ) { values[ type ].clear(); }

  void addValue( const string &type, const string &name, int v = 0 ) {

    for ( size_t i = 0; i < values[ type ].size(); i++ ) {
      if ( values[ type ][ i ].first == name ) {
        values[ type ][ i ].second.push_back( v );
        return;
      }
    }

    vector<int> vec_v;
    vec_v.push_back( v );
    values[ type ].push_back( make_pair( name, vec_v ) );
  }

  int getTypeNum( const string &type ) const {
    if ( values.find( type ) == values.end() ) {
      return 0;
    }
    return values.at( type ).size();
  }

  /**
   *
   *
   * @param type
   * @param values
   *
   * @return  -1 if name is not in values
   */
  int getId( const string &type, const string &name ) const {
    if ( values.find( type ) == values.end() ) {
      return -1;
    }

    for ( size_t i = 0; i < values.at( type ).size(); i++ ) {
      if ( values.at( type )[ i ].first == name ) {
        return i;
      }
    }
    return -1;
  }

  const pair<string, vector<int>> &getValue( const string &type,
                                             int           id ) const {
    return values.at( type )[ id ];
  }

  bool hasValue( const string &type, const string &name, int id = 0 ) const {
    if ( values.find( type ) == values.end() ) {
      return false;
    }
    for ( size_t i = 0; i < values.at( type ).size(); i++ ) {
      if ( values.at( type )[ i ].first == name ) {
        return id < (int) values.at( type )[ i ].second.size();
      }
    }
    return false;
  }

  int getValue( const string &type, const string &name, int id = 0 ) const {
    for ( size_t i = 0; i < values.at( type ).size(); i++ ) {
      if ( values.at( type )[ i ].first == name ) {
        return values.at( type )[ i ].second[ id ];
      }
    }
    assert( false );
    return -1;
  }

private:
  map<string, vector<pair<string, vector<int>>>> values;
};

class PointerData {

public:
  void clear() { values.clear(); }

  void addPointer( const string &type, void *v ) {
    values[ type ].push_back( v );
  }

  vector<void *> getPoints( const string &type ) const {
    if ( values.find( type ) == values.end() ) {
      vector<void *> temp;
      return temp;
    }
    return values.at( type );
  }

  void clearPoints( const string &type ) { values[ type ].clear(); }
  void clearPoints() { values.clear(); }

private:
  map<string, vector<void *>> values;
};

} // namespace graphsat

#endif
