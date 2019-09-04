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
template <typename C, typename CS, typename D> class Transition {

  typedef Transition<C, CS, D> Transition_t;

public:
  Transition() {
    source = target = -1;
    has_channel     = false;
  }
  Transition( int s, int t ) {
    source = s;
    target = t;
  }

  Transition( const Transition_t &other ) {
    source = other.source;
    target = other.target;
    guards = other.guards;
    for ( auto e : other.counter_cons ) {
      counter_cons.push_back( e->copy() );
    }

    has_channel = other.has_channel;

    Channel channel; // Only one synchronisation channels

    for ( auto a : other.actions ) {
      actions.push_back( a->copy() );
    }
    resets = other.resets;
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

  void setChannel( const Channel &ch ) {
    channel = ch;
    if ( ch.id > NO_CHANNEL ) {
      has_channel = true;
    }
  }

  const Channel &getChannel() const { return channel; }

  bool hasChannel() const { return has_channel; }

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
    counter_cons.push_back( guards );
  }

  /**
   * @brief Except synchronize signal, other state satisfies jump conditions
   *
   * @param comp
   * @param manager
   * @param state
   *
   * @return true if the gurad on this tranisition is true under state, false
   * otherwise.
   */
  bool ready( const int component, const StateManager<C> &manager,
              const C *const state ) const {
    if ( !guards.empty() ) {

      const D &dbm_manager = manager.getClockManager();
      const C *source_DBM  = manager.getDBM( state );
      assert( dbm_manager.isConsistent( source_DBM ) );
      C *copy_DBM = dbm_manager.createDBM( source_DBM );

      for ( auto cs : guards ) {
        dbm_manager.andImpl( copy_DBM, cs );
      }

      if ( !dbm_manager.isConsistent( copy_DBM ) ) {
        dbm_manager.destroyDBM( copy_DBM );
        return false;
      }
      dbm_manager.destroyDBM( copy_DBM );
    }

    if ( !counter_cons.empty() ) {
      const C *counter_value = manager.getCounterValue( state );

      for ( auto cs : counter_cons ) {
        if ( !( *cs )( manager.getParameterValue( component ),
                       counter_value ) ) {
          return false;
        }
      }
    }
    return true;
  }
  /**
   *
   *@brief  create new state
   *
   * @return  a new state
   */

  void operator()( const int component, const StateManager<C> &manager,
                   C *re_state ) const {
    assert( ready( component, manager, re_state ) );

    const D &dbm_manager = manager.getClockManager();

    C *source_DBM = manager.getDBM( re_state );

    /**
     * the state which statisfied the guards can jump this transition
     *
     */
    for ( auto cs : guards ) {
      dbm_manager.andImpl( source_DBM, cs );
    }

    for ( auto reset : resets ) {
      assert( reset.first > 0 );   // clock id start from 1
      assert( reset.second >= 0 ); // clock value must positive
      dbm_manager.resetImpl( source_DBM, reset.first, reset.second );
    }

    C *counterValue = manager.getCounterValue( re_state );

    for ( auto act : actions ) {
      ( *act )( manager.getParameterValue( component ), counterValue );
    }

    //    return re_state;
  }
  void clockShift( int shift ) {
    for ( size_t i = 0; i < guards.size(); i++ ) {
      guards[ i ].clockShift( shift );
    }
    for ( size_t i = 0; i < resets.size(); i++ ) {
      resets[ i ].first += shift;
    }
  }

private:
  int source, target; // source location and target location of this
  // transitionedge. The index of location in tma.locations
  vector<CS> guards; // set of constraint at this transitionedge

  vector<CounterConstraint *>
          counter_cons; // counter constraint like pid ==id or id==0
  Channel channel;      // Only one synchronisation channels
  bool    has_channel;

  vector<const CounterAction *>
                         actions; // set of actions at this transitionedge
  vector<pair<int, int>> resets;  // set of reset clock variables
};
} // namespace graphsat

#endif
