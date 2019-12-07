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
#include "util/dbmutil.hpp"
#include <map>
namespace graphsat {

enum CHANNEL_TYPE { ONE2ONE_CH, BROADCAST_CH, URGENT_CH };

enum CHANNEL_ACTION { CHANNEL_SEND, CHANNEL_RECEIVE };

const int NO_CHANNEL = 0;

class Channel {
 public:

  Channel();

  Channel( int id );

  Channel( int id, bool ref );

  void setType(const CHANNEL_TYPE t ){
    type=t;
    
  }
  void setIsRef( bool b ) { is_ref = b; }
  void globalIpUpdate( const std::vector<int> &id_map ) {
    if ( is_ref ) {
      gloabl_id = id_map[ local_id ];
    }
  }
  virtual  int getGlobalId(const int* state ) const{
    return gloabl_id;
  }
  int getType( ) const{
    return type;
  }
  bool isSend( ) const{
    return CHANNEL_SEND == action ;
  }
  bool isRecive( ) const{
    return CHANNEL_RECEIVE==action;
  }
  int            gloabl_id; // start from 1
  CHANNEL_TYPE   type;
 protected:
  int            local_id;
  CHANNEL_ACTION action;
  bool           is_ref;

};

class ArrayChannel: public Channel{
 public:

  ArrayChannel(const string & n,  int id):Channel( id){
    array_name=n;
    array_base=0;
  }
  ArrayChannel(const string & n,  int id, bool ref):Channel( id, ref){
    array_name=n;
    array_base=0;
  }
  virtual  int getGlobalId(const int* state ) const;
  void setBase(int b ){
    array_base=b;
  }
  
 protected:
  string array_name;
  int array_base;
    
};

class IndexChannel:public ArrayChannel{

 protected:
  IndexFun_t index_fun;
  
};




} // namespace graphsat

#endif
