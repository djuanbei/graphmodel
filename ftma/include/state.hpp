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

namespace graphsat {

using std::vector;

template <typename C> struct StateManager {
  /**
   * state is [loc, channel_state, counter_state, clock_state]
   *
   */

  int component_num;
  int stateLen;

  int counter_start_loc;

  vector<int> clock_start_loc;

  vector<DBMFactory<C>> clock_manager;

  StateManager() { component_num = stateLen = counter_start_loc = 0; }

  StateManager( int comp_num, int counter_num, vector<int> clock_num,
                vector<vector<C>>                  clockUpperBound,
                vector<vector<ClockConstraint<C>>> differenceCons ) {

    component_num = comp_num;

    counter_start_loc = 2 * component_num;

    stateLen = 2 * component_num + counter_num;
    ;

    for ( size_t i = 0; i < clock_num.size(); i++ ) {

      clock_start_loc.push_back( stateLen );

      stateLen += ( clock_num[ i ] + 1 ) * ( clock_num[ i ] + 1 );

      DBMFactory<C> temp =
          DBMFactory<C>( clock_num[ i ], clockUpperBound[ i ], differenceCons[ i ] );

      clock_manager.push_back( temp );
    }
  }

  NIntState *newMatrix() const {
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

  inline int getClockStart( int i ) const { return clock_start_loc[ i ]; }
  inline C * getkDBM( const int k, const NIntState *const state ) const {
    return state->value + getClockStart( k );
  }
  inline C* getCounterValue(NIntState * state ) const{
    return state->value+counter_start_loc;
  }

  inline const C* getCounterValue(const NIntState *const state ) const{
    return state->value+counter_start_loc;
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
                  C *d, const NIntState *state ) const {
    NIntState *re  = state->copy();
    int        len = 0;
    if ( id + 1 < (int) clock_start_loc.size() ) {
      len = clock_start_loc[ id + 1 ] - clock_start_loc[ id ];
    } else {
      len = stateLen - clock_start_loc[ id ];
    }
    memcpy( re->value + clock_start_loc[ id ], d, sizeof( C ) * len );

    re->value[ id ] = target;

    if ( !stateSet.add( re ) ) {
      delete re;
      return NULL;
    }
    return re;
  }
};
} // namespace graphsat
#endif
