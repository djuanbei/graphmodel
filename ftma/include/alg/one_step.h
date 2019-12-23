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
#include <vector>

#include "property/property.h"

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
bool doOneStep(D &data, const std::shared_ptr<const M> &manager,
               const Property *prop, const State_t *const state,
               vector<OneStep> &steps) {

  for (auto &e : steps) {
    State_t *cache_state = manager->newState(state);
    const std::vector<OneStep::Action> &actions = e.getAction();
    for (auto &a : actions) {
      switch (a.action) {
      case OneStep::DISCRETE_JUMP:

        break;
      case OneStep::CONTINUED_EVOLUTION:
        break;
      }
    }
    manager->destroyState(cache_state);
  }
  return false;
}

template <typename M, typename State_t>
void doDiscreteJump(const std::shared_ptr<const M> &manager, State_t *state,
                    const OneStep::Action &action) {
  int component = action.component;
  int link = action.transition;
}

} // namespace graphsat

#endif
