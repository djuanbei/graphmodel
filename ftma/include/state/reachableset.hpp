/**
 * @file   reachableset.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Apr 27 08:49:05 2019
 *
 * @brief  the data of reachableset
 *
 *
 */
#ifndef __REACH_SET_HPP
#define __REACH_SET_HPP
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "util/typedef.h"

#include "state/discretestate.hpp"

#include "alg/one_step.h"
#include "alg/ta_next_step.h"
#include "property/property.h"
#include "util/datacompression.h"

namespace graphsat {
using std::copy;
using std::deque;
using std::fill;
using std::ofstream;
using std::vector;

#ifdef ONLINE_CHECK
#define isReach(P, S) false
#endif

template <typename M>
class ReachableSet {
 public:
  typedef typename M::State_t State_t;
  ReachableSet(const shared_ptr<M> outta) : manager(outta) {
    current_state_id = -1;

    prop = nullptr;
    component_num = manager->getComponentNumber();
    state_len = manager->getStateLen();
    cache_state = manager->newState();

    convert_C_t = manager->newState();

    compression_size = manager->getCompressionSize();

    convert_UINT = new UINT[compression_size]();
    reach_set.setParam(manager->getCompressionSize(),
                       manager->getCompressionHeadSize(),
                       manager->getBodyCompression());

    const vector<int*>& states = manager->getInitialState();

    for (auto e : states) {
      add(e);
    }
  }

  ~ReachableSet() {
    reach_set.clear();
    manager->destroyState(cache_state);
    cache_state = nullptr;
    manager->destroyState(convert_C_t);
    convert_C_t = nullptr;
    delete[] convert_UINT;
    convert_UINT = nullptr;
  }

  void setProperty(const Property* p) { prop = p; }

  vector<UINT> getProcess_states() const {
    vector<UINT> re;
    for (size_t i = 0; i < size(); i++) {
      getCompressionStateAt(convert_UINT, i);
      re.insert(re.end(), convert_UINT, convert_UINT + compression_size);
    }
    return re;
  }

  /**
   * BFS
   * @return next state by order given order
   */
  State_t* next() {
    // State_t *state = wait_set.front(); // BFS
    // wait_set.pop_front();
    current_state_id++;

    // State_t *state = wait_set.back( ); //DFS
    // wait_set.pop_back();
    //    manager->getClockManager().decode(manager->getDBM(state));
    int* state1 = manager->newState();
    getStateAt(state1, current_state_id);

    //    assert( manager->equal( state, state1));
    return state1;
  }

  /**
    where there is no wait state
   */
  bool waitEmpty() const { return current_state_id + 1 >= reach_set.size(); }
  /**
   The number of state in the wait set
   */
  //  size_t waitSize() const { return wait_set.size(); }

  /**
   Search in the reachable set to check whether the prop satisfy.
   */
  Check_State search(const Property* prop) {
    for (auto state : reach_set) {
      manager->decode(convert_C_t, state);

      if ((*prop)(manager.get(), convert_C_t)) {
        return TRUE;
      }
    }
    return UNKOWN;
  }

  /**
   *
   *
   * @param prop  verifies property
   * @param state one reachable state
   *
   * @return ture if prop is true under state, false otherwise.
   */
#ifndef ONLINE_CHECK
  inline bool isReach(const State_t* const state) const {
    return (prop != nullptr) && ((*prop)(manager.get(), state));
  }
#endif

  Check_State add(const State_t* const state) {
    if (cache_state != state) {
      manager->copy(cache_state, state);
    }
    manager->setParent(cache_state, current_state_id);

    if (addToReachableSet(cache_state)) {
      if (isReach(cache_state)) {
        return TRUE;
      }
      return UNKOWN;
    }
    return FALSE;
  }

  void getCompressionStateAt(UINT* out, const int id) const {
    reach_set.getElementAt(out, id);
  }

  void getStateAt(int* out, const int id) const {
    getCompressionStateAt(convert_UINT, id);
    manager->decode(out, convert_UINT);
  }

  int getParentId(const int id) const {
    getStateAt(cache_state, id);
    return manager->getParentId(cache_state);
  }

