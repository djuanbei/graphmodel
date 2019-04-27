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

#include "ta.hpp"

namespace ftma {
using namespace std;

template <typename C, typename L, typename T> class ReachableSet {
public:
  typedef Constraint<C>    CS_t;
  typedef DBM<C, CS_t>     DBM_t;
  typedef DBMset<C, DBM_t> DSet_t;

private:
  vector<DSet_t>     reachSet;
  vector<DSet_t>     waitSet;
  vector<int>        lastChangedLocs;
  const TA<C, L, T> &ta;
  DBM_t              dbmManager;
  template <typename C1, typename L1, typename T1> friend class Reachability;

  bool isReachable( const vector<CS_t> &cons, const C *const dbm ) const {
    if ( cons.empty() ) {
      return true;
    }
    C *newdbm = dbmManager.And( dbm, cons[ 0 ] );
    if ( !dbmManager.isConsistent( newdbm ) ) {
      delete[] newdbm;
      return false;
    }

    for ( size_t i = 1; i < cons.size(); i++ ) {
      dbmManager.andImpl( newdbm, cons[ i ] );
      if ( !dbmManager.isConsistent( newdbm ) ) {
        delete[] newdbm;
        return false;
      }
    }
    delete[] newdbm;
    return true;
  }

public:
  ReachableSet( const TA<C, L, T> &outta )
      : ta( outta )
      , dbmManager( outta.getClockNum() ) {

    int vertex_num = ta.getLocationNum();
    waitSet.resize( vertex_num );
    reachSet.resize( vertex_num );

    C * D           = dbmManager.newMatrix();
    int initial_loc = ta.getInitialLoc();

    ta.getLocation( initial_loc )( dbmManager, D );

    waitSet[ initial_loc ].add( dbmManager, D );
    reachSet[ initial_loc ].add( dbmManager, D );

    lastChangedLocs.push_back( initial_loc );
  }

  ~ReachableSet() {

    for ( typename vector<DBMset<C, DBM_t>>::iterator it = reachSet.begin();
          it != reachSet.end(); it++ ) {
      it->deleteAll();
    }
    reachSet.clear();
  }

  const TA<C, L, T> &getTA( void ) const { return ta; }

  Check_State find( const int loc, const vector<CS_t> &cons ) {
    if ( loc < 0 || (size_t) loc >= reachSet.size() ) {
      return FALSE;
    }

    vector<C *> haveReached;
    reachSet[ loc ].toVector( haveReached );

    for ( size_t i = 0; i < haveReached.size(); i++ ) {
      if ( isReachable( cons, haveReached[ i ] ) ) {
        return TRUE;
      }
    }
    return UNKOWN;
  }
};

} // namespace ftma

#endif
