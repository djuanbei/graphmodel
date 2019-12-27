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

#ifdef DRAW_GRAPH
    current_parent = NOT_FOUND;
#endif
    prop = nullptr;
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

  void setProperty(const Property *p) { prop = p; }

  vector<UINT> getProcess_states() const { return process_states; }

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
      decode(convert_C_t, state );

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

  size_t size() const { return reach_set.size(); }
  int getCompressionSize() const { return compress_state.getCompressionSize(); }
  const StateSet<UINT> &getStates() const { return reach_set; }

  void encode(UINT *now, const int * const original) const {
    if(cache_state!=original){
      memcpy(cache_state, original, manager->getStateLen( )*sizeof( int) );
    }
    manager->getClockManager().encode(manager->getDBM(cache_state));
    compress_state.encode(now ,cache_state);
  }

  void decode(int *now, const UINT *const original) const {
    compress_state.decode( now, original);
    manager->getClockManager().decode(manager->getDBM(now));
  }

  /**
   * For one template  system, this function projection
   * state [ 0,m-1] of system
   *
   */
  template <typename PROJ>
  void project(const PROJ &proj, vector<vector<State_t>> &re) {
    re.clear();

    for (auto state : reach_set) {
      decode(convert_C_t, state);

      vector<State_t> dummy;
      proj(convert_C_t, dummy);

      re.push_back(dummy);
    }
    assert(re.size() == reach_set.size());
  }

  void incCurrentParent() {
#ifdef DRAW_GRAPH
    current_parent++;
#endif
  }

  vector<vector<State_t>> getPath(const State_t *const target) const {
    vector<vector<State_t>> re;
    int len = compress_state.getCompressionSize();

    int target_id = NOT_FOUND;
    for (size_t i = 0; i < state_parent.size(); i++) {

      decode(cache_state, &(process_states[i * len]));
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

  void generatorDot(const string &filename) {
#ifdef DRAW_GRAPH
    ofstream fout(filename);
    fout << "digraph G {" << endl;

    int len = compress_state.getCompressionSize();
    int clock_num = manager->getClockNumber();
    for (size_t i = 0; i < state_parent.size(); i++) {

      decode(cache_state, &(process_states[i * len]));
      fout << i << " [ shape=none, label=<";
      fout << "<table border=\"1\" >" << endl;
      fout << "<tr><td COLSPAN=\"" << clock_num + 1 << "\"> <b>" << i << " : "
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
      compress_state.decode(cache_state, &(process_states[parent * len]));
      compress_state.decode(convert_C_t, &(process_states[i * len]));
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
      compress_state.decode(cache_state, &(process_states[p.first * len]));
      compress_state.decode(convert_C_t, &(process_states[p.second * len]));

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

  bool contain(const State_t *const target) const {

    int len = compress_state.getCompressionSize();

    for (size_t i = 0; i < state_parent.size(); i++) {

      decode(cache_state, &(process_states[i * len]));
      if (manager->contain(cache_state, target)) {
        return true;
      }
    }
    return false;
  }
  /**
   * @brief Generate the generate trace from initial state to target
   *
   * @param filename
   * @param target
   */
  bool generatePath(const string &filename, const State_t *const target) const {
#ifdef DRAW_GRAPH

    int len = compress_state.getCompressionSize();
    int clock_num = manager->getClockNumber();
    int target_id = NOT_FOUND;
    for (size_t i = 0; i < state_parent.size(); i++) {

      decode(cache_state, &(process_states[i * len]));
      if (manager->contain(cache_state, target)) {
        target_id = i;
        break;
      }
    }
    if (target_id < 0) {

      return false;
    }

    vector<int> path;
    path.push_back(target_id);
    while (target_id > 0) {
      target_id = state_parent[target_id];
      path.push_back(target_id);
    }

    ofstream fout(filename);
    fout << "digraph G {" << endl;
    for (auto i : path) {
      decode(cache_state, &(process_states[i * len]));
      fout << i << " [ shape=none, label=<";
      fout << "<table border=\"1\" >" << endl;
      fout << "<tr><td COLSPAN=\"" << clock_num + 1 << "\"> <b>" << i << " : "
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
    for (auto i : path) {
      if (i == 0) {
        continue;
      }

      int parent = state_parent[i];
      compress_state.decode(cache_state, &(process_states[parent * len]));
      compress_state.decode(convert_C_t, &(process_states[i * len]));
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

    fout << "}" << endl;
    fout.close();

    return true;
#endif
    return false;
  }

private:
  void addToWait(const State_t *const state) {
    State_t *newState = manager->newState(state);
    wait_set.push_back(newState);
#ifdef DRAW_GRAPH
    compress_state.encode(convert_UINT, state);
    process_states.insert(process_states.end(), convert_UINT,
                          convert_UINT + compress_state.getCompressionSize());
    state_parent.push_back(current_parent);
#endif
  }

  inline bool addToReachableSet(const State_t *const state) {

    compress_state.encode(convert_UINT, state);
    int re = reach_set.add(convert_UINT);

    assert(reach_set.contain(convert_UINT) &&
           "The element resently add the reach set.");
#ifdef DRAW_GRAPH
    if (re == NOT_FOUND) {
      int target = findPassEd(convert_UINT);
      assert(target != NOT_FOUND);
      passed_pair.push_back(make_pair(current_parent, target));
    }
#endif
    return re != NOT_FOUND;
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
  const Property *prop;

#ifdef DRAW_GRAPH
  vector<UINT> process_states;
  vector<int> state_parent;
  int current_parent;
  vector<pair<int, int>> passed_pair;
#endif

  vector<State_t> getState(const int id) const {
    
    int len = compress_state.getCompressionSize();

    decode(cache_state, &(process_states[id * len]));
    int state_len = manager->getStateLen();
    vector<State_t> re( cache_state, cache_state + state_len);
    assert(contain(&(re[0])));
    return re;
  }
};

} // namespace graphsat

#endif
