
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

#include <map>
#include <set>
#include <vector>

#include "parallel.h"

namespace graphsat {

using namespace std;

template <typename ReachableSet> class Reachability {

private:
  int vertex_num; // number of locations in

  ReachableSet &                        data;
  const typename ReachableSet::Model_t &ta;

public:
  Reachability( ReachableSet &outData )
      : data( outData )
      , ta( data.getTA() ) {

    vertex_num = ta.getLocationNum();
  }

  ~Reachability() {}

  void computeAllReachableSet() {
    vector<typename ReachableSet::CS_t> cons;
    run( -1, cons );
  }

  bool reachable( const int loc ) {
    vector<typename ReachableSet::CS_t> cons;
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
  bool run( const int loc, const vector<typename ReachableSet::CS_t> &cons ) {

    Check_State re = data.find( loc, cons );

    if ( re != UNKOWN ) {
      if ( TRUE == re ) {
        return true;
      } else if ( FALSE == re ) {
        return false;
      }
    }

    set<int> secondChanged;

    // For given target find the source which change in last step
    map<int, vector<int>> lasetChangedLinks;

    vector<typename ReachableSet::DSet_t> secondWaitSet( vertex_num );

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
      int  target_size = (int) targets.size();
      bool find        = false;
      parallel_for( int i = 0; !find && i < target_size; i++ ) {
        int       link;
        const int target = targets[ i ]; // fixed target
        for ( vector<int>::iterator lit = vecRelatedLinks[ i ].begin();
              lit != vecRelatedLinks[ i ].end(); lit++ ) {
          link = *lit;
          // one edge transition (source --> target)
          if ( data.oneStep( loc, cons, target, link, secondChanged,
                             secondWaitSet ) ) { 
            find = true;
          }
        }
      }
      if ( find ) {
        return true;
      }
      data.update( secondChanged, secondWaitSet );
    }
    return false;
  }
};
} // namespace graphsat

#endif
