/**
 * @file   channelaction.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Oct 20 21:26:43 2019
 *
 * @brief  channel action send and receive
 *
 *
 */

#ifndef CHANNEL_ACTION_H
#define CHANNEL_ACTION_H
namespace graphsat {
enum ChannelActionT { SEND_T, RECEIVE_T };
class ChannelAction {
public
  ChannelAction(ChannelActionT t, bool isP, int elocal_channel_id) { type = t; }

private:
  ChannelActionT type;
  bool isParameter;
  int local_channel_id;
  int gloabl_channel_id;
};

}; // namespace graphsat

#endif
