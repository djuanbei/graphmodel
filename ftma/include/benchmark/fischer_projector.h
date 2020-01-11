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

#include "graph/graph.hpp"

#include "alg/projector.h"

#include "state/ta_statemanager.h"

namespace graphsat {

using namespace raptor;

class FischerProjector : public Projector {
 public:
  FischerProjector(const std::shared_ptr<TMStateManager>& manager,
                   const int pro_d);

  virtual void operator()(const int* original_state,
                          std::vector<int>& proj) const;

  std::vector<int> to_vec(const  TMStateManager * manager, const int * state) const;
  bool contain( const std::vector<int> & one, const std::vector<std::vector<int>>& rhs ) const;

  bool include(const std::vector<std::vector<int>>& lhs,
               const std::vector<std::vector<int>>& rhs) const;

  virtual bool projectEqualReach(
      const std::vector<std::vector<int>>& pre_projs,
      const ReachableSet<TMStateManager>& reach_set) const;

  virtual std::ostream& dump(const std::vector<int>& proj_e,
                             std::ostream& out) const {
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
  const std::shared_ptr<TMStateManager>& manager;
  int component_num;
  int pro_dim;
  int clock_start;
  std::set<BetaElement> projb;
  void constructState(int* state, const std::vector<std::vector<int>>& projs,
                      const std::vector<AbsOneDimState>& oneStataes,
                      const std::vector<int>& vertices,
                      const std::vector<int>& choose,
                      const std::vector<std::pair<int, int>>& link_src_snk_map,
                      const std::map<int, int>& link_map) const;

  // void constructState(int* state, const std::vector<std::vector<int>>& projs,
  //                     const std::vector<AbsOneDimState>& oneStataes,
  //                     const std::vector<int>& vertices,
  //                     const std::vector<int>& choose,
  //                     const std::map<int, int>& link_map) const;
};

}  // namespace graphsat

#endif
