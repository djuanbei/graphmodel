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

  if (manager->hasDiffCons()) {

    for (auto &e : steps) {
      State_t *cache_state = manager->newState(state);
      vector<State_t *> waitS;
      vector<State_t *> tempSet;
      waitS.push_back(cache_state);

      bool b = true;
      const std::vector<OneStep::Action> &actions = e.getAction();
      assert(!actions.empty() && "At least contain one step.");
      for (std::vector<OneStep::Action>::const_iterator it = actions.begin();
           b && (it != actions.end()); it++) {
        switch (it->action) {
        case OneStep::DISCRETE_JUMP:
          for (typename std::vector<State_t *>::iterator iit = waitS.begin();
               b && iit != waitS.end(); iit++) {
            b = doDiscreteJump(manager, it->component, it->transition, *iit);
          }
          break;
        case OneStep::CONTINUED_EVOLUTION:
          tempSet.clear();
          for (typename std::vector<State_t *>::iterator iit = waitS.begin();
               b && iit != waitS.end(); iit++) {
            if (it != actions.begin() &&
                !manager->hasMatchOutUrgentChan(cache_state)) {
              std::vector<State_t *> dummy =
                  doEvolution(manager, it->component, it->location, *iit);
              tempSet.insert(tempSet.end(), dummy.begin(), dummy.end());
            }
          }
          waitS.swap(tempSet);
          break;
        }
      }
      if (b) {
        for (auto e : waitS) {
          Check_State re = data->add(e);
          if (re == TRUE) {
            for (auto ee : waitS) {
              manager->destroyState(ee);
            }
            return re;
          }
        }
      }
      for (auto ee : waitS) {
        manager->destroyState(ee);
      }
    }

  } else {
    for (auto &e : steps) {

      State_t *cache_state = manager->newState(state);

      bool b = true;
      const std::vector<OneStep::Action> &actions = e.getAction();
      assert(!actions.empty() && "At least contain one step.");
      for (std::vector<OneStep::Action>::const_iterator it = actions.begin();
           b && (it != actions.end()); it++) {
        switch (it->action) {
        case OneStep::DISCRETE_JUMP:
          b = doDiscreteJump(manager, it->component, it->transition,
                             cache_state);
          break;
        case OneStep::CONTINUED_EVOLUTION:
          if (it != actions.begin() &&
              !manager->hasMatchOutUrgentChan(cache_state)) {
            doEvolution(manager, it->component, it->location, cache_state);
          }
          break;
        }
      }
      if (b) {
        Check_State re = data->add(cache_state);
        if (re == TRUE) {
          manager->destroyState(cache_state);
          return re;
        }
      }
      manager->destroyState(cache_state);
    }
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
vector<State_t *> doEvolution(const M *manager, const int component, int loc,
                              State_t *state) {
  assert(manager->isReachable(component, loc, state));
  assert(state[component] == loc);
  vector<State_t *> re = manager->evolution(component, loc, state);
  if (manager->hasDiffCons()) {
    for (auto &e : re) {
      for (int component_id = 0; component_id < manager->getComponentNum();
           component_id++) {
        manager->employLocInvariants(component_id, e);
      }
    }
  } else {
    for (int component_id = 0; component_id < manager->getComponentNum();
         component_id++) {
      manager->employLocInvariants(component_id, state);
    }
  }

  return re;
}

} // namespace graphsat

#endif
