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
using std::vector;
using std::pair;
class CounterAction {
public:
  virtual void operator()( const Parameter &p, int *value ) const = 0;
};

class DefaultCAction : public CounterAction {
public:
  DefaultCAction( vector<pair<int, vector<pair<int, int>>>> &relations1 ) {
    relations = relations1;
  }
  virtual void operator()( const Parameter &p, int *value ) const {
    for ( auto e : relations ) {
      value[ e.first ] = 0;
      for ( auto ee : e.second ) {
        value[ e.first ] += ee.first * p.getValue( ee.second );
      }
    }
  }

private:
  vector<pair<int, vector<pair<int, int>>>> relations;
};

} // namespace graphsat
#endif
