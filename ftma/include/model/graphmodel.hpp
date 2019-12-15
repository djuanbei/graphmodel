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
    clock_max_value.push_back(0);
    counter_num = clock_num = chan_num = 0;
  }

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
    clock_num = getTypeNumber(CLOCK_T);
    chan_num = getTypeNumber(CHAN_T);
    difference_cons.clear();

    for (auto &e : agents) {
      transfrom(e);
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
      node_n.push_back(agents[i]->agent_tempate->graph.getVertex_num());
    }
    vector<int> link_num;
    for (auto &e : agents) {
      link_num.push_back(e->agent_tempate->graph.getLink_num());
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
        (int)agents.size(), counters, clock_num, temp_clock_upperbound,
        difference_cons, node_n, link_num, chan_num));
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
          ->locations[stateManager->getLoc(component, state)]
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

  int getStartLoc(const TYPE_T type, const int template_id) const {
    int re = 0; // stateManager->getStart(type);
    if (CLOCK_T == type) {
      re = 1;
    }
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

private:
  void transfrom(shared_ptr<Agent_t> &agent) {

    agent->initFuns();
    agent->locations=agent->agent_tempate->template_locations;
    for (auto &e : agent->locations) {
      e.to_real(agent);
    }

    //the clock guard can not contain select variable
    // for (size_t i = 0;
    //      i < agent->agent_tempate->template_difference_cons.size(); i++) {
    //   agent->difference_cons[i].to_real(agent);
    // }
    agent->transitions.clear( );
    for (size_t i = 0; i < agent->agent_tempate->template_transitions.size(); i++) {
      if(agent->agent_tempate->template_transitions[ i].isSelect( ) ){
        T dummy(agent->agent_tempate->template_transitions[ i] );
        TypeDefArray select_domain = agent->getType(dummy.getSelectCollect( ));
        for( int i=select_domain.getLow( ); i!= select_domain.getHigh( ); i++ ){
          agent->setSelect( i);
          dummy.to_real( agent);
          agent->transitions.push_back(dummy );
        }
      }else{
        T dummy(agent->agent_tempate->template_transitions[ i] );
        dummy.to_real( agent);
        agent->transitions.push_back(dummy );
      }
    }
    
    agent->initial( );
    
    clock_num += agent->getClockNumber();
    chan_num += agent->getChannelNumber();

    initial_loc.push_back(agent->getInitialLoc());
    vec_clock_nums.push_back(agent->getClockNumber());

    for (size_t i = 1; i < agent->getClockMaxValue().size(); i++) {
      clock_max_value.push_back(agent->getClockMaxValue()[i]);
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

  int clock_num;
  int counter_num;
  int chan_num;

  vector<int> initial_loc;
  vector<int> vec_clock_nums;

  vector<int> clock_max_value;
  vector<ClockConstraint> difference_cons;

  shared_ptr<StateManager_t> stateManager;

  template <typename R1> friend class Reachability;
};

} // namespace graphsat

#endif
