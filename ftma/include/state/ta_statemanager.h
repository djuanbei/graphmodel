/**
 * @file   state.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat May 18 09:28:22 2019
 *
 * @brief  state
 *
 *
 */

#ifndef __STATE_MANAGER_H
#define __STATE_MANAGER_H
#include <vector>

#include "discretestate.hpp"
#include "model/parameter.h"
#include "model/graphmodel.hpp"
#include "util/datacompression.h"

namespace graphsat {

using std::vector;
  

class TMStateManager {

  /**
   * state is [loc, channel_state, freezeLocationNum, counter_state,
   * clock_state] loc is a vector with length  the number of components,
   * channel_state is aslo a vector whose length equals to the number of
   * component. If the corresponding loc is negative integer then this location
   * is a commit location the corresponding channel_state is the block channel.
   * The channel state is positive then it is a send channel,  when the value is
   * nonegative integer it is a receive channel. When channel equals to
   * NO_CHANNEL then there is no channel in this transition.
   *
   * freezeLocationNum is the number of components which staies on freeze
   * location. The cunter_state part is the vector value of counter variables
   *
   */
public:
  TMStateManager() {

    component_num = state_length = counter_start_loc = freeze_location_index =
        0;
    channel_num = 0;
  }

  TMStateManager( int comp_num, const vector<Counter> &ecounters, int clock_num,
                const vector<int> &                 oclock_upper_bounds,
                const vector<ClockConstraint> &edifference_cons,
                const vector<int> &nodes, const vector<int> &links,
                int channel_n ) {

    assert( (int) oclock_upper_bounds.size() == 2 * clock_num + 2 );
    state_length  = 0;
    component_num = comp_num;

    difference_constraints = edifference_cons;
    clock_upper_bounds     = oclock_upper_bounds;
    node_nums              = nodes;
    link_nums              = links;
    channel_num            = channel_n;

    if ( channel_num > 0 ) {
      counter_start_loc = 2 * component_num;
      state_length      = 2 * component_num + (int) ecounters.size();
    } else {
      counter_start_loc = component_num;
      state_length      = component_num + (int) ecounters.size();
    }
    freeze_location_index = state_length;

    state_length++;
    clock_start_loc = state_length;

    state_length += ( clock_num + 1 ) * ( clock_num + 1 );

    dbm_manager =
        DBMFactory( clock_num, clock_upper_bounds, difference_constraints );
    counters = ecounters;
  }
  
  int getStateLen() const { return state_length; }

  int getClockStart() const { return clock_start_loc; }

  int getFreezeLocation() const { return freeze_location_index; }

  bool isFreeze( const int *const state ) const {
    return state[ freeze_location_index ] > 0;
  }

  Compression<int> getHeadCompression() const {
    Compression<int> re_comp( clock_start_loc );
    for ( int component_id = 0; component_id < component_num; component_id++ ) {

      if ( channel_num > 1 ) {
        // the value contain link id
        // TODO:
        int m = max( node_nums[ component_id ], link_nums[ component_id ] );
        re_comp.setBound( component_id, -m, m );
      } else {
        re_comp.setBound( component_id, -node_nums[ component_id ],
                          node_nums[ component_id ] );
      }
    }
    if ( channel_num > 0 ) {
      for ( int component_id = 0; component_id < component_num;
            component_id++ ) {
        re_comp.setBound( component_id + component_num, -channel_num,
                          channel_num );
      }
    }
    int k = 0;
    for ( int i = counter_start_loc; i < freeze_location_index; i++ ) {
      re_comp.setBound( i, counters[ k ].getLB(), counters[ k ].getUP() );
      k++;
    }

    /**
     * At most all the component in freeze location
     *
     */
    re_comp.setBound( freeze_location_index, 0, component_num - 1 );

    return re_comp;
  }

  Compression<int> getBodyCompression() const {
    int            body_len = state_length - clock_start_loc;
    Compression<int> re_comp( body_len );
    return re_comp;
    /**
     * set the minimum and maximum for DBM matrix element
     * TODO: compress by the upper bound of clock constaint
     */

    int len = (int) clock_upper_bounds.size() / 2;
    assert( body_len == len * len );

    for ( int i = 0; i < body_len; i++ ) {
      int row = ( i ) / len;
      int col = ( i ) % len;
      if ( row == col ) {
        re_comp.setBound( i, LTEQ_ZERO, LTEQ_ZERO );
      } else if ( row == 0 ) {
        re_comp.setBound( i, clock_upper_bounds[ col + len ], LTEQ_ZERO );
      } else if ( col == 0 ) {
        re_comp.setBound( i, LTEQ_ZERO, clock_upper_bounds[ row ] + 1 );
      } else {
        re_comp.setBound( i, clock_upper_bounds[ col + len ],
                          clock_upper_bounds[ row ] + 1 );
      }
    }
    return re_comp;
  }

