/**
 * @file   one_step.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec 20 13:24:53 2019
 *
 * @brief  transition run one step
 *
 *
 */
#ifndef _ONE_STEP_H
#define _ONE_STEP_H
#include <cassert>
#include <vector>

#include "util/typedef.h"

namespace graphsat {

class OneStep {
public:
  enum Action_type { DISCRETE_JUMP = 0, CONTINUED_EVOLUTION = 1 };

  struct Action {
    Action()
        : component(0), location(0), transition(0), action(DISCRETE_JUMP),
          parameter(0) {}
    Action(int comp, int loc, int trans)
        : component(comp), location(loc), transition(trans),
          action(DISCRETE_JUMP), parameter(0) {}
    int component;
    int location;
    int transition;

    Action_type action;
    int parameter;
  };
  const std::vector<Action> &getAction() const { return actions; }
  void addAction(const Action &a) { actions.push_back(a); }

private:
  std::vector<Action> actions;
};

template <typename D, typename M, typename State_t>
Check_State doOneStep(D *data, const M *manager, const State_t *const state,
                      std::vector<OneStep> &steps) {

  for (auto &e : steps) {
    State_t *cache_state = manager->newState(state);
    bool b = true;
    const std::vector<OneStep::Action> &actions = e.getAction();
    assert(!actions.empty() && "At least contain one step.");
    for (std::vector<OneStep::Action>::const_iterator it = actions.begin();
         b && (it != actions.end()); it++) {
      switch (it->action) {
      case OneStep::DISCRETE_JUMP:
        b = doDiscreteJump(manager, it->component, it->transition, cache_state);
        break;
      case OneStep::CONTINUED_EVOLUTION:
        b = doEvolution(manager, it->component, it->location, cache_state);
        break;
      }
    }
    if (b) {
      Check_State re = data->add(cache_state);
      if (re == TRUE) {
        return re;
      }
    }
    manager->destroyState(cache_state);
  }
  return UNKOWN;
}

/**
 *
 *
 * @param manager
 * @param state
 * @param action
 *
 * @return  True if this action is defined, False otherwise.
 */
template <typename M, typename State_t>
bool doDiscreteJump(const M *manager, int component, int link, State_t *state) {

  if (manager->transitionReady(component, link, state)) {

    manager->discretRun(component, link, state);
    return true;
  }

  return false;
}

template <typename M, typename State_t>
bool doEvolution(const M *manager, const int component, int loc,
                 State_t *state) {
  if (manager->isReachable(component, loc, state)) {
    state[component] = loc;
    manager->evolution(component, loc, state);
    for (int component_id = 0; component_id < manager->getComponentNum();
         component_id++) {
      manager->employLocInvariants(component_id, state);
    }
    return true;
  }
  return false;
}

} // namespace graphsat

#endif
