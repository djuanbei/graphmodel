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

class Channel {
public:
  Channel();

  Channel(int id);

  Channel(int id, bool ref);

  Channel(const CHANNEL_ACTION a) {
    gloabl_id = 0;
    is_ref = false;
    action = a;
  }
  void shift(const int id) {
    // TODO:
  }

  void setType(const CHANNEL_TYPE t) { type = t; }
  void setIsRef(bool b) { is_ref = b; }
  void globalIpUpdate(const std::vector<int> &id_map) {
    if (is_ref) {
      gloabl_id = id_map[local_id];
    }
  }

  virtual int getGlobalId(int *state) const { return gloabl_id; }
  int getType() const { return type; }
  bool isSend() const { return CHANNEL_SEND == action; }
  bool isRecive() const { return CHANNEL_RECEIVE == action; }

protected:
  int local_id;

public:
  int gloabl_id; // start from 1
  CHANNEL_TYPE type;

protected:
  CHANNEL_ACTION action;
  bool is_ref;
};

class ArrayChannel : public Channel {
public:
  ArrayChannel(const string &n, int id) : Channel(id) {
    array_name = n;
    array_base = 0;
  }
  ArrayChannel(const string &n, int id, bool ref) : Channel(id, ref) {
    array_name = n;
    array_base = 0;
  }
  ArrayChannel(const string &n, const CHANNEL_ACTION a) : Channel(a) {
    array_name = n;
    array_base = 0;
  }
  virtual int getGlobalId(int *state) const;
  void setBase(int b) { array_base = b; }

protected:
  string array_name;
  int array_base;
};

class IndexChannel : public ArrayChannel {
public:
  IndexChannel(const string &n, int id, bool ref) : ArrayChannel(n, id, ref) {}
  IndexChannel(const string &n, const CHANNEL_ACTION a) : ArrayChannel(n, a) {}
  virtual int getGlobalId(int *state) const;
  void setIndexFun(IndexFun_t fun) { index_fun = fun; }
  void setFunName(const string &n) { fun_name = n; }

protected:
  IndexFun_t index_fun;
  string fun_name;
};

class SelectChannel : public ArrayChannel {
public:
  SelectChannel(const string &n, const int id, bool ref)
      : ArrayChannel(n, id, ref) {
    shift = 0;
  }
  SelectChannel(const string &n, const CHANNEL_ACTION a) : ArrayChannel(n, a) {}

  void setSelectVar(const string &n) { select_var = n; }

  int getGlobalId(int *state) const { return array_base + shift; }

  void setShift(const int f) { shift = f; }

protected:
  string select_var;
  int shift;
};

} // namespace graphsat

#endif
