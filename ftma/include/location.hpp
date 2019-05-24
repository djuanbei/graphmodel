
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
using std::vector;

enum Location_Type { NORMOAL_LOC, INIT_LOC, URGENT_LOC, COMMIT_LOC };

template <typename C_t, typename CS_t, typename DManager_t, typename DBMSet_t>
class Location {

public:
public:
  explicit Location( int loc ) {
    locationID = loc;
    type       = NORMOAL_LOC;
  }

  explicit Location( int loc, Location_Type t ) {
    locationID = loc;
    type       = t;
  }

  const vector<CS_t> &getInvarients() const { return invariants; }

  bool isCommit() const { return type == COMMIT_LOC; }


  bool operator()( const DManager_t &dbmManager, C_t *D ) const {

    /**
     * D reach Location first check D satisfies all the invariants in
     * this Location
     *
     */
    for( auto cs: invariants){
      dbmManager.andImpl( D, cs );
    }

    if ( dbmManager.isConsistent( D ) ) {
      /**
       * Urgent and commit locations freeze time; i.e. time is not allowed to
       * pass when a process is in an urgent location.
       *
       */

      if ( type != URGENT_LOC || type != COMMIT_LOC ) {
        dbmManager.upImpl( D );
        /**
         * After D satisfies the invarient then do operator up on D,
         * then left the area which satisfies all invariants
         *
         */
        for ( auto cs : invariants ) {
          dbmManager.andImpl( D, cs );
        }

        assert( dbmManager.isConsistent( D ) );
      }
      return true;

    } else {
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

private:
  vector<CS_t>  invariants; // set of invariants  in this Location
  int           locationID;
  Location_Type type;
};
} // namespace graphsat

#endif
