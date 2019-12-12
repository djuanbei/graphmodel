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
#include "templatemodel.hpp"

namespace graphsat {

template <typename L, typename T> class AgentSystem;

template <typename L, typename T> class Agent : public VariableMap {

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
  int getSYSStart( const TYPE_T type, const string & key) const{
    return agent_tempate->getSYSStart( type, key);
  }

  int *getValue(const TYPE_T type, int *state, const string &key) const {
    return state + getStart(type, key);
  }

  const shared_ptr<AgentTemplate_t> getTemplate() const {
    return agent_tempate;
  }

  RealArgument to_real(TYPE_T type, const Argument &arg) const {

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
    case FUN_POINTER_ARG:
      re.value = loadFun(re.name);
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

  // void toDot(ostream &out ) const{
  //   out<<"digraph G { "<<endl;

  //   for(auto e : transitions ){
  //     string source=locations[ e.source].
  //   }

  // }

private:
  int_fast64_t loadFun(const string &name) const {
    // TODO:
    return 0;
  }

  shared_ptr<AgentTemplate_t> agent_tempate;
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
