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

int getIndex(const RealArgument& arg, int* counter_value) {
  int shift = 0;
  if (nullptr != arg.index) {
    shift = getValue(arg.index.get(), counter_value);
  }
  switch (arg.type) {
    case CONST_ARG:
      assert(false && "It needs to return a index.");
      return arg.value + shift;
    case NORMAL_VAR_ARG:
      return arg.value + shift;

    case PARAMETER_ARG:
      assert(false && "It needs to return a index.");
      return arg.value + shift;
    case REF_PARAMETER_ARG:
      return arg.value + shift;
    case FUN_POINTER_ARG:
      assert(false && "It needs to return a index.");
      return (*((Function*)arg.value))(counter_value);
    case SELECT_VAR_ARG:
      assert(false && "It needs to return a index.");
      return arg.value;
    case EMPTY_ARG:
      return 0;
  }
}

int getValue(const RealArgument& arg, int* counter_value) {
  int shift = 0;
  if (nullptr != arg.index) {
    shift = getValue(arg.index.get(), counter_value);
  }
  switch (arg.type) {
    case CONST_ARG:
      return arg.value + shift;
    case NORMAL_VAR_ARG:
      return counter_value[arg.value + shift];
    case PARAMETER_ARG:
      return arg.value + shift;
    case REF_PARAMETER_ARG:
      return counter_value[arg.value + shift];
    case FUN_POINTER_ARG:
      return (*((Function*)arg.value))(counter_value, shift);

    case SELECT_VAR_ARG:
      return arg.value;
    case EMPTY_ARG:
      return 0;
  }
}

int getValue(const RealArgument* arg, int* counter_value) {
  switch (arg->type) {
    case CONST_ARG:
      return arg->value;
    case NORMAL_VAR_ARG:
      break;
      //  case SYSTEM_VAR_ARG:
      //    break;
    case PARAMETER_ARG:
      return arg->value;
    case REF_PARAMETER_ARG:
      break;

    case FUN_POINTER_ARG:
      return (*((Function*)arg->value))(counter_value);

    case SELECT_VAR_ARG:
      return arg->value;
    case EMPTY_ARG:
      return 0;
  }
  if (nullptr != arg->index) {
    int shift = getValue(arg->index.get(), counter_value);
    return arg->value + shift;
  }

  return counter_value[arg->value];
}

int_fast64_t getMapValue(const Argument& arg, const std::vector<int>& id_map,
                         const std::vector<int>& parameter_value) {
  switch (arg.type) {
    case CONST_ARG:
      return arg.value;
    case NORMAL_VAR_ARG:
      return arg.value;
      //  case SYSTEM_VAR_ARG:
      //    return arg.value;
    case PARAMETER_ARG:
      return parameter_value[arg.value];
    case REF_PARAMETER_ARG:
      return parameter_value[arg.value];
    case FUN_POINTER_ARG:
      return arg.value;

    case SELECT_VAR_ARG:
      return parameter_value.back();
    case EMPTY_ARG:
      return 0;
  }
}
}  // namespace graphsat
