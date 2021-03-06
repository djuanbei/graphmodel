/**
 * @file   fisher.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Wed Dec  4 16:10:23 2019
 *
 * @brief  Fischer's mutual  exclusion protocol
 *
 *
 */

#ifndef FISCHGER_H
#define FISCHGER_H
#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/system_generator.h"
#include "model/transition.h"

namespace graphmodel {
typedef AgentSystem<Location, Transition> INT_TAS_t;

class FischerGenerator : public SystemGenerator {
 public:
  FischerGenerator();

  INT_TAS_t generate(int n) const;

 private:
  size_t k = 0;

  INT_TAS_t sys;

  void initial(shared_ptr<typename INT_TAS_t::AgentTemplate_t>& tmt) const;
};

}  // namespace graphmodel

#endif
