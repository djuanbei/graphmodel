/**
 * @file   system_generator.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Jan 13 17:00:24 2020
 *
 * @brief  generatot system
 *
 *
 */

#ifndef SYSTEM_GENERATOR_H
#define SYSTEM_GENERATOR_H
#include "state/ta_statemanager.h"
namespace graphmodel {
typedef AgentSystem<Location, Transition> INT_TAS_t;

class SystemGenerator {
 public:
  virtual INT_TAS_t generate(int n) const = 0;
};
}  // namespace graphmodel

#endif
