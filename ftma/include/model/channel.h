/**
 * @file   channel.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Wed May 15 08:30:06 2019
 *
 * @brief
 *
 *
 */

#ifndef CHANNEL_H
#define CHANNEL_H
#include "to_real.h"
#include "util/dbmutil.hpp"

#include <map>
namespace graphsat {

class Channel {
 public:
  Channel() {}
  Channel(const Argument& arg)
      : chan_id(arg), type(ONE2ONE_CH), action(CHANNEL_SEND) {}
  Channel(const Argument& arg, const CHANNEL_TYPE& t)
      : chan_id(arg), type(t), action(CHANNEL_SEND) {}

  Channel(const Argument& arg, const CHANNEL_ACTION& a)
      : chan_id(arg), type(ONE2ONE_CH), action(a) {}

  Channel(const Argument& arg, const CHANNEL_TYPE& t, const CHANNEL_ACTION& a)
      : chan_id(arg), type(t), action(a) {}

  int operator()(int* counter_value) const {
    return getValue(real_chan_id, counter_value);
  }
  int getGlobalId(int* counter_value) const { return (*this)(counter_value); }
  void setAction(const CHANNEL_ACTION& a) { action = a; }

  void to_real(const TOReal* convertor) {
    real_chan_id = convertor->to_real(CHAN_T, chan_id);
    real_chan_id.type = CONST_ARG;
    type = convertor->getChanType(chan_id.name);
  }
  CHANNEL_TYPE getType(void) const { return type; }

  bool isSend() const { return CHANNEL_SEND == action; }
  bool isRecive() const { return CHANNEL_RECEIVE == action; }

 private:
  Argument chan_id;
  CHANNEL_TYPE type;
  CHANNEL_ACTION action;
  RealArgument real_chan_id;
};

}  // namespace graphsat

#endif
