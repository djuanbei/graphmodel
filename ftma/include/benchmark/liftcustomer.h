/**
 * @file   liftcustomer.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Dec 30 19:27:11 2019
 *
 * @brief  for the paper  "Safety Verification of Parametrized Systems via
 * Incremental Methods"
 *
 *
 */
#ifndef LIFT_CUSTOMER_H
#define LIFT_CUSTOMER_H

#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"

#include "model/system_generator.h"

namespace graphsat {

typedef AgentSystem<Location, Transition> INT_TAS_t;

class LiftCustomer : public SystemGenerator {
 public:
  INT_TAS_t generate(int n) const;
};
}  // namespace graphsat

#endif
