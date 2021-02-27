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
bool vect_cmp(const std::vector<T>& lhs, const std::vector<T>& rhs) {
  for (size_t i = 0; i < lhs.size(); i++) {
    if (lhs[i] < rhs[i]) {
      return true;
    } else if (lhs[i] > rhs[i]) {
      return false;
    }
  }
  return false;
}
template <typename T>
int findIndex(const std::vector<T>& values, const T& v) {
  typename std::vector<T>::const_iterator it =
      find(values.begin(), values.end(), v);
  if (it == values.end()) {
    return -1;
  }
  return it - values.begin();

}  // namespace graphsat

}  // namespace graphsat
#endif
