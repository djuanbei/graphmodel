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
#include <vector>

namespace graphsat {
using namespace std;

template <typename SYS> class ReachableSet {

public:
  ReachableSet( const SYS &outta )
      : sys( outta ) {

    manager       = sys.getStateManager();
    component_num = sys.getComponentNum();
    State_t *D    = manager.newMatrix();
    sys.initState( manager, D );

    waitSet.add( D );
    reachSet.add( D );
  }

  ~ReachableSet() { reachSet.clear(); }

  const SYS &getSYS( void ) const { return sys; }

  Check_State find( const vector<int> &loc, const vector<vector<CS_t>> &cons ) {
    if ( (int) loc.size() < component_num ) {
      return UNKOWN;
    }

    typename StateSet_t::iterator end1 = reachSet.end();
    for ( auto state : reachSet ) {

      if ( 0 ==
           memcmp( &loc[ 0 ], state->value, component_num * sizeof( int ) ) ) {

        if ( isReachable( cons, state ) ) {
          return TRUE;
        }
      }
    }
    return UNKOWN;
  }

  void update( StateSet_t &secondWaitSet ) {

    waitSet = secondWaitSet;
    secondWaitSet.clear();
  }

  bool oneStep( const vector<int> loc, const vector<vector<CS_t>> &cons,
                State_t * state, StateSet_t &secondWaitSet ) {
    int source = 0;
    for ( int comp = 0; comp < component_num; comp++ ) {
      if ( state->value[ comp + component_num ] != 0 ) {
        /**
         * waiting for synchronize signal
         *
         */
        continue;
      }
      int source    = state->value[ comp ];
      int outDegree = sys.tas[ comp ].graph.getOutDegree( source );
      for ( int j = 0; j < outDegree; j++ ) {
        int link = sys.tas[ comp ].graph.getAdj( source, j );

        const Channel &ch = sys.tas[ comp ].transitions[ link ].getChannel();
        if ( ch.id > -1 ) {
          int waitComp = -1;
          if ( CHANNEL_SEND == ch.action ) {
            waitComp = manager.blockComponent( -ch.id, state );
          } else if ( CHANNEL_RECEIVE == ch.action ) {
            waitComp = manager.blockComponent( ch.id, state );
          }
          if ( waitComp > -1 ) {
            State_t *temp                           = state->copy();
            temp->value[ waitComp + component_num ] = 0;
            int blockLink                           = temp->value[ waitComp ];
            int blockSource                         = 0;
            sys.tas[ waitComp ].graph.findSrc( blockLink, blockSource );
            temp->value[ waitComp ] = blockSource;
            if ( oneTranision( waitComp, blockLink, loc, cons, temp,
                               secondWaitSet ) ) {
              return true;
            }

            if ( oneTranision( comp, link, loc, cons, temp, secondWaitSet ) ) {
              return true;
            }

          } else {
            State_t *temp = state->copy();
            if ( CHANNEL_SEND == ch.action ) {
              temp->value[ comp + component_num ] = ch.id;
            } else if ( CHANNEL_RECEIVE == ch.action ) {
              temp->value[ comp + component_num ] = -ch.id;
            }

            temp->value[ comp ] = link;//block link
            if ( reachSet.add( temp ) ) {
              secondWaitSet.add( temp );
            }
          }

        } else {
          if ( oneTranision( comp, link, loc, cons, state, secondWaitSet ) ) {
            return true;
          }
        }
      }
    }
    return false;
  }

private:
  StateSet<State_t> reachSet;
  StateSet<State_t> waitSet;

  const SYS &    sys;
  StateManager_t manager;
  template <typename R1> friend class Reachability;
  int component_num;

  bool isReachable( const vector<vector<CS_t>> &cons,
                    const State_t *             state ) const {

    if ( cons.empty() ) {
      return true;
    }

    State_t *newstate = state->copy();
    for ( int i = 0; i < component_num; i++ ) {
      for ( auto cs : cons[ i ] ) {
        manager.andImpl( i, cs, newstate );
        if ( !manager.isConsistent( i, newstate ) ) {
          delete newstate;
          return false;
        }
      }
    }

    delete newstate;
    return true;
  }

  bool oneTranision( const int component, const int link, const vector<int> loc,
                     const vector<vector<CS_t>> &cons, State_t *  state,
                     StateSet_t &secondWaitSet ) {
    int target = 0;
    sys.tas[ component ].graph.findSnk( link, target );
    DBM_t discreteTransNext = sys.tas[ component ].transitions[ link ](
        manager.getClockManager( component ),
        manager.getkDBM( component, state ) );

    if ( discreteTransNext != NULL ) {

      vector<DBM_t> advanceNext;

      if ( sys.tas[ component ].locations[ target ](
               manager.getClockManager( component ), discreteTransNext,
               advanceNext ) ) {
        for ( auto next : advanceNext ) {

          State_t *temp =
              manager.add( component, target, reachSet, next, state );

          if ( temp != NULL ) {

            secondWaitSet.add( temp );
            if ( 0 == memcmp( &loc[ 0 ], temp->value,
                              component_num * sizeof( int ) ) ) {
              if ( isReachable( cons, temp ) ) {
                return true;
              }
            }
          }
        }
      }
    }
    return false;
  }
};
typedef ReachableSet<TAS_t> R_t;
} // namespace graphsat

#endif
