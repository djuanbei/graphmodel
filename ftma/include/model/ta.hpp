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
#include "domain/dbm.h"
#include "domain/dbmset.hpp"
#include "graph/graph.hpp"
#include "location.h"
#include "parameter.h"
#include "state/discretestate.hpp"
#include "state/statemanager.hpp"
#include "transition.hpp"

namespace graphsat {

using std::vector;

using namespace raptor;

template <typename C, typename L, typename T> class TAS;

/**
 *
 *  TA has one-component
 *
 */

template <typename C, typename L, typename T> class TA;

template <typename C, typename L, typename T> class TAT {

private:
  typedef C C_t;

  typedef ClockConstraint CS_t;

  typedef TAT<C, L, T> TAT_t;
  typedef TA<C, L, T>  TA_t;

public:
  TAT() { initial_loc = clock_num = -1; }
  TAT( int init, int clockNum ) {
    initial_loc = init;
    clock_num   = clockNum;
  }

  TAT( vector<L> &locs, vector<T> &es, int init, int vnum )
      : template_locations( locs )
      , template_transitions( es ) {
    initial_loc = init;
    clock_num   = vnum;
    initial();
  }

  void findRhs( const int link, const int lhs, int &rhs ) const {
    graph.findRhs( link, lhs, rhs );
  }
  vector<C> getClockMaxValue() const { return clock_max_value; }

  int getClockNum() const { return clock_num; }

  void setInitialLoc( int loc ) { initial_loc = loc; }

  int getInitialLoc() const { return initial_loc; }



private:
  vector<L> template_locations;
  vector<T> template_transitions;
  int       initial_loc;

  int clock_num;

  Graph_t<int> graph;

  vector<C> clock_max_value;

  vector<ClockConstraint> template_difference_cons;

  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;

  friend class TA<C, L, T>;

  friend class TAS<C, L, T>;

  void updateUpperAndDiff( const CS_t &cs ) {

    if ( cs.clock_x > 0 && cs.clock_y > 0 ) {
      template_difference_cons.push_back( cs );
    }
    C realRhs = getRight( cs.matrix_value );
    if ( cs.clock_x > 0 ) {
      if ( realRhs > clock_max_value[ cs.clock_x ] ) {
        clock_max_value[ cs.clock_x ] = realRhs;
      }
    }

    if ( cs.clock_y > 0 ) {
      if ( -realRhs > clock_max_value[ cs.clock_y ] ) {
        clock_max_value[ cs.clock_y ] = -realRhs;
      }
    }
  }

  void initial() {

    vector<int> srcs;
    vector<int> snks;

    for ( auto t : template_transitions ) {
      srcs.push_back( t.getSource() );
      snks.push_back( t.getTarget() );
    }

    graph.initial( srcs, snks );

    int vertex_num = graph.getVertex_num();

    // // There are no edges connect with  initial location
    assert( initial_loc >= 0 && initial_loc < vertex_num );

    template_difference_cons.clear();
    clock_max_value.resize( clock_num + 1 );

    fill( clock_max_value.begin(), clock_max_value.end(), 0 );

    for ( auto l : template_locations ) {
      const vector<CS_t> &invariants = l.getInvarients();
      for ( auto cs : invariants ) {
        updateUpperAndDiff( cs );
      }
    }

    for ( auto t : template_transitions ) {
      const vector<CS_t> &gurads = t.getGuards();
      for ( auto cs : gurads ) {
        updateUpperAndDiff( cs );
      }
    }
  }
};

template <typename C, typename L, typename T> class TA {

private:
  typedef C C_t;

  typedef ClockConstraint CS_t;

  typedef TA<C, L, T>  TA_t;
  typedef TAT<C, L, T> TAT_t;

public:
  TA( const TAT_t *tat, const Parameter &param ) {

    ta_tempate = tat;
    for ( auto e : tat->template_transitions ) {
      transitions.push_back( T( e, param ) );
    }

    locations       = tat->template_locations;
    difference_cons = tat->template_difference_cons;
  }

  void findRhs( const int link, const int lhs, int &rhs ) const {
    ta_tempate->graph.findRhs( link, lhs, rhs );
  }

  vector<C> getClockMaxValue() const { return ta_tempate->clock_max_value; }

  int getClockNum() const { return ta_tempate->clock_num; }

  int getInitialLoc() const { return ta_tempate->initial_loc; }

  bool transitionRun( int link, const DBMFactory &manager, C *D ) const {
    return transitions[ link ]( manager, D );
  }

  bool locationRun( int link, const DBMFactory &manager, C *D ) const {
    if ( !locations[ link ].isReachable( manager, D ) ) {
      return false;
    }
    locations[ link ]( manager, D );
    return true;
  }

  bool isCommit( int id ) const { return locations[ id ].isCommit(); }

  void updateCounterId( const map<int, int> &id_map ) {
    for ( auto e : transitions ) {
      for ( auto ee : e.counter_cons ) {
        ee->counterIpMap( id_map );
      }
      for ( auto aa : e.actions ) {
        aa->counterIpMap( id_map );
      }
    }
  }
  string getLocName( int node_id ) const {
    return locations[ node_id ].getName();
  }
  // void toDot(ostream &out ) const{
  //   out<<"digraph G { "<<endl;

  //   for(auto e : transitions ){
  //     string source=locations[ e.source].
  //   }

  // }

private:
  const TAT_t *ta_tempate;

  vector<L>                  locations;
  vector<T>                  transitions;
  vector<ClockConstraint> difference_cons;

  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;
  friend class TAS<C, L, T>;
};

template <typename C, typename L, typename T> class TAS {

public:
  typedef C                    C_t;
  typedef C_t *                DBM_t;
  typedef DBMFactory      DBMManager_t;
  typedef DBMset<C_t>          DBMSet_t;
  typedef C_t                  State_t;
  typedef StateSet<State_t>    StateSet_t;
  typedef ClockConstraint CS_t;

  typedef L L_t;

  typedef T T_t;

  typedef TA<C, L, T>  TA_t;
  typedef TAT<C, L, T> TAT_t;

  typedef StateManager<C_t> StateManager_t;

  typedef TAS<C, L, T> TAS_t;

  TAS() {
    clock_max_value.push_back( 0 );
    clock_num = 0;
  }
  TAS_t &operator+=( TA_t &ta ) {

    transfrom( ta );
    tas.push_back( ta );
    initial_loc.push_back( ta.getInitialLoc() );
    vec_clock_nums.push_back( ta.getClockNum() );

    for ( size_t i = 1; i < ta.getClockMaxValue().size(); i++ ) {
      clock_max_value.push_back( ta.getClockMaxValue()[ i ] );
    }

    difference_cons.insert( difference_cons.end(), ta.difference_cons.begin(),
                            ta.difference_cons.end() );

    return *this;
  }
  void setCounterNum( int n ) { counters.resize( n ); }
  void setCounter( int id, Counter c ) { counters[ id ] = c; }
  void setChannelNum( int n ) { channels.resize( n ); }
  void setChannel( int id, Channel ch ) { channels[ id ] = ch; }

  int             getComponentNum() const { return (int) tas.size(); }
  StateManager_t getStateManager() const {

    vector<C> temp_clock_upperbound( 2 * clock_num + 2, 0 );

    for ( int i = 0; i < clock_num + 1; i++ ) {
      temp_clock_upperbound[ i ] =
          getMatrixValue( clock_max_value[ i ], false );
    }

    for ( int i = 0; i < clock_num + 1; i++ ) {
      temp_clock_upperbound[ i + clock_num + 1 ] =
          getMatrixValue( -clock_max_value[ i ], true );
    }
    vector<int> node_n;
    for ( size_t i = 0; i < tas.size(); i++ ) {
      node_n.push_back( tas[ i ].ta_tempate->graph.getVertex_num() );
    }
    vector<int> link_num;
    for ( auto e : tas ) {
      link_num.push_back( e.ta_tempate->graph.getLink_num() );
    }

    StateManager_t re( (int) tas.size(), counters, clock_num,
                        temp_clock_upperbound, difference_cons, node_n,
                        link_num, (int) channels.size() );

    return re;
  }
  void initState( const StateManager_t &manager, State_t *state ) const {
    int  component_num = (int) tas.size();
    bool withoutCommit = true;
    for ( int component = 0; component < component_num; component++ ) {
      state[ component ] = initial_loc[ component ];
      if ( tas[ component ].isCommit( state[ component ] ) ) {
        manager.setCommitState( component, state );
        withoutCommit = false;
      }
    }
    if ( withoutCommit ) {
      for ( int component = 0; component < component_num; component++ ) {
        tas[ component ].locationRun( initial_loc[ component ],
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
  vector<ClockConstraint> difference_cons;

  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;
  void transfrom( TA_t &ta ) {

    if ( clock_num > 0 ) {

      for ( size_t i = 0; i < ta.locations.size(); i++ ) {
        ta.locations[ i ].clockShift( clock_num );
      }
      for ( size_t i = 0; i < ta.transitions.size(); i++ ) {
        ta.transitions[ i ].clockShift( clock_num );
      }
      for ( size_t i = 0; i < ta.ta_tempate->template_difference_cons.size();
            i++ ) {
        ta.difference_cons[ i ].clockShift( clock_num );
      }
    }
    for ( size_t i = 0; i < ta.transitions.size(); i++ ) {
      if ( ta.transitions[ i ].hasChannel() ) {
        ta.transitions[ i ].setChanType(
            channels[ ta.transitions[ i ].getChannel().gloabl_id ].type );
      }
    }

    clock_num += ta.getClockNum();
  }
};

typedef int C_t1;

typedef DBMFactory DBMManager_t1;
typedef DBMset<C_t1>     DBMSet_t1;

typedef ClockConstraint CS_t1;



typedef Transition< CS_t1, DBMManager_t1> T_t1;

typedef TAS<C_t1, Location, T_t1> INT_TAS_t;

} // namespace graphsat

#endif
