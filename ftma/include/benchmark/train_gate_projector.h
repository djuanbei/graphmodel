/**
 * @file   train_gate_projector.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Dec 16 20:15:30 2019
 *
 * @brief  projection for train-gate example
 *
 *
 */
#ifndef TRAIN_GATE_PROJECTIOR_H
#define TRAIN_GATE_PROJECTIOR_H
#include "alg/projector.h"
#include "state/ta_statemanager.h"

namespace graphsat {

class TrainGateProjector : public Projector {
 public:
  TrainGateProjector(const std::shared_ptr<TMStateManager>& out_manager,
                     const int pro_d);

  virtual void operator()(const int* original_state, vector<int>& proj) const;

  bool include(const std::vector<std::vector<int>>& lhs,
               const std::vector<std::vector<int>>& rhs) const;

  virtual bool projectEqualReach(
      const std::vector<std::vector<int>>& projs,
      const ReachableSet<TMStateManager>& next_reach_set) const {
    return false;
  }

  virtual std::ostream& dump(const std::vector<int>& proj_e,
                             std::ostream& out) const;

 private:
  const std::shared_ptr<TMStateManager> manager;
  int component_num;
  int pro_dim;
  int clock_start;
  int pro_clock_start;

  enum Order { UNKOWN = 0, EQUAL = 1, FRONTER = 2, LATER = 3 };
};

}  // namespace graphsat
#endif
