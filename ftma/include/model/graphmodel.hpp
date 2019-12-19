/**
 * @file   TA.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:25:54 2019
 *
 * @brief  a model for timed automata
 *
 *
 */

#ifndef __GRAPH_MODEL_
#define __GRAPH_MODEL_
#include <vector>

#include "action/counteraction.h"
#include "channel.h"
#include "counter.h"

#include "domain/dbmset.hpp"

#include "parameter.h"
#include "state/discretestate.hpp"

#include "agentmodel.hpp"

#include "templatemodel.hpp"

#include "vardecl.h"

namespace graphsat {

using std::vector;

template <typename L, typename T> class AgentSystem : public VarDecl {

public:
  typedef typename T::State_t State_t;

  typedef typename T::StateManager_t StateManager_t;
  typedef StateSet<State_t> StateSet_t;
  typedef ClockConstraint CS_t;

  typedef L L_t;

  typedef T T_t;

  typedef Agent<L, T> Agent_t;
  typedef AgentTemplate<L, T> AgentTemplate_t;

  AgentSystem() {
    clock_max_value[0] = 0;
    counter_num = chan_num = 0;
  }
  virtual ~AgentSystem() {}

  shared_ptr<AgentTemplate_t> createTemplate() {
    shared_ptr<AgentTemplate_t> re(new AgentTemplate_t(this));
    re->setParent(this);
    re->id = templates.size();
    templates.push_back(re);
    return re;
  }

  AgentSystem &operator+=(shared_ptr<Agent_t> &agent) {

    agents.push_back(agent);

    return *this;
  }

  void removeAgent() {
    agents.clear();
    for (auto &e : templates) {
      e->reset();
    }
  }

  int getComponentNumber() const { return (int)agents.size(); }

  int getChanNum() const { return chan_num; }

  const vector<ClockConstraint> &getDiffCons() const { return difference_cons; }

  shared_ptr<StateManager_t> getStateManager() const { return stateManager; }

  struct AgentCMP {
    bool operator()(const shared_ptr<Agent_t> &lhs,
                    const shared_ptr<Agent_t> &rhs) const {
      if (lhs->agent_tempate->id < rhs->agent_tempate->id) {
        return true;
      }
      if (lhs->agent_tempate->id > rhs->agent_tempate->id) {
        return false;
      }
      return (lhs->id < rhs->id);
    }
  };

  void build() {
    AgentCMP cmp;
    sort(agents.begin(), agents.end(), cmp);
    counter_num = getTypeNumber(INT_T);
    chan_num = getTypeNumber(CHAN_T);
    difference_cons.clear();

    for (auto &e : agents) {
      transfrom(e);
    }
    int clock_num = 1;
    for (auto &e : clock_max_value) {
      if (e.first > clock_num) {
        clock_num = e.first;
      }
    }

    vector<int> temp_clock_upperbound(2 * clock_num + 2, 0);

    for (int i = 0; i < clock_num + 1; i++) {
      temp_clock_upperbound[i] = getMatrixValue(clock_max_value[i], false);
    }

    for (int i = 0; i < clock_num + 1; i++) {
      temp_clock_upperbound[i + clock_num + 1] =
          getMatrixValue(-clock_max_value[i], true);
    }
    vector<int> node_n;
    for (size_t i = 0; i < agents.size(); i++) {
      node_n.push_back(agents[i]->graph.getVertex_num());
    }
    vector<int> link_num;
    for (auto &e : agents) {
      link_num.push_back(e->graph.getLink_num());
    }
    vector<Counter> counters;
    vector<BaseDecl> sysCounts = getInts();
    for (auto &e : sysCounts) {
      Counter counter(e.low, e.high);
      counters.push_back(counter);
    }

    for (auto &e : agents) {
      vector<BaseDecl> counts = e->agent_tempate->getInts();
      for (auto &ee : counts) {
        Counter counter(ee.low, ee.high);
        counters.push_back(counter);
      }
    }

    stateManager.reset(new StateManager_t(
        *this, counters, clock_num, temp_clock_upperbound, node_n, link_num));
  }

  template <typename D> void addInitState(D &data) const {
    State_t *state = stateManager->newState();
    int component_num = (int)agents.size();
    bool withoutCommit = true;
    for (int component = 0; component < component_num; component++) {
      state[component] = initial_loc[component];
      if (agents[component]->isCommit(state[component])) {
        stateManager->setCommitState(component, state);
        withoutCommit = false;
      }
    }
    if (withoutCommit) {
      for (int component = 0; component < component_num; component++) {
        agents[component]->locationRun(initial_loc[component],
                                       stateManager->getClockManager(),
                                       stateManager->getDBM(state));
      }
    }

    for (int component = 0; component < component_num; component++) {
      agents[component]
          ->locations[stateManager->getLocationID(component, state)]
          .employInvariants(stateManager->getClockManager(),
                            stateManager->getDBM(state));
    }
    if (stateManager->getClockManager().isConsistent(
            stateManager->getDBM(state))) {
      stateManager->norm(stateManager->getDBM(state));
      data.add(state);
    }
    stateManager->destroyState(state);
  }


  virtual vector<BaseDecl> getAllVar( const TYPE_T type) const{
    vector<BaseDecl> re=VarDecl::getAllVar( type);
    int id_start=0;
    for( auto &e: re){
      e.start_loc=id_start;
      id_start+=e.num;
    }
    for(auto &e :agents ){
      vector<BaseDecl> dummy=e->agent_tempate->getAllVar( type);
      for( auto &ee: dummy){
        ee.start_loc=id_start;
        id_start+=ee.num;
        re.push_back( ee);
      }
    }
    return re;
  }

  int getTypeStart(const TYPE_T type) const {
    // clock and channel id start with 1
    if (CLOCK_T == type || CHAN_T == type) {
      return 1;
    }
    return 0;
  }

  int getStartLoc(const TYPE_T type, const int template_id) const {
    int re = getTypeStart(type);
    for (auto &agent : agents) {
      if (agent->getTemplate()->id < template_id) {
        re += agent->getTemplate()->getTypeNumber(type);
      }
    }
    return re;
  }

  virtual Argument addClock(const string &n) {
    Argument dummy = VarDecl::addClock(n);
    dummy.type = NORMAL_VAR_ARG;
    return dummy;
  }

  int getSrc(const int component, const int link) const {
    int re = 0;
    agents[component]->graph.findSrc(link, re);
    return re;
  }

  string getLocationName(const int component, const int loc_ID) const {
    return agents[component]->getLocationName(loc_ID);
  }

  vector<int> getOutTransition(const int component, const int src) const {
    return agents[component]->graph.getAdj(src);
  }

  bool transitionReady(const int component, const int link,
                       const int *const state) const {
    return agents[component]->transitions[link].ready(component, stateManager,
                                                      state);
  }

  const Channel &getChan(const int component, const int link) const {
    return agents[component]->transitions[link].getChannel();
  }

private:
  void transfrom(shared_ptr<Agent_t> &agent) {

    agent->initFuns();
    agent->locations = agent->agent_tempate->template_locations;
    for (auto &e : agent->locations) {
      e.to_real(agent);
    }

    agent->transitions.clear();
    for (size_t i = 0; i < agent->agent_tempate->template_transitions.size();
         i++) {
      if (agent->agent_tempate->template_transitions[i].isSelect()) {
        T dummy(agent->agent_tempate->template_transitions[i]);
        TypeDefArray select_domain = agent->getType(dummy.getSelectCollect());
        for (int i = select_domain.getLow(); i <= select_domain.getHigh();
             i++) {
          agent->setSelect(i);
          dummy.to_real(agent);
          agent->transitions.push_back(dummy);
        }
      } else {
        T dummy(agent->agent_tempate->template_transitions[i]);
        dummy.to_real(agent);
        agent->transitions.push_back(dummy);
      }
    }

    agent->initial();

    chan_num += agent->getChannelNumber();

    initial_loc.push_back(agent->getInitialLoc());
    map<int, int> temp_max = agent->getClockMaxValue();
    for (auto &e : temp_max) {
      if (clock_max_value[e.first] < e.second) {
        clock_max_value[e.first] = e.second;
      }
    }

    difference_cons.insert(difference_cons.end(),
                           agent->difference_cons.begin(),
                           agent->difference_cons.end());
  }

  /**
   * multi-components
   *
   */
  vector<shared_ptr<AgentTemplate_t>> templates;

  vector<shared_ptr<Agent_t>> agents;

  int counter_num;
  int chan_num;

  vector<int> initial_loc;

  map<int, int> clock_max_value;
  vector<ClockConstraint> difference_cons;

  shared_ptr<StateManager_t> stateManager;
  template <typename TT> friend class Reachability;
};

} // namespace graphsat

#endif
