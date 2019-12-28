/**
 * @file   util.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec 27 17:37:51 2019
 *
 * @brief  algorithm set
 *
 *
 */

#ifndef ALG_UTIL_H
#define ALG_UTIL_H

namespace graphsat {

template <typename T>
bool vect_cmp(const vector<T> &lhs, const vector<T> &rhs) {
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

#endif
