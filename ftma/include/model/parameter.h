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
const static int NO_DEF = -1;
class Parameter {

public:
  Parameter( int n )
      : ref_parameter_id_map( n, NO_DEF )
      , parameter_value( n, NO_DEF )
      , chan_map( n, NO_DEF ) {}
  void setCounterMap( int local_id, int global_id ) {
    ref_parameter_id_map[ local_id ] = global_id;
  }
  void setParameterMap( int local_id, int value ) {
    parameter_value[ local_id ] = value;
  }

  void setChanMap( int local_id, int global_id ) {
    chan_map[ local_id ] = global_id;
  }

  const vector<int> &getCounterMap( void ) const {
    return ref_parameter_id_map;
  }
  const vector<int> &getParameterMap( void ) const { return parameter_value; }

  const vector<int> &getChanMap( void ) const { return chan_map; }

private:
  vector<int> ref_parameter_id_map;
  vector<int> parameter_value;
  vector<int> chan_map;
};
} // namespace graphsat

#endif
