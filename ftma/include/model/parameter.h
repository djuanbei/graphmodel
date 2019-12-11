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

#include "util/typedef.h"

namespace graphsat {
using std::map;
using std::vector;

class UppaalParser;

class Parameter {

public:
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
  int getCounter( const int id) const{
    return ref_parameter_id_map[ id];
  }
  const vector<int> &getParameterMap( void ) const { return parameter_value; }
  int getParameter( const int id) const{
    return parameter_value[ id];
  }

  const vector<int> &getChanMap( void ) const { return chan_map; }
  int getChan( const int id) const{
    return chan_map[ id];
  }
  void setSelect( const int v){
    select_value=v;
  }
  
  int getSelect( ) const{
    return select_value;
  }

private:
  Parameter( int n )
      : ref_parameter_id_map( n, NO_DEF )
      , parameter_value( n, NO_DEF )
      , chan_map( n, NO_DEF ) {}

  vector<int> ref_parameter_id_map;
  vector<int> parameter_value;
  vector<int> chan_map;
  int select_value;

  template <typename L1, typename T1> friend class AgentTemplate;
  template <typename L1, typename T1> friend class Agent;
  template <typename R1> friend class Reachability;
  friend class UppaalParser;
};
} // namespace graphsat

#endif
