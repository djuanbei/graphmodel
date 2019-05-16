
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

#include "parallel.h"
#include <map>
#include <model/ta.hpp>
#include <set>
#include <vector>

namespace graphsat {

using namespace std;

template <typename ReachableSet> class Reachability {

private:
  ReachableSet &data;
  const TAS_t & sys;
  int           component_num;

public:
  Reachability( ReachableSet &outData )
      : data( outData )
      , sys( data.getSYS() ) {
    component_num = sys.getComponentNum();
  }

  ~Reachability() {}

  void computeAllReachableSet() {

    vector<int>          loc( component_num, -1 );
    vector<vector<CS_t>> cons( component_num );

    run( loc, cons );
  }

  bool reachable( const vector<int> loc ) {
    vector<vector<CS_t>> cons( component_num );
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
  bool run( const vector<int> &loc, const vector<vector<CS_t>> &cons ) {
    // assert( loc.size( )==component_num && cons.size( )== component_num);

    Check_State re = data.find( loc, cons );

    if ( re != UNKOWN ) {
      if ( TRUE == re ) {
        return true;
      } else if ( FALSE == re ) {
        return false;
      }
    }

    // For given target find the source which change in last step

    StateSet_t secondWaitSet;
    bool       find = false;
    while ( !data.waitSet.empty() ) {

      /**
       * TODO:
       * parallel  section
       *
       */
      find = false;

      typename StateSet_t::iterator end1 = data.waitSet.end();

      for ( typename StateSet_t::iterator it = data.waitSet.begin();
            !find && ( it != end1 ); ++it ) {
        
        if ( data.oneStep( loc, cons, *it, secondWaitSet ) ) {
          find = true;
        }
      }
      data.update( secondWaitSet );
      if ( find ) {
        return true;
      }
    }
    return false;
  }
};
} // namespace graphsat

#endif
