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
#include "function.h"

#include "util/data.hpp"

namespace graphsat {

struct BaseDecl {
  int start_loc;
  std::string name;
  int num;
  int low, high;
  BaseDecl() {
    start_loc = 0;
    num = 0;
    low = DEFAULT_COUNTER_LOWER;
    high = DEFAULT_COUNTER_UPPER;
  }
  explicit BaseDecl(const std::string& n) : name(n), num(1) {
    start_loc = 0;
    low = DEFAULT_COUNTER_LOWER;
    high = DEFAULT_COUNTER_UPPER;
  }

  BaseDecl(const std::string& n, int len, int l, int h) {
    start_loc = 0;
    name = n;
    num = len;
    low = l;
    high = h;
  }
};

struct ChanDecl : public BaseDecl {
  CHANNEL_TYPE type;
  ChanDecl(const std::string& n) : BaseDecl(n) { type = ONE2ONE_CH; }
};

class VarDecl {
 public:
  VarDecl() : parent(nullptr) {}
  VarDecl(VarDecl* p) : parent(p) {}
  void setParent(VarDecl* p) { parent = p; }
  virtual Argument addClock(const std::string& n);

  virtual ~VarDecl();
  virtual int addInt(const BaseDecl& ch);

  virtual int addInt(const std::string& name, int num = 1);

  virtual int addInt(const std::string& name, int num, int low, int high);

  virtual int addChan(const ChanDecl& ch);

  virtual int addChan(const std::string& name, int num, CHANNEL_TYPE type);

  virtual int addFun(const std::string& name, std::shared_ptr<Function> fun);

  virtual std::shared_ptr<Function> getFun(const std::string& name) const;

  virtual const std::map<std::string, std::shared_ptr<Function>>& getFuns()
      const;

  virtual int addConstant(const std::string& n, const int v);

  virtual int addType(const std::string& n, const TypeDefArray& type);

  virtual std::vector<std::string> getKeys(const TYPE_T type) const;

  virtual std::vector<BaseDecl> getAllVar(const TYPE_T type) const;

  virtual std::vector<BaseDecl> getInts() const;

  /**
   *
   * low<= x<= high
   * @param n
   * @param low
   * @param high
   *
   * @return
   */
  virtual int addType(const std::string& n, const int low, const int high);

  TYPE_T getType(const std::string& name) const;

  virtual TypeDefArray getTypeDef(const std::string& n) const;

  virtual int getStartLoc(const TYPE_T type, const int template_id) const {
    assert(false);
    return 0;
  }

  virtual int getTypeStart(const TYPE_T type) const {
    assert(false);
    return 0;
  }

  bool contain(const std::string& n) const;

  int operator[](const std::string& k) const;

  int getCounterNumber() const;

  int getChanNumber() const;

  int getClockNumber() const;

  int getLocalKeyID(const TYPE_T type, const std::string& key) const;

  CHANNEL_TYPE getChanType(const std::string& name) const;

 protected:
  VarDecl* parent;
  PointerData data;

  std::map<std::string, int> const_values;
  std::vector<TypeDefArray> self_types;
  std::map<std::string, std::shared_ptr<Function>> functions;

  int getTypeNumber(const int type) const;
};
}  // namespace graphsat

#endif