  int findId(const int* const state) const {
    for (size_t i = 0; i < size(); i++) {
      getStateAt(cache_state, i);
      if (manager->contain(cache_state, state)) {
        return i;
      }
    }
    return NOT_FOUND;
  }
  /**
   * @brief find one execution path from initial to state
   */
  vector<int> findReachPath(const int* const state) const {
    vector<int> path;
    int sid = findId(state);
    if (sid == NOT_FOUND) {
      return path;
    }
    path.push_back(sid);
    while (sid > 0) {
      sid = getParentId(sid);
      path.push_back(sid);
    }
    reverse(path.begin(), path.end());
    return path;
  }
  /**
   * @brief lhs_state   is current exposure but it is contained in rhs_state
   * which has been  exposured.
   * @return vector (lhs_state, rhs_state)
   */
  const vector<pair<int, int>>& getFixPoindTail() const { return passed_pair; }

  size_t size() const { return reach_set.size(); }

  // clear all the data
  void clear() {
    reach_set.clear();
    passed_pair.clear();
    current_state_id = -1;
  }

  const StateSet<UINT>& getStates() const { return reach_set; }
  const M* getManager() const { return manager.get(); }

  /**
   * For one template  system, this function projection
   * state [ 0,m-1] of system
   *
   */
  template <typename PROJ>
  void project(PROJ& proj, vector<vector<State_t>>& re) {
    re.clear();

    for (auto state : reach_set) {
      manager->decode(convert_C_t, state);

      // vector<State_t> dummy=proj(convert_C_t);

      re.push_back(std::move(proj(convert_C_t)));
    }
    assert((int)re.size() == reach_set.size());
  }

  vector<vector<State_t>> getPath(const State_t* const target) const {
    vector<vector<State_t>> re;

    int target_id = NOT_FOUND;
    for (size_t i = 0; i < size(); i++) {
      getStateAt(cache_state, i);

      if (manager->equal(target, cache_state)) {
        target_id = i;
        break;
      }
    }
    if (target_id < 0) {
      return re;
    }
    vector<int> path;
    path.push_back(target_id);
    while (target_id > 0) {
      target_id = getParentId(target_id);
      path.push_back(target_id);
    }
    for (vector<int>::reverse_iterator it = path.rbegin(); it != path.rend();
         it++) {
      re.push_back(getState(*it));
    }
    return re;
  }

  bool contain(const State_t* const target) const {
    manager->copy(cache_state, target);
    manager->encode(convert_UINT, cache_state);
    return reach_set.contain(convert_UINT) != NOT_FOUND;
  }

  std::vector<std::vector<int>> selectByHead(
      const State_t* const target) const {
    manager->copy(cache_state, target);
    manager->encode(convert_UINT, cache_state);
    vector<const UINT*> bodies;
    reach_set.selectByHead(convert_UINT, bodies);
    std::vector<std::vector<int>> re;
    for (auto e : bodies) {
      reach_set.appendBody(convert_UINT, e);
      manager->decode(cache_state, convert_UINT);
      std::vector<int> dummy(cache_state, cache_state + state_len);
      re.push_back(dummy);
    }
    return re;
  }

  int getCurrentId() const { return current_state_id; }

 private:
  inline bool addToReachableSet(State_t* state) {
    manager->encode(convert_UINT, state);
    int re = reach_set.add(convert_UINT);

    assert(reach_set.contain(convert_UINT) != NOT_FOUND &&
           "The element resently add the reach set.");

    if (re != NOT_FOUND) {
      passed_pair.push_back(make_pair(current_state_id, re));
      return false;
    }

    return true;
  }

  int findPassEd(const UINT* state) const {
    int head_part_len = manager->getCompressionHeadSize();

    for (size_t i = 0; i < size(); i++) {
      getCompressionStateAt(convert_UINT, i);
      if (0 == memcmp(state, convert_UINT, head_part_len * sizeof(UINT))) {
        manager->decode(cache_state, state);
        getStateAt(convert_C_t, i);
        if (manager->contain(convert_C_t, cache_state)) {
          return i;
        }
      }
    }
    return NOT_FOUND;
  }

 private:
  shared_ptr<const M> manager;
  StateSet<UINT> reach_set;

  int state_len;
  State_t* cache_state;

  int component_num;
  std::default_random_engine generator;

  UINT* convert_UINT;
  State_t* convert_C_t;
  const Property* prop;

  int compression_size;

  int current_state_id;
  vector<pair<int, int>> passed_pair;

  vector<State_t> getState(const int id) const {
    reach_set.getElementAt(convert_UINT, id);
    manager->decode(cache_state, convert_UINT);

    vector<State_t> re(cache_state, cache_state + state_len);
    return re;
  }
};

}  // namespace graphsat

#endif
