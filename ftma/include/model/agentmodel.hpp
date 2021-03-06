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

#include <map>
#include <vector>

#include "function.h"
#include "graph/graph.hpp"
#include "location.h"
#include "templatemodel.hpp"
#include "to_real.h"
#include "transition.h"
#include "util/strutil.h"

namespace graphmodel {
using namespace raptor;

template <typename L, typename T>
class AgentSystem;

template <typename L, typename T>
class Agent : public VariableMap, public TOReal {
 private:
  typedef Agent<L, T> Agent_t;
  typedef AgentTemplate<L, T> AgentTemplate_t;

 public:
  virtual ~Agent() {}

  std::string getName(void) const {
    if (agent_tempate->agents.size() == 1) {
      return agent_tempate->getName();
    }
    return agent_tempate->getName() + "[" + to_string(id) + "]";
  }
  std::string getTemplateName(void) const { return agent_tempate->getName(); }

  void findRhs(const int link, const int lhs, int& rhs) const {
    graph.findRhs(link, lhs, rhs);
  }

  std::map<int, int> getClockMaxValue() const { return clock_max_value; }

  int getClockNumber() const { return agent_tempate->getClockNumber(); }

  int getChannelNumber() const { return agent_tempate->getChannelNumber(); }

  int getInitialLoc() const { return agent_tempate->initial_loc; }

  bool locationRun(int loc, const DBMManager& manager, int* D) const {
    if (!locations[loc].isReachable(manager, D)) {
      return false;
    }
    locations[loc](manager, D);
    return true;
  }

  bool isCommit(int id) const { return locations[id].isCommit(); }

  std::string getLocationName(int node_id) const {
    return locations[node_id].getName();
  }
  int getLocationID(const string& name) const {
    for (size_t i = 0; i < locations.size(); i++) {
      if (locations[i].getName() == name) {
        return i;
      }
    }
    return NOT_FOUND;
  }

  int getKeyID(const TYPE_T type, const std::string& key) const {
    int temp_var_id = agent_tempate->getLocalKeyID(type, key);
    if (temp_var_id > -1) {
      int start_loc = agent_tempate->getStart(type);
      start_loc += agent_tempate->getTypeNumber(type) * (id);
      start_loc += temp_var_id;
      return start_loc;
    } else {
      return agent_tempate->getParentKeyID(type, key);
    }
  }
  const vector<int>& getOutTransition(const int v) const { return outlinks[v]; }

  int* getValue(const TYPE_T type, int* state, const std::string& key) const {
    return state + getKeyID(type, key);
  }

  TypeDefArray getTypeDef(const std::string& name) const {
    return agent_tempate->getTypeDef(name);
  }

  TYPE_T getType(const std::string& name) const {
    return agent_tempate->getType(name);
  }

  const shared_ptr<AgentTemplate_t> getTemplate() const {
    return agent_tempate;
  }

  void setSelect(const int v) { parameter.setSelect(v); }

  RealArgument to_real(const TYPE_T& type, const Argument& arg) const {
    RealArgument re;
    re.setType(arg.getType());

    switch (re.getType()) {
      case CONST_ARG:
        re.setValue(arg.getValue());

        break;
      case NORMAL_VAR_ARG:
        re.setValue(getKeyID(type, arg.getName()));

        break;

      case PARAMETER_ARG:
        re.setValue(parameter.getParameter(arg.getName()));

        break;
      case REF_PARAMETER_ARG:
        re.setValue(parameter.getCounter(arg.getName()));

        break;
      case FUN_POINTER_ARG:
        loadFun(FUN_POINTER_ARG, arg.getName(), re);
        break;
      case SELECT_VAR_ARG:
        re.setValue(parameter.getSelect());

        break;
      case EMPTY_ARG:
        break;
    }
    if (arg.getIndex() != nullptr) {
      re.setIndex(to_real(type, *(arg.getIndex())));
    }

    return re;
  }

  virtual CHANNEL_TYPE getChanType(const std::string& chan_name) const {
    return agent_tempate->getChanType(chan_name);
  }

  shared_ptr<Function> getFun(const std::string& fun_name) const {
    if (fun_map.find(fun_name) != fun_map.end()) {
      return fun_map.at(fun_name);
    }
    return agent_tempate->getSYSFun(fun_name);
  }

  bool hasUrgentCh() const { return hasUrgentChan; }

  bool hasBroadcaseCh() const { return hasBroadcaseChan; }

  // void toDot(ostream &out ) const{
  //   out<<"digraph G { "<<endl;

  //   for(auto e : transitions ){
  //       std::string source=locations[ e.source].
  //   }

  // }

