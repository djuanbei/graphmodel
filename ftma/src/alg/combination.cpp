#include "alg/combination.h"

namespace graphsat {
Combination_n_k::Combination_n_k(int nn, int kk) : n(nn), k(kk), data(kk) {
  first = data.begin();
  last = data.end();
  std::generate(first, last, uniqueNumber);
}

bool Combination_n_k::next() {
  if ((*first) == n - k + 1) {
    return false;
  }

  std::vector<int>::iterator mt = last;
  while (*(--mt) == n - (last - mt) + 1)
    ;
  (*mt)++;
  while (++mt != last) *mt = *(mt - 1) + 1;

  return true;
}

FullChoose::FullChoose(const std::vector<int>& f)
    : perChoose(f), data(f.size()) {
  len = 1;
  for (auto e : perChoose) {
    assert(e > 0);
    len *= e;
  }
  current = 0;
}
FullChoose::FullChoose(int out_len, int base)
    : perChoose(out_len, base), data(out_len) {
  len = 1;
  for (int i = 0; i < out_len; i++) {
    len *= base;
  }
  current = 0;
}
bool FullChoose::next() {
  if (current >= len) {
    return false;
  }
  size_t temp = current;
  for (size_t i = 0; i < data.size(); i++) {
    data[i] = temp % perChoose[i];
    temp /= perChoose[i];
  }
  current++;
  return true;
}

}  // namespace graphsat
