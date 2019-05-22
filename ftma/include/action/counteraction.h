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
#include "counter.hpp"
#include "parameter.h"
#include <vector>

namespace graphsat {
using std::pair;
using std::vector;
class CounterAction {
public:
  virtual void operator()( const Parameter &p, int *counterValue ) const = 0;
};

class SimpleCounterAction : public CounterAction {
public:
  SimpleCounterAction( int cid, int v ) {
    counter_id = cid;
    rhs        = v;
  }
  virtual void operator()( const Parameter &p, int *counterValue ) const {
    counterValue[ counter_id ] = rhs;
  }

private:
  int counter_id;
  int rhs;
};

class SimpleCounterPAction : public CounterAction {
public:
  SimpleCounterPAction( int cid, int v ) {
    counter_id = cid;
    p_id       = v;
  }
  virtual void operator()( const Parameter &p, int *counterValue ) const {
    counterValue[ counter_id ] = p.getValue( p_id );
  }

private:
  int counter_id;
  int p_id;
};

class DefaultCAction : public CounterAction {
public:
  DefaultCAction( vector<pair<int, vector<pair<int, int>>>> &relations1 ) {
    relations = relations1;
  }
  virtual void operator()( const Parameter &p, int *counterValue ) const {
    for ( auto e : relations ) {
      counterValue[ e.first ] = 0;
      for ( auto ee : e.second ) {
        counterValue[ e.first ] += ee.first * p.getValue( ee.second );
      }
    }
  }

private:
  vector<pair<int, vector<pair<int, int>>>> relations;
};

} // namespace graphsat
#endif
