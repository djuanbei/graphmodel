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

  StateManager( int comp_num, int counter_num, int clock_num,
                const vector<C> &                 clockUpperBound,
                const vector<ClockConstraint<C>> &edifferenceCons,
                const vector<Parameter> &ps, bool haveCh ) {

    haveChannel           = haveCh;
    component_num         = comp_num;
    maximum_upper         = clockUpperBound;
    differenceConstraints = edifferenceCons;

    if ( haveChannel ) {
      counter_start_loc = 2 * component_num;
      stateLen          = 2 * component_num + counter_num;
    } else {
      counter_start_loc = component_num;
      stateLen          = component_num + counter_num;
    }

    clock_start_loc = stateLen;

    stateLen += ( clock_num + 1 ) * ( clock_num + 1 );

    dbmManager =
        DBMFactory<C>( clock_num, clockUpperBound, differenceConstraints );

    parameters = ps;
  }
  int getStateLen() const { return stateLen; }

  int getStateStart() const { return clock_start_loc; }

  bool hasChannel() const { return haveChannel; }

  int getLoc( int component, const State_t *const state ) const {
    if ( isCommitComp( component, state ) ) { // commit location
      return getCommitLoc( component, state );
    }
    return state[ component ];
  }

  State_t *newState() const {

    State_t *re_state = new State_t[ stateLen ];

    fill( re_state, re_state + clock_start_loc, 0 );

    dbmManager.init( re_state + clock_start_loc );

    return re_state;
  }

  State_t *newState( const State_t *s ) const {
    State_t *re = new State_t[ stateLen ];
    memcpy( re, s, stateLen * sizeof( State_t ) );
    return re;
  }

  void destroyState( State_t *s ) const { delete[] s; }

  inline int                  getComponentNum() const { return component_num; }
  inline const DBMFactory<C> &getClockManager() const { return dbmManager; }
  inline const int *          getParameterValue( const int i ) const {
    return parameters[ i ].getValue();
  }

  void norm( const State_t *const dbm, vector<C *> &re_vec ) const {
    State_t *newDBM = dbmManager.createDBM( dbm );
    dbmManager.norm( newDBM, re_vec );
  }

  inline C *getDBM( State_t *state ) const { return state + clock_start_loc; }

  inline const C *getDBM( const State_t *const state ) const {
    return state + clock_start_loc;
  }

  inline C *getCounterValue( State_t *state ) const {
    return state + counter_start_loc;
  }

  inline const C *getCounterValue( const State_t *const state ) const {
    return state + counter_start_loc;
  }

  inline void andImpl( const ClockConstraint<C> &cs, State_t *state ) const {
    return dbmManager.andImpl( getDBM( state ), cs );
  }
  inline bool isConsistent( State_t *state ) const {
    return dbmManager.isConsistent( getDBM( state ) );
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

  State_t *add( const int component_id, const int target,
                StateSet<State_t> &stateSet, const State_t *const state, C *dbm,
                bool isCommit ) const {
    State_t *re_state = newState( state );

    re_state[ component_id ] = target;
    memcpy( re_state + clock_start_loc, dbm,
            ( stateLen - clock_start_loc ) * sizeof( C ) );
    if ( isCommit ) {
      setCommitState( component_id, target, re_state );
    }

    // #ifndef CHECK_MEMORY
    if ( !stateSet.add( re_state ) ) {
      destroyState( re_state );
      return NULL;
    }
    // #endif

    return re_state;
  }

  inline bool isCommitComp( const int            component_id,
                            const State_t *const state ) const {
    return state[ component_id ] < 0;
  }

  inline void setCommitState( const int component_id, const int target,
                              State_t *state ) const {
    state[ component_id ] = -1 - state[ component_id ];
  }

  inline int getCommitLoc( const int            component_id,
                           const State_t *const state ) const {
    return -( state[ component_id ] ) - 1;
  }

private:
  bool haveChannel;
  int  component_num;
  int  stateLen;

  int counter_start_loc;
  int clock_start_loc;

  vector<C> maximum_upper;

  vector<ClockConstraint<C>> differenceConstraints;

  DBMFactory<C> dbmManager;

  vector<Parameter> parameters;
};
} // namespace graphsat
#endif
