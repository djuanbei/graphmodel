#include "constraint/countercons.h"

namespace graphmodel {

void CounterConstraint::to_real(const TOReal* convertor) {
  first_value = convertor->to_real(INT_T, first);
  second_value = convertor->to_real(INT_T, second);
  rhs_value = convertor->to_real(INT_T, rhs);
}
void CounterConstraint::setSelectValue(const int select_value) {
  first_value.setSelectValue(select_value);
  second_value.setSelectValue(select_value);
  rhs_value.setSelectValue(select_value);
}

CounterConstraint* createConstraint(const Argument& out_first,
                                    const Argument& out_second,
                                    COMP_OPERATOR oop,
                                    const Argument& out_rhs) {
  return new CounterConstraint(out_first, out_second, oop, out_rhs);
}
}  // namespace graphmodel
