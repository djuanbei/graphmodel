
/**
 * @file   reachability.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Apr 27 08:49:40 2019
 *
 * @brief  reachability problem of timed automata
 *
 *
 */

#ifndef __REACHABILITY_HPP
#define __REACHABILITY_HPP

#include <cassert>
#include <map>
#include <random>
#include <set>
#include <vector>

#include "util/macrodef.h"
#include "util/parallel.h"

#include "model/channel.h"
#include "property/property.h"

#include "alg/ta_next_step.h"

namespace graphsat {

template <typename SYS>
class Reachability {
  typedef typename SYS::State_t State_t;

 public:
  Reachability(SYS& s) : sys(s), nextS(s) {
    component_num = sys.getComponentNumber();
    manager = s.getStateManager();

    cache_state = manager->newState();
    next_state = manager->newState();
  }

  ~Reachability() {
    if (cache_state != nullptr) {
      manager->destroyState(cache_state);
      cache_state = nullptr;
    }
    if (next_state != nullptr) {
      manager->destroyState(next_state);
      next_state = nullptr;
    }
  }
  template <typename D>
  void computeAllReachableSet(D* data) {
    Property prop;

    run(data, &prop);
  }

  /**
   * @brief whether the sysyem satisfies the prop
   *
   * @param prop The property which want to verify.
   *
   * @return ture if there is a reachable state which makes prop ture,
   * false otherwise.
   */

  template <typename D>
  bool satisfy(D* data, const Property* prop) {
    return run(data, prop);
  }

  /**
   * @param prop The property which want to verify.
   *
   * @return true if ther is a reachable state which make prop ture,
   * false otherwise.
   */
  template <typename D>
  bool run(D* data, const Property* prop) {
    Check_State re = data->search(prop);

    if (re != UNKOWN) {
      if (TRUE == re) {
        return true;
      } else if (FALSE == re) {
        return false;
      }
    }
    data->setProperty(prop);

    // For given target find the source which change in last step

    while (!data->waitEmpty()) {
      typename SYS::State_t* state = data->next();

      if (oneDiscreteStep(data, state)) {
        manager->destroyState(state);
        return true;
      }
      manager->destroyState(state);
    }

    return false;
  }

 private:
  /**
   One discrete step
   */
  template <typename D>
  bool oneDiscreteStep(D* data, const State_t* const state) {
#ifdef PRINT_STATE
    manager->dump(state);
#endif

    std::vector<OneStep> re = nextS.getNextStep(const_cast<int*>(state));
    return doOneStep(data, manager.get(), state, re) == TRUE;
  }

  const SYS& sys;
  TANextStep nextS;

  shared_ptr<const typename SYS::StateManager_t> manager;
  int component_num;

  State_t* cache_state;
  State_t* next_state;
  std::default_random_engine generator;
};
}  // namespace graphsat

#endif
