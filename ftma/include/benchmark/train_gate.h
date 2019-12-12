/**
 * @file   train_gate.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec  6 19:40:44 2019
 *
 * @brief  train gate example
 *
 *
 */

#ifndef TRAIN_GATE_H
#define TRAIN_GATE_H

#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"

namespace graphsat {
typedef AgentSystem<Location, Transition> INT_TAS_t;
class TrainGate {
public:
  TrainGate() {}
  INT_TAS_t generate(int n) const;
};

void enqueue(typename INT_TAS_t::Agent_t &agent, int *state, const int element);

void dequeue(typename INT_TAS_t::Agent_t &agent, int *state);

int front(typename INT_TAS_t::Agent_t &agent, int *state);

int tail(typename INT_TAS_t::Agent_t &agent, int *state);

} // namespace graphsat

#endif
