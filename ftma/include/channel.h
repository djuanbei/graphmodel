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
  bool           is_ref;

  Channel()
      : local_id( -1 )
      , gloabl_id( -1 )
      , type( ONE2ONE_CH )
      , is_urgent( false )
      , action( CHANNEL_RECEIVE )
      , is_ref( false ) {}
  Channel( int id )
      : local_id( -1 )
      , gloabl_id( -1 )
      , type( ONE2ONE_CH )
      , is_urgent( false )
      , action( CHANNEL_RECEIVE )
      , is_ref( false ) {
    if ( id > 0 ) {
      action    = CHANNEL_SEND;
      gloabl_id = id;
    } else {
      action    = CHANNEL_RECEIVE;
      gloabl_id = -id;
    }
  }
  Channel( int id, bool ref )
      : local_id( -1 )
      , gloabl_id( -1 )
      , type( ONE2ONE_CH )
      , is_urgent( false )
      , action( CHANNEL_RECEIVE )
      , is_ref( false ) {
    is_ref = ref;
    if ( id > 0 ) {
      action = CHANNEL_SEND;
    } else {
      action = CHANNEL_RECEIVE;
      id *= -1;
    }
    if ( ref ) {
      local_id = id;
    } else {
      gloabl_id = id;
    }
  }

  void setIsRef( bool b ) { is_ref = b; }
  void globalIpUpdate( const std::vector<int> &id_map ) {
    if ( is_ref ) {
      gloabl_id = id_map[ local_id ];
    }
  }
};

} // namespace graphsat

#endif
