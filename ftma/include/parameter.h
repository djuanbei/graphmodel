/**
 * @file   parameter.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat May 18 11:36:23 2019
 *
 * @brief
 *
 *
 */

#ifndef __PARAMETER_H
#define __PARAMETER_H
#include <map>
#include <vector>
namespace graphsat {
using std::map;
using std::vector;
class Parameter {

public:
  Parameter() {}
  void setCounterMap( int local_id, int global_id ) {
    counter_map[ local_id ] = global_id;
  }
  void setChanMap( int local_id, int global_id ) {
    chan_map[ local_id ] = global_id;
  }

  const map<int, int> &getCounterMap( void ) const { return counter_map; }
  const map<int, int> &getChanMap( void ) const { return chan_map; }
  void addParameterValue( int v ) { paramter_value.push_back( v ); }

  const vector<int> &getParameterValue() const { return paramter_value; }

private:
  map<int, int> counter_map;
  map<int, int> chan_map;
  vector<int>   paramter_value;
};
} // namespace graphsat

#endif
