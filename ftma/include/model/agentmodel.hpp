/**
 * @file   agentmodel.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec  6 20:00:07 2019
 *
 * @brief  agent model
 *
 *
 */
#ifndef AGENT_MODEL_H
#define AGENT_MODEL_H
#include "function.h"
#include "templatemodel.hpp"
#include "to_real.h"

namespace graphsat {

template <typename L, typename T> class AgentSystem;

template <typename L, typename T>
class Agent : public VariableMap, public TOReal {

private:
  typedef Agent<L, T> Agent_t;
  typedef AgentTemplate<L, T> AgentTemplate_t;

public:
  Agent(const shared_ptr<AgentTemplate_t> &template_arg, const Parameter &param)
      : parameter(param) {
    agent_tempate = template_arg;
    for (auto e : template_arg->template_transitions) {
      transitions.push_back(T(this, e, param));
    }

    locations = template_arg->template_locations;
    difference_cons = template_arg->template_difference_cons;
    id = template_arg->agents.size();
    template_arg->agents.push_back(this);
  }

  void findRhs(const int link, const int lhs, int &rhs) const {
    agent_tempate->graph.findRhs(link, lhs, rhs);
  }

  vector<int> getClockMaxValue() const {
    return agent_tempate->clock_max_value;
  }

  int getClockNumber() const { return agent_tempate->getClockNumber(); }

  int getChannelNumber() const { return agent_tempate->getChannelNumber(); }

  int getInitialLoc() const { return agent_tempate->initial_loc; }

  bool transitionRun(int link, const DBMFactory &manager, int *D) const {
    return transitions[link](manager, D);
  }

  bool locationRun(int link, const DBMFactory &manager, int *D) const {
    if (!locations[link].isReachable(manager, D)) {
      return false;
    }
    locations[link](manager, D);
    return true;
  }

  bool isCommit(int id) const { return locations[id].isCommit(); }

  void updateCounterId(const map<int, int> &id_map) {
    for (auto e : transitions) {
      for (auto ee : e.counter_cons) {
        ee->counterIpMap(id_map);
      }
      for (auto aa : e.actions) {
        aa->counterIpMap(id_map);
      }
    }
  }
  string getLocName(int node_id) const { return locations[node_id].getName(); }

  int getStart(const TYPE_T type, const string &key) const {
    int start_loc = agent_tempate->getStart(type);
    start_loc += agent_tempate->getTypeNumber(type) * (id);
    start_loc += agent_tempate->getKeyStart(type, key);
    return start_loc;
  }
  int getSYSStart(const TYPE_T type, const string &key) const {
    return agent_tempate->getSYSStart(type, key);
  }

  int *getValue(const TYPE_T type, int *state, const string &key) const {
    return state + getStart(type, key);
  }

  const shared_ptr<AgentTemplate_t> getTemplate() const {
    return agent_tempate;
  }

  RealArgument to_real(const TYPE_T &type, const Argument &arg) const {

    RealArgument re;
    re.type = arg.type;
    re.name = arg.name;
    switch (re.type) {
    case CONST_ARG:
      re.value = arg.value;
      break;
    case TEMPLATE_VAR_ARG:
      re.value = getStart(type, arg.name);
      break;
    case SYSTEM_VAR_ARG:
      re.value = getSYSStart(type, arg.name);
      break;
    case PARAMETER_ARG:
      re.value = parameter.getParameter(arg.value);
      break;
    case REF_PARAMETER_ARG:
      re.value = parameter.getCounter(arg.value);
      break;
    case TEMPALTE_FUN_POINTER_ARG:
      re.value = loadFun(TEMPALTE_FUN_POINTER_ARG, re.name);
      break;
    case SYSTEM_FUN_POINTER_ARG:
      re.value = loadFun(SYSTEM_FUN_POINTER_ARG, re.name);
      break;
    case SELECT_VAR_ARG:
      re.value = parameter.getSelect();
      break;
    case EMPTY_ARG:
      break;
    }
    if (arg.index != nullptr) {
      re.index.reset(new RealArgument(to_real(type, *arg.index)));
    }

    return re;
  }

  shared_ptr<Function> getFun(const string &name) const { return fun_map.at(name); }

  // void toDot(ostream &out ) const{
  //   out<<"digraph G { "<<endl;

  //   for(auto e : transitions ){
  //     string source=locations[ e.source].
  //   }

  // }

private:

  void initFuns( ){
    const map<string, shared_ptr<Function> >& funs=agent_tempate->getFuns( );
    for( auto& e: funs ){
      shared_ptr<Function> dummy((e.second->copy( ))) ;
      setFun( e.first, dummy );
    }
  }
  int_fast64_t loadFun(const ARGUMENT_TYPE type, const string &name) const {
    if(TEMPALTE_FUN_POINTER_ARG==type ){
      return (int_fast64_t)(getFun( name).get( ));
    }else if( SYSTEM_FUN_POINTER_ARG==type){
      return (int_fast64_t)agent_tempate->getSYSFun( name);
    }
    // TODO:
    return 0;
  }

  void initFunction(const shared_ptr<Function> &fun) const {
    vector<string> int_vars = agent_tempate->getKeys(INT_T);
    for (auto &e : int_vars) {
      (*fun)[e] = getStart(INT_T, e);
    }
  }

  void setFun(const string &name, const shared_ptr<Function> &fun) {
    initFunction(fun);
    if (fun_map.find(name) != fun_map.end()) {
      assert(false && "It does not allow two functions has same name.");
    }
    fun_map[name] = fun;
  }



  shared_ptr<AgentTemplate_t> agent_tempate;
  map<string, shared_ptr<Function>> fun_map;
  Parameter parameter;
  int id; // the interbal of instance of ta_tempate

  vector<L> locations;
  vector<T> transitions;
  vector<ClockConstraint> difference_cons;

  template <typename R1> friend class Reachability;

  template <typename L1, typename T1> friend class AgentSystem;
};

} // namespace graphsat
#endif
