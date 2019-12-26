/**
 * @file   symmetry.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Dec 26 15:05:11 2019
 *
 * @brief  state symmetry  property
 *
 *
 */

#ifndef _SYMMETRY_H
#define _SYMMETRY_H
#include <algorithm>
#include <vector>

namespace graphsat {
using namespace std;
class Symmetry {
public:
  Symmetry(int n) : len(n), swap(n) {
    for (int i = 0; i < n; i++) {
      swap[i] = i;
    }
    random_shuffle(swap.begin(), swap.end());
  }
  bool next(vector<int> &row);

private:
  int len;
  vector<int> swap;
};

} // namespace graphsat

#endif
