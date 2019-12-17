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
  TrainGateProjector(const shared_ptr<TMStateManager> &out_manager,
                     const int pro_d);

  virtual void operator()(const int *original_state, vector<int> &proj) const;

  bool include(const vector<vector<int>> &lhs,
               const vector<vector<int>> &rhs) const;

  void setLocName(const int id, const string &n) { loc_name_map[id] = n; }

  virtual ostream &dump(const vector<int> &proj_e, ostream &out) const ;
  

  virtual ostream &dump(const vector<vector<int>> &proj, ostream &out) const {
    for (auto &e : proj) {
      dump(e, out);
    }
    return out;
  }

private:
  const shared_ptr<TMStateManager> manager;
  int component_num;
  int pro_dim;
  int clock_start;
  int pro_clock_start;
  map<int, string> loc_name_map;

  enum Order { UNKOWN = 0, EQUAL = 1, FRONTER = 2, LATER = 3 };
};

} // namespace graphsat
#endif
