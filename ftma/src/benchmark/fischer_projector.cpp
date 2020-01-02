#include "benchmark/fischer_projector.h"

namespace graphsat {

FischerProjector::FischerProjector(
    const std::shared_ptr<TMStateManager>& manager, const int pro_d) {
  component_num = manager->getComponentNumber();
  pro_dim = pro_d;
  clock_start = manager->getClockStart();
}

void FischerProjector::operator()(const int* original_state,
                                  vector<int>& proj) const {
  for (int i = 0; i < pro_dim; i++) {
    proj.push_back(original_state[i]);
  }
  for (int i = 0; i <= pro_dim; i++) {
    for (int j = 0; j <= pro_dim; j++) {
      proj.push_back(original_state[i * (component_num + 1) + j + clock_start]);
    }
  }
}

bool FischerProjector::include(const vector<vector<int>>& lhs,
                               const vector<vector<int>>& rhs) const {
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
      for (; k < (size_t)pro_dim; k++) {
        if (lhs[i][k] != rhs[j][k]) {
          break;
        }
      }

      if (k == (size_t)pro_dim) {
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

}  // namespace graphsat
