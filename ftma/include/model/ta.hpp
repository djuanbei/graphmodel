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
  vector<C> getClockUppuerBound() const { return clockUpperBound; }

  vector<ClockConstraint<C>> getDifferenceCons() const {
    return differenceCons;
  }

  int getLocationNum() const { return (int) locations.size(); }
  int getTransitionNum() const { return (int) transitions.size(); }

  int getClockNum() const { return clock_num; }

  void setInitialLoc( int loc ) { initial_loc = loc; }
  int  getInitialLoc() const { return initial_loc; }

  bool locationRun( int i, const DBM<C> &manager, C *D ) const {
    return locations[ i ]( manager, D );
  }

  bool transitionRun( int link, const DBM<C> &manager, C *D ) const {
    return transitions[ link ]( manager, D );
  }

private:
  vector<L> locations;
  vector<T> transitions;
  int       initial_loc;

  int clock_num;

  graph_t<int> graph;

  vector<C> clockUpperBound;

  vector<ClockConstraint<C>> differenceCons;

  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;

  void updateUpperAndDiff( const CS_t &cs ) {

    if ( cs.x > 0 && cs.y > 0 ) {
      differenceCons.push_back( cs );
    }
    C realRhs = getRight( cs.matrix_value );
    if ( cs.x > 0 ) {
      if ( realRhs > clockUpperBound[ cs.x ] ) {
        clockUpperBound[ cs.x ] = realRhs;
      }
    }

    if ( cs.y > 0 ) {
      if ( -realRhs > clockUpperBound[ cs.y ] ) {
        clockUpperBound[ cs.y ] = -realRhs;
      }
    }
  }

  void initial() {

    vector<int> srcs;
    vector<int> snks;

    for ( auto t : transitions ) {
      srcs.push_back( t.getSource() );
      snks.push_back( t.getTarget() );
    }

    graph.initial( srcs, snks );

    int vertex_num = graph.getVertex_num();

    // // There are no edges connect with  initial location
    assert( initial_loc >= 0 && initial_loc < vertex_num );

    differenceCons.clear();
    clockUpperBound.resize( 2 * ( clock_num + 1 ) );
    fill( clockUpperBound.begin(), clockUpperBound.end(), 0 );

    for ( auto l : locations ) {

      const vector<CS_t> &invariants = l.getInvarients();
      for ( auto cs : invariants ) {
        updateUpperAndDiff( cs );
      }
    }
    for ( auto t : transitions ) {

      const vector<CS_t> &gurads = t.getGuards();
      for ( auto cs : gurads ) {
        updateUpperAndDiff( cs );
      }
    }

    /**
     *      x-y<= k_i
     *      x-y< -k_{i+n+1}
     *
     */

    for ( int i = 1; i < clock_num + 1; i++ ) {
      clockUpperBound[ i + clock_num + 1 ] =
          getMatrixValue( -clockUpperBound[ i ], true );
    }

    for ( int i = 1; i <= clock_num + 1; i++ ) {
      clockUpperBound[ i ] = getMatrixValue( clockUpperBound[ i ], false );
    }
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
                vector<vector<C>>                  clockUpperBound,
                vector<vector<ClockConstraint<C>>> differenceCons ) {

    component_num = comp_num;

    counter_start_loc = 2 * component_num;

    stateLen = 2 * component_num + counter_num;
    ;

    for ( size_t i = 0; i < clock_num.size(); i++ ) {

      clock_start_loc.push_back( stateLen );

      stateLen += ( clock_num[ i ] + 1 ) * ( clock_num[ i ] + 1 );

      DBM<C> temp =
          DBM<C>( clock_num[ i ], clockUpperBound[ i ], differenceCons[ i ] );

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
  inline  C * getkDBM( const int k,const State_t * const state ) const {
    return state->value + getClockStart( k );
  }
  inline void andImpl( const int id, const CS_t &cs, State_t *state ) const {
    return getClockManager( id ).andImpl( getkDBM( id, state ), cs );
  }
  inline bool isConsistent( const int id, State_t *state ) const {
    return getClockManager( id ).isConsistent( getkDBM( id, state ) );
  }

  inline int blockComponent( const int            chid,
                             const State_t *const state ) const {
    for ( int i = 0; i < component_num; i++ ) {
      /**
       * return first componment which waits for this signal chid
       *
       */
      if ( state->value[ i + component_num ] == chid ) {
        return i;
      }
    }
    return -1;
  }

  State_t *add( const int id, const int target, StateSet_t &stateSet, DBM_t d,
                const State_t *state ) const {
    State_t *re  = state->copy();
    int      len = 0;
    if ( id + 1 < (int) clock_start_loc.size() ) {
      len = clock_start_loc[ id + 1 ] - clock_start_loc[ id ];
    } else {
      len = stateLen - clock_start_loc[ id ];
    }
    memcpy( re->value + clock_start_loc[ id ], d, sizeof( C ) * len );

    re->value[ id ] = target;

    if ( !stateSet.add( re ) ) {
      delete re;
      return NULL;
    }
    return re;
  }
};

template <typename C, typename L, typename T> class TAS {

public:
  TAS<C, L, T> &operator+=( TA_t &ta ) {
    tas.push_back( ta );
    initial_loc.push_back( ta.getInitialLoc() );
    clock_num.push_back( ta.getClockNum() );
    clockUpperBound.push_back( ta.getClockUppuerBound() );
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

    StateManager<C> re( tas.size(), counters.size(), clock_num, clockUpperBound,
                        differenceCons );

    return re;
  }
  void initState( const StateManager<C> &manager, NIntState *value ) const {
    int component_num = tas.size();
    for ( int i = 0; i < component_num; i++ ) {

      tas[ i ].locationRun( initial_loc[ i ], manager.getClockManager( i ),
                            manager.getkDBM( i, value ) );
      value->value[ i ] = initial_loc[ i ];
    }
  }

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

  vector<vector<C>>                  clockUpperBound;
  vector<vector<ClockConstraint<C>>> differenceCons;

  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;
};

typedef StateManager<C_t> StateManager_t;

typedef TAS<C_t, L_t, T_t> TAS_t;

} // namespace graphsat

#endif
