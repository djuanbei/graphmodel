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

namespace graphsat {
using namespace std;

template <typename Model> class ReachableSet {
public:
  typedef Model Model_t;
 
  typedef typename  Model_t::CS_t       CS_t;
  typedef typename Model_t::D_t        D_t;
  typedef typename Model_t::DManager_t DManager_t;
  typedef typename Model_t::DSet_t     DSet_t;

private:
  vector<DSet_t> reachSet;
  vector<DSet_t> waitSet;
  vector<int>    lastChangedLocs;
  const Model_t &ta;
  DManager_t     dManager;
  template <typename R1> friend class Reachability;

  bool isReachable( const vector<CS_t> &cons, const D_t dbm ) const {
    if ( cons.empty() ) {
      return true;
    }
    D_t newdbm = dManager.And( dbm, cons[ 0 ] );
    if ( !dManager.isConsistent( newdbm ) ) {
      dManager.deleteD( newdbm );

      return false;
    }

    for ( size_t i = 1; i < cons.size(); i++ ) {
      dManager.andImpl( newdbm, cons[ i ] );
      if ( !dManager.isConsistent( newdbm ) ) {
        dManager.deleteD( newdbm );
        return false;
      }
    }
    dManager.deleteD( newdbm );
    return true;
  }

public:
  ReachableSet( const Model_t &outta )
      : ta( outta ) {

    dManager = DManager_t( ta.getClockNum(), ta.getClockUppuerBound(),
                           ta.getDifferenceCons() );

    int vertex_num = ta.getLocationNum();
    waitSet.resize( vertex_num );
    reachSet.resize( vertex_num );

    D_t D           = dManager.newMatrix();
    int initial_loc = ta.getInitialLoc();

    ta.getLocation( initial_loc )( dManager, D );

    waitSet[ initial_loc ].add( dManager, D );
    reachSet[ initial_loc ].add( dManager, D );

    lastChangedLocs.push_back( initial_loc );
  }

  ~ReachableSet() {

    for ( typename vector<DSet_t>::iterator it = reachSet.begin();
          it != reachSet.end(); it++ ) {
      it->deleteAll();
    }
    reachSet.clear();
  }

  const Model_t &getTA( void ) const { return ta; }

  Check_State find( const int loc, const vector<CS_t> &cons ) {
    if ( loc < 0 || (size_t) loc >= reachSet.size() ) {
      return FALSE;
    }
 
    typename DSet_t::iterator end1 = reachSet[ loc ].end();
    for ( typename DSet_t::iterator it = reachSet[ loc ].begin(); it != end1;
          ++it ) {

      if ( isReachable( cons, *it ) ) {
        return TRUE;
      }
    }
    return UNKOWN;
  }

  void update( set<int> &secondChanged, vector<DSet_t> &secondWaitSet ) {
    lastChangedLocs.clear();

    lastChangedLocs.insert( lastChangedLocs.begin(), secondChanged.begin(),
                            secondChanged.end() );
    secondChanged.clear();

    waitSet = secondWaitSet;
    for ( size_t i = 0; i < lastChangedLocs.size(); i++ ) {
      int source = lastChangedLocs[ i ];
      secondWaitSet[ source ].clear();
    }
  }

  bool oneStep( const int loc, const vector<CS_t> &cons, const int target,
                int link, set<int> &secondChanged,
                vector<DSet_t> &secondWaitSet ) {
    int source = 0;
    ta.findRhs( link, target, source );

    DSet_t discreteTransNext;
    if ( ta.transitions[ link ]( dManager, waitSet[ source ],
                                 discreteTransNext ) ) {

      vector<D_t> advanceNext;

      if ( ta.locations[ target ]( dManager, discreteTransNext,
                                   advanceNext ) ) {

        for ( size_t h = 0; h < advanceNext.size(); h++ ) {

          if ( reachSet[ target ].add( dManager, advanceNext[ h ] ) ) {
            secondWaitSet[ target ].add( dManager, advanceNext[ h ] );
            if ( loc == target ) {
              if ( isReachable( cons, advanceNext[ h ] ) ) {
                secondChanged.insert( target );
                update( secondChanged, secondWaitSet );

                return true;
              }
            }
          }
        }
        secondChanged.insert( target );
      }
    }
    return false;
  }
};

} // namespace graphsat

#endif
