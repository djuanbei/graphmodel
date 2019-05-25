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

#include <vector>

#include "channel.h"
#include "constraint/countercons.h"
#include "statemanager.hpp"

namespace graphsat {
using std::vector;
template <typename C, typename CS, typename D, typename DSet> class Transition {

  typedef Transition<C, CS, D, DSet> Transition_t;

public:
  Transition() { source = target = -1; }
  Transition( int s, int t ) {
    source = s;
    target = t;
  }

  void setSource( int s ) { source = s; }

  int getSource() const { return source; }

  void setTarget( int t ) { target = t; }

  int getTarget() const { return target; }

  const vector<CS> &getGuards() const { return guards; }

  /**
   * add constraint to Transition
   *
   * @param lhs
   * @param cs
   *
   * @return
   */

  friend Transition_t &operator+( Transition_t &lhs, CS &cs ) {
    lhs.guards.push_back( cs );
    return lhs;
  }

  /**
   *  add one constraint to this transition
   *
   * @param cs  constraint
   *
   * @return
   */
  Transition_t &operator+=( CS &cs ) {
    guards.push_back( cs );
    return *this;
  }

  void setChannel( const Channel &ch ) { channel = ch; }

  const Channel &getChannel() const { return channel; }

  /**
   *
   *  add one action to this transition
   *
   * @param a
   *
   * @return
   */
  void addCounterAction( const CounterAction *action ) {
    actions.push_back( action );
  }

  /**
   * add one clock reset  to this transition
   *
   * @param r
   *
   * @return
   */
  void addReset( pair<int, int> &reset ) { resets.push_back( reset ); }

  void addCounterCons( CounterConstraint *guards ) {
    counterCons.push_back( guards );
  }

  /**
   * @brief Except synchronize signal, other state satisfies jump conditions
   *
   * @param comp
   * @param manager
   * @param state
   *
   * @return
   */
  bool ready( const int component, const StateManager<C> &manager,
              const NIntState *state ) const {
    if ( !guards.empty() ) {

      const D &dbmManager = manager.getClockManager( component );
      const C *sourceDBM  = manager.getkDBM( component, state );
      assert( dbmManager.isConsistent( sourceDBM ) );
      C *copyDBM = dbmManager.createDBM( sourceDBM );

      for ( auto cs : guards ) {
        dbmManager.andImpl( copyDBM, cs );
      }

      if ( !dbmManager.isConsistent( copyDBM ) ) {
        dbmManager.destroyDBM( copyDBM );
        return false;
      }
      dbmManager.destroyDBM( copyDBM );
    }

    if ( !counterCons.empty() ) {
      const C *counterValue = manager.getCounterValue( state );

      for ( auto cs : counterCons ) {
        if ( !( *cs )( manager.getParameterValue( component ),
                       counterValue ) ) {
          return false;
        }
      }
    }

    return true;
  }

  NIntState *operator()( const int component, const StateManager<C> &manager,
                         const NIntState *const state ) const {
    assert( ready( component, manager, state ) );

    NIntState *re_state = state->copy();

    const D &dbmManager = manager.getClockManager( component );

    C *sourceDBM = manager.getkDBM( component, re_state );

    for ( auto reset : resets ) {
      assert( reset.first > 0 );   // clock id start from 1
      assert( reset.second >= 0 ); // clock value must positive
      dbmManager.resetImpl( sourceDBM, reset.first, reset.second );
    }

    C *counterValue = manager.getCounterValue( re_state );

    for ( auto act : actions ) {
      ( *act )( manager.getParameterValue( component ), counterValue );
    }

    return re_state;
  }

private:
  int source, target; // source location and target location of this
  // transitionedge. The index of location in tma.locations
  vector<CS> guards; // set of constraint at this transitionedge

  vector<CounterConstraint *>
          counterCons; // counter constraint like pid ==id or id==0
  Channel channel;     // Only one synchronisation channels

  vector<const CounterAction *>
                         actions; // set of actions at this transitionedge
  vector<pair<int, int>> resets;  // set of reset clock variables
};
} // namespace graphsat

#endif
