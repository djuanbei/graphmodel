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
#include<set>

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
template <typename L, typename T>
class AgentSystem;

class TMStateManager : public ComponentInfo {
  /**
   * state is [loc, freezeLocationNum, counter_state,
   * clock_state, parent_state_id ] loc is a vector with length  the number of
   * components, channel_state is aslo a vector whose length equals to the
   * number of component. If the corresponding loc is negative integer then this
   * location is a commit location the corresponding channel_state is the block
   * channel. The channel state is positive then it is a send channel,  when the
   * value is nonegative integer it is a receive channel. When channel equals to
   * NO_CHANNEL then there is no channel in this transition.
   *
   * freezeLocationNum is the number of components which staies on freeze
   * location. The cunter_state part is the vector value of counter variables
   *
   */
 public:
  typedef int State_t;
  typedef AgentSystem<Location, Transition> INT_TAS_t;

  TMStateManager(const INT_TAS_t& s, const vector<Counter>& ecounters,
                 int clock_num, const vector<int>& oclock_upper_bounds,
                 const vector<int>& nodes, const vector<int>& links);

  virtual ~TMStateManager() {
    for (auto e : init_states) {
      destroyState(e);
    }
    init_states.clear();
    destroyState(cache_state);
  }

  int getStateLen() const { return state_length; }

  int getCompressionSize() const { return compress_state.getCompressionSize(); }
  int getCompressionHeadSize() const {
    return compress_state.getCompressionHeadSize();
  }
  /**
   * @brief get start location of type value in one state
   */
  int getTypeStart(const TYPE_T type) const;

  /**
   *   @brief get number of int  type value in one state
   */
  int getTypeNumber(const TYPE_T type) const;

  int getCounterStart() const { return counter_start_loc; }
  // number of int  in one state for int variables
  int getCounterNumber() const {
    return freeze_location_index - counter_start_loc;
  }

  int getClockStart() const { return clock_start_loc; }
  // number of int in one state for clock variables
  int getClockNumber() const { return parent_location_index - clock_start_loc; }

  // Return the number of clock variables in the system
  int getSystemClockNumber() const { return clock_num; }

  Compression<int> getHeadCompression() const;

  Compression<int> getBodyCompression() const;

  std::string getLocationName(const int component, const int loc_ID) const;

  inline bool withoutChannel(const int component,
                             const int* const state) const {
    return state[component + component_num] == NO_CHANNEL;
  }

  std::vector<int> getChanLinks(const int component, const int source, int chid,
                           int* state) const;

  int* newState() const;

  int* newState(const int* const state) const {
    int* re = new int[state_length];
    memcpy(re, state, state_length * sizeof(int));
    return re;
  }

  void reset(int* state) const;

  int* randState() const;

  void copy(int* des_state, const int* const source_state) const {
    memcpy(des_state, source_state, state_length * sizeof(int));
  }
  /**
   * @brief ignore parent_location_index value
   */
  bool equal(const int* const lhs, const int* const rhs) const {
    return memcmp(lhs, rhs, (state_length - 1) * sizeof(int)) == 0;
  }

  void incFreeze(int* state) const { state[freeze_location_index]++; }
  void decFreeze(int* state) const { state[freeze_location_index]--; }
  int getFreezeComponentNumber(const int* const state) const {
    return state[freeze_location_index];
  }
  
  std::vector<int> getCommitComponents(const int * const state  ) const;

  int getParentId(const int* const state) const {
    return state[parent_location_index];
  }
  void setParent(int* state, int parent) const {
    state[parent_location_index] = parent;
  }

  int getLocationID(const int component, const int* const state) const;

  // check whether this state allow time delay
  bool isFreeze(const int* const state) const {
    return state[freeze_location_index] > 0;
  }

  bool transitionReady(const int component, const int link,
                       const int* const state) const;

  bool isReachable(const int component, const int loc, int* state) const;

  // check wether there is a pair out transion with pair urgent channels
  bool hasMatchOutUrgentChan(const int* const state) const;

  // check wether there is a out transition with  breakcast send channel
  bool hasOutSendBroadcastChan(const int* const state) const;

  /**
   * @brief Get the key value in the state under component context. If the can
   * not find the key then abort.
   * @param component
   * @param state
   * @param key
   *
   * @return
   */
  int getValue(const int component, const int* const state,
               const std::string& key) const;

  void setValue(const int component, int* state, const std::string& key,
                int value) const;

  int& getValue(const int component, int* state, const std::string& key) const;

  /**
   * @brief As the state is  abstract symbolic state. A symbolic state
   * represention a set of concrete states.
   *
   * @param lhs
   * @param rhs
   *
   * @return  lhs >= rhs
   */
  bool contain(const int* const lhs, const int* const rhs) const;

