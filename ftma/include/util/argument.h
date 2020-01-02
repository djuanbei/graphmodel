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

struct RealArgument {
  ARGUMENT_TYPE type;
  int_fast64_t value;

  std::shared_ptr<RealArgument> index;
  RealArgument() : type(EMPTY_ARG), value(0), index(nullptr) {}
  RealArgument(ARGUMENT_TYPE t, int v) : type(t), value(v), index(nullptr) {}
};

class ClockConstraint;
class DBMFactory;
struct Argument {
  ARGUMENT_TYPE type;
  int_fast64_t value;
  std::string name;
  std::shared_ptr<Argument> index;
  Argument() : type(EMPTY_ARG), value(0), index(nullptr) {}
  explicit Argument(int v) : type(CONST_ARG), value(v), index(nullptr) {}
  explicit Argument(ARGUMENT_TYPE t, const std::string& n)
      : type(t), value(0), name(n), index(nullptr) {}

  explicit Argument(ARGUMENT_TYPE t, int v)
      : type(t), value(v), index(nullptr) {}

  void setIndex(std::shared_ptr<Argument>& out_index) { index = out_index; }
  std::string to_string() const;

 private:
  friend class ClockConstraint;
  friend class DBMFactory;
  friend ClockConstraint randConst(const int num, const int low, const int up);
};

int getIndex(const RealArgument& arg, int* counter_value);

int getValue(const RealArgument& arg, int* counter_value);

int getValue(const RealArgument* arg, int* counter_value);

int_fast64_t getMapValue(const Argument& arg, const std::vector<int>& id_map,
                         const std::vector<int>& parameter_value);

}  // namespace graphsat
#endif
