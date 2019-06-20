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
  typedef SYS                 SYS_t;
  typedef typename SYS_t::C_t C_t;
  ReachableSet( const SYS &outta )
      : sys( outta ) {

    manager       = sys.getStateManager();
    component_num = sys.getComponentNum();

    cache_state = manager.newState();
    next_state  = manager.newState();

    sys.initState( manager, cache_state );

    reachSet.setParam( manager.getStateLen(), manager.getStateStart() );
    if ( manager.getClockManager().isConsistent(
             manager.getDBM( cache_state ) ) ) {
      reachSet.add( cache_state );
      addToWait( cache_state );
    }
  }

  ~ReachableSet() {
    reachSet.clear();
    manager.destroyState( cache_state );
    manager.destroyState( next_state );
    while ( !waitSet.empty() ) {
      C_t *temp_state = waitSet.front();
      waitSet.pop_front();
      manager.destroyState( temp_state );
    }
  }

  const SYS &getSYS( void ) const { return sys; }

  Check_State search( const Property *prop ) {

    typename SYS::StateSet_t::iterator end1 = reachSet.end();
    for ( auto state : reachSet ) {

      if ( isReach( prop, state ) ) {
        return TRUE;
      }
    }
    return UNKOWN;
  }

  bool oneStep( const Property *prop, C_t *state ) {
#ifdef PRINT_STATE
    for ( int i = 0; i < component_num; i++ ) {
      cout << state[ i ] << " ";
    }
    cout << endl
         << manager.getClockManager().dump( manager.getDBM( state ) ) << endl;
#endif
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
  void   addToWait( const C_t *const state ) {
    C_t *newState = manager.newState( state );
    waitSet.push_back( newState );
  }

private:
  StateSet<C_t> reachSet;
  deque<C_t *>  waitSet;

  C_t *                        cache_state;
  C_t *                        next_state;
  const SYS &                  sys;
  typename SYS::StateManager_t manager;
  template <typename R1> friend class Reachability;
  int                        component_num;
  std::default_random_engine generator;

  bool isReach( const Property *prop, const C_t *const state ) const {
    return ( *prop )( manager, state );
  }

  bool oneComponent( int component, const Property *prop, C_t *state ) {

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

  bool unBlockOne( const int block_component_id, const int link, C_t *state,
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

  bool doSynchronize( int component, const Property *prop, C_t *state, int link,
                      const Channel &channel ) {

    vector<int> waitComponents;
    if ( CHANNEL_SEND == channel.action ) {
      waitComponents = manager.blockComponents( -channel.id, state );
    } else if ( CHANNEL_RECEIVE == channel.action ) {
      waitComponents = manager.blockComponents( channel.id, state );
    }
    if ( !waitComponents.empty() ) {
      if ( channel.type == ONE2ONE_CH ) {
        std::uniform_int_distribution<int> distribution(
            0, waitComponents.size() - 1 );
        int id                 = distribution( generator );
        int block_component_id = waitComponents[ id ];

        if ( unBlockOne( block_component_id, link, state, prop ) ) {
          return true;
        }
      } else if ( channel.type == BROADCAST_CH ) {
        for ( auto id : waitComponents ) {
          int block_component_id = waitComponents[ id ];
          if ( unBlockOne( block_component_id, link, state, prop ) ) {
            return true;
          }
        }
      }

    } else {
      C_t *temp_state = manager.newState( state );
      if ( CHANNEL_SEND == channel.action ) {
        temp_state[ component + component_num ] = channel.id;
      } else if ( CHANNEL_RECEIVE == channel.action ) {
        temp_state[ component + component_num ] = -channel.id;
      }

      temp_state[ component ] = link; // block link
      if ( reachSet.add( temp_state ) ) {
        addToWait( temp_state );
      }
      manager.destroyState( temp_state );
    }
    return false;
  }

  bool oneTranision( const int component, const int link, const Property *prop,
                     const C_t *const state ) {
    manager.copy( next_state, state );
    int target = 0;
    sys.tas[ component ].graph.findSnk( link, target );

    sys.tas[ component ].transitions[ link ]( component, manager,
                                              next_state ); // update state

    next_state[ component ] = target;
    bool isCommit = sys.tas[ component ].locations[ target ].isCommit();

    bool re_bool = false;
    if ( sys.tas[ component ].locations[ target ](
             manager.getClockManager(), manager.getDBM( next_state ) ) ) {

      for ( int comp_id = 0; comp_id < component_num; comp_id++ ) {

        sys.tas[ comp_id ]
            .locations[ manager.getLoc( comp_id, next_state ) ]
            .employInvariants( manager.getClockManager(),
                               manager.getDBM( next_state ) );
      }
      if ( manager.hasDiffCons() ) {

        vector<typename SYS::C_t *> next_dbms;
        manager.norm( manager.getDBM( next_state ), next_dbms );

        for ( auto dbm : next_dbms ) {

          if ( manager.add( component, target, reachSet, next_state, dbm,
                            isCommit, cache_state ) ) { // add to reachableSet
            addToWait( cache_state );
            if ( isReach( prop, cache_state ) ) {
              re_bool = true;
              break;
            }
          }
        }
        for ( auto dbm : next_dbms ) {
          manager.getClockManager().destroyDBM( dbm );
        }
      } else {
        manager.norm( manager.getDBM( next_state ) );
        if ( manager.add( component, target, reachSet, next_state,
                          isCommit ) ) { // add to reachableSet
          addToWait( next_state );
          if ( isReach( prop, next_state ) ) {
            return true;
          }
        }
      }
    }
    return re_bool;
  }
};
// typedef ReachableSet< TAS_t> R_t;
} // namespace graphsat

#endif
