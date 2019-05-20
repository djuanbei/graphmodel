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
#include <vector>

#include "channel.h"
#include "counteraction.h"
#include "discretestate.hpp"
#include "domain/dbm.hpp"
#include "domain/dbmset.hpp"
#include "graph/graph.hpp"
#include "location.hpp"
#include "parameter.h"
#include "statemanager.hpp"
#include "transition.hpp"

namespace graphsat {

using std::vector;

using namespace raptor;

typedef int                  C_t;
typedef C_t *                DBM_t;
typedef DBMFactory<C_t>      DBMManager_t;
typedef DBMset<C_t>          DBMSet_t;
typedef NIntState            State_t;
typedef StateSet<NIntState>  StateSet_t;
typedef ClockConstraint<C_t> CS_t;

typedef Location<C_t, CS_t, DBMManager_t, DBMSet_t>   L_t;
typedef Transition<C_t, CS_t, DBMManager_t, DBMSet_t> T_t;



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

  bool locationRun( int i, const DBMFactory<C> &manager, C *D ) const {
    return locations[ i ]( manager, D );
  }
  bool isCommit( int id) const{
    return locations[ id].isCommit( );
  }

  bool transitionRun( int link, const DBMFactory<C> &manager, C *D ) const {
    return transitions[ link ]( manager, D );
  }

private:
  Parameter parameter;
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
      if( tas[i].isCommit( value->value[ i ] )){
        manager.setCommitState( i, value->value[ i ],  value );
      }
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
