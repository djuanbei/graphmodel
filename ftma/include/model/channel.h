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

namespace graphmodel {

class Channel {
 public:
  Channel() {}
  Channel(const Argument& arg)
      : chan_id(arg), type(ONE2ONE_CH), action(CHANNEL_SEND) {}
  Channel(const Argument& arg, const CHANNEL_TYPE& t)
      : chan_id(arg), type(t), action(CHANNEL_SEND) {}

  Channel(const Argument& arg, const CHANNEL_ACTION& a)
      : chan_id(arg), type(ONE2ONE_CH), action(a) {
    if (action == CHANNEL_SEND) {
      ch_sigin = 1;
    } else {
      ch_sigin = -1;
    }
  }

  Channel(const Argument& arg, const CHANNEL_TYPE& t, const CHANNEL_ACTION& a)
      : chan_id(arg), type(t), action(a) {
    if (action == CHANNEL_SEND) {
      ch_sigin = 1;
    } else {
      ch_sigin = -1;
    }
  }

  int operator()(int* counter_value) const {
    return real_chan_id.getValue(counter_value);
  }
  int getGlobalId(int* counter_value) const { return (*this)(counter_value); }
  int getSiginGlobalId(int* counter_value) const {
    return ch_sigin * (*this)(counter_value);
  }
  void setAction(const CHANNEL_ACTION& a) {
    action = a;
    if (action == CHANNEL_SEND) {
      ch_sigin = 1;
    } else {
      ch_sigin = -1;
    }
  }

  void to_real(const TOReal* convertor) {
    real_chan_id = convertor->to_real(CHAN_T, chan_id);
    real_chan_id.setType(CONST_ARG);
    type = convertor->getChanType(chan_id.getName());
  }
  void setSelectValue(const int select_value) {
    real_chan_id.setSelectValue(select_value);
  }
  CHANNEL_TYPE getType(void) const { return type; }

  bool isSend() const { return CHANNEL_SEND == action; }
  bool isRecive() const { return CHANNEL_RECEIVE == action; }

 private:
  Argument chan_id{0};
  CHANNEL_TYPE type{ONE2ONE_CH};
  CHANNEL_ACTION action{CHANNEL_SEND};
  RealArgument real_chan_id;
  int ch_sigin{1};
};

}  // namespace graphmodel

#endif
