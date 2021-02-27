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

#include "alg/projector.h"
#include "graph/graph.hpp"
#include "state/ta_statemanager.h"

namespace graphmodel {

using namespace raptor;

class LiftCustomerProjector : public Projector {
 public:
  LiftCustomerProjector(const std::shared_ptr<TMStateManager>& manager,
                        const int pro_d);

  // std::vector<int> operator()(const int* original_state
  //                         ) const override final;

  std::vector<int> to_vec(const TMStateManager* manager,
                          const int* state) const override final;

  std::vector<int> getSrc(const std::vector<int>& proj) const override final;

  std::vector<int> getSnk(const std::vector<int>& proj) const override final;

  bool contain(const std::vector<int>& one,
               const std::vector<std::vector<int>>& rhs) const override final;

  bool constructState(TMStateManager* manager, int* state,
                      const std::vector<std::vector<int>>& projs,
                      const std::vector<std::vector<int>>& vertices,
                      const std::vector<int>& choose,
                      const std::vector<std::pair<int, int>>& link_src_snk_map
                      // const std::map<int, int>& link_map
  ) const override final;

  std::ostream& dump(const std::vector<int>& proj_e,
                     std::ostream& out) const override final {
    out << proj_e[0] << endl;
    return out;
  }

 private:
  int component_num;
};

}  // namespace graphmodel

#endif
