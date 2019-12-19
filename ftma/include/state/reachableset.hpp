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
    State_t *state = wait_set.front(); // BFS
    wait_set.pop_front();
    // State_t *state = wait_set.back( ); //DFS
    // wait_set.pop_back();
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

  /**
   * For one template  system, this function projection
   * state [ 0,m-1] of system
   *
   */
  template <typename PROJ>
  void project(const PROJ &proj, vector<vector<State_t>> &re) {
    re.clear();
    // int clock_start_loc = manager->getClockStart();

    for (auto state : reach_set) {

      compress_state.decode(state, convert_C_t);
      vector<State_t> dummy;
      proj(convert_C_t, dummy);

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
    int clock_num = manager->getClockNumber();
    for (size_t i = 0; i < state_parent.size(); i++) {
      compress_state.decode(&(process_states[i * len]), cache_state);
      fout << i << " [ shape=none, label=<";
      fout << "<table border=\"1\" >" << endl;
      fout << "<tr><td COLSPAN=\"" << clock_num + 1
           << "\"> <b>" << i << " : "
           << manager->getLocDotLabel(cache_state) << "</b></td> </tr> "
           << endl;
      vector<string> couter_labels = manager->getCounterDotLabel(cache_state);
      for (auto &l : couter_labels) {
        fout << "<tr><td COLSPAN=\"" << clock_num + 1
             << "\"> <font color=\"blue\">" << l << "</font></td> </tr> "
             << endl;
      }

      manager->getClockManager().dumpDot(fout, manager->getDBM(cache_state));
      fout << "</table>";
      fout << ">];" << endl;
    }

    for (size_t i = 1; i < state_parent.size(); i++) {
      int parent = state_parent[i];
      compress_state.decode(&(process_states[parent * len]), cache_state);
      compress_state.decode(&(process_states[i * len]), convert_C_t);
      fout << "\t" << state_parent[i] << " -> " << i << "  [label=\"";
      int diff_num = 0;
      for (int j = 0; j < component_num; j++) {
        if (manager->getLocationID(j, cache_state) !=
            manager->getLocationID(j, convert_C_t)) {
          if (0 == diff_num) {
            fout << j;
          } else {
            fout << ", " << j;
          }
          diff_num++;
        }
      }
      fout << "\"];" << endl;
    }
    for (auto &p : passed_pair) {
      compress_state.decode(&(process_states[p.first * len]), cache_state);
      compress_state.decode(&(process_states[p.second * len]), convert_C_t);

      fout << "\t" << p.first << " -> " << p.second
           << "  [style=dotted color=red label=\"";
      int diff_num = 0;
      for (int j = 0; j < component_num; j++) {
        if (manager->getLocationID(j, cache_state) !=
            manager->getLocationID(j, convert_C_t)) {
          if (0 == diff_num) {
            fout << j;
          } else {
            fout << ", " << j;
          }
          diff_num++;
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
#ifdef DRAW_GRAPH
    int re = reach_set.add(convert_UINT);
    if (re != NOT_FOUND) {
      return true;
    } else {
      int target = findPassEd(convert_UINT);
      assert(target != NOT_FOUND);
      passed_pair.push_back(make_pair(current_parent, target));
      return false;
    }
#else
    return reach_set.add(convert_UINT) != NOT_FOUND;
#endif
  }

  int findPassEd(const UINT *state) const {
    int len = compress_state.getCompressionSize();
    int head_part_len = compress_state.getCompressionHeadSize();
    int bodySize = len - head_part_len;

    for (size_t i = 0; i < state_parent.size(); i++) {
      if (0 == memcmp(state, &(process_states[i * len]),
                      head_part_len * sizeof(UINT))) {
        int j = 0;
        for (; j < bodySize; j++) {
          if (state[head_part_len + j] >
              process_states[i * len + head_part_len + j]) {
            break;
          }
        }
        if (j == bodySize) {
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
  StateConvert<State_t> compress_state;
  UINT *convert_UINT;
  State_t *convert_C_t;

#ifdef DRAW_GRAPH
  vector<UINT> process_states;
  vector<int> state_parent;
  int current_parent;
  vector<pair<int, int>> passed_pair;
#endif

}; // namespace graphsat

} // namespace graphsat

#endif
