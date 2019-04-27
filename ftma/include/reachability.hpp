
/**
 * @file   reachability.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Apr 27 08:49:40 2019
 *
 * @brief  reachability problem of timed automata
 *
 *
 */

#ifndef __REACHABILITY_HPP
#define __REACHABILITY_HPP
#include "dbm.hpp"
#include "dbmset.hpp"
#include "graph/graph.hpp"
#include "reachableset.hpp"
#include "ta.hpp"
#include <vector>

namespace ftma {

using namespace std;
using namespace raptor;

template <typename C, typename L, typename T> class Reachability {

private:
  typedef Constraint<C>    CS_t;
  typedef DBM<C, CS_t>     DBM_t;
  typedef DBMset<C, DBM_t> DSet_t;

  int initial_loc; // the initial location of timed automata
  int vertex_num;  // number of locations in

  ReachableSet<C, L, T> &data;
  const TA<C, L, T> &    ta;

  DBM_t dbmManager;

  friend TA<C, L, T>; // the algorithm can direct get data
  friend ReachableSet<C, L, T>;

public:
  Reachability( ReachableSet<C, L, T> &odata )
      : data( odata )
      , ta( data.getTA() )
      , dbmManager( ta.getClockNum() ) {

    initial_loc = ta.getInitialLoc();
  }

  ~Reachability() {}

  void computeAllReachableSet() {
    vector<CS_t> cons;
    run( -1, cons );
  }

  bool reachable( const int loc ) {
    vector<CS_t> cons;
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
  bool run( const int loc, const vector<CS_t> &cons ) {

    Check_State re = data.find( loc, cons );

    if ( re != UNKOWN ) {
      if ( TRUE == re ) {
        return true;
      } else if ( FALSE == re ) {
        return false;
      }
    }

    const vector<T> &transitions = ta.getTransitions();
    const vector<L> &locations   = ta.getLocations();

    set<int> secondChanged;

    // For given target find the source which change in last step
    map<int, vector<int>> lasetChangedLinks;

    vector<DSet_t> secondWaitSet( vertex_num );

    while ( !data.lastChangedLocs.empty() ) {
      lasetChangedLinks.clear();
      for ( size_t i = 0; i < data.lastChangedLocs.size(); i++ ) {
        int source = data.lastChangedLocs[ i ];
        int link, outDegree, target;
        outDegree = ta.graph.getOutDegree( source );
        for ( int j = 0; j < outDegree; j++ ) {
          link = ta.graph.getAdj( source, j );

          ta.graph.findRhs( link, source, target );
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

      for ( size_t i = 0; i < targets.size(); i++ ) {

        int source, target, link;
        target = targets[ i ];
        source = 0;
        for ( vector<int>::iterator lit = vecRelatedLinks[ i ].begin();
              lit != vecRelatedLinks[ i ].end(); lit++ ) {
          link = *lit;
          ta.graph.findRhs( link, target, source );

          DSet_t discreteTransNext;
          if ( transitions[ link ]( dbmManager, data.waitSet[ source ],
                                    discreteTransNext ) ) {
            DSet_t advanceNext;
            ta.graph.findRhs( link, source, target );
            if ( locations[ target ]( dbmManager, discreteTransNext,
                                      advanceNext ) ) {
              vector<C *> vec;
              advanceNext.toVector( vec );
              for ( size_t k = 0; k < vec.size(); k++ ) {
                vector<C *> normVecDBM;
                dbmManager.norm( vec[ k ], ta.clockUppuerBound,
                                 ta.differenceCons, normVecDBM );

                for ( size_t h = 0; h < normVecDBM.size(); h++ ) {

                  if ( data.reachSet[ target ].add( dbmManager,
                                                    normVecDBM[ h ] ) ) {
                    if ( loc == target ) {
                      if ( data.isReachable( cons, normVecDBM[ h ] ) ) {
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

      data.lastChangedLocs.clear();
      data.lastChangedLocs.insert( data.lastChangedLocs.begin(),
                                   secondChanged.begin(), secondChanged.end() );
      secondChanged.clear();

      data.waitSet = secondWaitSet;
      for ( size_t i = 0; i < data.lastChangedLocs.size(); i++ ) {
        int source = data.lastChangedLocs[ i ];
        secondWaitSet[ source ].clear();
      }
    }

    return false;
  }
};
} // namespace ftma

#endif
