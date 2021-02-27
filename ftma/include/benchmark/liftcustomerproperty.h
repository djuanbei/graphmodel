/**
 * @file   liftcustomerproperty.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Jan 12 10:11:56 2020
 *
 * @brief  lift customer property: there is no lift in overload state
 *
 *
 */

#ifndef LIFT_CUSTOMER_PROPERTTY_H
#define LIFT_CUSTOMER_PROPERTTY_H

#include <vector>

#include "property/property.h"

namespace graphmodel {

class LiftCustomerProperty : public Property {
 public:
  LiftCustomerProperty(vector<int>& ls) { loc = ls; }
  virtual bool operator()(const void* manager, const int* const state) const {
    return (0 == memcmp(state, &loc[0], loc.size() * sizeof(int)));
  }

 private:
  std::vector<int> loc;
};
}  // namespace graphmodel

#endif
