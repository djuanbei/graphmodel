#include "constraint/countercons.h"

namespace graphsat {

void CounterConstraint::to_real(const TOReal *convertor) {
  first_value = convertor->to_real(INT_T, first);
  second_value = convertor->to_real(INT_T, second);
  rhs_value = convertor->to_real(INT_T, rhs);
}

CounterConstraint *createConstraint(const Argument &out_first,
                                    const Argument &out_second,
                                    COMP_OPERATOR oop,
                                    const Argument &out_rhs) {
  return new CounterConstraint(out_first, out_second, oop, out_rhs);
}
} // namespace graphsat