  void destroyState(int* state) const { delete[] state; }

  int getComponentNumber() const { return component_num; }

  inline const DBMManager& getClockManager() const { return dbm_manager; }

  void norm(const int* const dbm, std::vector<int*>& re_vec) const {
    int* newDBM = dbm_manager.createDBM(dbm);
    dbm_manager.norm(newDBM, re_vec);
  }

  inline void norm(int* dbm) const { dbm_manager.norm(dbm); }

  MatrixValue getClockLowerBound(const int component, const std::string& key,
                                 const int* const state) const;

  void setClockLowerBound(const int component, const std::string& key,
                          int* state, const MatrixValue& value) const;

  MatrixValue getClockUpperBound(const int component, const std::string& key,
                                 const int* const state) const;

  void setClockUpperBound(const int component, const std::string& key,
                          int* state, const MatrixValue& value) const;

  // keyA- keyB > ( >=) re
  MatrixValue getClockDiffLowerBound(const int componentA,
                                     const std::string& keyA,
                                     const int componentB,
                                     const std::string& keyB,
                                     const int* const state) const;

  void setClockLowerBound(const int componentA, const std::string& keyA,
                          const int componentB, const std::string& keyB,
                          int* state, const MatrixValue& value) const;

  MatrixValue getClockUpperBound(const int componentA, const std::string& keyA,
                                 const int componentB, const std::string& keyB,
                                 const int* const state) const;

  void setClockUpperBound(const int componentA, const std::string& keyA,
                          const int componentB, const std::string& keyB,
                          int* state, const MatrixValue& value) const;

  inline int* getDBM(int* state) const { return state + clock_start_loc; }

  inline const int* getDBM(const int* const state) const {
    return state + clock_start_loc;
  }

  inline int* getCounterValue(int* state) const {
    return state + counter_start_loc;
  }

  inline const int* getCounterValue(const int* const state) const {
    return state + counter_start_loc;
  }

  inline void andImpl(const ClockConstraint& cs, int* state) const {
    return dbm_manager.andImpl(getDBM(state), cs);
  }
  inline bool isConsistent(int* state) const {
    return dbm_manager.isConsistent(getDBM(state));
  }

  std::vector<int> blockComponents(const int chid,
                                   const int* const state) const;

  const std::vector<int*>& getInitialState() const { return init_states; }

  void constructState(const int* const state, const int* const dbm,
                      int* re_state) const;

  /**
   * @brief Get the start location of comp_id component counter in state
   *
   * @param comp_id   component id
   */
  int getComponentCounterStartLoc(const int comp_id) const;
  /**
   * @brief Get the start location of comp_id component clock in state
   *
   * @param comp_id   component id
   */
  int getComponentClockStartLoc(const int comp_id) const;

  /**
   * @brief swap component i and component j local state. The component i and j
   * must from a same template
   *
   * @param i component id
   * @param j component id
   * @param state  original state
   */
  void swap(const int i, const int j, int* state) const;

  std::vector<int> getEnableOutLinks( const int component, int loc, int *state ) const;
  
  std::set<int> getEnableOutBroadcast(const int component, const int loc,
                                         int* state) const;

  std::set<int> getEnableOutUrgent(const int component, const int loc,
                                      int* state) const;

  std::set<int> getEnableOutNormalChan(const int component, const int loc,
                                          int* state) const;



  bool hasDiffCons() const;

  void employLocInvariants(const int component, int* state) const;

  void discreteRun(const int component, const int link, int* state) const;

  vector<int*> evolution(const int component, const int loc, int* state) const;

  std::string getLocDotLabel(const int* const state) const;

  std::vector<std::string> getCounterDotLabel(const int* const state) const;

  void encode(UINT* now, int* original) const;

  void decode(int* now, const UINT* const original) const;

  std::ostream& dump(const int* const state, std::ostream& out) const;

  void dump(const int* const state) const { dump(state, cout); }

  void dump(const vector<int>& state) const { dump(&(state[0]), cout); }

 private:
  const INT_TAS_t& sys;

  int component_num;
  int state_length;
  int parent_location_index;
  int freeze_location_index;

  int counter_start_loc;
  int clock_start_loc;

  vector<int> clock_upper_bounds;
  DBMManager dbm_manager;
  vector<Counter> counters;
  vector<Parameter> parameters;
  vector<int> node_nums;
  vector<int> link_nums;
  int clock_num;
  bool hasDiff;
  StateConvert<int> compress_state;
  vector<int*> init_states;
  int* cache_state;
};
}  // namespace graphsat
#endif
