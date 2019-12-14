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
  void setCounterMap(const string &key, int value) {
    assert( contain( key));
    ref_parameter_id_map[key] = value;
  }
  void setParameterMap(const string &key, int value)  {
    assert( contain( key));
    parameter_value[key] = value;
  }

  void setChanMap(const string &key, int value)  {
    assert( contain( key));
    chan_map[key] = value;
  }

  int getCounter(const string & key) const {

    return ref_parameter_id_map.at( key); }

  int getParameter(const string &key) const { return parameter_value.at( key); }


  int getChan(const string &key) const { return chan_map.at( key); }
  void setSelect(const int v) { select_value = v; }

  int getSelect() const { return select_value; }
  
  bool contain( const string& key) const{
    return  find(parameters.begin( ), parameters.end( ), key )!= parameters.end( );
  }

private:
  Parameter( const vector<string> & ps):parameters( ps){
    
  }

  vector<string> parameters;
  map<string,int> ref_parameter_id_map;
  map<string, int> parameter_value;
  map<string, int> chan_map;
  int select_value;

  template <typename L1, typename T1> friend class AgentTemplate;
  template <typename L1, typename T1> friend class Agent;
  template <typename R1> friend class Reachability;
  friend class UppaalParser;
};
} // namespace graphsat

#endif
