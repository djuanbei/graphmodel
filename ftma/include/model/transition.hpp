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
#include "state/statemanager.hpp"

#include "util/instancefactory.h"

namespace graphsat {
using std::vector;
 class Location;

template < typename CS, typename DBMManager> class Transition {

  typedef Transition<CS, DBMManager> Transition_t;

public:
  Transition() {
    source = target = -1;
    has_channel     = false;
  }
  Transition( int s, int t ) {
    source      = s;
    target      = t;
    has_channel = false;
  }
  Transition( const Location &lhs,
              const Location &rhs ) {
    source      = lhs.getId();
    target      = rhs.getId();
    has_channel = false;
  }

  Transition( const Transition_t &other, const Parameter &param ) {
    source = other.source;
    target = other.target;
    guards = other.guards;

    for ( auto e : other.counter_cons ) {
      CounterConstraint *dummy = copy( e );
      dummy->globalUpdate( param.getCounterMap(), param.getParameterMap() );
      counter_cons.push_back( dummy );
    }

    has_channel = other.has_channel;
    if ( has_channel ) {
      channel = other.channel;
      channel.globalIpUpdate( param.getChanMap() );
    }

    for ( auto a : other.actions ) {
      CounterAction *dummy = a->copy();
      dummy->globalUpdate( param.getCounterMap(), param.getParameterMap() );
      actions.push_back( dummy );
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
   * @param lhs The original transition
   * @param cs  constraint
   *
   * @return Transition
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
   */
  Transition_t &operator+=( CS &cs ) {
    guards.push_back( cs );
    return *this;
  }

  void setChannel( const Channel &ch ) {
    channel     = ch;
    has_channel = true;
  }

  const Channel &getChannel() const { return channel; }

  void setChanType( CHANNEL_TYPE type ) { channel.type = type; }

  bool hasChannel() const { return has_channel; }

  /**
   *
   *  add one action to this transition
   *
   * @param action Add one counter action
   *
   */
  void addCounterAction( const void *action ) {
    actions.push_back( (CounterAction *) action );
  }

  /**
   * add one clock reset  to this transition
   *
   * @param reset The reset
   *
   */
  void addReset( pair<int, int> &reset ) { resets.push_back( reset ); }

  void addCounterCons( void *guards ) {
    counter_cons.push_back( (CounterConstraint *) guards );
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
  bool ready( const int component, const StateManager &manager,
              const int *const state ) const {
    if ( !guards.empty() ) {

      const DBMManager &dbm_manager = manager.getClockManager();
      const int *         source_DBM  = manager.getDBM( state );
      assert( dbm_manager.isConsistent( source_DBM ) );
      int *copy_DBM = dbm_manager.createDBM( source_DBM );

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
      const int *counter_value = manager.getCounterValue( state );

      for ( auto cs : counter_cons ) {
        if ( !( *cs )( counter_value ) ) {
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
   */

  void operator()( const int component, const StateManager &manager,
                   int *re_state ) const {
    assert( ready( component, manager, re_state ) );

    const DBMManager &dbm_manager = manager.getClockManager();

    int *source_DBM = manager.getDBM( re_state );

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

    if ( !actions.empty() ) {

      int *counterValue = manager.getCounterValue( re_state );

      for ( auto act : actions ) {
        ( *act )( counterValue );
      }
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
