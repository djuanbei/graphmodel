/**
 * @file   fisher.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Wed Dec  4 16:10:23 2019
 *
 * @brief  Fischer's mutual  exclusion protocol
 *
 *
 */

#ifndef FISGER_H
#define FISGER_H
#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"

namespace graphsat {
typedef AgentSystem<TMStateManager, Location, Transition> INT_TAS_t;

class FisherGenerator {
public:
  FisherGenerator();

  INT_TAS_t generate( int n ) const;

private:
  size_t                              k = 0;
  typename INT_TAS_t::AgentTemplate_t tmt;
  INT_TAS_t sys;
};

} // namespace graphsat

#endif
