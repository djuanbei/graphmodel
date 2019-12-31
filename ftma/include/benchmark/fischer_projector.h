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
#include "alg/projector.h"

#include "state/ta_statemanager.h"

namespace graphsat {

class FischerProjector : public Projector {
 public:
  FischerProjector(const shared_ptr<TMStateManager>& manager, const int pro_d);

  virtual void operator()(const int* original_state, vector<int>& proj) const;

  bool include(const vector<vector<int>>& lhs,
               const vector<vector<int>>& rhs) const;

  virtual ostream& dump(const vector<int>& proj_e, ostream& out) const {
    return out;
  }

 private:
  int component_num;
  int pro_dim;
  int clock_start;
};

}  // namespace graphsat

#endif
