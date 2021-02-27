/**
 * @file   train_gate_controll.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Dec 30 13:10:28 2019
 *
 * @brief  Railroad corssing
 * ref in <<Principles in model checking>> page. 51
 *
 *
 */

#ifndef TRAIN_GATE_CONTROLL_H
#define TRAIN_GATE_CONTROLL_H
#include "model/function.h"
#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"

namespace graphmodel {
typedef AgentSystem<Location, Transition> INT_TAS_t;

class TrainGateControl {
 public:
  TrainGateControl();
  INT_TAS_t generate(int n) const;

 private:
};

}  // namespace graphmodel

#endif
