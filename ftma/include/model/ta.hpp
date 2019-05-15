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

#include "discretestate.hpp"

#include <vector>

namespace graphsat {

using namespace std;
using namespace raptor;

/** 
 * 
 *  TA has multi-components
 * 
 */

template <typename C, typename L, typename T> class TA {

public:

  //typedef C* D_t;

  //typedef DBM<C>          DManager_t;

  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;

private:
  typedef ClockConstraint<C>         CS_t;
  
  vector<L> locations;
  vector<T> transitions;
  int       initial_loc;

  int       clock_num;
  
  graph_t<int>  graph;

  vector<C>  clockUppuerBound;
   
  vector< ClockConstraint<C>>  differenceCons;
  

public:
  TA() { initial_loc  =clock_num= -1; }
  TA( int init, int clockNum) {
    initial_loc = init;
    clock_num=clockNum;
  }
  
  TA( vector<L> &locs, vector<T> &es, int init, int vnum  )
      : locations( locs )
      , transitions( es ) {
    initial_loc = init;
    clock_num=vnum;
    initial( );
  }
  
  void findRhs( const int link, const int lhs, int &rhs ) const {
    graph.findRhs( link, lhs, rhs );
  }
  vector<C> getClockUppuerBound() const { return clockUppuerBound; }

  vector<ClockConstraint<C>> getDifferenceCons() const { return differenceCons; }

  int getLocationNum() const { return (int) locations.size(); }
  int getTransitionNum() const { return (int) transitions.size(); }



  
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
    
    differenceCons.clear( );
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

  
  int getClockNum() const { return clock_num; }
  


  void setInitialLoc( int loc ) { initial_loc = loc; }
  int  getInitialLoc() const { return initial_loc; }

  bool upInLocation( int i, const DBM<C> &manager, C* D  ) const{
    return locations[ i](manager, D);
  }
  

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



template <typename C, typename L, typename T> class TAS {
public:
  

  typedef NIntState* D_t;
  typedef ClockConstraint<C>         CS_t;

  /** 
 * state is [channel_state, counter_state, clock_state]
 * 
 */
  
struct  StateManager{

  int compoment_num;
  int stateLen;

  int counter_start_loc;

  vector<int> clock_start_loc;
  
  vector<DBM<C> >  clock_manager;

  StateManager( ){
    compoment_num=stateLen=counter_start_loc=0;
  }
  
  StateManager(int compoment_num,  int counter_num, vector<int> clock_num,
               vector<vector <C> > clockUppuerBound,   vector<vector  <ClockConstraint<C>> > differenceCons ){
    
    compoment_num= clock_num.size( );
       
    counter_start_loc=compoment_num;
    
    stateLen=compoment_num+counter_num;;
    for( size_t i=0; i<clock_num.size( ); i++ ){
      
      clock_start_loc.push_back(stateLen );
      
      stateLen+=(clock_num[i]+1 )*(clock_num[ i]+1 );
      
      DBM<C> temp =DBM<C>(clock_num[ i], clockUppuerBound[ i], differenceCons[ i] );
       
      clock_manager.push_back( temp);
    }

  }

  D_t newMatrix( ) const{
    
    NIntState *re= new NIntState( stateLen);


    for(int i=0; i<compoment_num; i++ ){
      clock_manager[ i].init( re->value+clock_start_loc[ i]);
    }

    return re;
  }
  
  inline const DBM<C>&  getClockManager( int i) const{
    return clock_manager[ i];
  }
  
  inline int getClockStart( int i) const{
    return clock_start_loc[ i];
  }
  
};
  
  typedef StateManager          DManager_t;
  typedef DBMset<C, DManager_t> DSet_t;

  template <typename R1> friend class Reachability;
  template <typename R2> friend class ReachableSet;
  typedef TA<C,L,T> TA_t;
private:
  /**
   * multi-components
   * 
   */
  vector<TA_t>  tas;
  vector<Channel> channels;
  vector<Counter> counters;  
  vector<int> initial_loc;
  vector<int> clock_num;
  
  vector<vector <C> > clockUppuerBound;
  vector<vector <ClockConstraint<C>> > differenceCons;
    
 public:
  TAS<C,L,T> &operator += ( TA_t &ta ){
    tas.push_back( ta);
    initial_loc.push_back( ta.getInitialLoc( ));
    clock_num.push_back( ta.getClockNum( ));
    clockUppuerBound.push_back( ta.getClockUppuerBound( ));
    differenceCons.push_back( ta.getDifferenceCons( ));
    
    return *this;
  }
  TAS<C,L,T> & operator+=( Channel &ch){
    channels.push_back( ch);
    return *this;
  }

  TAS<C,L,T> & operator+=( Counter &c){
    counters.push_back( c);
    return *this;
  }

  StateManager getStateManager( )const{
    
    StateManager re(tas.size( ),  counters.size( ), clock_num, clockUppuerBound, differenceCons );
    
    return re;
  }
  void initState(const StateManager & manager, NIntState *value ) const{

    for( size_t i=0; i< tas.size( ); i++){
      tas[i].upInLocation( initial_loc[ i], manager.getClockManager(i), value->value+manager.getClockStart( i) );
      value->value[ i]=initial_loc[ i];
    }
  }

};
} // namespace graphsat

#endif
