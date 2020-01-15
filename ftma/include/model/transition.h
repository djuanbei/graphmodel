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

#include "action/clockreset.h"
#include "action/counteraction.h"

#include "constraint/clockdiffcons.h"

#include "model/vardecl.h"

namespace graphsat {

class Location;

class Transition {
 public:
  typedef int State_t;
  typedef TMStateManager StateManager_t;

  void setSource(int s) { source = s; }

  int getSource() const { return source; }

  void setTarget(int t) { target = t; }

  int getTarget() const { return target; }

  const vector<ClockConstraint>& getGuards() const { return guards; }

  /**
   * add constraint to Transition
   *
   * @param lhs The original transition
   * @param cs  constraint
   *
   * @return Transition
   */

  /**
   *  add one constraint to this transition
   *
   * @param cs  constraint
   *
   */
  Transition& operator+=(const ClockConstraint& cs) {
    guards.push_back(cs);
    return *this;
  }
  /**
   *
   *  add one action to this transition
   *
   * @param action Add one counter action
   *
   */
  Transition& operator+=(const CounterAction action) {
    actions.push_back(action);
    return *this;
  }

  /**
   * add one clock reset  to this transition
   *
   * @param reset The reset
   *
   */

  Transition& operator+=(const ClockReset& reset) {
    reset_arg.push_back(reset);
    return *this;
  }

  Transition& operator+=(const CounterConstraint& guard) {
    counter_cons.push_back(guard);
    return *this;
  }

  void setChannel(const Channel& ch) {
    channel = ch;
    has_channel = true;
    
    has_send_channel=ch.isSend( );
  }

  const Channel& getChannel() const { return channel; }

  bool hasChannel() const { return has_channel; }

  bool hasSendChannel( ) const{
    return has_send_channel;
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

  bool ready(const TMStateManager* manager, const int* const state) const;

  /**
   *
   *@brief  create new state
   *
   */

  void operator()(const TMStateManager* const manager, int* re_state) const;

  void to_real(const TOReal* convertor);

  void setSelectVar(const string& n) { select_var = n; }

  string getSelectVar(void) const { return select_var; }

  void setSelectCollect(const string& c) { select_collect = c; }

  void setSelectDomain( const TypeDefArray & td){
    select_domain=td;
  }

  string getSelectCollect(void) const { return select_collect; }

  bool isSelect() const { return (select_var != "") && (select_collect != ""); }

  std::ostream& dump2Dot(std::ostream& out) const;

 private:
  Transition() {
    source = target = -1;
    has_channel = false;
    has_send_channel=false;
  }

  explicit Transition(const Location* lhs, const Location* rhs)
      : source(lhs->getId()), target(rhs->getId()), has_channel(false), has_send_channel( false) {}

  explicit Transition(const int lhs, const int rhs)
      : source(lhs), target(rhs), has_channel(false), has_send_channel( false) {}

  int source, target;  // source location and target location of this
  // transitionedge. The index of location in tma.locations
  std::vector<ClockConstraint> guards;  // set of constraint at this transitionedge

  std::vector<CounterConstraint>
      counter_cons;  // counter constraint like pid ==id or id==0
  Channel channel;   // Only one synchronisation channels
  bool has_channel;
  bool has_send_channel;

  std::vector<CounterAction> actions;  // set of actions at this transitionedge

  std::vector<pair<int, int>> resets;  // set of reset clock variables

  std::vector<ClockReset> reset_arg;

  string select_var;
  string select_collect;
  
  TypeDefArray  select_domain;

  template <typename LL, typename TT>
  friend class AgentTemplate;
};
}  // namespace graphsat

#endif
