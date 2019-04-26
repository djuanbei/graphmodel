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
#include "dbm.hpp"
#include "dbmset.hpp"
#include <vector>

namespace ftma {
using namespace std;
template <typename C, typename CS, typename A> struct Transition {
  typedef DBM<C, CS>     DBM;
  typedef DBMset<C, DBM> DSet;

  int source, target;  // source location and target location of this transitionedge. The
                       // index of location in tma.locations
  vector<CS>  cons;    // set of constraint at this transitionedge
  vector<A>   actions; // set of actions at this transitionedge
  vector<int> reset;   // set of reset clock variables


  /** 
   * add constraint to Transition
   * 
   * @param lhs 
   * @param cs 
   * 
   * @return 
   */
 
  friend Transition<C,CS, A>& operator + (Transition<C,CS, A> & lhs, CS & cs  ){
    lhs.cons.push_back( cs);
    return lhs;
  }

  Transition<C,CS, A>& operator += ( CS & cs  ){
    cons.push_back( cs);
    return *this;
  }
  
  
  /**
   *
   *
   * @param dbmManager
   * @param Ds  The DBM matrix of source location. The transitionedge can not change the
   * value of it.
   * @param next  Compute the target DBM matrix after apply this transitionedge on Ds.
   *
   * @return true if next is nonempty, false otherwise.
   */
  bool apply( const DBM &dbmManager, const DSet &Ds, DSet &next ) const {

    next.deleteAll();
    vector<C *> vecSet;
    Ds.toVector( vecSet );

    for ( typename vector<C *>::const_iterator it = vecSet.begin();
          it != vecSet.end(); it++ ) {
      C *D = dbmManager.newMatrix( *it );
      for ( typename vector<CS>::const_iterator cit = cons.begin();
            cit != cons.end(); cit++ ) {
        dbmManager.andImpl( D, *cit );
      }
      if ( dbmManager.isConsistent( D ) ) {
        for ( vector<int>::const_iterator rit = reset.begin();
              rit != reset.end(); rit++ ) {
          assert( *rit > 0 ); // clock id start from 1
          dbmManager.resetImpl( D, *rit, 0 );
        }
        next.add( dbmManager, D );
      } else {
        delete[] D;
      }
    }
    return next.size() > 0;
  }

  bool apply( const DBM &dbmManager, const C *const Din ) const {
    C *D = dbmManager.newMatrix( Din );
    for ( typename vector<CS>::iterator cit = cons.begin(); cit != cons.end();
          cit++ ) {
      dbmManager.andImpl( D, *cit );
    }
    if ( dbmManager.isConsistent( D ) ) {
      for ( vector<int>::iterator rit = reset.begin(); rit != reset.end();
            rit++ ) {
        assert( *rit > 0 ); // clock id start from 1
        dbmManager.resetImpl( D, *rit, 0 );
      }
      return true;
    } else {
      return false;
    }
  }
};
} // namespace ftma

#endif
