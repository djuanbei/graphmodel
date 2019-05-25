
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

#include "model/ta.hpp"
#include "property/property.h"
#include "util/parallel.h"

namespace graphsat {

using std::map;
using std::set;
using std::vector;

template <typename ReachableSet> class Reachability {

public:
  Reachability( ReachableSet &outData )
      : data( outData )
      , sys( data.getSYS() ) {
    component_num = sys.getComponentNum();
  }

  ~Reachability() {}

  void computeAllReachableSet() {
    Property prop;

    run( &prop );
  }

  /**
   * @brief whether the sysyem satisfies the prop
   *
   * @param prop
   *
   * @return
   */
  bool satisfy( const Property *prop ) { return run( prop ); }

  /**
   * @param L
   * @param TA
   * @param C
   * @param reachSet
   *
   * @return
   */
  bool run( const Property *prop ) {

    Check_State re = data.search( prop );

    if ( re != UNKOWN ) {
      if ( TRUE == re ) {
        return true;
      } else if ( FALSE == re ) {
        return false;
      }
    }

    // For given target find the source which change in last step


    while ( !data.waitSet.empty() ) {
      const State_t *state = data.waitSet.front();
      data.waitSet.pop_front();
      if ( data.oneStep( prop, state ) ) {
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
