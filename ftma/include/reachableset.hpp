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

  typedef typename Model_t::CS_t       CS_t;
  typedef typename Model_t::D_t        D_t;
  typedef typename Model_t::DManager_t DManager_t;
  typedef typename Model_t::DSet_t     DSet_t;

private:
  StateSet<NIntState> reachSet;
  StateSet<NIntState> waitSet;

  const Model_t &ta;
  DManager_t     dManager;
  template <typename R1> friend class Reachability;

  bool isReachable( const vector<CS_t> &cons, const D_t dbm ) const {
    if ( cons.empty() ) {
      return true;
    }
    D_t newdbm =dManager.newMatrix( dbm);

    for ( size_t i = 0; i < cons.size(); i++ ) {
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

    dManager =ta.getStateManager( );

    D_t D  = dManager.newMatrix( );
    ta.initState(dManager, D);
    
    waitSet.add(D );

    reachSet.add( D);


  }

  ~ReachableSet() {

    reachSet.clear();
  }

  const Model_t &getTA( void ) const { return ta; }

  Check_State find( const vector<int> &loc, const vector< vector<CS_t> > &cons ) {
    
    typename DSet_t::iterator end1 = reachSet.end();
    for ( typename DSet_t::iterator it = reachSet.begin(); it != end1;
          ++it ) {

      if ( isReachable( cons, *it ) ) {
        return TRUE;
      }
    }
    return UNKOWN;
  }

  void update(  DSet_t &secondWaitSet ) {


    waitSet = secondWaitSet;
    secondWaitSet.clear( );
    

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
