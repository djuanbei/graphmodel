#include "problem/pmcp.hpp"

namespace graphsat {

bool element_cmp(const vector<int> &lhs, const vector<int> &rhs) {
  for (size_t i = 0; i < lhs.size(); i++) {
    if (lhs[i] < rhs[i]) {
      return true;
    } else if (lhs[i] > rhs[i]) {
      return false;
    }
  }
  return false;
}
} // namespace graphsat