 private:
  void initFuns() {
    const std::map<std::string, shared_ptr<Function>>& funs =
        agent_tempate->getFuns();
    for (auto& e : funs) {
      shared_ptr<Function> dummy((e.second->copy()));
      setFun(e.first, dummy);
    }
  }
  void loadFun(const ARGUMENT_TYPE type, const std::string& name,
               RealArgument& re) const {
    std::string fun_name = getFunName(name);
    re.setValue((int_fast64_t)(getFun(fun_name).get()));

    std::string var = getFunArg(name);
    if (var != "") {  // If the function has argument then let the argument as
      // select variable
      re.setIndex(RealArgument(SELECT_VAR_ARG, parameter.getSelect()));
    }
  }

  void initFunction(const shared_ptr<Function>& fun) const {
    vector<std::string> int_vars = agent_tempate->getKeys(INT_T);
    for (auto& e : int_vars) {
      (*fun)[e] = getKeyID(INT_T, e);
    }
  }

  void setFun(const std::string& name, const shared_ptr<Function>& fun) {
    initFunction(fun);
    if (fun_map.find(name) != fun_map.end()) {
      assert(false && "It does not allow two functions has same name.");
    }
    fun_map[name] = fun;
  }

  void initial() {
    vector<int> srcs;
    vector<int> snks;

    for (auto& t : transitions) {
      srcs.push_back(t.getSource());
      snks.push_back(t.getTarget());
    }

    graph.initial(srcs, snks);

    outlinks.clear();
    int vertex_num = graph.getVertex_num();
    for (int i = 0; i < vertex_num; i++) {
      vector<int> outs = graph.getAdj(i);
      outlinks.push_back(outs);
    }

    // // There are no edges connect with  initial location
    if (!transitions.empty()) {
      assert(initial_loc >= 0 && initial_loc < graph.getVertex_num());
    }

    difference_cons.clear();

    clock_max_value.clear();

    for (auto& loc : locations) {
      const vector<ClockConstraint>& invariants = loc.getInvarients();
      for (auto& cs : invariants) {
        updateUpperAndDiff(cs);
      }
    }

    for (auto& t : transitions) {
      const vector<ClockConstraint>& gurads = t.getGuards();
      for (auto& cs : gurads) {
        updateUpperAndDiff(cs);
      }
      if (t.hasChannel()) {
        if (t.getChannel().getType() == URGENT_CH) {
          hasUrgentChan = true;
        } else if (t.getChannel().getType() == BROADCAST_CH) {
          hasBroadcaseChan = true;
        }
      }
    }
    for (size_t i = 0; i < locations.size(); i++) {
      vector<int> outs = graph.getAdj(i);
      for (auto link : outs) {
        if (transitions[link].hasChannel()) {
          if (transitions[link].getChannel().getType() == URGENT_CH) {
            locations[i].setHasOutUrgentCh(true);
          } else if (transitions[link].getChannel().getType() == BROADCAST_CH &&
                     transitions[link].getChannel().isSend()) {
            locations[i].setHasOutBreakcastSendCh(true);
          } else if (transitions[link].getChannel().getType() == ONE2ONE_CH) {
            locations[i].setHasNormalCh(true);
          }
        }
      }
    }
  }

  void updateUpperAndDiff(const ClockConstraint& cs) {
    if (cs.clock_x > 0 && cs.clock_y > 0) {
      difference_cons.push_back(cs);
    }
    int realRhs = getRight(cs.matrix_value);
    if (cs.clock_x > 0) {
      if (realRhs > clock_max_value[cs.clock_x]) {
        clock_max_value[cs.clock_x] = realRhs;
      }
    }

    if (cs.clock_y > 0) {
      if (-realRhs > clock_max_value[cs.clock_y]) {
        clock_max_value[cs.clock_y] = -realRhs;
      }
    }
  }

  shared_ptr<AgentTemplate_t> agent_tempate;
  std::map<std::string, shared_ptr<Function>> fun_map;
  Parameter parameter;
  int id;  // the interbal of instance of ta_tempate
 public:
  Graph_t<int> graph;  // topology

  std::vector<L> locations;
  std::vector<T> transitions;

 private:
  std::vector<ClockConstraint> difference_cons;
  std::vector<std::vector<int>> outlinks;

  int initial_loc;
  std::map<int, int> clock_max_value;

  bool hasUrgentChan;
  bool hasBroadcaseChan;

  template <typename R1>
  friend class Reachability;

  template <typename L1, typename T1>
  friend class AgentSystem;

  Agent(const shared_ptr<AgentTemplate_t>& template_arg, const Parameter& param)
      : parameter(param) {
    agent_tempate = template_arg;
    initial_loc = template_arg->initial_loc;
    id = template_arg->agents.size();
    template_arg->agents.push_back(this);
    hasUrgentChan = false;
    hasBroadcaseChan = true;
  }
};

}  // namespace graphmodel
#endif
