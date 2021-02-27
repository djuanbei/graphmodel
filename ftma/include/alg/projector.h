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

#include "model/system_generator.h"
#include "state/reachableset.hpp"
#include "state/ta_statemanager.h"

namespace graphmodel {

class Projector {
 public:
  Projector(const std::shared_ptr<TMStateManager>& out_manager)
      : manager(out_manager) {}
  std::vector<int> operator()(const int* original_state) const {
    return to_vec(manager.get(), original_state);
  }

  virtual bool contain(const std::vector<int>& one,
                       const std::vector<std::vector<int>>& rhs) const = 0;

  virtual bool include(const std::vector<std::vector<int>>& lhs,
                       const std::vector<std::vector<int>>& rhs) const;

  virtual bool projectStableCheck(
      const std::vector<std::vector<int>>& two_dim_projs,
      const ReachableSet<TMStateManager>& reach_set,
      const SystemGenerator* generator) const;

  virtual bool constructState(
      TMStateManager* manager, int* state,
      const std::vector<std::vector<int>>& projs,
      const std::vector<std::vector<int>>& vertices,
      const std::vector<int>& choose,
      const std::vector<std::pair<int, int>>& link_src_snk_map
      // const std::map<int, int>& link_map
  ) const = 0;

  virtual std::vector<int> getSrc(const std::vector<int>& proj) const = 0;

  virtual std::vector<int> getSnk(const std::vector<int>& proj) const = 0;

  virtual std::vector<int> to_vec(const TMStateManager* manager,
                                  const int* state) const = 0;

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

 protected:
  const std::shared_ptr<TMStateManager> manager;
};

}  // namespace graphmodel

#endif
