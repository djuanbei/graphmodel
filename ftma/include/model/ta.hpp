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
#include "domain/dbm.hpp"
#include "domain/dbmset.hpp"
#include "graph/graph.hpp"
#include "channel.h"

#include <vector>

namespace graphsat {

using namespace std;
using namespace raptor;


template <typename C, typename L, typename T> class TA {

public:

  typedef C* D_t;
  typedef ClockConstraint<C>         CS_t;
  typedef DBM<C>          DManager_t;
  typedef DBMset<C, DManager_t> DSet_t;

  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;

private:
  vector<L> locations;
  vector<T> transitions;
  int       initial_loc;
  int       clock_num;
  int counter_num;
  
  graph_t<int> graph;

  vector<C> clockUppuerBound;
   
  vector<ClockConstraint<C>> differenceCons;

  vector<Channel> channels;
  

public:
  TA() { initial_loc = clock_num =counter_num= -1; }
  TA( int init, int vnum, int cnum =0) {
    initial_loc = init;
    clock_num   = vnum;
    counter_num=cnum;
  }
  TA( vector<L> &locs, vector<T> &es, int init, int vnum, int cnum=0 )
      : locations( locs )
      , transitions( es ) {
    initial_loc = init;
    clock_num   = vnum;
    counter_num=cnum;
  }
  void findRhs( const int link, const int lhs, int &rhs ) const {
    graph.findRhs( link, lhs, rhs );
  }
  vector<C> getClockUppuerBound() const { return clockUppuerBound; }

  vector<ClockConstraint<C>> getDifferenceCons() const { return differenceCons; }

  int getLocationNum() const { return (int) locations.size(); }
  int getTransitionNum() const { return (int) transitions.size(); }

  const L &getLocation( int id ) const { return locations[ id ]; }

  const T &getTransition( int id ) const { return transitions[ id ]; }


  
  void initial() {

    vector<int> srcs;
    vector<int> snks;

    for ( typename vector<T>::iterator it = transitions.begin();
          it != transitions.end(); it++ ) {
      srcs.push_back( it->getSource() );
      snks.push_back( it->getTarget() );
    }

    graph.initial( srcs, snks );

    int vertex_num = graph.getVertex_num();

    // // There are no edges connect with  initial location
    assert( initial_loc >= 0 && initial_loc < vertex_num );

    clockUppuerBound.resize( 2 * ( clock_num + 1 ), LTEQ_ZERO );

    for ( typename vector<L>::const_iterator it = locations.begin();
          it != locations.end(); it++ ) {

      const vector<CS_t> &invariants = it->getInvarients();
      for ( typename vector<CS_t>::const_iterator cit = invariants.begin();
            cit != invariants.end(); cit++ ) {

        if ( cit->x > 0 && cit->y > 0 ) {
          differenceCons.push_back( *cit );
        } else {
          if ( cit->x > 0 ) {
            if ( cit->matrix_value > clockUppuerBound[ cit->x ] ) {
              clockUppuerBound[ cit->x ] = cit->matrix_value;
            } else {
              CS_t temp = cit->neg();
              if ( temp.matrix_value > clockUppuerBound[ temp.x ] ) {
                clockUppuerBound[ temp.x ] = temp.matrix_value;
              }
            }
          }
        }
      }
    }
    for ( int i = 1; i <= clock_num + 1; i++ ) {
      clockUppuerBound[ i ] =
          getMatrixValue( getRight( clockUppuerBound[ i ] ), false );
    }

    /**
     *      x-y<= k_i
     *      x-y< -k_{i+n+1}
     *
     */

    for ( int i = 1; i < clock_num + 1; i++ ) {
      clockUppuerBound[ i + clock_num + 1 ] = LTEQ_ZERO - clockUppuerBound[ i ];
    }
  }

  void setClockNum( int num ) { clock_num = num; }

  int getClockNum() const { return clock_num; }

  void setInitialLoc( int loc ) { initial_loc = loc; }
  int  getInitialLoc() const { return initial_loc; }

  const vector<T> &getTransitions( void ) const { return transitions; }

  const vector<L> &getLocations( void ) const { return locations; }
  /**
   * Add one location to this timed automata
   *
   * @param location
   *
   * @return
   */
  TA<C, L, T> &operator+=( L &location ) {
    locations.push_back( location );
    return *this;
  }

  /**
   * Add one transition to this timed automata
   *
   * @param edge
   *
   * @return
   */
  TA<C, L, T> &operator+=( T &edge ) {
    transitions.push_back( edge );
    return *this;
  }
};

} // namespace graphsat

#endif
