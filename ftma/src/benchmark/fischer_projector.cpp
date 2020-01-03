#include "benchmark/fischer_projector.h"

namespace graphsat {

FischerProjector::FischerProjector(
    const std::shared_ptr<TMStateManager>& out_manager, const int pro_d)
    : manager(out_manager) {
  component_num = manager->getComponentNumber();
  pro_dim = pro_d;
  clock_start = manager->getClockStart();
}

void FischerProjector::operator()(const int* original_state,
                                  vector<int>& proj) const {
  for (int i = 0; i < pro_dim; i++) {
    proj.push_back(original_state[i]);
  }
  int id = manager->getValue(0, original_state, "id");
  for (int i = 0; i < pro_dim; i++) {
    proj.push_back(id == i + 1);
  }
  const int* dbm = manager->getDBM(original_state);
  for (int i = 0; i <= pro_dim; i++) {
    for (int j = 0; j <= pro_dim; j++) {
      proj.push_back(manager->getClockManager().at(dbm, i, j));
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
      if (0 ==
          memcmp(&(lhs[i][0]), &(rhs[j][0]), (pro_dim + 1) * sizeof(int))) {
        k = 2 * pro_dim;
      }

      if (k == (size_t)(2 * pro_dim)) {
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
