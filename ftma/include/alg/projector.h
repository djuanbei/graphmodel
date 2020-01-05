/**
 * @file   projector.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Dec 16 19:33:24 2019
 *
 * @brief  state projector
 *
 *
 */

#ifndef PROJECTOR_H
#define PROJECTOR_H
#include <iostream>
#include <vector>

#include "state/reachableset.hpp"
#include "state/ta_statemanager.h"

namespace graphsat {

class Projector {
 public:
  virtual void operator()(const int* original_state,
                          std::vector<int>& proj) const = 0;
  virtual bool include(const std::vector<std::vector<int>>& lhs,
                       const std::vector<std::vector<int>>& rhs) const = 0;

  virtual bool projectEqualReach(
      const std::vector<std::vector<int>>& projs,
      const ReachableSet<TMStateManager>& next_reach_set) const = 0;

  virtual std::ostream& dump(const std::vector<int>& proj_e,
                             std::ostream& out) const = 0;

  virtual std::ostream& dump(const std::vector<std::vector<int>>& proj,
                             std::ostream& out) const {
    for (auto& e : proj) {
      dump(e, out);
    }
    return out;
  }

  void dump_D(const std::vector<int>& proj_e) const { dump(proj_e, std::cout); }

  void dump_D(const std::vector<std::vector<int>>& proj) const {
    dump(proj, std::cout);
  }
};

}  // namespace graphsat

#endif
