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
namespace graphsat {

enum CHANNEL_TYPE { ONE2ONE_CH, BROADCAST_CH };

enum CHANNEL_ACTION { CHANNEL_SEND, CHANNEL_RECEIVE };

const int NO_CHANNEL = 0;

struct Channel {
  int            id; // start from 1
  CHANNEL_TYPE   type;
  bool           isUrgent;
  CHANNEL_ACTION action;

  Channel()
      : id( -1 )
      , type( ONE2ONE_CH )
      , isUrgent( false ) {}
};

} // namespace graphsat

#endif
