/**
 * @file   state.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat May 18 09:28:22 2019
 *
 * @brief  state
 *
 *
 */

#ifndef __STATE_H
#define __STATE_H
#include <vector>

#include "discretestate.hpp"
#include "model/ta.hpp"
#include "parameter.h"

namespace graphsat {

using std::vector;

typedef int State_t;

template <typename C> class StateManager {

  /**
   * state is [loc, channel_state, counter_state, clock_state]
   *
   */
public:
  StateManager() {
    haveChannel   = false;
    component_num = stateLen = counter_start_loc = 0;
  }

  StateManager( int comp_num, int counter_num, vector<int> clock_num,
                vector<vector<C>>                  clockUpperBound,
                vector<vector<ClockConstraint<C>>> differenceCons,
                const vector<Parameter> &ps, bool haveCh ) {

    haveChannel   = haveCh;
    component_num = comp_num;
    if ( haveChannel ) {
      counter_start_loc = 2 * component_num;
      stateLen          = 2 * component_num + counter_num;
    } else {
      counter_start_loc = component_num;
      stateLen          = component_num + counter_num;
    }

    for ( size_t i = 0; i < clock_num.size(); i++ ) {

      clock_start_loc.push_back( stateLen );

      stateLen += ( clock_num[ i ] + 1 ) * ( clock_num[ i ] + 1 );

      DBMFactory<C> dbmManager = DBMFactory<C>(
          clock_num[ i ], clockUpperBound[ i ], differenceCons[ i ] );

      clock_manager.push_back( dbmManager );
    }

    parameters = ps;
  }
  int getStateLen() const { return stateLen; }

  int getStateStart() const { return clock_start_loc[ 0 ]; }

  bool hasChannel() const { return haveChannel; }

  State_t *newState() const {
    if ( clock_start_loc.empty() ) {
      State_t *re_state = new C[ stateLen ];
      fill( re_state, re_state + stateLen, 0 );
      return re_state;
    } else {
      State_t *re_state = new State_t[ stateLen ];
      fill( re_state, re_state + stateLen, 0 );
      for ( int i = 0; i < component_num; i++ ) {
        clock_manager[ i ].init( re_state + clock_start_loc[ i ] );
      }

      return re_state;
    }
  }

  State_t *newState( const State_t *s ) const {
    State_t *re = new State_t[ stateLen ];
    memcpy( re, s, stateLen * sizeof( State_t ) );
    return re;
  }
  void destroyState( State_t *s ) const { delete[] s; }

  inline int                  getComponentNum() const { return component_num; }
  inline const DBMFactory<C> &getClockManager( int i ) const {
    return clock_manager[ i ];
  }
  inline const int *getParameterValue( const int i ) const {
    return parameters[ i ].getValue();
  }


  inline int getClockStart( int i ) const { return clock_start_loc[ i ]; }
  inline C * getkDBM( const int k, State_t *state ) const {
    return state + getClockStart( k );
  }

  inline const C *getkDBM( const int k, const State_t *const state ) const {
    return state + getClockStart( k );
  }

  inline C *getCounterValue( State_t *state ) const {
    return state + counter_start_loc;
  }

  inline const C *getCounterValue( const State_t *const state ) const {
    return state + counter_start_loc;
  }

  inline void andImpl( const int component_id, const ClockConstraint<C> &cs,
                       State_t *state ) const {
    return getClockManager( component_id ).andImpl( getkDBM( component_id, state ), cs );
  }
  inline bool isConsistent( const int component_id, State_t *state ) const {
    return getClockManager( component_id).isConsistent( getkDBM(component_id, state ) );
  }

  inline vector<int> blockComponents( const int            chid,
                                      const State_t *const state ) const {
    vector<int> reBlockComponents;
    for ( int i = 0; i < component_num; i++ ) {

      /**
       * return entire componments which waits for this signal chid
       *
       */

      if ( state[ i + component_num ] == chid ) {
        reBlockComponents.push_back( i );
      }
    }
    return reBlockComponents;
  }

  bool add( const int component_id, const int target,
            StateSet<State_t> &stateSet, State_t *state ) const {

    state[ component_id ] = target;

    if ( !stateSet.add( state ) ) {
      return false;
    }
    return true;
  }

  inline bool isCommitComp( const int            component_id,
                            const State_t *const state ) const {
    return state[ component_id ] < 0;
  }

  inline void setCommitState( const int component_id, const int target,
                              State_t *state ) const {
    state[ component_id ] = -1 - state[ component_id ];
  }

  inline int getCommitLoc( const int component_id, const State_t *const state ) const {
    return -( state[component_id ] ) - 1;
  }

private:
  bool haveChannel;
  int  component_num;
  int  stateLen;

  int counter_start_loc;

  vector<int> clock_start_loc;

  vector<DBMFactory<C>> clock_manager;

  vector<Parameter> parameters;
};
} // namespace graphsat
#endif
