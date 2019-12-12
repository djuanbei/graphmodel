/**
 * @file   property.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon May 20 20:53:42 2019
 *
 * @brief   LTL ( Linear temper logical)
 *
 *
 */
#ifndef _PROPERTY_H
#define _PROPERTY_H
#include <vector>

namespace graphsat {
using std::vector;

class Property {
public:
  virtual bool operator()(const void *manager, const int *const state) const {
    return false;
  }
};

} // namespace graphsat

#endif
