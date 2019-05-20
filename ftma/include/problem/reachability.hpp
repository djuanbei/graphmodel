
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

#include "util/parallel.h"
#include <map>
#include <model/ta.hpp>
#include <set>
#include <vector>

namespace graphsat {

using namespace std;

template <typename ReachableSet> class Reachability {

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

  bool reach( const vector<int> loc ) {
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

    Check_State re = data.find( loc, cons );

    if ( re != UNKOWN ) {
      if ( TRUE == re ) {
        return true;
      } else if ( FALSE == re ) {
        return false;
      }
    }

    // For given target find the source which change in last step

    //  StateSet_t secondWaitSet;

    while ( !data.waitSet.empty() ) {
      const State_t *state = data.waitSet.front();
      data.waitSet.pop_front();
      if ( data.oneStep( loc, cons, state ) ) {
        return true;
      }
    }

    return false;
  }

private:
  ReachableSet &data;
  const TAS_t & sys;
  int           component_num;
};
} // namespace graphsat

#endif
