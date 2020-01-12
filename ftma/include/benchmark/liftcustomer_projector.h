/**
 * @file   liftcustomer_projector.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Jan 12 09:06:24 2020
 *
 * @brief  for the paper  "Safety Verification of Parametrized Systems via
 * Incremental Methods"
 *
 *
 */

#ifndef LIFT_CUSTOMER_PROJECTOR_H
#define LIFT_CUSTOMER_PROJECTOR_H

#include <set>

#include "graph/graph.hpp"

#include "alg/projector.h"

#include "state/ta_statemanager.h"

namespace graphsat {

using namespace raptor;

class LiftCustomerProjector : public Projector {
 public:
  LiftCustomerProjector(const std::shared_ptr<TMStateManager>& manager,
                        const int pro_d);

  virtual void operator()(const int* original_state,
                          std::vector<int>& proj) const;

  std::vector<int> to_vec(const TMStateManager* manager,
                          const int* state) const;

  bool contain(const std::vector<int>& one,
               const std::vector<std::vector<int>>& rhs) const;

  bool include(const std::vector<std::vector<int>>& lhs,
               const std::vector<std::vector<int>>& rhs) const;

  virtual bool projectEqualReach(
      const std::vector<std::vector<int>>& pre_projs,
      const ReachableSet<TMStateManager>& reach_set) const;

  virtual std::ostream& dump(const std::vector<int>& proj_e,
                             std::ostream& out) const {
    out << proj_e[0] << endl;
    return out;
  }

 private:
  const std::shared_ptr<TMStateManager>& manager;
  int component_num;

  void constructState(int* state, const std::vector<std::vector<int>>& projs,
                      const std::vector<std::vector<int>>& oneStataes,
                      const std::vector<int>& vertices,
                      const std::vector<int>& choose,
                      const std::vector<std::pair<int, int>>& link_src_snk_map,
                      const std::map<int, int>& link_map) const;
};

}  // namespace graphsat

#endif
