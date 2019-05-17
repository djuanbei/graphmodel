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

      for ( size_t j = 0; j < cons[ i ].size(); j++ ) {
        manager.andImpl( i, cons[ i ][ j ], newstate );
        if ( !manager.isConsistent( i, newstate ) ) {
          delete newstate;
          return false;
        }
      }
    }

    delete newstate;
    return true;
  }

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
    for ( typename StateSet_t::iterator it = reachSet.begin(); it != end1;
          ++it ) {
      if ( 0 == memcmp( &loc[ 0 ], ( *it )->value,
                        component_num * sizeof( int ) ) ) {

        if ( isReachable( cons, *it ) ) {
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
                State_t *state, StateSet_t &secondWaitSet ) {

    int source = 0;

    for ( int i = 0; i < component_num; i++ ) {
      if ( state->value[ i + component_num ] != 0 ) {
        /**
         * waiting for synchronize signal
         *
         */
        continue;
      }

      int source    = state->value[ i ];
      int target    = 0;
      int outDegree = sys.tas[ i ].graph.getOutDegree( source );
      for ( int j = 0; j < outDegree; j++ ) {
        int link = sys.tas[ i ].graph.getAdj( source, j );
        sys.tas[ i ].graph.findRhs( link, source, target );
        DBM_t discreteTransNext = sys.tas[ i ].transitions[ link ](
            manager.getClockManager( i ), manager.getkDBM( i, state ) );

        if ( discreteTransNext != NULL ) {

          vector<C_t *> advanceNext;

          if ( sys.tas[ i ].locations[ target ]( manager.getClockManager( i ),
                                                 discreteTransNext,
                                                 advanceNext ) ) {

            for ( size_t h = 0; h < advanceNext.size(); h++ ) {

              State_t *temp =
                  manager.add( i, target, reachSet, advanceNext[ h ], state );

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
      }
    }
    return false;
  }
};
typedef ReachableSet<TAS_t> R_t;
} // namespace graphsat

#endif
