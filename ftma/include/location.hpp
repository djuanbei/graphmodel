/**
 * @file   Location.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:33:47 2019
 *
 * @brief  Timed automata Location class
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
template <typename C, typename CS> class Location {
 public:
  typedef DBM<C, CS>     DBM_t;
  typedef DBMset<C, DBM_t> DSet;
  
 private:
  vector<CS>             invariants; // set of invariants  in this Location

 public:


  const vector<CS> & getInvarients( ) const{
    return invariants;
  }
  

  /**
   *
   *
   * @param Ds  the DBM set of start value in this Location
   * @param dbmManager
   * @param next return DBM set of possible value stay in this Location
   *
   * @return true if next is not empty
   *         false otherwise.
   */
  
  bool operator( ) ( const DBM_t &dbmManager, DSet &Ds, DSet &next ) const {
    next.deleteAll();

    vector<C *> vecSet;
    Ds.toVector( vecSet );

    for ( typename vector<C *>::iterator it = vecSet.begin();
          it != vecSet.end(); it++ ) {

      C *D = *it;
      /**
       * D reach Location first check D satisfies all the invariants in
       * this Location
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
   * @param D  A DBM matrix of start value  in this Location
   * @param dbmManager
   *
   * @return  true if the final set in this Location is non-empty
   */
  bool operator( ) ( const DBM_t &dbmManager, C *D ) const {
    dbmManager.upImpl( D );
    for ( typename vector<CS>::const_iterator cit = invariants.begin();
          cit != invariants.end(); cit++ ) {
      dbmManager.andImpl( D, *cit );
    }
    return dbmManager.isConsistent( D );
  }


  /** 
   * Add one invariant to this location
   * 
   * @param cs 
   * 
   * @return 
   */
  Location<C, CS> & operator +=( CS & cs){
    invariants.push_back( cs);
    return *this;
  }

  
};
} // namespace ftma

#endif
