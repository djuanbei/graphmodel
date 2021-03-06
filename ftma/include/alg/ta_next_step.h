/**
 * @file   ta_next_step.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec 20 13:58:26 2019
 *
 * @brief  timed automata next step
 *
 *
 */

#ifndef TA_NEXT_STEP_H
#define TA_NEXT_STEP_H
#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"
#include "next_step.h"

namespace graphmodel {
typedef AgentSystem<Location, Transition> INT_TAS_t;

class TANextStep : public NextStep {
 public:
  TANextStep(INT_TAS_t& s) : sys(s) {
    manager = s.getStateManager();
    component_num = manager->getComponentNumber();
  }
  virtual std::vector<OneStep> getNextStep(void* state) const;

 private:
  const INT_TAS_t& sys;
  shared_ptr<const typename INT_TAS_t::StateManager_t> manager;
  int component_num;

  void doCommit(int* state, std::vector<OneStep>& re) const;

  int getCommitCount(const int component, const int link, int count) const;

  void doCommitComponent(int* state, int component,
                         std::vector<OneStep>& re) const;

  void discrete(const int* const state, std::vector<pair<int, int>>& paths,
                std::vector<OneStep>& re) const;

  void doNormal(int* state, std::vector<OneStep>& re) const;

  /**
   * @brief  Urgent channels are similar to regular channels, except that it is
   * not possible to delay in the source state if it is possible to trigger a
   * synchronisation over an urgent channel. Notice that clock guards are not
   * allowed on edges synchronising over urgent channels
   *
   * @param state
   * @param re
   */
  void doUrgant(int* state, std::vector<OneStep>& re) const;

 private:
  /**
   * @brief Broadcast channels allow 1-to-many synchronisations. The intuition
   * is that an edge with synchronisation label e! emits a broadcast on the
   * channel e and that any enabled edge with synchronisation label e? will
   * synchronise with the emitting process. I.e. an edge with an
   * emit-synchronisation on a broadcast channel can always fire (provided that
   * the guard is satisfied), no matter if any receiving edges are enabled. But
   * those receiving edges, which are enabled will synchronise. Notice that
   * clock guards are not allowed on edges receiving on a broadcast channel. The
   * update on the emitting edge is executed first. The update on the receiving
   * edges are executed left-to-right in the order the processes are given in
   * the system definition.
   *
   * @param state
   * @param re
   */
  void doBroadcast(int* state, std::vector<OneStep>& re) const;
};

}  // namespace graphmodel

#endif
