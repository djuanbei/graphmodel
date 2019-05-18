/**
 * @file   transitionedge.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:45:30 2019
 *
 * @brief  transitionedge class in timed automata
 *
 *
 */
#ifndef EDGE_HPP
#define EDGE_HPP

#include <vector>

#include "channel.h"
#include "constraint/countercons.h"
#include "state.hpp"

namespace graphsat {
using namespace std;
template <typename C, typename CS, typename D, typename DSet, typename A>
class Transition {

  typedef Transition<C, CS, D, DSet, A> Transition_t;

public:
  Transition() { source = target = -1; }
  Transition( int s, int t ) {
    source = s;
    target = t;
  }

  void setSource( int s ) { source = s; }

  int getSource() const { return source; }

  void setTarget( int t ) { target = t; }

  int getTarget() const { return target; }

  const vector<CS> &getGuards() const { return guards; }

  /**
   * add constraint to Transition
   *
   * @param lhs
   * @param cs
   *
   * @return
   */

  friend Transition_t &operator+( Transition_t &lhs, CS &cs ) {
    lhs.guards.push_back( cs );
    return lhs;
  }

  /**
   *  add one constraint to this transition
   *
   * @param cs  constraint
   *
   * @return
   */
  Transition_t &operator+=( CS &cs ) {
    guards.push_back( cs );
    return *this;
  }

  void setChannel( const Channel &ch ) { channel = ch; }

  const Channel &getChannel() const { return channel; }

  /**
   *
   *  add one action to this transition
   *
   * @param a
   *
   * @return
   */
  Transition_t &operator+=( A &a ) {
    actions.push_back( a );
    return *this;
  }

  /**
   * add one clock reset  to this transition
   *
   * @param r
   *
   * @return
   */
  Transition_t &operator+=( int r ) {
    reset.push_back( r );
    return *this;
  }

  Transition_t &operator+=( CounterConstraint &guards ) {
    counterCons.push_back( guards );
  }
  /** 
   * @brief Except synchronize signal, other state satisfies jump conditions
   * 
   * @param comp 
   * @param manager 
   * @param state 
   * 
   * @return 
   */
  bool isOK( const int comp, const StateManager<C> &manager,
             const  NIntState *state ) const {
    const  D& dbmManager=manager.getClockManager( comp);
    
    const C*sourceDBM=manager.getkDBM( comp, state);
    
    C *copyDBM = dbmManager.newMatrix( sourceDBM );
    for ( auto cs : guards ) {
      dbmManager.andImpl( copyDBM, cs );
    }
    if ( !dbmManager.isConsistent( copyDBM ) ) {
      dbmManager.deleteD( copyDBM);
      return false;
    }
    
    dbmManager.deleteD( copyDBM);
    
    const   C *counterValue=manager.getCounterValue( state);
    
    for( auto cs: counterCons){
      if(!cs( counterValue) ){
        return false;
      }
    }
    
    return true;
  }

  NIntState *operator()(const int comp, const StateManager<C> &manager,
             NIntState *state ) const {
    assert(isOK( comp, manager, state)) ;
    
    NIntState* re=state->copy( );

    const  D& dbmManager=manager.getClockManager( comp);
        
    C* sourceDBM=manager.getkDBM( comp, re);

    for ( auto r : reset ) {
      assert( r > 0 ); // clock id start from 1
      dbmManager.resetImpl( sourceDBM, r, 0 );
    }
    
    C *counterValue=manager.getCounterValue( state);

    for(  auto act: actions){
      act(counterValue );
    }
    
    return re;
  }

private:
  int source, target; // source location and target location of this
  // transitionedge. The index of location in tma.locations
  vector<CS> guards; // set of constraint at this transitionedge

  vector<CounterConstraint>
          counterCons; // counter constraint like pid ==id or id==0
  Channel channel;     // Only one synchronisation channels

  vector<A>   actions; // set of actions at this transitionedge
  vector<int> reset;   // set of reset clock variables
};
} // namespace graphsat

#endif
