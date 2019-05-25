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
  virtual void operator()( const int *parameterValue,
                           int *      counterValue ) const = 0;
};

class SimpleCounterAction : public CounterAction {
public:
  virtual void operator()( const int *parameterValue,
                           int *      counterValue ) const {
    counterValue[ counter_id ] = rhs;
  }

private:
  SimpleCounterAction( int cid, int v ) {
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
  virtual void operator()( const int *parameterValue,
                           int *      counterValue ) const {
    counterValue[ counter_id ] = parameterValue[ p_id ];
  }

private:
  SimpleCounterPAction( int cid, int v ) {
    counter_id = cid;
    p_id       = v;
  }
  ~SimpleCounterPAction() {}
  int counter_id;
  int p_id;
  friend class CounterActionFactory;
};

class DefaultCAction : public CounterAction {
public:
  virtual void operator()( const int *parameterValue,
                           int *      counterValue ) const {
    for ( auto e : relations ) {
      counterValue[ e.first ] = 0;
      for ( auto ee : e.second ) {
        counterValue[ e.first ] += ee.first * parameterValue[ ee.second ];
      }
    }
  }

private:
  DefaultCAction( vector<pair<int, vector<pair<int, int>>>> &relations1 ) {
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
    pdata.addPointer( STRING( SimpleCounterAction ), re );
    return re;
  }

  SimpleCounterPAction *createSimpleCounterPAction( int cid, int v ) {
    SimpleCounterPAction *re = new SimpleCounterPAction( cid, v );
    pdata.addPointer( STRING( SimpleCounterPAction ), re );
    return re;
  }

  DefaultCAction *createDefaultCAction(
      vector<pair<int, vector<pair<int, int>>>> &relations1 ) {
    DefaultCAction *re = new DefaultCAction( relations1 );
    pdata.addPointer( STRING( DefaultCAction ), re );
    return re;
  }
  void destroy() {
    deleteType( SimpleCounterAction );
    deleteType( SimpleCounterPAction );
    deleteType( DefaultCAction );
    pdata.clear();
  }

private:
  PointerData pdata;
};

} // namespace graphsat
#endif
