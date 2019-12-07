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
#include "counter.h"



#include "domain/dbmset.hpp"


#include "parameter.h"
#include "state/discretestate.hpp"


#include "agentmodel.hpp"

#include "templatemodel.hpp"

#include "vardecl.h"


namespace graphsat {

using std::vector;



template <typename M, typename L, typename T> class AgentSystem:public VarDecl {

public:
  typedef typename T::State_t State_t;

  typedef typename T::StateManager_t StateManager_t;
  typedef StateSet<State_t>          StateSet_t;
  typedef ClockConstraint            CS_t;

  typedef L L_t;

  typedef T T_t;

  typedef Agent<L, T>         Agent_t;
  typedef AgentTemplate<L, T> AgentTemplate_t;

  typedef AgentSystem<M, L, T> AgentSystem_t;

  AgentSystem() {
    clock_max_value.push_back( 0 );
    clock_num = 0;
  }
  AgentSystem_t &operator+=( Agent_t &ta ) {

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

  int getComponentNum() const { return (int) tas.size(); }

  M getStateManager() const {

    vector<int> temp_clock_upperbound( 2 * clock_num + 2, 0 );

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

    M re( (int) tas.size(), counters, clock_num, temp_clock_upperbound,
          difference_cons, node_n, link_num, (int) channels.size() );

    return re;
  }

  template <typename D> void addInitState( D &data, const M &manager ) const {
    State_t *state         = manager.newState();
    int      component_num = (int) tas.size();
    bool     withoutCommit = true;
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
    if ( manager.getClockManager().isConsistent( manager.getDBM( state ) ) ) {
      manager.norm( manager.getDBM( state ) );
      data.add( state );
    }
    manager.destroyState( state );
  }

private:
  /**
   * multi-components
   *
   */
  vector<AgentTemplate_t> templates;
  
  vector<Agent_t> tas;
  vector<Channel> channels;
  vector<Counter> counters;





  int clock_num;

  vector<int> initial_loc;
  vector<int> vec_clock_nums;

  vector<int>             clock_max_value;
  vector<ClockConstraint> difference_cons;

  template <typename R1> friend class Reachability;

  void transfrom( Agent_t &ta ) {

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
            channels[ ta.transitions[ i ].getChannel()-> gloabl_id ].type );
      }
    }

    clock_num += ta.getClockNum();
  }
};

} // namespace graphsat

#endif
