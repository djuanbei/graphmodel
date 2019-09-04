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
protected:
  bool is_update;

public:
  virtual void operator()( const int *parameterValue,
                           int *      counterValue ) const = 0;
  /**
   *Deley set the counter id
   */
  virtual void counterIpMap( const map<int, int> &id_map ) = 0;

  virtual CounterAction *copy() const = 0;
};

class SimpleCounterAction : public CounterAction {
public:
  void operator()( const int *parameterValue, int *counter_value ) const {
    counter_value[ counter_id ] = rhs;
  }

  void counterIpMap( const map<int, int> &id_map ) {
    if ( !is_update ) {
      is_update  = true;
      counter_id = id_map.at( counter_id );
    }
  }
  CounterAction *copy() const {
    assert( !is_update );
    return new SimpleCounterAction( counter_id, rhs );
  }

private:
  SimpleCounterAction( int cid, int v ) {
    is_update  = false;
    counter_id = cid;
    rhs        = v;
  }
  ~SimpleCounterAction() {}
  int counter_id;
  int rhs;
  friend class CounterActionFactory;
};

class SimpleCounterPAction : public CounterAction {
public:
  void operator()( const int *parameter_value, int *counterValue ) const {
    counterValue[ counter_id ] = parameter_value[ parameter_id ];
  }
  void counterIpMap( const map<int, int> &id_map ) {
    if ( !is_update ) {
      is_update  = true;
      counter_id = id_map.at( counter_id );
    }
  }
  CounterAction *copy() const {
    assert( !is_update );
    return new SimpleCounterPAction( counter_id, parameter_id );
  }

private:
  SimpleCounterPAction( int cid, int eparameter_id ) {
    is_update    = false;
    counter_id   = cid;
    parameter_id = eparameter_id;
  }
  ~SimpleCounterPAction() {}
  int counter_id;
  int parameter_id;
  friend class CounterActionFactory;
};

class DefaultCAction : public CounterAction {
public:
  void operator()( const int *parameter_value, int *counter_value ) const {
    for ( auto e : relations ) {
      counter_value[ e.first ] = 0;
      for ( auto ee : e.second ) {
        counter_value[ e.first ] += ee.first * parameter_value[ ee.second ];
      }
    }
  }
  void counterIpMap( const map<int, int> &id_map ) {
    if ( !is_update ) {
      is_update = true;
      for ( auto e : relations ) {
        e.first = id_map.at( e.first );
        for ( auto ee : e.second ) {
          ee.second = id_map.at( ee.second );
        }
      }
    }
  }

  CounterAction *copy() const {
    assert( !is_update );
    return new DefaultCAction( relations );
  }

private:
  DefaultCAction(
      const vector<pair<int, vector<pair<int, int>>>> &relations1 ) {
    is_update = false;
    relations = relations1;
  }
  ~DefaultCAction() {}
  vector<pair<int, vector<pair<int, int>>>> relations;
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
