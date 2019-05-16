/**
 * @file   TA.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:25:54 2019
 *
 * @brief  a model for timed automata
 *
 *
 */

#ifndef __TIMED_AUTOMATA_
#define __TIMED_AUTOMATA_
#include "action.hpp"
#include "channel.h"
#include "discretestate.hpp"
#include "domain/dbm.hpp"
#include "domain/dbmset.hpp"
#include "graph/graph.hpp"
#include "location.hpp"
#include "transition.hpp"

#include <vector>

namespace graphsat {

using namespace std;
using namespace raptor;

typedef int                  C_t;
typedef C_t *                DBM_t;
typedef DBM<C_t>             DBMManager_t;
typedef DBMset<C_t>          DBMSet_t;
typedef NIntState            State_t;
typedef StateSet<NIntState>  StateSet_t;
typedef ClockConstraint<C_t> CS_t;

typedef Location<C_t, CS_t, DBMManager_t, DBMSet_t>           L_t;
typedef Transition<C_t, CS_t, DBMManager_t, DBMSet_t, Action> T_t;

/**
 *
 *  TA has multi-components
 *
 */

template <typename C, typename L, typename T> class TA {

public:
  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;

private:
  vector<L> locations;
  vector<T> transitions;
  int       initial_loc;

  int clock_num;

  graph_t<int> graph;

  vector<C> clockUppuerBound;

  vector<ClockConstraint<C>> differenceCons;

public:
  TA() { initial_loc = clock_num = -1; }
  TA( int init, int clockNum ) {
    initial_loc = init;
    clock_num   = clockNum;
  }

  TA( vector<L> &locs, vector<T> &es, int init, int vnum )
      : locations( locs )
      , transitions( es ) {
    initial_loc = init;
    clock_num   = vnum;
    initial();
  }

  void findRhs( const int link, const int lhs, int &rhs ) const {
    graph.findRhs( link, lhs, rhs );
  }
  vector<C> getClockUppuerBound() const { return clockUppuerBound; }

  vector<ClockConstraint<C>> getDifferenceCons() const {
    return differenceCons;
  }

  int getLocationNum() const { return (int) locations.size(); }
  int getTransitionNum() const { return (int) transitions.size(); }

  void initial() {

    vector<int> srcs;
    vector<int> snks;

    for ( typename vector<T>::iterator it = transitions.begin();
          it != transitions.end(); it++ ) {
      srcs.push_back( it->getSource() );
      snks.push_back( it->getTarget() );
    }

    graph.initial( srcs, snks );

    int vertex_num = graph.getVertex_num();

    // // There are no edges connect with  initial location
    assert( initial_loc >= 0 && initial_loc < vertex_num );

    differenceCons.clear();
    clockUppuerBound.resize( 2 * ( clock_num + 1 ), LTEQ_ZERO );

    for ( typename vector<L>::const_iterator it = locations.begin();
          it != locations.end(); it++ ) {

      const vector<CS_t> &invariants = it->getInvarients();
      for ( typename vector<CS_t>::const_iterator cit = invariants.begin();
            cit != invariants.end(); cit++ ) {

        if ( cit->x > 0 && cit->y > 0 ) {
          differenceCons.push_back( *cit );
        } else {
          if ( cit->x > 0 ) {
            if ( cit->matrix_value > clockUppuerBound[ cit->x ] ) {
              clockUppuerBound[ cit->x ] = cit->matrix_value;
            } else {
              CS_t temp = cit->neg();
              if ( temp.matrix_value > clockUppuerBound[ temp.x ] ) {
                clockUppuerBound[ temp.x ] = temp.matrix_value;
              }
            }
          }
        }
      }
    }
    for ( int i = 1; i <= clock_num + 1; i++ ) {
      clockUppuerBound[ i ] =
          getMatrixValue( getRight( clockUppuerBound[ i ] ), false );
    }

    /**
     *      x-y<= k_i
     *      x-y< -k_{i+n+1}
     *
     */

    for ( int i = 1; i < clock_num + 1; i++ ) {
      clockUppuerBound[ i + clock_num + 1 ] = LTEQ_ZERO - clockUppuerBound[ i ];
    }
  }

  int getClockNum() const { return clock_num; }

  void setInitialLoc( int loc ) { initial_loc = loc; }
  int  getInitialLoc() const { return initial_loc; }

  bool locationRun( int i, const DBM<C> &manager, C *D ) const {
    return locations[ i ]( manager, D );
  }

  bool transitionRun( int link, const DBM<C> &manager, C *D ) const {
    return transitions[ link ]( manager, D );
  }

  /**
   * Add one location to this timed automata
   *
   * @param location
   *
   * @return
   */
  TA<C, L, T> &operator+=( L &location ) {
    locations.push_back( location );
    return *this;
  }

  /**
   * Add one transition to this timed automata
   *
   * @param edge
   *
   * @return
   */
  TA<C, L, T> &operator+=( T &edge ) {
    transitions.push_back( edge );
    return *this;
  }
};

typedef TA<C_t, L_t, T_t> TA_t;

template <typename C> struct StateManager {
  /**
   * state is [loc, channel_state, counter_state, clock_state]
   *
   */

  int component_num;
  int stateLen;

  int counter_start_loc;

  vector<int> clock_start_loc;

  vector<DBM<C>> clock_manager;

  StateManager() { component_num = stateLen = counter_start_loc = 0; }

  StateManager( int comp_num, int counter_num, vector<int> clock_num,
                vector<vector<C>>                  clockUppuerBound,
                vector<vector<ClockConstraint<C>>> differenceCons ) {

    component_num = comp_num;

    counter_start_loc = 2 * component_num;

    stateLen = 2 * component_num + counter_num;
    ;

    for ( size_t i = 0; i < clock_num.size(); i++ ) {

      clock_start_loc.push_back( stateLen );

      stateLen += ( clock_num[ i ] + 1 ) * ( clock_num[ i ] + 1 );

      DBM<C> temp =
          DBM<C>( clock_num[ i ], clockUppuerBound[ i ], differenceCons[ i ] );

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

  inline const DBM<C> &getClockManager( int i ) const {
    return clock_manager[ i ];
  }

  inline int getClockStart( int i ) const { return clock_start_loc[ i ]; }
  inline C * getkDBM( const int k, State_t *state ) const {
    return state->value + getClockStart( k );
  }
  inline void andImpl( const int id, const CS_t &cs, State_t *state ) const {
    return getClockManager( id ).andImpl( getkDBM( id, state ), cs );
  }
  inline bool isConsistent( const int id, State_t *state ) const {
    return getClockManager( id ).isConsistent( getkDBM( id, state ) );
  }

  State_t* add( const int id, const int target,  StateSet_t &stateSet, DBM_t d, const State_t *state ) const {
    State_t* re      = state->copy();
    int len = 0;
    if ( id + 1 < (int) clock_start_loc.size() ) {
      len = clock_start_loc[ id + 1 ] - clock_start_loc[ id ];
    } else {
      len = stateLen - clock_start_loc[ id ];
    }
    memcpy( re->value + clock_start_loc[ id ], d, sizeof( C ) * len );
    
    re->value[id ]=target;

    if ( !stateSet.add( re ) ) {
      delete re;
      return NULL;
    }
    return re;
  }
};

template <typename C, typename L, typename T> class TAS {
public:
  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;

private:
  /**
   * multi-components
   *
   */
  vector<TA_t>    tas;
  vector<Channel> channels;
  vector<Counter> counters;
  vector<int>     initial_loc;
  vector<int>     clock_num;

  vector<vector<C>>                  clockUppuerBound;
  vector<vector<ClockConstraint<C>>> differenceCons;

public:
  TAS<C, L, T> &operator+=( TA_t &ta ) {
    tas.push_back( ta );
    initial_loc.push_back( ta.getInitialLoc() );
    clock_num.push_back( ta.getClockNum() );
    clockUppuerBound.push_back( ta.getClockUppuerBound() );
    differenceCons.push_back( ta.getDifferenceCons() );

    return *this;
  }
  int           getComponentNum() const { return tas.size(); }
  TAS<C, L, T> &operator+=( Channel &ch ) {
    channels.push_back( ch );
    return *this;
  }

  TAS<C, L, T> &operator+=( Counter &c ) {
    counters.push_back( c );
    return *this;
  }

  StateManager<C> getStateManager() const {

    StateManager<C> re( tas.size(), counters.size(), clock_num,
                        clockUppuerBound, differenceCons );

    return re;
  }
  void initState( const StateManager<C> &manager, NIntState *value ) const {
    int component_num=tas.size( );
    for ( int i = 0; i < component_num; i++ ) {
      
      tas[ i ].locationRun( initial_loc[ i ], manager.getClockManager( i ),
                            manager.getkDBM(i, value));
      value->value[ i ] = initial_loc[ i ];
    }
  }
};

typedef StateManager<C_t> StateManager_t;

typedef TAS<C_t, L_t, T_t> TAS_t;

} // namespace graphsat

#endif
