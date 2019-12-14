/**
 * @file   transitionedge.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:45:30 2019
 *
 * @brief  transitionedge class in timed automata
 *
 *
 */
#ifndef EDGE_HPP
#define EDGE_HPP

#include <memory>
#include <vector>

#include "channel.h"
#include "clock.h"

#include "constraint/countercons.h"
#include "model/location.h"
#include "state/ta_statemanager.h"

#include "constraint/clockdiffcons.h"
#include "util/instancefactory.h"

namespace graphsat {

using std::vector;
class Location;
class TMStateManager;

class Transition {

public:
  typedef int State_t;
  typedef TMStateManager StateManager_t;

  Transition() {
    source = target = -1;
    has_channel = false;
  }

  Transition(const Location &lhs, const Location &rhs)
      : source(lhs.getId()), target(rhs.getId()), has_channel(false) {
  }

  Transition( const Transition &other
            );

  void setSource(int s) { source = s; }

  int getSource() const { return source; }

  void setTarget(int t) { target = t; }

  int getTarget() const { return target; }

  const vector<ClockConstraint> &getGuards() const { return guards; }

  /**
   * add constraint to Transition
   *
   * @param lhs The original transition
   * @param cs  constraint
   *
   * @return Transition
   */

  friend Transition &operator+(Transition &lhs, ClockConstraint &cs) {
    lhs.guards.push_back(cs);
    return lhs;
  }

  /**
   *  add one constraint to this transition
   *
   * @param cs  constraint
   *
   */
  Transition &operator+=(ClockConstraint &cs) {
    guards.push_back(cs);
    return *this;
  }

  void setChannel(Channel *ch) {
    channel.reset(ch);
    has_channel = true;
  }

  const shared_ptr<Channel> &getChannel() const { return channel; }

 // void setChanType(CHANNEL_TYPE type) { channel->setType(type); }

  bool hasChannel() const { return has_channel; }

  /**
   *
   *  add one action to this transition
   *
   * @param action Add one counter action
   *
   */
  void addCounterAction(const CounterAction action) {
    actions.push_back(action);
  }

  /**
   * add one clock reset  to this transition
   *
   * @param reset The reset
   *
   */
  void addReset(const Argument &arg, int v) {
    assert(arg.type == NORMAL_VAR_ARG && "At now is only support this case.");
    pair<int, int> dummy(arg.value, v);
    resets.push_back(dummy);
  }

  void addCounterCons(CounterConstraint guard) {
    counter_cons.push_back(guard);
  }

  /**
   * @brief Except synchronize signal, other state satisfies jump conditions
   *
   * @param component component id
   * @param manager state manager
   * @param state current state
   *
   * @return true if the gurad on this tranisition is true under state, false
   * otherwise.
   */
  bool ready(const int component,
             const shared_ptr<const TMStateManager> &manager,
             const int *const state) const;

  /**
   *
   *@brief  create new state
   *
   */

  void operator()(const int component,
                  const shared_ptr<const TMStateManager> &manager,
                  int *re_state) const;

  void clockShift(const int shift);

  void  to_real(const shared_ptr<TOReal> &convertor);
  
  void chanShift(const int shift);

private:
  int source, target; // source location and target location of this
  // transitionedge. The index of location in tma.locations
  vector<ClockConstraint> guards; // set of constraint at this transitionedge

  vector<CounterConstraint >
      counter_cons;            // counter constraint like pid ==id or id==0
  shared_ptr<Channel> channel; // Only one synchronisation channels
  bool has_channel;

  vector< CounterAction >
      actions;                   // set of actions at this transitionedge
  vector<pair<int, int>> resets; // set of reset clock variables
};
} // namespace graphsat

#endif
