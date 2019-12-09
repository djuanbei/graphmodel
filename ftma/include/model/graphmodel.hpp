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

template <typename L, typename T> class AgentSystem : public VarDecl {

public:
  typedef typename T::State_t State_t;

  typedef typename T::StateManager_t StateManager_t;
  typedef StateSet<State_t>          StateSet_t;
  typedef ClockConstraint            CS_t;

  typedef L L_t;

  typedef T T_t;

  typedef Agent<L, T>         Agent_t;
  typedef AgentTemplate<L, T> AgentTemplate_t;

  AgentSystem() {
    clock_max_value.push_back( 0 );
    clock_num = 0;
  }

  shared_ptr<AgentTemplate_t> createTemplate() {
    shared_ptr<AgentTemplate_t> re( new AgentTemplate_t( this ) );
    re->id = templates.size();
    templates.push_back( re );
    return re;
  }

  AgentSystem &operator+=( Agent_t &agent ) {

    transfrom( agent );
    agents.push_back( agent );
    initial_loc.push_back( agent.getInitialLoc() );
    vec_clock_nums.push_back( agent.getClockNum() );

    for ( size_t i = 1; i < agent.getClockMaxValue().size(); i++ ) {
      clock_max_value.push_back( agent.getClockMaxValue()[ i ] );
    }

    difference_cons.insert( difference_cons.end(),
                            agent.difference_cons.begin(),
                            agent.difference_cons.end() );
    update();
    return *this;
  }
  void setCounterNum( int n ) {
    assert( agents.empty() );
    counters.resize( n );
  }
  void setCounter( int id, Counter c ) {
    assert( agents.empty() );
    counters[ id ] = c;
  }
  void setChannelNum( int n ) {
    assert( agents.empty() );
    channels.resize( n );
  }
  void setChannel( int id, Channel ch ) {
    assert( agents.empty() );
    channels[ id ] = ch;
  }

  int getComponentNum() const { return (int) agents.size(); }

  shared_ptr<StateManager_t> getStateManager() const { return stateManager; }

  void update() {
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
    for ( size_t i = 0; i < agents.size(); i++ ) {
      node_n.push_back( agents[ i ].ta_tempate->graph.getVertex_num() );
    }
    vector<int> link_num;
    for ( auto e : agents ) {
      link_num.push_back( e.ta_tempate->graph.getLink_num() );
    }

    stateManager.reset( new StateManager_t(
        (int) agents.size(), counters, clock_num, temp_clock_upperbound,
        difference_cons, node_n, link_num, (int) channels.size() ) );

    // return re;
  }

  template <typename D> void addInitState( D &data ) const {
    State_t *state         = stateManager->newState();
    int      component_num = (int) agents.size();
    bool     withoutCommit = true;
    for ( int component = 0; component < component_num; component++ ) {
      state[ component ] = initial_loc[ component ];
      if ( agents[ component ].isCommit( state[ component ] ) ) {
        stateManager->setCommitState( component, state );
        withoutCommit = false;
      }
    }
    if ( withoutCommit ) {
      for ( int component = 0; component < component_num; component++ ) {
        agents[ component ].locationRun( initial_loc[ component ],
                                         stateManager->getClockManager(),
                                         stateManager->getDBM( state ) );
      }
    }

    for ( int component = 0; component < component_num; component++ ) {
      agents[ component ]
          .locations[ stateManager->getLoc( component, state ) ]
          .employInvariants( stateManager->getClockManager(),
                             stateManager->getDBM( state ) );
    }
    if ( stateManager->getClockManager().isConsistent(
             stateManager->getDBM( state ) ) ) {
      stateManager->norm( stateManager->getDBM( state ) );
      data.add( state );
    }
    stateManager->destroyState( state );
  }

  int getCounterStartLoc( const int id ) const {
    int re = stateManager->getCounterStart();
    for ( auto &agent : agents ) {
      if ( agent.getTemplate()->id < id ) {
        re += agent.getTemplate()->getCounterNumber();
      }
    }
    return re;
  }

private:
  /**
   * multi-components
   *
   */
  vector<shared_ptr<AgentTemplate_t>> templates;

  vector<Agent_t> agents;
  vector<Channel> channels;
  vector<Counter> counters;

  int clock_num;

  vector<int> initial_loc;
  vector<int> vec_clock_nums;

  vector<int>             clock_max_value;
  vector<ClockConstraint> difference_cons;

  shared_ptr<StateManager_t> stateManager;

  template <typename R1> friend class Reachability;

  void transfrom( Agent_t &agent ) {

    if ( clock_num > 0 ) {

      for ( size_t i = 0; i < agent.locations.size(); i++ ) {
        agent.locations[ i ].clockShift( clock_num );
      }
      for ( size_t i = 0; i < agent.transitions.size(); i++ ) {
        agent.transitions[ i ].clockShift( clock_num );
      }
      for ( size_t i = 0; i < agent.ta_tempate->template_difference_cons.size();
            i++ ) {
        agent.difference_cons[ i ].clockShift( clock_num );
      }
    }
    for ( size_t i = 0; i < agent.transitions.size(); i++ ) {
      if ( agent.transitions[ i ].hasChannel() ) {
        agent.transitions[ i ].setChanType(
            channels[ agent.transitions[ i ].getChannel()->gloabl_id ].type );
      }
    }

    clock_num += agent.getClockNum();
  }
};

} // namespace graphsat

#endif
