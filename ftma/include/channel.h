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
#include <map>
namespace graphsat {

enum CHANNEL_TYPE { ONE2ONE_CH, BROADCAST_CH, URGENT_CH };

enum CHANNEL_ACTION { CHANNEL_SEND, CHANNEL_RECEIVE };

const int NO_CHANNEL = 0;

struct Channel {
  int            local_id;
  int            gloabl_id; // start from 1
  CHANNEL_TYPE   type;
  bool           is_urgent;
  CHANNEL_ACTION action;

  Channel()
      : local_id( -1 )
      , gloabl_id( -1 )
      , type( ONE2ONE_CH )
      , is_urgent( false ) {}
  void globalIpUpdate( const std::map<int, int> &id_map ) {

    gloabl_id = id_map.at( local_id );
  }
};

} // namespace graphsat

#endif
