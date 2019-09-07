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

class InstanceFactory;

class CounterAction {

public:
  virtual void operator()( int *counter_value ) const = 0;
  /**
   *Deley set the counter id
   */
  virtual void globalUpdate( const map<int, int> &id_map,
                             const vector<int> &  parameter_value ) = 0;

private:
  virtual CounterAction *copy() const = 0;

  friend class InstanceFactory;
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

private:
  SimpleCounterAction( int cid, int v ) {
    local_counter_id  = cid;
    global_counter_id = 0;
    rhs               = v;
  }

  ~SimpleCounterAction() {}

  CounterAction *copy() const {
    return new SimpleCounterAction( local_counter_id, rhs );
  }

  int local_counter_id;
  int global_counter_id;
  int rhs;
  friend class InstanceFactory;
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

private:
  SimpleCounterPAction( int cid, int eparameter_id ) {

    local_counter_id  = cid;
    global_counter_id = 0;
    parameter_id      = eparameter_id;
    parameter_v       = 0;
  }
  ~SimpleCounterPAction() {}

  CounterAction *copy() const {
    return new SimpleCounterPAction( local_counter_id, parameter_id );
  }

  int local_counter_id;
  int global_counter_id;
  int parameter_id;
  int parameter_v;
  friend class InstanceFactory;
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

private:
  DefaultCAction(
      const vector<pair<int, vector<pair<int, int>>>> &relations1 ) {

    local_relations  = relations1;
    global_relations = relations1;
  }
  ~DefaultCAction() {}

  CounterAction *copy() const { return new DefaultCAction( local_relations ); }

  vector<pair<int, vector<pair<int, int>>>> local_relations;
  vector<pair<int, vector<pair<int, int>>>> global_relations;
  friend class InstanceFactory;
};

} // namespace graphsat
#endif
