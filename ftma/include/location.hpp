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
template <typename C_t, typename CS_t, typename D_t, typename DSet_t>
class Location {
public:
private:
  vector<CS_t> invariants; // set of invariants  in this Location
  int          locationID;

public:
public:
  explicit Location( int loc ) { locationID = loc; }

  const vector<CS_t> &getInvarients() const { return invariants; }

  /**
   *
   *
   * @param reachDBMS  the D_t set of start value in this Location
   * @param dbmManager
   * @param advanceNext return D_t set of possible value stay in this Location
   *
   * @return true if advanceNext is not empty
   *         false otherwise.
   */

  bool operator()( const D_t &dbmManager, DSet_t &reachDBMS,
                   vector<C_t *> &reNormVecDBM ) const {
    assert( reNormVecDBM.empty() );
    DSet_t                    advanceNext;
    typename DSet_t::iterator end1 = reachDBMS.end();
    for ( typename DSet_t::iterator it = reachDBMS.begin(); it != end1; ++it ) {

      C_t *D = *it;
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
        if ( dbmManager.isConsistent( D ) ) {
          advanceNext.add( dbmManager, D );
        } else {
          delete[] D;
        }
      } else {
        delete[] D;
      }
    }
    reachDBMS.clear();

    if ( 0 == advanceNext.size() ) {
      return false;
    }

    typename DSet_t::iterator end2 = advanceNext.end();
    for ( typename DSet_t::iterator it = advanceNext.begin(); it != end2;
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
   * @param D  A D_t matrix of start value  in this Location
   * @param dbmManager
   *
   * @return  true if the final set in this Location is non-empty
   */
  bool operator()( const D_t &dbmManager, C_t *D ) const {
    dbmManager.upImpl( D );
    for ( typename vector<CS_t>::const_iterator cit = invariants.begin();
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
  Location<C_t, CS_t, D_t, DSet_t> &operator+=( CS_t &cs ) {
    invariants.push_back( cs );
    return *this;
  }
};
} // namespace graphsat

#endif
