/**
 * @file   state.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat May 18 09:28:22 2019
 *
 * @brief  state  Corresponding all the API about state
 *
 *
 */

#ifndef __STATE_MANAGER_H
#define __STATE_MANAGER_H
#include <vector>

#include "discretestate.hpp"
#include "domain/dbm.h"
#include "model/counter.h"
#include "model/parameter.h"
#include "state/componentstate.h"
#include "util/datacompression.h"

namespace graphsat {

using std::vector;
class Location;
class Transition;
template <typename L, typename T> class AgentSystem;

class TMStateManager : public ComponentInfo {

  /**
   * state is [loc, channel_state, freezeLocationNum, counter_state,
   * clock_state] loc is a vector with length  the number of components,
   * channel_state is aslo a vector whose length equals to the number of
   * component. If the corresponding loc is negative integer then this location
   * is a commit location the corresponding channel_state is the block channel.
   * The channel state is positive then it is a send channel,  when the value is
   * nonegative integer it is a receive channel. When channel equals to
   * NO_CHANNEL then there is no channel in this transition.
   *
   * freezeLocationNum is the number of components which staies on freeze
   * location. The cunter_state part is the vector value of counter variables
   *
   */
public:
  typedef int State_t;
  typedef AgentSystem<Location, Transition> INT_TAS_t;

  TMStateManager(const INT_TAS_t &s, const vector<Counter> &ecounters,
                 int clock_num, const vector<int> &oclock_upper_bounds,
                 const vector<int> &nodes, const vector<int> &links);

  virtual ~TMStateManager() {}

  int getStateLen() const { return state_length; }

  int getClockStart() const { return clock_start_loc; }

  int getClockNumber() const { return clock_num; }

  int getFreezeLocation() const { return freeze_location_index; }

  // check whether this state allow time delay
  bool isFreeze(const int *const state) const {
    return state[freeze_location_index] > 0;
  }

  void unBlock(const int comp_id, int *state) const {
    state[comp_id + component_num] = NO_CHANNEL;
  }

  bool isBlock(const int comp_id, const int *const state) const;

  bool transitionReady(const int component, const int link,
                       const int *const state) const;

  bool isReachable(const int component, const int loc, int *state) const;

  // check wether there is a pair out transion with pair urgent channels
  bool hasMatchOutUrgentChan(const int *const state) const;

  // check wether there is a out transition with  breakcast send channel
  bool hasOutBreakcastChan(const int *const state) const;

  Compression<int> getHeadCompression() const;

  Compression<int> getBodyCompression() const;

  int getLocationID(const int component, const int *const state) const;

  string getLocationName(const int component, const int loc_ID) const;

  inline bool withoutChannel(const int component,
                             const int *const state) const {
    return state[component + component_num] == NO_CHANNEL;
  }

  vector<int> getChanLinks(const int component, const int source, int chid,
                           int *state) const;

  int *newState() const;

  int *newState(const int *const state) const {
    int *re = new int[state_length];
    memcpy(re, state, state_length * sizeof(int));
    return re;
  }

  void copy(int *des_state, const int *const source_state) const {
    memcpy(des_state, source_state, state_length * sizeof(int));
  }

  void destroyState(int *state) const { delete[] state; }

  int getComponentNum() const { return component_num; }
  inline const DBMFactory &getClockManager() const { return dbm_manager; }

  void norm(const int *const dbm, vector<int *> &re_vec) const {
    int *newDBM = dbm_manager.createDBM(dbm);
    dbm_manager.norm(newDBM, re_vec);
  }

  void norm(int *dbm) const { dbm_manager.norm(dbm); }

  inline int *getDBM(int *state) const { return state + clock_start_loc; }

  inline const int *getDBM(const int *const state) const {
    return state + clock_start_loc;
  }

  inline int *getCounterValue(int *state) const {
    return state + counter_start_loc;
  }

  inline const int *getCounterValue(const int *const state) const {
    return state + counter_start_loc;
  }

  inline void andImpl(const ClockConstraint &cs, int *state) const {
    return dbm_manager.andImpl(getDBM(state), cs);
  }
  inline bool isConsistent(int *state) const {
    return dbm_manager.isConsistent(getDBM(state));
  }

  vector<int> blockComponents(const int chid, const int *const state) const;

  void constructState(const int component_id, const int target,
                      const int *const state, int *dbm, bool isCommit,
                      int *re_state) const;

  void constructState(const int *const state, const int *const dbm,
                      int *re_state) const;

  void constructState(const int component_id, const int target, bool isCommit,
                      int *state) const;

  // inline bool isCommitComp(const int component_id,
  //                          const int *const state) const {

  //   return state[component_id] < 0;
  // }

  /**
   * @brief  comonent_id in a commit location
   *
   * @param component_id component id
   * @param state set state
   */
  inline void setCommitState(const int component_id, int *state) const {
    state[component_id] = -1 - state[component_id];
  }

  inline int getCommitLoc(const int component_id,
                          const int *const state) const {
    return -(state[component_id]) - 1;
  }

  vector<int> getEnableOutUrgent(const int component, const int loc,
                                 int *state) const;

  vector<int> getEnableOutNormalChan(const int component, const int loc,
                                     int *state) const;

  bool hasDiffCons() const;

  void employLocInvariants(const int component, int *state) const;

  void discretRun(const int component, const int link, int *state) const;

  vector<int *> evolution(const int component, const int loc, int *state) const;

  string getLocDotLabel(const int *const state) const;

  vector<string> getCounterDotLabel(const int *const state) const;

  ostream &dump(const int *const state, ostream &out) const;

  void dump(const int *const state) const { dump(state, cout); }

private:
  const INT_TAS_t &sys;

  int component_num;
  int state_length;
  int freeze_location_index;

  int counter_start_loc;
  int clock_start_loc;

  vector<int> clock_upper_bounds;
  DBMFactory dbm_manager;
  vector<Counter> counters;
  vector<Parameter> parameters;
  vector<int> node_nums;
  vector<int> link_nums;
  int clock_num;
  int chan_num;
  bool hasDiff;
  //  bool hasChannel() const;
};
} // namespace graphsat
#endif
