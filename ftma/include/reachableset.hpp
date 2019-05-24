/**
 * @file   reachset.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Apr 27 08:49:05 2019
 *
 * @brief  the data of reachset
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

    manager       = sys.getStateManager();
    component_num = sys.getComponentNum();
    State_t *D    = manager.newState();
    sys.initState( manager, D );

    waitSet.push_back( D );
    reachSet.add( D );
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

  bool oneStep( const Property *prop, const State_t *const state ) {
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
      if ( state->value[ component + component_num ] != 0 ) {
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

  size_t size( ) const{
    return reachSet.size( );
  }

private:
  StateSet<State_t>      reachSet;
  deque<const State_t *> waitSet;

  const SYS &    sys;
  StateManager_t manager;
  template <typename R1> friend class Reachability;
  int                        component_num;
  std::default_random_engine generator;

  bool isReach( const Property *prop, const State_t *const state ) const {
    return ( *prop )( manager, state->value );
  }

  bool oneComponent( int component, const Property *prop,
                     const State_t *const state ) {

    int source = state->value[ component ];

    if ( manager.isCommitComp( component, state ) ) { // commit location
      source = manager.getCommitLoc( component, state );
    }

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

      const Channel &ch = sys.tas[ component ].transitions[ link ].getChannel();
      if ( ch.id > -1 ) {
        vector<int> waitComponents;
        if ( CHANNEL_SEND == ch.action ) {
          waitComponents = manager.blockComponents( -ch.id, state );
        } else if ( CHANNEL_RECEIVE == ch.action ) {
          waitComponents = manager.blockComponents( ch.id, state );
        }
        if ( !waitComponents.empty() ) {
          if ( ch.type == ONE2ONE ) {
            std::uniform_int_distribution<int> distribution(
                0, waitComponents.size() - 1 );
            int id  = distribution( generator );
            int cid = waitComponents[ id ];

            if ( unBlockOne( cid, link, state, prop ) ) {
              return true;
            }
          } else if ( ch.type == ONE2ALL ) {
            for ( auto id : waitComponents ) {
              int cid = waitComponents[ id ];
              if ( unBlockOne( cid, link, state, prop ) ) {
                return true;
              }
            }
          }

        } else {
          State_t *temp_state = state->copy();
          if ( CHANNEL_SEND == ch.action ) {
            temp_state->value[ component + component_num ] = ch.id;
          } else if ( CHANNEL_RECEIVE == ch.action ) {
            temp_state->value[ component + component_num ] = -ch.id;
          }

          temp_state->value[ component ] = link; // block link
          if ( reachSet.add( temp_state ) ) {
            waitSet.push_back( temp_state );
          }
        }

      } else {
        if ( oneTranision( component, link, prop, state ) ) {
          return true;
        }
      }
    }
    return false;
  }

  bool oneTranision( const int component, const int link, const Property *prop,
                     const State_t *const state ) {
    int target = 0;
    sys.tas[ component ].graph.findSnk( link, target );
    State_t *next_state =
        sys.tas[ component ].transitions[ link ]( component, manager, state );

    if ( sys.tas[ component ].locations[ target ](
            manager.getClockManager( component ),
            manager.getkDBM( component, next_state ) ) ) {

      if(manager.add( component, target, reachSet,
                      next_state)){// add to reachse

        if ( sys.tas[ component ].locations[ target ].isCommit() ) {
          manager.setCommitState( component, target, next_state );
        }

        waitSet.push_back( next_state );

        if ( isReach( prop, next_state ) ) {
          
          return true;
        }

      }else{
        delete next_state;
      }
    }else{
      delete next_state;
    }
 
    return false;
  }

  bool unBlockOne( const int cid, const int link, const State_t *const state,
                   const Property *prop ) {
    const int blockChannel              = state->value[ cid + component_num ];
    state->value[ cid + component_num ] = 0;
    const int blockLink                 = state->value[ cid ];
    int       blockSource               = 0;
    sys.tas[ cid ].graph.findSrc( blockLink, blockSource );

    state->value[ cid ] = blockSource;
    if ( oneTranision( cid, blockLink, prop, state ) ) {
      state->value[ cid + component_num ] = blockChannel;
      state->value[ cid ]                 = blockLink;
      return true;
    }

    if ( oneTranision( cid, link, prop, state ) ) {
      state->value[ cid + component_num ] = blockChannel;
      state->value[ cid ]                 = blockLink;

      return true;
    }
    state->value[ cid + component_num ] = blockChannel;
    state->value[ cid ]                 = blockLink;
    return false;
  }
};
typedef ReachableSet<TAS_t> R_t;
} // namespace graphsat

#endif
