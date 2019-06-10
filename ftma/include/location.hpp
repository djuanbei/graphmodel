
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
  explicit Location( int loc_id ) {
    locationID = loc_id;
    type       = NORMOAL_LOC;
  }

  explicit Location( int loc_id, Location_Type etype ) {
    locationID = loc_id;
    type       = etype;
  }

  void setName( const string &n ) { name = n; }

  const vector<CS_t> &getInvarients() const { return invariants; }

  bool isCommit() const { return type == COMMIT_LOC; }

  bool isUrgent() const { return type == URGENT_LOC; }

  void employInvariants( const DManager_t &dbmManager, C_t *dbm ) const {
    for ( auto cs : invariants ) {
      dbmManager.andImpl( dbm, cs );
    }
  }

  bool operator()( const DManager_t &dbmManager, C_t *dbm ) const {

    /**
     * D reach Location first check D satisfies all the invariants in
     * this Location
     *
     */
    for ( auto cs : invariants ) {
      dbmManager.andImpl( dbm, cs );
    }

    if ( dbmManager.isConsistent( dbm ) ) {
      /**
       * Urgent and commit locations freeze time; i.e. time is not allowed to
       * pass when a process is in an urgent location.
       *
       */

      if ( type != URGENT_LOC || type != COMMIT_LOC ) {
        dbmManager.upImpl( dbm );
        /**
         * After D satisfies the invarient then do operator up on D,
         * then left the area which satisfies all invariants
         *
         */

        assert( dbmManager.isConsistent( dbm ) );
      }
      return true;

    } else {
      return false;
    }
  }

  bool operator()( const DManager_t &dbmManager, const C_t *const dbm,
                   vector<C_t *> &re_vec ) const {
    C_t *newDBM = dbmManager.createDBM( dbm );
    bool re     = ( *this )( dbmManager, newDBM );
    if ( !re ) {
      dbmManager.destroyDBM( newDBM );
      return false;
    }

    dbmManager.norm( newDBM, re_vec );

    return ( !re_vec.empty() );
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

  void clockShift( int shift ) {
    for ( size_t i = 0; i < invariants.size(); i++ ) {
      invariants[ i ].clockShift( shift );
    }
  }

private:
  vector<CS_t>  invariants; // set of invariants  in this Location
  int           locationID;
  string        name;
  Location_Type type;
};
} // namespace graphsat

#endif
