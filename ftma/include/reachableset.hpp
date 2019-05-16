/**
 * @file   reachset.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Apr 27 08:49:05 2019
 *
 * @brief  the data of reachset
 *
 *
 */
#ifndef __REACH_SET_HPP
#define __REACH_SET_HPP
#include <vector>

namespace graphsat {
using namespace std;

template <typename SYS> class ReachableSet {
public:

private:
  StateSet<State_t> reachSet;
  StateSet<State_t> waitSet;

  const SYS &ta;
  StateManager_t     manager;
  template <typename R1> friend class Reachability;
  int component_num;
  
  bool isReachable( const vector< vector <CS_t> > &cons, const State_t* dbm ) const {
    if ( cons.empty() ) {
      return true;
    }
    
    State_t* newdbm = dbm->copy( );
    for( int i=0; i< component_num; i++){
      
      for ( size_t j = 0; j < cons[i].size(); j++ ) {
        manager.getClockManager(i ).andImpl( newdbm->value+manager.getClockStart( i), cons[ i ][ j] );

        if ( !manager.getClockManager(i ).isConsistent( newdbm->value+manager.getClockStart( i) ) ) {
          delete newdbm;
          return false;
        }
      }
    }

    delete newdbm;
    return true;
  }

public:
  ReachableSet( const SYS &outta )
      : ta( outta ) {

    manager =ta.getStateManager( );
    component_num=ta.getComponentNum( );
    State_t* D  = manager.newMatrix( );
    ta.initState(manager, D);
    
    waitSet.add(D );

    reachSet.add( D);


  }

  ~ReachableSet() {

    reachSet.clear();
  }

  const SYS &getTA( void ) const { return ta; }

  Check_State find( const vector<int> &loc, const vector< vector<CS_t> > &cons ) {
    
    typename StateSet_t::iterator end1 = reachSet.end();
    for ( typename StateSet_t::iterator it = reachSet.begin(); it != end1;
          ++it ) {

      if ( isReachable( cons, *it ) ) {
        return TRUE;
      }
    }
    return UNKOWN;
  }

  void update(  StateSet_t &secondWaitSet ) {


    waitSet = secondWaitSet;
    secondWaitSet.clear( );

  }

  bool oneStep( const vector<int> loc, const vector< vector<CS_t> > &cons, 
                NIntState * state, 
                StateSet_t &secondWaitSet ) {
    
    int source = 0;

    DBMSet_t discreteTransNext;
    
    for( int i=0; i< component_num; i++) {
      int source=state->value[ i];
      int target=0;
      int outDegree = ta.tas[i].graph.getOutDegree(source);
      for( int j=0; j< outDegree; j++){
        int link = ta.tas[i].graph.getAdj(source, j);
        ta.tas[i].graph.findRhs(link, source, target);
        
        ta.tas[i].transitions[link]( manager.getClockManager( i),
                                    state->value+manager.getClockStart( i), discreteTransNext );
        
        vector<C_t *> advanceNext;

        if ( ta.tas[i].locations[ target ](manager.getClockManager( i), discreteTransNext,
                                        advanceNext ) ) {
          

          for ( size_t h = 0; h < advanceNext.size(); h++ ) {

            // if ( reachSet.add( manager, advanceNext[ h ] ) ) {
            //   secondWaitSet[ target ].add( manager, advanceNext[ h ] );
            //   // if ( loc == target ) {
            //   //   if ( isReachable( cons, advanceNext[ h ] ) ) {
            //   //     secondChanged.insert( target );
            //   //     update( secondChanged, secondWaitSet );

            //   //     return true;
            //   //   }
            //   // }
            // }
          }

        }

      }

    }
    return false;
  }
};
typedef ReachableSet<TAS_t> R_t;
} // namespace graphsat

#endif
