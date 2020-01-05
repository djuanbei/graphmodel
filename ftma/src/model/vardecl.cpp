#include "model/vardecl.h"

namespace graphsat {
VarDecl::~VarDecl() {
  const std::vector<std::pair<std::string, std::vector<void*>>>& int_values =
      data.getValue(INT_T);
  for (auto e : int_values) {
    for (auto ee : e.second) {
      delete (BaseDecl*)ee;
    }
  }
  data.clear(INT_T);

  const std::vector<std::pair<std::string, std::vector<void*>>>& clock_values =
      data.getValue(CLOCK_T);
  for (auto e : clock_values) {
    for (auto ee : e.second) {
      delete (BaseDecl*)ee;
    }
  }
  data.clear(CLOCK_T);

  const std::vector<std::pair<std::string, std::vector<void*>>>& chan_values =
      data.getValue(CHAN_T);
  for (auto e : chan_values) {
    for (auto ee : e.second) {
      delete (ChanDecl*)ee;
    }
  }
  data.clear(CHAN_T);
}
Argument VarDecl::addClock(const std::string& n) {
  assert(!contain(n));
  int re = data.getTypeNum(CLOCK_T) + 1;
  data.addValue(CLOCK_T, n, new BaseDecl(n));
  Argument temp(CONST_ARG, re);
  temp.setName(n);
  // temp.name = n;
  return temp;
}

int VarDecl::addInt(const std::string& n, int num) {
  BaseDecl b(n);
  b.num = num;
  return addInt(b);
}

int VarDecl::addInt(const std::string& name, int num, int low, int high) {
  BaseDecl b(name, num, low, high);
  return addInt(b);
}

int VarDecl::addInt(const BaseDecl& ch) {
  assert(!contain(ch.name));
  int re = data.getTypeNum(INT_T);
  data.addValue(INT_T, ch.name, new BaseDecl(ch));
  return re;
}

int VarDecl::addConstant(const std::string& n, const int v) {
  assert(!contain(n));
  int re = const_values.size();
  const_values[n] = v;
  return re;
}

int VarDecl::addChan(const ChanDecl& ch) {
  assert(!contain(ch.name));
  int re = data.getTypeNum(CHAN_T);
  data.addValue(CHAN_T, ch.name, new ChanDecl(ch));
  return re;
}
int VarDecl::addChan(const std::string& name, int num, CHANNEL_TYPE type) {
  ChanDecl dummy(name);
  dummy.type = type;
  dummy.num = num;
  return addChan(dummy);
}

int VarDecl::addFun(const std::string& name, std::shared_ptr<Function> fun) {
  int re = functions.size();
  functions[name] = fun;
  return re;
}

std::shared_ptr<Function> VarDecl::getFun(const std::string& name) const {
  return functions.at(name);
}

const std::map<std::string, std::shared_ptr<Function>>& VarDecl::getFuns()
    const {
  return functions;
}

int VarDecl::addType(const std::string& n, const TypeDefArray& type) {
  assert(!contain(n));
  int re = self_types.size();
  self_types.push_back(type);
  return re;
}

int VarDecl::addType(const std::string& n, const int low, const int high) {
  assert(!contain(n));
  int re = self_types.size();
  TypeDefArray t(n, 0, high);  // typedef int[ 0,N-1] id_t;
  self_types.push_back(t);
  return re;
}

TYPE_T VarDecl::getType(const std::string& name) const {
  int re = data.getType(name);
  if (re == NOT_FOUND) {
    return NO_T;
  }
  return (TYPE_T)re;
}

TypeDefArray VarDecl::getTypeDef(const std::string& n) const {
  for (auto& e : self_types) {
    if (e.getName() == n) {
      return e;
    }
  }
  if (nullptr != parent) {
    return parent->getTypeDef(n);
  }
  return TypeDefArray();
}

std::vector<BaseDecl> VarDecl::getInts() const {
  const std::vector<std::pair<std::string, std::vector<void*>>>& temp =
      data.getValue(INT_T);
  std::vector<BaseDecl> re;
  for (auto& e : temp) {
    BaseDecl dummy = *((BaseDecl*)e.second[0]);
    if (1 == dummy.num) {
      re.push_back(dummy);
    } else {
      for (int i = 0; i < dummy.num; i++) {
        BaseDecl t = dummy;
        t.name += std::to_string(i);
        re.push_back(t);
      }
    }
  }
  return re;
}

int VarDecl::operator[](const std::string& k) const {
  assert(contain(k));
  return const_values.at(k);
}

bool VarDecl::contain(const std::string& n) const {
  if (data.hasValue(CLOCK_T, n)) {
    return true;
  }
  if (data.hasValue(INT_T, n)) {
    return true;
  }

  if (data.hasValue(CHAN_T, n)) {
    return true;
  }

  for (auto& e : const_values) {
    if (e.first == n) {
      return true;
    }
  }
  for (auto& e : self_types) {
    if (e.getName() == n) {
      return true;
    }
  }

  return false;
}
int VarDecl::getChanNumber() const { return getTypeNumber(CHAN_T); }
int VarDecl::getCounterNumber() const { return getTypeNumber(INT_T); }
int VarDecl::getClockNumber() const { return getTypeNumber(CLOCK_T); }

int VarDecl::getTypeNumber(const int type) const {
  int re = 0;
  std::vector<std::pair<std::string, std::vector<void*>>> clocks =
      data.getValue(type);
  for (auto& e : clocks) {
    re += ((BaseDecl*)e.second[0])->num;
  }
  return re;
}

std::vector<std::string> VarDecl::getKeys(const TYPE_T type) const {
  std::vector<std::string> re;
  std::vector<std::pair<std::string, std::vector<void*>>> counters =
      data.getValue(type);
  for (std::vector<std::pair<std::string, std::vector<void*>>>::const_iterator
           it = counters.begin();
       it != counters.end(); it++) {
    re.push_back(((BaseDecl*)it->second[0])->name);
  }
  return re;
}

std::vector<BaseDecl> VarDecl::getAllVar(const TYPE_T type) const {
  std::vector<BaseDecl> re;
  std::vector<std::pair<std::string, std::vector<void*>>> counters =
      data.getValue(type);
  for (std::vector<std::pair<std::string, std::vector<void*>>>::const_iterator
           it = counters.begin();
       it != counters.end(); it++) {
    re.push_back(*((BaseDecl*)it->second[0]));
  }
  return re;
}

int VarDecl::getLocalKeyID(const TYPE_T type, const std::string& key) const {
  int re = 0;
  //  if(type==CLOCK_T|| type== CHAN_T){
  //    re=1;
  //  }
  std::vector<std::pair<std::string, std::vector<void*>>> counters =
      data.getValue(type);
  for (std::vector<std::pair<std::string, std::vector<void*>>>::const_iterator
           it = counters.begin();
       it != counters.end(); it++) {
    if (((BaseDecl*)it->second[0])->name == key) {
      return re;
    }
    re += ((BaseDecl*)it->second[0])->num;
  }
  // assert( false );
  return NOT_FOUND;
}
CHANNEL_TYPE VarDecl::getChanType(const std::string& name) const {
  std::vector<std::pair<std::string, std::vector<void*>>> temp =
      data.getValue(CHAN_T);
  for (auto& e : temp) {
    if (e.first == name) {
      return ((ChanDecl*)e.second[0])->type;
    }
  }
  if (nullptr != parent) {
    return parent->getChanType(name);
  }
  return ONE2ONE_CH;
}

}  // namespace graphsat
