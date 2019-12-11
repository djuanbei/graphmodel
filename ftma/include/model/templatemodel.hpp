/**
 * @file   templatemodel.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec  6 19:58:40 2019
 *
 * @brief  template model
 *
 *
 */

#ifndef TEMPLATE_MODEL_H
#define TEMPLATE_MODEL_H

#include "graph/graph.hpp"
#include "graphmodel.hpp"
#include "vardecl.h"

namespace graphsat {
using namespace raptor;

/**
 *
 *  TA has one-component
 *
 */

template <typename L, typename T> class Agent;

template <typename L, typename T> class AgentSystem;

template <typename L, typename T> class AgentTemplate : public VarDecl {

private:
  typedef ClockConstraint CS_t;

  typedef Agent<L, T> Agent_t;

public:
  void initial( vector<L> &locs, vector<T> &es, int init ) {
    template_locations   = locs;
    template_transitions = es;
    initial_loc          = init;
    initial();
  }

  void findRhs( const int link, const int lhs, int &rhs ) const {
    graph.findRhs( link, lhs, rhs );
  }

  vector<int> getClockMaxValue() const { return clock_max_value; }

  int getClockNum() const { return data.getTypeNum( CLOCK_T ); }

  void setInitialLoc( int loc ) { initial_loc = loc; }

  int getInitialLoc() const { return initial_loc; }

  int addPara( const string &p ) {
    int re = parameters.size();
    parameters.push_back( p );
    return re;
  }
  Parameter getParameter() const {
    Parameter re( parameters.size() );
    return re;
  }

  int getParaId( const string &p ) const {
    for ( size_t i = 0; i < parameters.size(); i++ ) {
      if ( p == parameters[ i ] ) {
        return i;
      }
    }
    return NOT_FOUND;
  }
  void reset() { agents.clear(); }

private:
  AgentTemplate( const AgentSystem<L, T> *s )
      : sys( s ) {
    initial_loc = -1;
  }
  AgentTemplate( const AgentTemplate &other )
      : sys( NULL ) {
    assert( false );
  }

  AgentTemplate &operator=( const AgentTemplate &other ) {
    assert( false );
    return *this;
  }

  void updateUpperAndDiff( const CS_t &cs ) {

    if ( cs.clock_x > 0 && cs.clock_y > 0 ) {
      template_difference_cons.push_back( cs );
    }
    int realRhs = getRight( cs.matrix_value );
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

  int getStart(const TYPE_T type ) const{
    return sys->getStartLoc( type, id);
  }

  //int getCounterStartLoc() const { return sys->getCounterStartLoc( id ); }

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
    clock_max_value.resize( getClockNumber() + 1 ); // clock is start with 1

    fill( clock_max_value.begin(), clock_max_value.end(), 0 );

    for ( auto loc : template_locations ) {
      const vector<CS_t> &invariants = loc.getInvarients();
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
  const AgentSystem<L, T> *sys;
  int                      id;
  int                      number_children;

  vector<L> template_locations;
  vector<T> template_transitions;
  int       initial_loc;

  Graph_t<int> graph;

  vector<int> clock_max_value;

  vector<ClockConstraint> template_difference_cons;

  vector<string>        parameters;
  vector<Agent<L, T> *> agents;

  template <typename R1> friend class Reachability;

  friend class Agent<L, T>;
  template <typename L1, typename T1> friend class AgentSystem;
};
} // namespace graphsat

#endif
