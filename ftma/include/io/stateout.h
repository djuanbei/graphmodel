/**
 * @file   stateout.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec 27 17:18:57 2019
 *
 * @brief  reachable set output
 *
 *
 */
#ifndef STATE_OUTPUT_H
#define STATE_OUTPUT_H

#include "state/reachableset.hpp"
#include "state/ta_statemanager.h"
namespace graphsat {

class StateOutput {
 public:
  static void generatorDot(const ReachableSet<TMStateManager>& reachSet,
                           const string& filename);
  /**
   * @brief Generate the generate trace from initial state to target
   */
  static bool generatePath(const ReachableSet<TMStateManager>& reachSet,
                           const string& filename, const int* const target);
};

}  // namespace graphsat

#endif
