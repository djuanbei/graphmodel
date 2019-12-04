
#include "state/ta_statemanager.h"
#include<cassert>

namespace graphsat {
TMStateManager::TMStateManager( int comp_num, const vector<Counter> &ecounters,
                                int                clock_num,
                                const vector<int> &oclock_upper_bounds,
                                const vector<ClockConstraint> &edifference_cons,
                                const vector<int> &            nodes,
                                const vector<int> &links, int channel_n ) {

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

int *TMStateManager::newState() const {

  int *re_state = new int[ state_length ];

  fill( re_state, re_state + clock_start_loc, 0 );

  dbm_manager.init( re_state + clock_start_loc );

  return re_state;
}

Compression<int> TMStateManager::getHeadCompression() const {
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
    for ( int component_id = 0; component_id < component_num; component_id++ ) {
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

Compression<int> TMStateManager::getBodyCompression() const {
  int              body_len = state_length - clock_start_loc;
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

int TMStateManager::getLoc( int component, const int *const state ) const {
  if ( isCommitComp( component, state ) ) { // commit location
    return getCommitLoc( component, state );
  }

  return state[ component ];
}

vector<int> TMStateManager::blockComponents( const int        chid,
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

void TMStateManager::constructState( const int component_id, const int target,
                                     const int *const state, int *dbm,
                                     bool isCommit, int *re_state ) const {

  memcpy( re_state, state, clock_start_loc * sizeof( int ) );

  re_state[ component_id ] = target;
  memcpy( re_state + clock_start_loc, dbm,
          ( state_length - clock_start_loc ) * sizeof( int ) );
  if ( isCommit ) {
    setCommitState( component_id, re_state );
  }
}
void TMStateManager::constructState( const int component_id, const int target,
                                     bool isCommit, int *state ) {
  state[ component_id ] = target;
  if ( isCommit ) {
    setCommitState( component_id, state );
  }
}

} // namespace graphsat
