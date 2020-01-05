#include <cassert>
#include <iostream>
#include <string>

#include "util/argument.h"

#include "model/function.h"

namespace graphsat {
std::string Argument::to_string() const {
  if (type == EMPTY_ARG) {
    return "";
  }
  if ("" == name) {
    return std::to_string(value);
  } else {
    if (index != nullptr) {
      return name + "[" + index->to_string() + "]";
    } else {
      return name;
    }
  }
}

int RealArgument::getIndex(int* counter_value) const {
  int shift = 0;
  // const RealArgument *index=arg.getIndex( );
  if (nullptr != index) {
    shift = index->getValue(counter_value);
  }
  switch (type) {
    case CONST_ARG:
      assert(false && "It needs to return a index.");
      return value + shift;
    case NORMAL_VAR_ARG:
      return value + shift;

    case PARAMETER_ARG:
      assert(false && "It needs to return a index.");
      return value + shift;
    case REF_PARAMETER_ARG:
      return value + shift;
    case FUN_POINTER_ARG:
      assert(false && "It needs to return a index.");
      return (*((Function*)value))(counter_value);
    case SELECT_VAR_ARG:
      assert(false && "It needs to return a index.");
      return value;
    case EMPTY_ARG:
      return 0;
  }
}

int RealArgument::getValue(int* counter_value) const {
  int shift = 0;
  if (nullptr != index) {
    shift = index->getValue(counter_value);
  }
  switch (type) {
    case CONST_ARG:
      return value + shift;
    case NORMAL_VAR_ARG:
      return counter_value[value + shift];
    case PARAMETER_ARG:
      return value + shift;
    case REF_PARAMETER_ARG:
      return counter_value[value + shift];
    case FUN_POINTER_ARG:
      return (*((Function*)value))(counter_value, shift);

    case SELECT_VAR_ARG:
      return value;
    case EMPTY_ARG:
      return 0;
  }
}

// int getValue(const RealArgument* arg, int* counter_value) {
//   switch (arg->type) {
//     case CONST_ARG:
//       return arg->value;
//     case NORMAL_VAR_ARG:
//       break;

//     case PARAMETER_ARG:
//       return arg->value;
//     case REF_PARAMETER_ARG:
//       break;

//     case FUN_POINTER_ARG:
//       return (*((Function*)arg->value))(counter_value);

//     case SELECT_VAR_ARG:
//       return arg->value;
//     case EMPTY_ARG:
//       return 0;
//   }
//   if (nullptr != arg->index) {
//     int shift = getValue(arg->index, counter_value);
//     return arg->value + shift;
//   }

//   return counter_value[arg->value];
// }

int_fast64_t Argument::getMapValue(
    const std::vector<int>& id_map,
    const std::vector<int>& parameter_value) const {
  switch (type) {
    case CONST_ARG:
      return value;
    case NORMAL_VAR_ARG:
      return value;

    case PARAMETER_ARG:
      return parameter_value[value];
    case REF_PARAMETER_ARG:
      return parameter_value[value];
    case FUN_POINTER_ARG:
      return value;

    case SELECT_VAR_ARG:
      return parameter_value.back();
    case EMPTY_ARG:
      return 0;
  }
}

RealArgument& RealArgument::operator=(const RealArgument& other) {
  type = other.type;
  value = other.value;
  if (index != nullptr) {
    delete index;
  }
  index = nullptr;
  if (other.index != nullptr) {
    index = new RealArgument(*(other.index));
  }
  return *this;
}
}  // namespace graphsat
