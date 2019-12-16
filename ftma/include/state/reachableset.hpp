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

#include "util/datacompression.h"

namespace graphsat {
using std::copy;
using std::deque;
using std::fill;
using std::ofstream;
using std::vector;

template <typename M> class ReachableSet {
public:
  typedef typename M::State_t State_t;
  ReachableSet(const shared_ptr<M> outta) : manager(outta) {

#ifdef DRAW_GRAPH
    current_parent = -1;
#endif

    component_num = manager->getComponentNum();

    cache_state = manager->newState();

    convert_C_t = manager->newState();

    int body_length = manager->getStateLen() - manager->getClockStart();

    compress_state = StateConvert<State_t>(
        manager->getClockStart(), body_length, manager->getHeadCompression(),
        manager->getBodyCompression());

    convert_UINT = new UINT[compress_state.getCompressionSize()]();
    reach_set.setParam(compress_state.getCompressionSize(),
                       compress_state.getCompressionHeadSize());
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

  /**
   * BFS
   * @return next state by order given order
   */
  State_t *next() {
    State_t *state = wait_set.front();
    wait_set.pop_front();
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
      compress_state.decode(state, convert_C_t);
      if ((*prop)(manager.get(), convert_C_t)) {
        return TRUE;
      }
      //      if ( isReach( prop, convert_C_t ) ) {
      //        return TRUE;
      //      }
    }
    return UNKOWN;
  }

  bool add(const State_t *const state) {
    if (addToReachableSet(state)) {
      addToWait(state);
      return true;
    }
    return false;
  }

  size_t size() const { return reach_set.size(); }

  void project(int m, vector<vector<State_t>> &re) {
    re.clear();
    int clock_start_loc = manager->getClockStart();

    for (auto state : reach_set) {

      compress_state.decode(state, convert_C_t);

      vector<State_t> dummy;
      for (int i = 0; i < m; i++) {
        dummy.push_back(convert_C_t[i]);
      }
      for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= m; j++) {
          dummy.push_back(
              convert_C_t[i * (component_num + 1) + j + clock_start_loc]);
        }
      }

      re.push_back(dummy);
    }
  }

  void incCurrentParent() {
#ifdef DRAW_GRAPH
    current_parent++;
#endif
  }

  void generatorDot(const string &filename) {
#ifdef DRAW_GRAPH
    ofstream fout(filename);
    fout << "digraph G {" << endl;
    int len = compress_state.getCompressionSize();
    for (size_t i = 1; i < state_parent.size(); i++) {
      int parent = state_parent[i];
      compress_state.decode(&(process_states[parent * len]), cache_state);
      compress_state.decode(&(process_states[i * len]), convert_C_t);
      fout << "\t" << state_parent[i] << " -> " << i << "  [label=\"";
      for (int j = 0; j < component_num; j++) {
        if (cache_state[j] != convert_C_t[j]) {
          fout << j << " ";
        }
      }
      fout << "\"];" << endl;
    }
    fout << "}";
    fout.close();
#endif
  }

private:
  void addToWait(const State_t *const state) {
    State_t *newState = manager->newState(state);
    wait_set.push_back(newState);
#ifdef DRAW_GRAPH
    compress_state.encode(state, convert_UINT);
    process_states.insert(process_states.end(), convert_UINT,
                          convert_UINT + compress_state.getCompressionSize());
    state_parent.push_back(current_parent);
#endif
  }

  inline bool addToReachableSet(const State_t *const state) {

    compress_state.encode(state, convert_UINT);
    return reach_set.add(convert_UINT) > -1;

    return true;
  }

private:
  shared_ptr<const M> manager;
  StateSet<UINT> reach_set;
  deque<State_t *> wait_set;

  State_t *cache_state;

  int component_num;
  std::default_random_engine generator;
  StateConvert<State_t> compress_state;
  UINT *convert_UINT;
  State_t *convert_C_t;

#ifdef DRAW_GRAPH
  vector<UINT> process_states;
  vector<int> state_parent;
  int current_parent;
#endif

}; // namespace graphsat

} // namespace graphsat

#endif
