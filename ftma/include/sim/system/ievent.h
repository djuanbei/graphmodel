//
// Created by yun on 2021/2/27.
//

#ifndef GMODEL_IEVENT_H
#define GMODEL_IEVENT_H
#include "sim/data/trace.h"
namespace graphmodel {
namespace sim {
class IEvent {
 public:
  IEvent(time_t time, Trace t) {
    createTime = this->time = time;
    state = t;
  }
  IEvent(time_t time, int priority, Trace t) {
    createTime = this->time = time;
    this->priority = priority;
    state = t;
  }

  time_t getTime() const { return time; }
  time_t getCreateTime() const { return createTime; }

  void setTime(time_t t) { time = t; }
  int getTryTime() const { return tryTime; }
  void incTryTime() { tryTime++; }

  int getPriority() const { return priority; }

 private:
  time_t time;
  int priority = 0;
  Trace state;
  int tryTime = 0;
  time_t createTime;
};

}  // namespace sim
}  // namespace graphmodel
#endif  // GMODEL_IEVENT_H
