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

#include "model/function.h"
#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"

#include "property/train_gate_prop.h"

namespace graphsat {
typedef AgentSystem<Location, Transition> INT_TAS_t;
class TrainGate {
public:
  TrainGate() {}
  INT_TAS_t generate(int n) const;
};

class Enqueue_F : public Function {
public:
  virtual int operator()(int *state...);
  virtual Function *copy() const { return new Enqueue_F(); }
};

class Dequeue_F : public Function {
public:
  virtual int operator()(int *state...);
  virtual Function *copy() const { return new Dequeue_F(); }
};

class Front_F : public Function {
public:
  virtual int operator()(int *state...);

  virtual Function *copy() const { return new Front_F(); }
};

class Tail_F : public Function {
public:
  virtual int operator()(int *state...);

  virtual Function *copy() const { return new Tail_F(); }
};

} // namespace graphsat

#endif
