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
#include "model/ta.hpp"
#include "util/datacompression.h"

namespace graphsat {

using std::vector;

template <typename C> class StateConvert {

public:
  StateConvert() {
    head_length      = 0;
    head_comp        = 0;
    compression_size = 0;
  }
  StateConvert( int hLen, int bLen, Compression<C> hCom, Compression<C> bCom ) {
    head_length = hLen;

    com_head_length = hCom.getCompressionSize();

    compression_size = hCom.getCompressionSize() + bCom.getCompressionSize();
    head_comp        = hCom;
    body_comp        = bCom;
  }

  void encode( const C *data, UINT *out ) const {
    head_comp.encode( data, out );
    body_comp.encode( data + head_length, out + com_head_length );
  }

  void decode( const UINT *data, C *out ) const {
    head_comp.decode( data, out );
    body_comp.decode( data + com_head_length, out + head_length );
  }
  int getCompressionSize() const { return compression_size; }

  int getCompressionHeadSize() const { return head_comp.getCompressionSize(); }

private:
  int head_length;
  int com_head_length;

  int            compression_size;
  Compression<C> head_comp;
  Compression<C> body_comp;
};
  
  

template <typename C> class StateManager {

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
  StateManager() {

    component_num = state_length = counter_start_loc = freeze_location_index =
        0;
    channel_num = 0;
  }

  StateManager( int comp_num, const vector<Counter> &ecounters, int clock_num,
                const vector<C> &                 oclock_upper_bounds,
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

  bool isFreeze( const C *const state ) const {
    return state[ freeze_location_index ] > 0;
  }

  Compression<C> getHeadCompression() const {
    Compression<C> re_comp( clock_start_loc );
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

  Compression<C> getBodyCompression() const {
    int            body_len = state_length - clock_start_loc;
    Compression<C> re_comp( body_len );
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

  int getLoc( int component, const C *const state ) const {
    if ( isCommitComp( component, state ) ) { // commit location
      return getCommitLoc( component, state );
    }

    return state[ component ];
  }

  bool withoutChannel( int component, const C *const state ) const {
    return state[ component + component_num ] == NO_CHANNEL;
  }

  C *newState() const {

    C *re_state = new C[ state_length ];

    fill( re_state, re_state + clock_start_loc, 0 );

    dbm_manager.init( re_state + clock_start_loc );

    return re_state;
  }

  void copy( C *des_state, const C *const source_state ) const {
    memcpy( des_state, source_state, state_length * sizeof( C ) );
  }

  C *newState( const C *s ) const {
    C *re = new C[ state_length ];
    memcpy( re, s, state_length * sizeof( C ) );
    return re;
  }

  void destroyState( C *s ) const { delete[] s; }

  inline int                  getComponentNum() const { return component_num; }
  inline const DBMFactory &getClockManager() const { return dbm_manager; }

  void norm( const C *const dbm, vector<C *> &re_vec ) const {
    C *newDBM = dbm_manager.createDBM( dbm );
    dbm_manager.norm( newDBM, re_vec );
  }

  void norm( C *dbm ) { dbm_manager.norm( dbm ); }

  inline C *getDBM( C *state ) const { return state + clock_start_loc; }

  inline const C *getDBM( const C *const state ) const {
    return state + clock_start_loc;
  }

  inline C *getCounterValue( C *state ) const {
    return state + counter_start_loc;
  }

  inline const C *getCounterValue( const C *const state ) const {
    return state + counter_start_loc;
  }

  inline void andImpl( const ClockConstraint &cs, C *state ) const {
    return dbm_manager.andImpl( getDBM( state ), cs );
  }
  inline bool isConsistent( C *state ) const {
    return dbm_manager.isConsistent( getDBM( state ) );
  }

  inline vector<int> blockComponents( const int      chid,
                                      const C *const state ) const {
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
                              const C *const state, C *dbm, bool isCommit,
                              C *re_state ) const {

    memcpy( re_state, state, clock_start_loc * sizeof( C ) );

    re_state[ component_id ] = target;
    memcpy( re_state + clock_start_loc, dbm,
            ( state_length - clock_start_loc ) * sizeof( C ) );
    if ( isCommit ) {
      setCommitState( component_id, re_state );
    }
  }

  inline void constructState( const int component_id, const int target,
                              bool isCommit, C *state ) {
    state[ component_id ] = target;
    if ( isCommit ) {
      setCommitState( component_id, state );
    }
  }

  inline bool isCommitComp( const int      component_id,
                            const C *const state ) const {
    return state[ component_id ] < 0;
  }

  /**
   * @brief  comonent_id in a commit location
   *
   * @param component_id component id
   * @param state set state
   */
  inline void setCommitState( const int component_id, C *state ) const {
    state[ component_id ] = -1 - state[ component_id ];
  }

  inline int getCommitLoc( const int      component_id,
                           const C *const state ) const {
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
  vector<C>                  clock_upper_bounds;
  DBMFactory              dbm_manager;
  vector<Counter>            counters;
  vector<Parameter>          parameters;
  vector<int>                node_nums;
  vector<int>                link_nums;
};
} // namespace graphsat
#endif
