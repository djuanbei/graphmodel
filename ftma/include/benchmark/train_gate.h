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

namespace graphsat{
typedef AgentSystem<TMStateManager, Location, Transition> INT_TAS_t;
class TrainGate{
  TrainGate( );
  
  INT_TAS_t generate( int n ) const;

 private:
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> train_tmt;
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> gate_tmt;
  INT_TAS_t sys;
  
  int n;//number of trains
  
};

void enqueue(typename INT_TAS_t::Agent_t  & agent,  int *state, const  int element);

void dequeue(typename INT_TAS_t::Agent_t  & agent,  int *state );

int front(typename INT_TAS_t::Agent_t  & agent,  int *state  );

int tail( typename INT_TAS_t::Agent_t  & agent,  int *state);


}

#endif
