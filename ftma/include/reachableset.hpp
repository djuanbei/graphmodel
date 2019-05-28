/**
 * @file   reachableset.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Apr 27 08:49:05 2019
 *
 * @brief  the data of reachableset
 *
 *
 */
#ifndef __REACH_SET_HPP
#define __REACH_SET_HPP
#include <algorithm>
#include <deque>
#include <random>
#include <vector>

namespace graphsat {
using std::deque;
using std::fill;
using std::vector;

template <typename SYS> class ReachableSet {

 public:
  ReachableSet( const SYS &outta )
      : sys( outta ) {

    manager        = sys.getStateManager();
    component_num  = sys.getComponentNum();
    State_t *state = manager.newState();

    sys.initState( manager, state );
    
    reachSet.setParam( manager.getStateLen(), manager.getStateStart() );
    if( manager.getClockManager( ).isConsistent( manager.getDBM( state) )){
      waitSet.push_back( state );

      reachSet.add( state );
    }else{
      manager.destroyState( state);
    }
  }

  ~ReachableSet() { reachSet.clear(); }

  const SYS &getSYS( void ) const { return sys; }

  Check_State search( const Property *prop ) {

    typename StateSet_t::iterator end1 = reachSet.end();
    for ( auto state : reachSet ) {

      if ( isReach( prop, state ) ) {
        return TRUE;
      }
    }
    return UNKOWN;
  }

  bool oneStep( const Property *prop, State_t *state ) {

    int commit_component = -1;
    for ( int component = 0; component < component_num; component++ ) {
      if ( manager.isCommitComp( component, state ) ) {
        commit_component = component;
        break;
      }
    }
    if ( commit_component > -1 ) {
      return oneComponent( commit_component, prop, state );
    }
    for ( int component = 0; component < component_num; component++ ) {

      if ( manager.hasChannel() && state[ component + component_num ] != 0 ) {
        /**
         * Waiting for synchronize signal
         *
         */
        continue;
      }
      if ( oneComponent( component, prop, state ) ) {

        return true;
      }
    }
    return false;
  }

  size_t size() const { return reachSet.size(); }

 private:
  StateSet<State_t> reachSet;
  deque<State_t *>  waitSet;

  const SYS &    sys;
  StateManager_t manager;
  template <typename R1> friend class Reachability;
  int                        component_num;
  std::default_random_engine generator;

  bool isReach( const Property *prop, const State_t *const state ) const {
    return ( *prop )( manager, state );
  }

  bool oneComponent( int component, const Property *prop, State_t *state ) {

    int source = manager.getLoc( component, state );

    int outDegree = sys.tas[ component ].graph.getOutDegree( source );
    for ( int j = 0; j < outDegree; j++ ) {

      int link = sys.tas[ component ].graph.getAdj( source, j );
      /**
       * Whether the jump conditions satisfies except synchronize signal
       *
       */

      if ( !sys.tas[ component ].transitions[ link ].ready( component, manager,
                                                            state ) ) {

        continue;
      }

      const Channel &channel =
          sys.tas[ component ].transitions[ link ].getChannel();

      if ( channel.id > -1 ) {
        if ( doSynchronize( component, prop, state, link, channel ) ) {
          return true;
        }
      } else {
        if ( oneTranision( component, link, prop, state ) ) {
          return true;
        }
      }
    }
    return false;
  }

  bool unBlockOne( const int block_component_id, const int link, State_t *state,
                   const Property *prop ) {
    const int blockChannel = state[ block_component_id + component_num ];
    state[ block_component_id + component_num ] = 0;
    const int blockLink                         = state[ block_component_id ];
    int       blockSource                       = 0;
    sys.tas[ block_component_id ].graph.findSrc( blockLink, blockSource );

    state[ block_component_id ] = blockSource;
    if ( oneTranision( block_component_id, blockLink, prop, state ) ) {
      state[ block_component_id + component_num ] = blockChannel;
      state[ block_component_id ]                 = blockLink;
      return true;
    }

    if ( oneTranision( block_component_id, link, prop, state ) ) {
      state[ block_component_id + component_num ] = blockChannel;
      state[ block_component_id ]                 = blockLink;

      return true;
    }
    state[ block_component_id + component_num ] = blockChannel;
    state[ block_component_id ]                 = blockLink;
    return false;
  }

  bool doSynchronize( int component, const Property *prop, State_t *state,
                      int link, const Channel &channel ) {

    vector<int> waitComponents;
    if ( CHANNEL_SEND == channel.action ) {
      waitComponents = manager.blockComponents( -channel.id, state );
    } else if ( CHANNEL_RECEIVE == channel.action ) {
      waitComponents = manager.blockComponents( channel.id, state );
    }
    if ( !waitComponents.empty() ) {
      if ( channel.type == ONE2ONE ) {
        std::uniform_int_distribution<int> distribution(
            0, waitComponents.size() - 1 );
        int id                 = distribution( generator );
        int block_component_id = waitComponents[ id ];

        if ( unBlockOne( block_component_id, link, state, prop ) ) {
          return true;
        }
      } else if ( channel.type == ONE2ALL ) {
        for ( auto id : waitComponents ) {
          int block_component_id = waitComponents[ id ];
          if ( unBlockOne( block_component_id, link, state, prop ) ) {
            return true;
          }
        }
      }

    } else {
      State_t *temp_state = manager.newState( state );
      if ( CHANNEL_SEND == channel.action ) {
        temp_state[ component + component_num ] = channel.id;
      } else if ( CHANNEL_RECEIVE == channel.action ) {
        temp_state[ component + component_num ] = -channel.id;
      }

      temp_state[ component ] = link; // block link
      if ( reachSet.add( temp_state ) ) {
        waitSet.push_back( temp_state );
      }
    }
    return false;
  }

  

  bool oneTranision( const int component, const int link, const Property *prop,
                     const State_t *const state ) {
    int target = 0;
    sys.tas[ component ].graph.findSnk( link, target );
    State_t *next_state = sys.tas[ component ].transitions[ link ](
        component, manager, state ); // new state

    if ( sys.tas[ component ].locations[ target ](
            manager.getClockManager(), manager.getDBM( next_state ) ) ) {

      for ( int comp_id = 0; comp_id < component_num; comp_id++ ) {
        sys.tas[ comp_id ]
            .locations[ manager.getLoc( comp_id, next_state ) ]
            .employInvariants( manager.getClockManager(),
                               manager.getDBM( next_state ) );
      }
      if ( manager.add( component, target, reachSet,
                        next_state ) ) { // add to reachableSet

        if ( sys.tas[ component ].locations[ target ].isCommit() ) {
          manager.setCommitState( component, target, next_state );
        }

        waitSet.push_back( next_state );

        if ( isReach( prop, next_state ) ) {

          return true;
        }

      } else {
        manager.destroyState( next_state );
      }
    } else {
      manager.destroyState( next_state );
    }

    return false;
  }




};
typedef ReachableSet<TAS_t> R_t;
} // namespace graphsat

#endif
