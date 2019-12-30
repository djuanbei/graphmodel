#include <iostream>
#include <string>

#include "util/typedef.h"

namespace graphsat {
string Argument::to_string() const {
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
} // namespace graphsat
