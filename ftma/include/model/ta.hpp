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

#include "action/counteraction.h"
#include "channel.h"
#include "counter.hpp"
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
typedef C_t                  State_t;
typedef StateSet<State_t>    StateSet_t;
typedef ClockConstraint<C_t> CS_t;

typedef Location<C_t, CS_t, DBMManager_t, DBMSet_t>   L_t;
typedef Transition<C_t, CS_t, DBMManager_t, DBMSet_t> T_t;

template <typename C, typename L, typename T> class TAS;
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
    clock_num  = clockNum;
  }

  TA( vector<L> &locs, vector<T> &es, int init, int vnum )
      : locations( locs )
      , transitions( es ) {
    initial_loc = init;
    clock_num  = vnum;
    initial();
  }
  void addOnePara( int defaultValue = 0 ) {
    parameter.addValue( defaultValue );
  }

  const Parameter &getParameter() const { return parameter; }
  bool             hasParam() const { return !parameter.empty(); }
  void             setParam( int p ) { parameter.setValue( 0, p ); }

  friend TAS<C, L, T> operator*( int n, TA<C, L, T> &ta ) {
    TAS<C, L, T> re;
    for ( int i = 0; i < n; i++ ) {
      if ( ta.hasParam() ) {
        ta.setParam( i );
      }
      re += ta;
    }
    return re;
  }

  TAS<C, L, T> operator*( int n ) const {
    assert( n > 0 );
    TAS<C, L, T> re;
    for ( int i = 0; i < n; i++ ) {
      re += ( *this );
    }
    return re;
  }

  void findRhs( const int link, const int lhs, int &rhs ) const {
    graph.findRhs( link, lhs, rhs );
  }
  vector<C> getClockMaxValue() const { return clock_max_value; }

  vector<ClockConstraint<C>> getDifferenceCons() const {
    return differenceCons;
  }

  int getClockNum() const { return clock_num; }

  void setInitialLoc( int loc ) { initial_loc = loc; }
  int  getInitialLoc() const { return initial_loc; }

  bool locationRun( int i, const DBMFactory<C> &manager, C *D ) const {
    return locations[ i ]( manager, D );
  }
  bool isCommit( int id ) const { return locations[ id ].isCommit(); }

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

  vector<C> clock_max_value;

  vector<ClockConstraint<C>> differenceCons;

  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;
  friend class TAS<C, L, T>;
  void updateUpperAndDiff( const CS_t &cs ) {

    if ( cs.x > 0 && cs.y > 0 ) {
      differenceCons.push_back( cs );
    }
    C realRhs = getRight( cs.matrix_value );
    if ( cs.x > 0 ) {
      if ( realRhs > clock_max_value[ cs.x ] ) {
        clock_max_value[ cs.x ] = realRhs;
      }
    }

    if ( cs.y > 0 ) {
      if ( -realRhs > clock_max_value[ cs.y ] ) {
        clock_max_value[ cs.y ] = -realRhs;
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
    clock_max_value.resize( clock_num + 1 );

    fill( clock_max_value.begin(), clock_max_value.end(), 0 );

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
  }
};

typedef TA<C_t, L_t, T_t> TA_t;

template <typename C, typename L, typename T> class TAS {

public:
  TAS() {
    clock_max_value.push_back( 0 );
    clock_num = 0;
  }
  TAS<C, L, T> &operator+=( const TA_t &ta ) {

    TA_t ta1 = transfrom( ta );
    tas.push_back( ta1 );
    initial_loc.push_back( ta1.getInitialLoc() );
    vec_clock_nums.push_back( ta1.getClockNum() );

    for ( size_t i = 1; i < ta1.getClockMaxValue().size(); i++ ) {
      clock_max_value.push_back( ta1.getClockMaxValue()[ i ] );
    }

    differenceCons.insert( differenceCons.end(),
                           ta1.getDifferenceCons().begin(),
                           ta1.getDifferenceCons().end() );

    parameters.push_back( ta1.getParameter() );

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

    bool      hasChannel = !channels.empty();
    vector<C> temp_clock_upperbound( 2 * clock_num + 2, 0 );

    for ( int i = 1; i < clock_num + 1; i++ ) {
      temp_clock_upperbound[ i + clock_num + 1 ] =
          getMatrixValue( -clock_max_value[ i ], true );
    }

    for ( int i = 1; i <= clock_num + 1; i++ ) {
      temp_clock_upperbound[ i ] =
          getMatrixValue( clock_max_value[ i ], false );
    }

    StateManager<C> re( tas.size(), counters.size(), clock_num,
                        temp_clock_upperbound, differenceCons, parameters,
                        hasChannel );

    return re;
  }
  void initState( const StateManager<C> &manager, State_t *state ) const {
    int component_num = tas.size();
    bool withoutCommit=true;
    for ( int component = 0; component < component_num; component++ ) {
      state[ component ] = initial_loc[ component ];
      if ( tas[ component ].isCommit( state[ component ] ) ) {
        manager.setCommitState( component, state[ component ], state );
        withoutCommit=false;
      }
    }
    if( withoutCommit){
      for ( int component = 0; component < component_num; component++ ) {
        tas[ component ].locationRun( initial_loc[component ],
                                      manager.getClockManager(),
                                      manager.getDBM( state ) );
      }
    }

    for ( int component = 0; component < component_num; component++ ) {
      tas[ component ]
          .locations[ manager.getLoc( component, state ) ]
          .employInvariants( manager.getClockManager(),
                             manager.getDBM( state ) );
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

  int clock_num;

  vector<int> initial_loc;
  vector<int> vec_clock_nums;

  vector<C>                  clock_max_value;
  vector<ClockConstraint<C>> differenceCons;
  vector<Parameter>          parameters;

  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;
  TA_t transfrom( const TA_t &ta ) {
    TA_t ta1 = ta;
    if ( clock_num > 0 ) {
      for ( size_t i = 0; i < ta1.locations.size(); i++ ) {
        ta1.locations[ i ].clockShift( clock_num );
      }

      for ( size_t i = 0; i < ta1.transitions.size(); i++ ) {
        ta1.transitions[ i ].clockShift( clock_num );
      }
      for ( size_t i = 0; i < ta1.differenceCons.size(); i++ ) {
        ta1.differenceCons[ i ].clockShift( clock_num );
      }
    }
    clock_num += ta1.getClockNum();
    return ta1;
  }
};

typedef StateManager<C_t> StateManager_t;

typedef TAS<C_t, L_t, T_t> TAS_t;

} // namespace graphsat

#endif
