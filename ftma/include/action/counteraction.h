/**
 * @file   Action.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:39:22 2019
 *
 * @brief  Action class which  do x=x+c where x is a counter
 *        variable and c is a constant integer
 *
 *
 */
#ifndef ACTION_HPP
#define ACTION_HPP

#include <vector>

#include "util/data.hpp"

namespace graphsat {
using std::pair;
using std::vector;
class CounterAction {

public:
  virtual void operator()( int *counter_value ) const = 0;
  /**
   *Deley set the counter id
   */
  virtual void globalUpdate( const map<int, int> &id_map,
                             const vector<int> &  parameter_value ) = 0;

  virtual CounterAction *copy() const = 0;
};

class SimpleCounterAction : public CounterAction {
public:
  void operator()( int *counter_value ) const {
    counter_value[ global_counter_id ] = rhs;
  }

  void globalUpdate( const map<int, int> &id_map,
                     const vector<int> &  parameter_value ) {

    global_counter_id = id_map.at( local_counter_id );
  }

  CounterAction *copy() const {
    return new SimpleCounterAction( local_counter_id, rhs );
  }

private:
  SimpleCounterAction( int cid, int v ) {
    local_counter_id  = cid;
    global_counter_id = 0;
    rhs               = v;
  }

  ~SimpleCounterAction() {}

  int local_counter_id;
  int global_counter_id;
  int rhs;
  friend class CounterActionFactory;
};

class SimpleCounterPAction : public CounterAction {
public:
  void operator()( int *counter_value ) const {
    counter_value[ global_counter_id ] = parameter_v;
  }

  void globalUpdate( const map<int, int> &id_map,
                     const vector<int> &  parameter_value ) {
    global_counter_id = id_map.at( local_counter_id );
    parameter_v       = parameter_value[ parameter_id ];
  }

  CounterAction *copy() const {
    return new SimpleCounterPAction( local_counter_id, parameter_id );
  }

private:
  SimpleCounterPAction( int cid, int eparameter_id ) {

    local_counter_id  = cid;
    global_counter_id = 0;
    parameter_id      = eparameter_id;
    parameter_v       = 0;
  }
  ~SimpleCounterPAction() {}
  int local_counter_id;
  int global_counter_id;
  int parameter_id;
  int parameter_v;
  friend class CounterActionFactory;
};

class DefaultCAction : public CounterAction {
public:
  void operator()( int *counter_value ) const {
    for ( auto e : global_relations ) {
      counter_value[ e.first ] = 0;
      for ( auto ee : e.second ) {
        counter_value[ e.first ] += ee.first * ee.second;
      }
    }
  }

  void globalUpdate( const map<int, int> &id_map,
                     const vector<int> &  parameter_value ) {

    for ( size_t i = 0; i < local_relations.size(); i++ ) {
      global_relations[ i ].first = id_map.at( local_relations[ i ].first );
    }

    for ( size_t i = 0; i < local_relations.size(); i++ ) {

      for ( size_t j = 0; j < local_relations[ i ].second.size(); j++ ) {

        global_relations[ i ].second[ j ].second =
            parameter_value[ local_relations[ i ].second[ j ].second ];
      }
    }
  }

  CounterAction *copy() const { return new DefaultCAction( local_relations ); }

private:
  DefaultCAction(
      const vector<pair<int, vector<pair<int, int>>>> &relations1 ) {

    local_relations  = relations1;
    global_relations = relations1;
  }
  ~DefaultCAction() {}
  vector<pair<int, vector<pair<int, int>>>> local_relations;
  vector<pair<int, vector<pair<int, int>>>> global_relations;
  friend class CounterActionFactory;
};

class CounterActionFactory {

  SINGLETON( CounterActionFactory );

public:
  SimpleCounterAction *createSimpleCounterAction( int cid, int v ) {
    SimpleCounterAction *re = new SimpleCounterAction( cid, v );
    pdata.addValue( STRING( CounterAction ), STRING( SimpleCounterAction ),
                    re );
    return re;
  }

  SimpleCounterPAction *createSimpleCounterPAction( int cid, int v ) {
    SimpleCounterPAction *re = new SimpleCounterPAction( cid, v );
    pdata.addValue( STRING( CounterAction ), STRING( SimpleCounterPAction ),
                    re );
    return re;
  }

  DefaultCAction *createDefaultCAction(
      vector<pair<int, vector<pair<int, int>>>> &relations1 ) {
    DefaultCAction *re = new DefaultCAction( relations1 );
    pdata.addValue( STRING( CounterAction ), STRING( DefaultCAction ), re );
    return re;
  }
  void destroy() {
    deleteType( pdata, CounterAction, SimpleCounterAction,
                SimpleCounterAction );
    deleteType( pdata, CounterAction, SimpleCounterPAction,
                SimpleCounterPAction );
    deleteType( pdata, CounterAction, DefaultCAction, DefaultCAction );
    pdata.clear();
  }

private:
  PointerData pdata;
};

} // namespace graphsat
#endif
