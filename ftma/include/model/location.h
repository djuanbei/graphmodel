
/**
 * @file   Location.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:33:47 2019
 *
 * @brief  Timed automata Location class
 *
 *
 */
#ifndef __LOCATION_H
#define __LOCATION_H

#include <iostream>
#include <string>
#include <vector>

#include "constraint/clockdiffcons.h"
#include "domain/dbm.h"

namespace graphsat {
using std::string;
using std::to_string;
using std::vector;
enum Location_Type { NORMOAL_LOC, INIT_LOC, URGENT_LOC, COMMIT_LOC };

class Location {

public:
public:
  explicit Location(int loc_id) {
    location_id = loc_id;
    type = NORMOAL_LOC;
    name = LOC_NAME_PRE + to_string(loc_id);
  }

  explicit Location(int loc_id, Location_Type etype) {
    location_id = loc_id;
    type = etype;
    name = LOC_NAME_PRE + to_string(loc_id);
  }

  int getId() const { return location_id; }

  void setName(const string &n) { name = n; }

  string getName(void) const { return name; }

  const vector<ClockConstraint> &getInvarients() const { return invariants; }

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
  /**
   In  freeze location the time can not go.
   */
  inline bool isFreezeLocation() const { return (isUrgent()) || (isCommit()); }

  inline void employInvariants(const DBMFactory &dbm_manager, int *dbm) const {
    for (auto cs : invariants) {
      dbm_manager.andImpl(dbm, cs);
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

  inline bool isReachable(const DBMFactory &dbm_manager, int *dbm) const {
    /**
     * D reach Location first check D satisfies all the invariants in
     * this Location
     *
     */
    employInvariants(dbm_manager, dbm);

    return dbm_manager.isConsistent(dbm);
  }

  inline void operator()(const DBMFactory &dbm_manager, int *dbm) const {
    assert(isReachable(dbm_manager, dbm));
    assert(!isFreezeLocation());

    dbm_manager.upImpl(dbm);

    assert(dbm_manager.isConsistent(dbm));
  }

  bool operator()(const DBMFactory &dbm_manager, const int *const dbm,
                  vector<int *> &re_vec) const;

  /**
   * Add one invariant to this location
   *
   * @param cs atomic constraint
   *
   * @return a new location
   */
  Location &operator+=(ClockConstraint &cs);

  void clockShift(const int shift) {
    for (size_t i = 0; i < invariants.size(); i++) {
      invariants[i].clockShift(shift);
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
  friend std::ostream &operator<<(std::ostream &os, const Location &loc);

private:
  vector<ClockConstraint> invariants; // set of invariants  in this Location
  int location_id;
  string name;
  Location_Type type;
};
} // namespace graphsat

#endif
