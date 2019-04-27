
/**
 * @file   reachset.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:24:17 2019
 *
 * @brief  reachable analysis of an TA
 *
 *
 */

#ifndef __REACH_SET_HPP
#define __REACH_SET_HPP
#include "dbm.hpp"
#include "dbmset.hpp"
#include "graph/graph.hpp"
#include "tma.hpp"
#include <vector>

namespace ftma {

using namespace std;
using namespace raptor;
using namespace std;

template <typename C, typename L, typename T> class reach {

private:
  typedef Constraint<C>  CS;
  typedef DBM<C, CS>     DBM_t;
  typedef DBMset<C, DBM_t> DSet;

  int initial_loc; // the initial location of timed automata
  int vertex_num;  // number of locations in

  TA<L, T> &  tma;
  DBM_t          dbmManager;
  vector<DSet> reachSet;
  vector<DSet> waitSet;

  vector<int> lastChangedLocs;
  
  graph_t<int> graph;
  C *          clockUppuerBound;

  vector<Constraint<C>> differenceCons;

  bool isReachable( const vector<CS> &cons, const C *const dbm  ) const {
    if ( cons.empty() ) {
      return true;
    }
    C *D = dbmManager.And( dbm, cons[ 0 ] );
    if ( !dbmManager.isConsistent( D ) ) {
      return false;
    }

    for ( size_t i = 1; i < cons.size(); i++ ) {
      dbmManager.andImpl( D, cons[ i ] );
      if ( !dbmManager.isConsistent( D ) ) {
        return false;
      }
    }
    return true;
  }

public:
  reach( TA<L, T> &otma )
      : tma( otma )
      , dbmManager( otma.getClockNum( ) ) {

    initial_loc = tma.getInitialLoc();

    
    vector<int> srcs;
    vector<int> snks;
    const vector<T> & transitions= tma.getTransitions( );
    for(typename vector<T>::const_iterator it=transitions.begin( ); it!= transitions.end( ); it++){
      srcs.push_back( it->getSource() );
      snks.push_back( it->getTarget() );
    }

    graph.initial( srcs, snks );

    vertex_num = graph.getVertex_num();

    
    // There are no edges connect with  initial location
    assert( initial_loc >= 0 && initial_loc< vertex_num );

    waitSet.resize ( vertex_num );
    reachSet.resize( vertex_num );
    
    C *D = dbmManager.newMatrix();
    const vector<L> & locations=tma.getLocations( );
    locations[ initial_loc]( dbmManager, D);


    waitSet[ initial_loc ].add( dbmManager, D );
    reachSet[ initial_loc ].add( dbmManager, D );

    lastChangedLocs.push_back( initial_loc );

    int clock_num=tma.getClockNum( );
    clockUppuerBound = new C[ 2 * ( clock_num + 1 ) ];
    fill( clockUppuerBound, clockUppuerBound + 2 * ( clock_num + 1 ), LTEQ_ZERO );

    for ( typename vector<L>::const_iterator it = locations.begin();
          it != locations.end(); it++ ) {
      
      const vector<CS> & invariants=it->getInvarients( );
      for ( typename vector<CS>::const_iterator cit = invariants.begin();
            cit != invariants.end(); cit++ ) {

        if ( cit->x > 0 && cit->y > 0 ) {
          differenceCons.push_back( *cit );
        } else {
          if ( cit->x > 0 ) {
            if ( cit->matrix_value > clockUppuerBound[ cit->x ] ) {
              clockUppuerBound[ cit->x ] = cit->matrix_value;
            } else {
              CS temp = cit->neg();
              if ( temp.matrix_value > clockUppuerBound[ temp.x ] ) {
                clockUppuerBound[ temp.x ] = temp.matrix_value;
              }
            }
          }
        }
      }
    }
    for ( int i = 1; i <= clock_num + 1; i++ ) {
      clockUppuerBound[ i ] = getMatrixValue( getRight( clockUppuerBound[ i ] ), false );
    }

    /**
     *      x-y<= k_i
     *      x-y< -k_{i+n+1}
     *
     */

    for ( int i = 1; i < clock_num + 1; i++ ) {
      clockUppuerBound[ i + clock_num + 1 ] = LTEQ_ZERO - clockUppuerBound[ i ];
    }
  }
  ~reach() {
    delete[] clockUppuerBound;
    deleteData();
  }
  
  void computeAllReachableSet() {
    vector<CS> cons;
    run( -1, cons );
  }

  bool reachable( const int loc ) {
    vector<CS> cons;
    return run( loc, cons );
  }

  /**
   * @param L
   * @param TA
   * @param C
   * @param reachSet
   *
   * @return
   */
  bool run( const int loc, const vector<CS> &cons ) {
    
    if( loc<0 || loc>=vertex_num){
      return false;
    }
    vector<C*> haveReached;
    reachSet[ loc].toVector( haveReached);
    
    for(size_t i=0; i< haveReached.size( );i++){
      if( isReachable( cons, haveReached[ i] )){
        return true;
      }
    }
    const vector<T> & transitions= tma.getTransitions( );
    const vector<L> & locations=tma.getLocations( );    

    set<int>    secondChanged;

    // For given target find the source which change in last step
    map<int, vector<int>> lasetChangedLinks;


    vector<DSet> secondWaitSet( vertex_num );

    while ( !lastChangedLocs.empty() ) {
      lasetChangedLinks.clear();
      for ( size_t i = 0; i < lastChangedLocs.size(); i++ ) {
        int source = lastChangedLocs[ i ];
        int link, outDegree, target;
        outDegree = graph.getOutDegree( source );
        for ( int j = 0; j < outDegree; j++ ) {
          link = graph.getAdj( source, j );

          graph.findRhs( link, source, target );
          lasetChangedLinks[ target ].push_back( link );
        }
      }

      vector<int>         targets;
      vector<vector<int>> vecRelatedLinks;
      for ( map<int, vector<int>>::iterator it = lasetChangedLinks.begin();
            it != lasetChangedLinks.end(); it++ ) {
        targets.push_back( it->first );
        vecRelatedLinks.push_back( it->second );
      }
      
      /**
       * TODO:
       * parallel  section
       *
       */

      for ( size_t i = 0; i <  targets.size(); i++ ) {

        int source, target, link;
        target = targets[ i ];
        source = 0;
        for ( vector<int>::iterator lit = vecRelatedLinks[ i ].begin();
              lit != vecRelatedLinks[ i ].end(); lit++ ) {
          link = *lit;
          graph.findRhs( link, target, source );

          DSet discreteTransNext;
          if ( transitions[ link ]( dbmManager, waitSet[ source ],
                                        discreteTransNext ) ) {
            DSet advanceNext;
            graph.findRhs( link, source, target );
            if (locations[ target ]( dbmManager, discreteTransNext, advanceNext ) ) {
              vector<C *> vec;
              advanceNext.toVector( vec );
              for ( size_t k = 0; k < vec.size(); k++ ) {
                vector<C *> normVecDBM;
                dbmManager.norm( vec[ k ], clockUppuerBound, differenceCons, normVecDBM );

                for ( size_t h = 0; h < normVecDBM.size(); h++ ) {
                  
                  if ( reachSet[ target ].add( dbmManager, normVecDBM[ h ] ) ) {
                    if ( loc == target ) {
                      if ( isReachable( cons, normVecDBM[ h ] ) ) {
                        return true;
                      }
                    }
                    secondWaitSet[ target ].add( dbmManager, normVecDBM[ h ] );
                  }
                }
              }
              secondChanged.insert( target );
            }
          }
        }
      }

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
    
    return false;
  }

  void deleteData() {
    for ( typename vector<DBMset<C, DBM_t>>::iterator it = reachSet.begin();
          it != reachSet.end(); it++ ) {
      it->deleteAll();
    }
    reachSet.clear();
  }
};
} // namespace ftma

#endif
