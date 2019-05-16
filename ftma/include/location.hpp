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

#include <vector>

namespace graphsat {
using namespace std;

enum Location_Type{
  NORMOAL_LOC, INIT_LOC,
  URGENT_LOC, COMMIT_LOC
};

template <typename C_t, typename CS_t, typename DManager_t, typename DBMSet_t>
class Location {
public:
private:
  vector<CS_t> invariants; // set of invariants  in this Location

  int          locationID;
  Location_Type type;
  

public:
public:
  explicit Location( int loc ) { locationID = loc;
    type=NORMOAL_LOC;
  }

  explicit Location( int loc, Location_Type t ) {
    locationID = loc;
    type=t;
  }

  const vector<CS_t> &getInvarients() const { return invariants; }

  /**
   *
   * @param reachDBMS  the DManager_t set of start value in this Location
   * @param dbmManager
   * @param advanceNext return DManager_t set of possible value stay in this Location
   *
   * @return true if advanceNext is not empty
   *         false otherwise.
   */

  bool operator()( const DManager_t &dbmManager, const DBMSet_t &reachDBMS,
                   vector<C_t *> &reNormVecDBM ) const {
    
    assert( reNormVecDBM.empty() );
    
    DBMSet_t                    advanceNext;
    
    typename DBMSet_t::const_iterator end1 = reachDBMS.end();
    for ( typename DBMSet_t::const_iterator it = reachDBMS.begin(); it != end1; ++it ) {

      C_t *D = dbmManager.newMatrix(*it);
      /**
       * D reach Location first check D satisfies all the invariants in
       * this Location
       *
       */

      for ( typename vector<CS_t>::const_iterator cit = invariants.begin();
            cit != invariants.end(); cit++ ) {
        dbmManager.andImpl( D, *cit );
      }
      
      if ( dbmManager.isConsistent( D ) ) {
        /**
         * Urgent and commit locations freeze time; i.e. time is not allowed to pass when a process is in an urgent location.
         * 
         */

        if(type!=URGENT_LOC || type!= COMMIT_LOC ){
          dbmManager.upImpl( D );
          /**
           * After D satisfies the invarient then do operator up on D,
           * then left the area which satisfies all invariants
           *
           */
          for ( typename vector<CS_t>::const_iterator cit = invariants.begin();
                cit != invariants.end(); cit++ ) {
            dbmManager.andImpl( D, *cit );
          }
          assert(dbmManager.isConsistent( D ) );
        }
        
        advanceNext.add( dbmManager, D );
      }
    }


    if ( 0 == advanceNext.size() ) {
      return false;
    }

    typename DBMSet_t::iterator end2 = advanceNext.end();
    for ( typename DBMSet_t::iterator it = advanceNext.begin(); it != end2;
          ++it ) {

      vector<C_t *> normVecDBM;
      dbmManager.norm( locationID, *it, normVecDBM );

      reNormVecDBM.insert( reNormVecDBM.end(), normVecDBM.begin(),
                           normVecDBM.end() );
    }

    return reNormVecDBM.size() > 0;
  }

  bool operator( )( const DManager_t &dbmManager, C_t *D, vector<C_t *> &reNormVecDBM ) const {
    
    assert( reNormVecDBM.empty() );
    
    DBMSet_t                    advanceNext;
    

    /**
     * D reach Location first check D satisfies all the invariants in
     * this Location
     *
     */

    for ( typename vector<CS_t>::const_iterator cit = invariants.begin();
          cit != invariants.end(); cit++ ) {
      dbmManager.andImpl( D, *cit );
    }
      
    if ( dbmManager.isConsistent( D ) ) {
      /**
       * Urgent and commit locations freeze time; i.e. time is not allowed to pass when a process is in an urgent location.
       * 
       */

      if(type!=URGENT_LOC || type!= COMMIT_LOC ){
        dbmManager.upImpl( D );
        /**
         * After D satisfies the invarient then do operator up on D,
         * then left the area which satisfies all invariants
         *
         */
        for ( typename vector<CS_t>::const_iterator cit = invariants.begin();
              cit != invariants.end(); cit++ ) {
          dbmManager.andImpl( D, *cit );
        }
        assert(dbmManager.isConsistent( D ) );
      }
        
      advanceNext.add( dbmManager, D );
    } else {
      delete[] D;
    }
  

    if ( 0 == advanceNext.size() ) {
      return false;
    }

    typename DBMSet_t::iterator end2 = advanceNext.end();
    for ( typename DBMSet_t::iterator it = advanceNext.begin(); it != end2;
          ++it ) {

      vector<C_t *> normVecDBM;
      dbmManager.norm( locationID, *it, normVecDBM );

      reNormVecDBM.insert( reNormVecDBM.end(), normVecDBM.begin(),
                           normVecDBM.end() );
    }

    return reNormVecDBM.size() > 0;
  }

  /**
   *
   * @param D  A DManager_t matrix of start value  in this Location
   * @param dbmManager
   *
   * @return  true if the final set in this Location is non-empty
   */
  bool operator()( const DManager_t &dbmManager, C_t *D ) const {
    
    if(!dbmManager.isConsistent( D )){
      return false;
    }
    /**
     * D reach Location first check D satisfies all the invariants in
     * this Location
     *
     */

    for ( typename vector<CS_t>::const_iterator cit = invariants.begin();
          cit != invariants.end(); cit++ ) {
      dbmManager.andImpl( D, *cit );
    }
    
    if (dbmManager.isConsistent( D ) ) {
      dbmManager.upImpl( D );
      for ( typename vector<CS_t>::const_iterator cit = invariants.begin();
            cit != invariants.end(); cit++ ) {
        dbmManager.andImpl( D, *cit );
      }
      
      assert(dbmManager.isConsistent( D )  );
      return true;
      
    }else{
      return false;
    }
  }

  /**
   * Add one invariant to this location
   *
   * @param cs
   *
   * @return
   */
  Location<C_t, CS_t, DManager_t, DBMSet_t> &operator+=( CS_t &cs ) {
    invariants.push_back( cs );
    return *this;
  }
};
} // namespace graphsat

#endif