  bool hasChannel() const { return channel_num > 0; }

  int getLoc( int component, const int *const state ) const {
    if ( isCommitComp( component, state ) ) { // commit location
      return getCommitLoc( component, state );
    }

    return state[ component ];
  }

  bool withoutChannel( int component, const int *const state ) const {
    return state[ component + component_num ] == NO_CHANNEL;
  }

  int *newState() const {

    int *re_state = new int [ state_length ];

    fill( re_state, re_state + clock_start_loc, 0 );

    dbm_manager.init( re_state + clock_start_loc );

    return re_state;
  }

  void copy( int *des_state, const int *const source_state ) const {
    memcpy( des_state, source_state, state_length * sizeof( int ) );
  }

  int *newState( const int *s ) const {
    int *re = new int[ state_length ];
    memcpy( re, s, state_length * sizeof( int ) );
    return re;
  }

  void destroyState( int *s ) const { delete[] s; }

  inline int                  getComponentNum() const { return component_num; }
  inline const DBMFactory &getClockManager() const { return dbm_manager; }

  void norm( const int  *const dbm, vector<int *> &re_vec ) const {
    int  *newDBM = dbm_manager.createDBM( dbm );
    dbm_manager.norm( newDBM, re_vec );
  }

  void norm( int *dbm ) { dbm_manager.norm( dbm ); }

  inline int *getDBM( int *state ) const { return state + clock_start_loc; }

  inline const int *getDBM( const int *const state ) const {
    return state + clock_start_loc;
  }

  inline int *getCounterValue( int *state ) const {
    return state + counter_start_loc;
  }

  inline const int *getCounterValue( const int *const state ) const {
    return state + counter_start_loc;
  }

  inline void andImpl( const ClockConstraint &cs, int *state ) const {
    return dbm_manager.andImpl( getDBM( state ), cs );
  }
  inline bool isConsistent( int *state ) const {
    return dbm_manager.isConsistent( getDBM( state ) );
  }

  inline vector<int> blockComponents( const int      chid,
                                      const int *const state ) const {
    vector<int> re_block_components;
    for ( int i = 0; i < component_num; i++ ) {

      /**
       * return entire componments which waits for this signal chid
       *
       */

      if ( state[ i + component_num ] == chid ) {
        re_block_components.push_back( i );
      }
    }
    return re_block_components;
  }

  inline void constructState( const int component_id, const int target,
                              const int *const state, int *dbm, bool isCommit,
                              int *re_state ) const {

    memcpy( re_state, state, clock_start_loc * sizeof( int ) );

    re_state[ component_id ] = target;
    memcpy( re_state + clock_start_loc, dbm,
            ( state_length - clock_start_loc ) * sizeof( int ) );
    if ( isCommit ) {
      setCommitState( component_id, re_state );
    }
  }

  inline void constructState( const int component_id, const int target,
                              bool isCommit, int *state ) {
    state[ component_id ] = target;
    if ( isCommit ) {
      setCommitState( component_id, state );
    }
  }

  inline bool isCommitComp( const int      component_id,
                            const int *const state ) const {
    return state[ component_id ] < 0;
  }

  /**
   * @brief  comonent_id in a commit location
   *
   * @param component_id component id
   * @param state set state
   */
  inline void setCommitState( const int component_id, int *state ) const {
    state[ component_id ] = -1 - state[ component_id ];
  }

  inline int getCommitLoc( const int      component_id,
                           const int *const state ) const {
    return -( state[ component_id ] ) - 1;
  }
  bool hasDiffCons() const { return !difference_constraints.empty(); }

private:
  int channel_num;
  int component_num;
  int state_length;
  int freeze_location_index;

  int counter_start_loc;
  int clock_start_loc;

  vector<ClockConstraint> difference_constraints;
  vector<int>                  clock_upper_bounds;
  DBMFactory              dbm_manager;
  vector<Counter>            counters;
  vector<Parameter>          parameters;
  vector<int>                node_nums;
  vector<int>                link_nums;
};
} // namespace graphsat
#endif
