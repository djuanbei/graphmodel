/**
 * @file   agentmodel.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec  6 20:00:07 2019
 * 
 * @brief  agent model
 * 
 * 
 */
#ifndef AGENT_MODEL_H
#define AGENT_MODEL_H
#include "templatemodel.hpp"


namespace graphsat{

template <typename M, typename L, typename T> class AgentSystem;

template <typename L, typename T> class Agent {

private:
  typedef Agent<L, T>         TA_t;
  typedef AgentTemplate<L, T> TAT_t;

public:
  Agent( const TAT_t *tat, const Parameter &param ) {

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

  vector<int> getClockMaxValue() const { return ta_tempate->clock_max_value; }

  int getClockNum() const { return ta_tempate->getClockNum( ); }

  int getInitialLoc() const { return ta_tempate->initial_loc; }

  bool transitionRun( int link, const DBMFactory &manager, int *D ) const {
    return transitions[ link ]( manager, D );
  }

  bool locationRun( int link, const DBMFactory &manager, int *D ) const {
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

  vector<L>               locations;
  vector<T>               transitions;
  vector<ClockConstraint> difference_cons;

  template <typename R1> friend class Reachability;

  template <typename M1, typename L1, typename T1> friend class AgentSystem;
};

}
#endif 
