/**
 * @file   fisher_projector.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Dec 16 19:47:22 2019
 *
 * @brief  projection fisher reachable set
 *
 *
 */
#ifndef FISHER_PROJECTOR_H
#define FISHER_PROJECTOR_H
#include "alg/projector.h"

namespace graphsat {
template <typename StateManager_t>
class FisherProjector : public Projector<StateManager_t> {
public:
  FisherProjector(const shared_ptr<StateManager_t> &manager, const int pro_d) {
    component_num = manager->getComponentNum();
    pro_dim = pro_d;
    clock_start = manager->getClockStart();
  }

  virtual void operator()(const int *original_state, vector<int> &proj) const {
    for (int i = 0; i < pro_dim; i++) {
      proj.push_back(original_state[i]);
    }
    for (int i = 0; i <= pro_dim; i++) {
      for (int j = 0; j <= pro_dim; j++) {
        proj.push_back(
            original_state[i * (component_num + 1) + j + clock_start]);
      }
    }
  }

  bool include(const vector<vector<int>> &lhs,
               const vector<vector<int>> &rhs) const {
    if (lhs.empty()) {
      return rhs.empty();
    }
    if (rhs.empty()) {
      return false;
    }
    assert(lhs[0].size() == rhs[0].size());

    size_t n = lhs[0].size();

    for (size_t i = 0; i < lhs.size(); i++) {
      size_t j = 0;
      for (; j < rhs.size(); j++) {
        size_t k = 0;
        for (; k < pro_dim; k++) {
          if (lhs[i][k] != rhs[j][k]) {
            break;
          }
        }

        if (k == pro_dim) {
          for (; k < n; k++) {
            if (lhs[i][k] > rhs[j][k]) {
              break;
            }
          }
        }
        if (k == n) {
          break;
        }
      }
      if (j == rhs.size()) {
        return false;
      }
    }
    return true;
  }

  virtual ostream &dump(const vector<int> &proj_e, ostream &out) const {
    return out;
  }

  virtual ostream &dump(const vector<vector<int>> &proj, ostream &out) const {
    return out;
  }

private:
  int component_num;
  int pro_dim;
  int clock_start;
};

} // namespace graphsat

#endif
