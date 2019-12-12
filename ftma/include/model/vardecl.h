/**
 * @file   vardecl.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec  6 20:13:19 2019
 *
 * @brief  variable dec function
 *
 *
 */

#ifndef VAR_DECL_H
#define VAR_DECL_H
#include <map>
#include <string>

#include "arraytype.h"
#include "channel.h"
#include "clock.h"
#include "util/data.hpp"

namespace graphsat {

using namespace std;
struct BaseDecl {
  string name;
  int num;
  int low, high;
  BaseDecl() {
    num = 0;
    low = DEFAULT_COUNTER_LOWER;
    high = DEFAULT_COUNTER_UPPER;
  }
  BaseDecl(const string &n) : name(n), num(1) {
    low = DEFAULT_COUNTER_LOWER;
    high = DEFAULT_COUNTER_UPPER;
  }

  BaseDecl(const string &n, int len, int l, int h) {
    name = n;
    num = len;
    low = l;
    high = h;
  }
};

struct ChanDecl : public BaseDecl {
  CHANNEL_TYPE type;
  ChanDecl(const string &n) : BaseDecl(n) { type = ONE2ONE_CH; }
};

class VarDecl {
public:
  virtual Argument addClock(const std::string &n);

  virtual int addInt(const BaseDecl &ch);

  virtual int addInt(const string &name, int num = 1);

  virtual int addInt(const string &name, int num, int low, int high);

  virtual int addChan(const ChanDecl &ch);

  virtual int addChan(const string &name, int num, CHANNEL_TYPE type);

  virtual int addConstant(const string &n, const int v);

  virtual int addType(const string &n, const TypeDefArray &type);

  virtual vector<BaseDecl> getInts() const;

  /**
   *
   * low<= x<= high
   * @param n
   * @param low
   * @param high
   *
   * @return
   */
  virtual int addType(const string &n, const int low, const int high);

  bool contain(const string &n) const;

  int operator[](const string &k) const;

  int getCounterNumber() const;

  int getChanNumber() const;

  int getClockNumber() const;

  int getKeyStart(const TYPE_T type, const string &key) const;

protected:
  PointerData data;

  map<string, int> const_values;
  vector<TypeDefArray> self_types;

  int getTypeNumber(const int type) const;
};
} // namespace graphsat

#endif
