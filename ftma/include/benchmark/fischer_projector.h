/**
 * @file   fisher_projector.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Dec 16 19:47:22 2019
 *
 * @brief  projection fisher reachable set
 *
 *
 */
#ifndef FISCHER_PROJECTOR_H
#define FISCHER_PROJECTOR_H
#include <set>

#include "alg/projector.h"
#include "graph/graph.hpp"
#include "state/ta_statemanager.h"

namespace graphmodel {

using namespace raptor;

class FischerProjector : public Projector {
 public:
  FischerProjector(const std::shared_ptr<TMStateManager>& manager,
                   const int pro_d);

  // std::vector<int> operator()(const int* original_state) const override
  // final;

  std::vector<int> to_vec(const TMStateManager* manager,
                          const int* state) const override final;
  bool contain(const std::vector<int>& one,
               const std::vector<std::vector<int>>& rhs) const override final;

  std::vector<int> getSrc(const std::vector<int>& proj) const override final;

  std::vector<int> getSnk(const std::vector<int>& proj) const override final;

  bool constructState(TMStateManager* manager, int* state,
                      const std::vector<std::vector<int>>& projs,
                      const std::vector<std::vector<int>>& vertices,
                      const std::vector<int>& choose,
                      const std::vector<std::pair<int, int>>& link_src_snk_map
                      // const std::map<int, int>& link_map
  ) const override final;

  std::ostream& dump(const std::vector<int>& proj_e,
                     std::ostream& out) const override final {
    return out;
  }

  struct BetaElement {
    int A_loc;
    int B_loc;
    int A_has_id;  // 1 -> true, 0 -> false
    int B_has_id;  // 1 -> true, 0 -> false
    int clock_dbm[9];
    friend bool operator<(const BetaElement& lhs, const BetaElement& rhs);
  };

  struct AbsOneDimState {
    int loc;
    int has_id;  // 1 -> true, 0 -> false

    int clock_lower_bound;
    int clock_upper_bound;
    friend bool operator<(const AbsOneDimState& lhs, const AbsOneDimState& rhs);
  };

  BetaElement beta(const std::vector<int>& one) const;

 private:
  // const std::shared_ptr<TMStateManager>& manager;
  int component_num;
  int pro_dim;
  int clock_start;
  std::set<BetaElement> projb;
};

}  // namespace graphmodel

#endif
