
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

template <typename C_t, typename CS_t, typename DBMManager_t, typename DBMSet_t>
class Location {

public:
public:
  explicit Location( int loc_id ) {
    location_id = loc_id;
    type        = NORMOAL_LOC;
    name= to_string( loc_id);
  }

  explicit Location( int loc_id, Location_Type etype ) {
    location_id = loc_id;
    type        = etype;
    name= to_string( loc_id);
  }

  void setName( const string &n ) { name = n; }

  string getName( void ) { return name; }

  const vector<CS_t> &getInvarients() const { return invariants; }

  /**
   * @brief  the commit freeze time. Semantically, urgent locations are
   * equivalent to:
   * 1.  adding sn extra clock x, that is reset on every  incomming edge, and
   * 2. adding an invariant x<=0 to the location
   *
   * @return  true if this location is urgent location, false otherwise.
   */

  inline bool isUrgent() const { return type == URGENT_LOC; }

  /**
   * @brief  the commit freeze time. Furthermore, if any process is in a
   * committed location, the next transition must involve an edge from one of
   * the committed locations.
   *
   *
   * @return true if this location is a commit, false otherwise.
   */
  inline bool isCommit() const { return type == COMMIT_LOC; }

  inline bool isFreezeLocation() const {
    return ( isUrgent() ) || ( isCommit() );
  }

  inline void employInvariants( const DBMManager_t &dbm_manager,
                                C_t *             dbm ) const {
    for ( auto cs : invariants ) {
      dbm_manager.andImpl( dbm, cs );
    }
  }

  /**
   * @brief check whether the state dbm satisfies the invariant of this location
   *
   * @param dbm_manager dbm manager.
   * @param dbm a dbm matrix.
   *
   * @return  true if dbm  satisfies invariant, false otherwise.
   */

  inline bool isReachable( const DBMManager_t &dbm_manager, C_t *dbm ) const {
    /**
     * D reach Location first check D satisfies all the invariants in
     * this Location
     *
     */
    employInvariants( dbm_manager, dbm );

    return dbm_manager.isConsistent( dbm );
  }

  inline void operator()( const DBMManager_t &dbm_manager, C_t *dbm ) const {
    assert( isReachable( dbm_manager, dbm ) );
    assert( !isFreezeLocation() );

    dbm_manager.upImpl( dbm );

    assert( dbm_manager.isConsistent( dbm ) );
  }

  bool operator()( const DBMManager_t &dbm_manager, const C_t *const dbm,
                   vector<C_t *> &re_vec ) const {
    C_t *newDBM = dbm_manager.createDBM( dbm );
    bool re     = ( *this )( dbm_manager, newDBM );
    if ( !re ) {
      dbm_manager.destroyDBM( newDBM );
      return false;
    }

    dbm_manager.norm( newDBM, re_vec );

    return ( !re_vec.empty() );
  }

  /**
   * Add one invariant to this location
   *
   * @param cs atomic constraint
   *
   * @return a new location
   */
  Location<C_t, CS_t, DBMManager_t, DBMSet_t> &operator+=( CS_t &cs ) {
    invariants.push_back( cs );
    return *this;
  }

  void clockShift( int shift ) {
    for ( size_t i = 0; i < invariants.size(); i++ ) {
      invariants[ i ].clockShift( shift );
    }
  }
  
  // string to_string( ) const{
  //   string re_str="name: "+name;
  //   if( !invariants.empty( )){
  //     re_str+"\n"+"invariants: ";
  //     for( auto e: invariants){
        
  //     }
  //   }
  // }
  friend ostream & operator << (ostream &os, const Location<C_t, CS_t, DBMManager_t, DBMSet_t>& loc);

private:
  vector<CS_t>  invariants; // set of invariants  in this Location
  int           location_id;
  string        name;
  Location_Type type;
};
} // namespace graphsat

#endif
