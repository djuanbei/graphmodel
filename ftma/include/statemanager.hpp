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
#include "parameter.h"
namespace graphsat {

using std::vector;

template <typename C> class StateManager {
  /**
   * state is [loc, channel_state, counter_state, clock_state]
   *
   */
public:
  StateManager() { component_num = stateLen = counter_start_loc = 0; }

  StateManager( int comp_num, int counter_num, vector<int> clock_num,
                vector<vector<C>>                  clockUpperBound,
                vector<vector<ClockConstraint<C>>> differenceCons,
                const vector<Parameter> &          ps ) {

    component_num = comp_num;

    counter_start_loc = 2 * component_num;

    stateLen = 2 * component_num + counter_num;

    for ( size_t i = 0; i < clock_num.size(); i++ ) {

      clock_start_loc.push_back( stateLen );

      stateLen += ( clock_num[ i ] + 1 ) * ( clock_num[ i ] + 1 );

      DBMFactory<C> temp = DBMFactory<C>( clock_num[ i ], clockUpperBound[ i ],
                                          differenceCons[ i ] );

      clock_manager.push_back( temp );
    }

    parameters = ps;
  }

  NIntState *newState() const {
    if ( clock_start_loc.empty() ) {
      NIntState *re = new NIntState( stateLen );
      return re;
    } else {

      NIntState *re = new NIntState( stateLen, clock_start_loc[ 0 ] );

      for ( int i = 0; i < component_num; i++ ) {
        clock_manager[ i ].init( re->value + clock_start_loc[ i ] );
      }
      return re;
    }
  }

  inline const DBMFactory<C> &getClockManager( int i ) const {
    return clock_manager[ i ];
  }
  inline const Parameter &getParameter( const int i ) const {
    return parameters[ i ];
  }

  inline int getClockStart( int i ) const { return clock_start_loc[ i ]; }
  inline C * getkDBM( const int k, const NIntState *const state ) const {
    return state->value + getClockStart( k );
  }
  inline C *getCounterValue( NIntState *state ) const {
    return state->value + counter_start_loc;
  }

  inline const C *getCounterValue( const NIntState *const state ) const {
    return state->value + counter_start_loc;
  }

  inline void andImpl( const int id, const ClockConstraint<C> &cs,
                       NIntState *state ) const {
    return getClockManager( id ).andImpl( getkDBM( id, state ), cs );
  }
  inline bool isConsistent( const int id, NIntState *state ) const {
    return getClockManager( id ).isConsistent( getkDBM( id, state ) );
  }

  inline vector<int> blockComponents( const int              chid,
                                      const NIntState *const state ) const {
    vector<int> temp;
    for ( int i = 0; i < component_num; i++ ) {

      /**
       * return entire componments which waits for this signal chid
       *
       */

      if ( state->value[ i + component_num ] == chid ) {
        temp.push_back( i );
      }
    }
    return temp;
  }

  NIntState *add( const int id, const int target, StateSet<NIntState> &stateSet,
                  const C *const dbm, const NIntState *const state ) const {
    NIntState *re  = state->copy();
    int        len = 0;
    if ( id + 1 < (int) clock_start_loc.size() ) {
      len = clock_start_loc[ id + 1 ] - clock_start_loc[ id ];
    } else {
      len = stateLen - clock_start_loc[ id ];
    }
    memcpy( re->value + clock_start_loc[ id ], dbm, sizeof( C ) * len );

    re->value[ id ] = target;

    if ( !stateSet.add( re ) ) {
      delete re;
      return NULL;
    }
    return re;
  }
  inline bool isCommitComp( const int id, const NIntState *const state ) const {
    return state->value[ id ] < 0;
  }

  inline void setCommitState( const int id, const int target,
                              NIntState *state ) const {
    state->value[ id ] = -1 - state->value[ id ];
  }

  inline int getCommitLoc( const int id, const NIntState *const state ) const {
    return -( state->value[ id ] ) - 1;
  }

private:
  int component_num;
  int stateLen;

  int counter_start_loc;

  vector<int> clock_start_loc;

  vector<DBMFactory<C>> clock_manager;

  vector<Parameter> parameters;
};
} // namespace graphsat
#endif
