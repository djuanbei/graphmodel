#ifndef _ARGMENT_H
#define _ARGMENT_H
#include <memory>
#include <string>
#include <vector>

namespace graphsat {

#define AVOID_COPY(T)                  \
 private:                              \
  T(const T& other) { assert(false); } \
  T& operator=(const T& other) {       \
    assert(false);                     \
    return *this;                      \
  }

enum ARGUMENT_TYPE {
  CONST_ARG,          // constant value
  NORMAL_VAR_ARG,     // template variable id
  PARAMETER_ARG,      // template argument id (pass value)
  REF_PARAMETER_ARG,  // temmplate argument if (pass reference)
  FUN_POINTER_ARG,    // template function
  SELECT_VAR_ARG,     // select variable
  EMPTY_ARG           // otherwise
};

class RealArgument {
 public:
  RealArgument() : type(EMPTY_ARG), value(0), index(nullptr) {}
  RealArgument(ARGUMENT_TYPE t, int v) : type(t), value(v), index(nullptr) {}
  ~RealArgument() {
    if (index != nullptr) {
      delete index;
      index=nullptr;
    }
  }
  RealArgument(const RealArgument& other)
      : type(other.type), value(other.value), index(nullptr) {
    if (nullptr != other.index) {
      index = new RealArgument(*(other.index));
    }
  }
  void setType(ARGUMENT_TYPE t) { type = t; }
  ARGUMENT_TYPE getType() const { return type; }
  void setValue(int_fast64_t v) { value = v; }
  
  void setSelectValue( const int v);

  int_fast64_t getValue() const { return value; }

  void setIndex(const RealArgument& real) {
    if (nullptr != index) {
      delete index;
    }
    index = new RealArgument(real);
  }

  const RealArgument* getIndex() const { return index; }

  RealArgument& operator=(const RealArgument& other);

  int getIndex(int* count_value) const;
  int getValue(int* counter_value) const;

 private:
  ARGUMENT_TYPE type;
  int_fast64_t value;

  RealArgument* index;
};

class ClockConstraint;
class DBMManager;
class Argument {
 public:
  Argument() : type(EMPTY_ARG), value(0), index(nullptr) {}
  Argument(const Argument& other) {
    type = other.type;
    value = other.value;
    name = other.name;

    index = nullptr;
    if (nullptr != other.index) {
      index = new Argument(*(other.index));
    }
  }
  ~Argument() {
    if (nullptr != index) {
      delete index;
      index = nullptr;
    }
  }
  explicit Argument(int v) : type(CONST_ARG), value(v), index(nullptr) {}
  explicit Argument(ARGUMENT_TYPE t, const std::string& n)
      : type(t), value(0), name(n), index(nullptr) {}

  explicit Argument(ARGUMENT_TYPE t, int v)
      : type(t), value(v), index(nullptr) {}

  void setType(ARGUMENT_TYPE t) { type = t; }
  ARGUMENT_TYPE getType() const { return type; }

  int_fast64_t getValue() const { return value; }
  void setValue(int_fast64_t v) { value = v; }

  std::string getName() const { return name; }

  void setName(const std::string n) { name = n; }
  const Argument* getIndex() const { return index; }

  void setIndex(const Argument& out_index) {
    if (nullptr != index) {
      delete index;
    }
    index = new Argument(out_index);
  }
  Argument& operator=(const Argument& other) {
    if (index != nullptr) {
      delete index;
    }

    type = other.type;
    value = other.value;
    name = other.name;

    index = nullptr;
    if (nullptr != other.index) {
      index = new Argument(*(other.index));
    }
    return *this;
  }
  int_fast64_t getMapValue(const std::vector<int>& id_map,
                           const std::vector<int>& parameter_value) const;
  std::string to_string() const;

 private:
  ARGUMENT_TYPE type;
  int_fast64_t value;
  std::string name;

  Argument* index;
  friend class ClockConstraint;
  friend class DBMManager;
  friend ClockConstraint randConst(const int num, const int low, const int up);
};

// int_fast64_t getMapValue(const Argument& arg, const std::vector<int>& id_map,
//                          const std::vector<int>& parameter_value);

}  // namespace graphsat
#endif
