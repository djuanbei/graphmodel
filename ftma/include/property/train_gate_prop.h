/**
 * @file   train_gate_prop.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Dec 16 19:12:39 2019
 *
 * @brief  train gate example property. There is at most one train enetr Cross
 * location at one time.
 *
 *
 */
#ifndef TRAIN_GATE_PROPERTY_H
#define TRAIN_GATE_PROPERTY_H
#include "property/critical_section.h"

namespace graphmodel {
class TrainGatePro : public CriticalSection {
 public:
  TrainGatePro(const int n) : train_num(n) {}

 protected:
  virtual bool isLookComponent(const int component) const {
    return component < train_num;
  }

 private:
  int train_num;
};

}  // namespace graphmodel

#endif
