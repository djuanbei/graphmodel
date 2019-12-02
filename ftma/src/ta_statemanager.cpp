//
//#include "state/ta_statemanager.h"
//
//namespace graphsat {
//  TMStateManager::TMStateManager( int comp_num, const vector<Counter> &ecounters, int clock_num,
//                 const vector<int> &                 oclock_upper_bounds,
//                 const vector<ClockConstraint> &edifference_cons,
//                 const vector<int> &nodes, const vector<int> &links,
//                 int channel_n ) {
//    
//    assert( (int) oclock_upper_bounds.size() == 2 * clock_num + 2 );
//    state_length  = 0;
//    component_num = comp_num;
//    
//    difference_constraints = edifference_cons;
//    clock_upper_bounds     = oclock_upper_bounds;
//    node_nums              = nodes;
//    link_nums              = links;
//    channel_num            = channel_n;
//    
//    if ( channel_num > 0 ) {
//      counter_start_loc = 2 * component_num;
//      state_length      = 2 * component_num + (int) ecounters.size();
//    } else {
//      counter_start_loc = component_num;
//      state_length      = component_num + (int) ecounters.size();
//    }
//    freeze_location_index = state_length;
//    
//    state_length++;
//    clock_start_loc = state_length;
//    
//    state_length += ( clock_num + 1 ) * ( clock_num + 1 );
//    
//    dbm_manager =
//    DBMFactory( clock_num, clock_upper_bounds, difference_constraints );
//    counters = ecounters;
//  }
//}
