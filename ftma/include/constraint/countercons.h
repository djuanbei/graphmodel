/**
 * @file   countercons.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu May  9 10:02:20 2019
 *
 * @brief  counter constraint like  pid==id
 *
 *
 */

#ifndef COUNTER_CONS_H
#define COUNTER_CONS_H

#include <iomanip>
#include <iostream>
#include <vector>

#include "model/to_real.h"
#include "util/data.hpp"
#include "util/dbmutil.hpp"

namespace graphmodel {

// first - second op rhs
class CounterConstraint {
 public:
  CounterConstraint(const Argument& out_first, const Argument& out_second,
                    COMP_OPERATOR oop, const Argument& out_rhs)
      : first(out_first), second(out_second), op(oop), rhs(out_rhs) {}
  bool operator()(int* counter_value) const {
    int one = first_value.getValue(counter_value);
    int two = second_value.getValue(counter_value);
    int three = rhs_value.getValue(counter_value);
    return executeOp(one - two, op, three);
  }
  void to_real(const TOReal* convertor);
  void setSelectValue(const int select_value);

 private:
  Argument first;
  Argument second;
  COMP_OPERATOR op;
  Argument rhs;

  RealArgument first_value;
  RealArgument second_value;
  RealArgument rhs_value;
};

CounterConstraint* createConstraint(const Argument& out_first,
                                    const Argument& out_second,
                                    COMP_OPERATOR oop, const Argument& out_rhs);
}  // namespace graphmodel

#endif
