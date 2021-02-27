/**
 * @file   locreachprop.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Dec  5 09:40:36 2019
 *
 * @brief  loc reach properpty
 *
 *
 */

#ifndef LOC_REACH_PROP_H
#define LOC_REACH_PROP_H
#include "property/property.h"

namespace graphmodel {

class LocReachProperty : public Property {
 public:
  LocReachProperty(vector<int>& ls) { loc = ls; }
  virtual bool operator()(const void* manager, const int* const state) const {
    return (0 == memcmp(state, &loc[0],
                        ((ComponentInfo*)manager)->getComponentNumber() *
                            sizeof(int)));
  }

 private:
  vector<int> loc;
};
}  // namespace graphmodel

#endif
