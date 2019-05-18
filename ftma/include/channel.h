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

enum CHANNEL_TYPE { ONE2ONE, ONE2ALL };

enum CHANNEL_ACTION { CHANNEL_SEND, CHANNEL_RECEIVE };

struct Channel {
  int            id; // start from 1
  CHANNEL_TYPE   type;
  CHANNEL_ACTION action;
  Channel()
      : id( -1 )
      , type( ONE2ONE ) {}
};

} // namespace graphsat

#endif
