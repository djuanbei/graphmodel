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

template <typename M> class ReachableSet {
public:
  typedef typename M::State_t State_t;
  ReachableSet(const shared_ptr<M> outta) : manager(outta) {

    current_parent = NOT_FOUND;

    prop = nullptr;
    component_num = manager->getComponentNum();

    cache_state = manager->newState();

    convert_C_t = manager->newState();


    compression_size = manager->getCompressionSize();

    convert_UINT = new UINT[compression_size]();
    reach_set.setParam(manager->getCompressionSize(),
                       manager->getCompressionHeadSize(),
                       manager->getBodyCompression());
  }

  ~ReachableSet() {
    reach_set.clear();
    manager->destroyState(cache_state);
    cache_state = nullptr;
    manager->destroyState(convert_C_t);
    convert_C_t = nullptr;
    delete[] convert_UINT;
    convert_UINT = nullptr;
    while (!wait_set.empty()) {
      State_t *temp_state = wait_set.front();
      wait_set.pop_front();
      manager->destroyState(temp_state);
    }
  }

  void setProperty(const Property *p) { prop = p; }

  vector<UINT> getProcess_states() const {
    vector<UINT> re;
    for( size_t i=0; i< size( ); i++){
      getCompressionStateAt( convert_UINT, i);
      re.insert( re.end( ), convert_UINT, convert_UINT+compression_size);
    }
    return re;
  }

  /**
   * BFS
   * @return next state by order given order
   */
  State_t *next() {
    State_t *state = wait_set.front(); // BFS
    wait_set.pop_front();
    // State_t *state = wait_set.back( ); //DFS
    // wait_set.pop_back();
    manager->getClockManager().decode(manager->getDBM(state));
    return state;
  }

  /**
    where there is no wait state
   */
  bool waitEmpty() const { return wait_set.empty(); }
  /**
   The number of state in the wait set
   */
  size_t waitSize() const { return wait_set.size(); }

  /**
   Search in the reachable set to check whether the prop satisfy.
   */
  Check_State search(const Property *prop) {

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
  inline bool isReach(const State_t *const state) const {
    return (prop != nullptr) && ((*prop)(manager.get(), state));
  }
#endif

  Check_State add(State_t *state) {
    vector<int> dummy(state, state + manager->getStateLen());
    manager->getClockManager().encode(manager->getDBM(state));
    if (addToReachableSet(state)) {

      addToWait(state);
      if (isReach(state)) {
        return TRUE;
      }
      return UNKOWN;
    }
    return FALSE;
  }


  void getCompressionStateAt( UINT *out, const int id)const{
    reach_set.getElementAt( out, id);
  }

  void getStateAt(int *out, const int id) const {
    getCompressionStateAt( convert_UINT, id);
    manager->decode(out, convert_UINT);
  }
  int getParentId(const int id) const { return state_parent[id]; }

  int findId(const int *const state) const {
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
  vector<int> findReachPath(const int *const state) const {
    vector<int> path;
    int sid = findId(state);
    if (sid == NOT_FOUND) {
      return path;
    }
    path.push_back(sid);
    while (sid > 0) {
      sid = state_parent[sid];
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
  const vector<pair<int, int>> &getFixPoindTail() const { return passed_pair; }

  size_t size() const { return reach_set.size(); }

  const StateSet<UINT> &getStates() const { return reach_set; }
  const M *getManager() const { return manager.get(); }

  /**
   * For one template  system, this function projection
   * state [ 0,m-1] of system
   *
   */
  template <typename PROJ>
  void project(const PROJ &proj, vector<vector<State_t>> &re) {
    re.clear();

    for (auto state : reach_set) {
      manager->decode(convert_C_t, state);

      vector<State_t> dummy;
      proj(convert_C_t, dummy);

      re.push_back(dummy);
    }
    assert(re.size() == reach_set.size());
  }

  void incCurrentParent() { current_parent++; }

  vector<vector<State_t>> getPath(const State_t *const target) const {
    vector<vector<State_t>> re;

    int target_id = NOT_FOUND;
    for (size_t i = 0; i < state_parent.size(); i++) {
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
      target_id = state_parent[target_id];
      path.push_back(target_id);
    }
    for (vector<int>::reverse_iterator it = path.rbegin(); it != path.rend();
         it++) {
      re.push_back(getState(*it));
    }
    return re;
  }

  bool contain(const State_t *const target) const {

    for (size_t i = 0; i < state_parent.size(); i++) {
      getStateAt(cache_state, i);

      if (manager->contain(cache_state, target)) {
        return true;
      }
    }
    return false;
  }

private:
  void addToWait(const State_t *const state) {
    State_t *newState = manager->newState(state);
    wait_set.push_back(newState);

    manager->encode(convert_UINT, state);


    state_parent.push_back(current_parent);
  }

  inline bool addToReachableSet(const State_t *const state) {

    manager->encode(convert_UINT, state);
    int re = reach_set.add(convert_UINT);

    assert(reach_set.contain(convert_UINT) &&
           "The element resently add the reach set.");

    if (re == NOT_FOUND) {
      int target = findPassEd(convert_UINT);
      assert(target != NOT_FOUND);
      passed_pair.push_back(make_pair(current_parent, target));
    }

    return re != NOT_FOUND;
  }

  int findPassEd(const UINT *state) const {

    int head_part_len = manager->getCompressionHeadSize();

    for (size_t i = 0; i < state_parent.size(); i++) {
      getCompressionStateAt( convert_UINT, i);
      if (0 == memcmp(state, convert_UINT,
                      head_part_len * sizeof(UINT))) {
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
  deque<State_t *> wait_set;

  State_t *cache_state;

  int component_num;
  std::default_random_engine generator;

  UINT *convert_UINT;
  State_t *convert_C_t;
  const Property *prop;

  int compression_size;

  vector<int> state_parent;

  int current_parent;
  vector<pair<int, int>> passed_pair;

  vector<State_t> getState(const int id) const {
    reach_set.getElementAt(convert_UINT, id );
    manager->decode(cache_state, convert_UINT);
    int state_len = manager->getStateLen();
    vector<State_t> re(cache_state, cache_state + state_len);
    assert(contain(&(re[0])));
    return re;
  }
};

} // namespace graphsat

#endif
