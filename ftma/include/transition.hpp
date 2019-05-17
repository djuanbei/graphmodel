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

namespace graphsat {
using namespace std;
template <typename C, typename CS, typename D, typename DSet, typename A>
class Transition {
public:
private:
  typedef Transition<C, CS, D, DSet, A> Transition_t;
  int source, target; // source location and target location of this
                      // transitionedge. The index of location in tma.locations
  vector<CS> guards;  // set of constraint at this transitionedge

  vector<CounterConstraint>
                  counterCons; // counter constraint like pid ==id or id==0
  vector<Channel> channels;    // the Synchronisation channels

  vector<A>   actions; // set of actions at this transitionedge
  vector<int> reset;   // set of reset clock variables

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

  friend Transition_t &operator+( Transition_t &lhs, Channel &ch ) {
    lhs.channels.push_back( ch );
    return lhs;
  }

  Transition_t &operator+=( Channel &ch ) {
    channels.push_back( ch );
    return *this;
  }

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
   *
   * @param dbmManager
   * @param sourceDBMs  The D matrix of source location. The transitionedge
   * can not change the value of it.
   * @param nextDBMs  Compute the target D matrix after apply this
   * transitionedge on sourceDBMs.
   *
   * @return true if nextDBMs is nonempty, false otherwise.
   */
  bool operator()( const D &dbmManager, const DSet &sourceDBMs,
                   DSet &nextDBMs ) const {

    nextDBMs.deleteAll();

    typename DSet::const_iterator end1 = sourceDBMs.end();

    for ( typename DSet::const_iterator it = sourceDBMs.begin(); it != end1;
          ++it ) {
      C *d1 = dbmManager.newMatrix( *it );
      for ( typename vector<CS>::const_iterator cit = guards.begin();
            cit != guards.end(); cit++ ) {
        dbmManager.andImpl( d1, *cit );
      }
      if ( dbmManager.isConsistent( d1 ) ) {
        for ( vector<int>::const_iterator rit = reset.begin();
              rit != reset.end(); rit++ ) {
          assert( *rit > 0 ); // clock id start from 1
          dbmManager.resetImpl( d1, *rit, 0 );
        }
        nextDBMs.add( dbmManager, d1 );
      } else {
        delete[] d1;
      }
    }
    return nextDBMs.size() > 0;
  }

  C *operator()( const D &dbmManager, const C *const sourceDBM ) const {
    C *re = dbmManager.newMatrix( sourceDBM );
    for ( typename vector<CS>::const_iterator cit = guards.begin();
          cit != guards.end(); cit++ ) {
      dbmManager.andImpl( re, *cit );
    }
    if ( dbmManager.isConsistent( re ) ) {
      for ( vector<int>::const_iterator rit = reset.begin(); rit != reset.end();
            rit++ ) {
        assert( *rit > 0 ); // clock id start from 1
        dbmManager.resetImpl( re, *rit, 0 );
      }
    } else {
      delete[] re;
      re = NULL;
      return re;
    }
    return re;
  }

  // bool operator()( const D &dbmManager, const C *const Din ) const {
  //   C *d1 = dbmManager.newMatrix( Din );
  //   for ( typename vector<CS>::iterator cit = guards.begin(); cit !=
  //   guards.end();
  //         cit++ ) {
  //     dbmManager.andImpl( d1, *cit );
  //   }
  //   if ( dbmManager.isConsistent( d1 ) ) {
  //     for ( vector<int>::iterator rit = reset.begin(); rit != reset.end();
  //           rit++ ) {
  //       assert( *rit > 0 ); // clock id start from 1
  //       dbmManager.resetImpl( d1, *rit, 0 );
  //     }
  //     dbmManager.deleteVectorM( d1);
  //     return true;
  //   } else {
  //     dbmManager.deleteVectorM( d1);
  //     return false;
  //   }
  // }
};
} // namespace graphsat

#endif
