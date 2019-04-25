/**
 * @file   location.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:33:47 2019
 *
 * @brief  Timed automata location class
 *
 *
 */
#ifndef __LOCATION_HPP
#define __LOCATION_HPP
#include "dbm.hpp"
#include "dbmset.hpp"
#include <vector>

namespace ftma {
using namespace std;
template <typename C, typename CS> struct location {
  typedef dbm<C, CS>     DBM;
  typedef dbmset<C, DBM> DSet;
  vector<CS>             invariants; // set of invariants  in this location

  /**
   *
   *
   * @param Ds  the dbm set of start value in this location
   * @param dbmManager
   * @param next return dbm set of possible value stay in this location
   *
   * @return true if next is not empty
   *         false otherwise.
   */
  bool apply( const DBM &dbmManager, DSet &Ds, DSet &next ) const {
    next.deleteAll();

    vector<C *> vecSet;
    Ds.toVector( vecSet );

    for ( typename vector<C *>::iterator it = vecSet.begin();
          it != vecSet.end(); it++ ) {

      C *D = *it;
      /**
       * D reach location first check D satisfies all the invariants in
       * this location
       *
       */

      for ( typename vector<CS>::const_iterator cit = invariants.begin();
            cit != invariants.end(); cit++ ) {
        dbmManager.andImpl( D, *cit );
      }

      if ( dbmManager.isConsistent( D ) ) {

        dbmManager.upImpl( D );
        /**
         * After D satisfies the invarient then do operator up on D,
         * then left the area which satisfies all invariants
         *
         */

        for ( typename vector<CS>::const_iterator cit = invariants.begin();
              cit != invariants.end(); cit++ ) {
          dbmManager.andImpl( D, *cit );
        }
        if ( dbmManager.isConsistent( D ) ) {
          next.add( dbmManager, D );
        } else {
          delete[] D;
        }
      } else {
        delete[] D;
      }
    }
    Ds.clear();

    return next.size() > 0;
  }

  /**
   *
   * @param D  A dbm matrix of start value  in this location
   * @param dbmManager
   *
   * @return  true if the final set in this location is non-empty
   */
  bool apply( const DBM &dbmManager, C *D ) const {
    dbmManager.upImpl( D );
    for ( typename vector<CS>::const_iterator cit = invariants.begin();
          cit != invariants.end(); cit++ ) {
      dbmManager.andImpl( D, *cit );
    }
    return dbmManager.isConsistent( D );
  }
};
} // namespace ftma

#endif
