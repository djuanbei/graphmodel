/**
 * @file   Action.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:39:22 2019
 *
 * @brief  Action class which  do x = (+=, -=)  y where x is a counter
 *        variable and y is a counter, constant integer or function
 *
 *
 */
#ifndef ACTION_HPP
#define ACTION_HPP

#include <cassert>
#include <cstdint>
#include <vector>

#include "model/function.h"
#include "model/to_real.h"

#include "util/data.hpp"
#include "util/dbmutil.hpp"

namespace graphsat {
using std::ostream;
using std::pair;
using std::setw;
using std::vector;

#define ACTION_TYPE_CASE(op)                                                   \
  switch (rhs.type) {                                                          \
  case CONST_ARG:                                                              \
    counter_value[lhs_value] op rhs_value;                                     \
    return;                                                                    \
  case NORMAL_VAR_ARG:                                                         \
    counter_value[lhs_value] op counter_value[rhs_value];                      \
    return;                                                                    \
  case PARAMETER_ARG:                                                          \
    counter_value[lhs_value] op rhs_value;                                     \
    return;                                                                    \
  case REF_PARAMETER_ARG:                                                      \
    counter_value[lhs_value] op counter_value[rhs_value];                      \
    return;                                                                    \
  case FUN_POINTER_ARG:                                                        \
    counter_value[lhs_value] op (*((Function *)rhs_value))(counter_value);     \
    return;                                                                    \
  case SELECT_VAR_ARG:                                                         \
    counter_value[lhs_value] op rhs_value;                                     \
    return;                                                                    \
  case EMPTY_ARG:                                                              \
    assert(false);                                                             \
  }

#define ACTION_TYPE_CASE_OUT(op_str)                                           \
  switch (act.rhs.type) {                                                      \
  case CONST_ARG:                                                              \
    out << "counter_" << act.lhs_value << setw(OP_OUT_WIDTH) << op_str         \
        << setw(VALUE_OUT_WIDTH) << act.rhs_value;                             \
    return out;                                                                \
  case NORMAL_VAR_ARG:                                                         \
    out << "counter_" << act.lhs_value << setw(OP_OUT_WIDTH) << op_str         \
        << "counter_" << act.rhs_value;                                        \
    return out;                                                                \
  case PARAMETER_ARG:                                                          \
    out << "counter_" << act.lhs_value << setw(OP_OUT_WIDTH) << op_str         \
        << setw(VALUE_OUT_WIDTH) << act.rhs_value;                             \
    return out;                                                                \
  case REF_PARAMETER_ARG:                                                      \
    out << "counter_" << act.lhs_value << setw(OP_OUT_WIDTH) << op_str         \
        << setw(VALUE_OUT_WIDTH) << "counter_" << act.rhs_value;               \
    return out;                                                                \
  case FUN_POINTER_ARG:                                                        \
    out << "counter_" << act.lhs_value << setw(OP_OUT_WIDTH) << op_str         \
        << setw(VALUE_OUT_WIDTH) << "function *" << act.rhs_value;             \
    return out;                                                                \
  case SELECT_VAR_ARG:                                                         \
    out << "counter_" << act.lhs_value << setw(OP_OUT_WIDTH) << op_str         \
        << setw(VALUE_OUT_WIDTH) << "function *" << act.rhs.name;              \
  case EMPTY_ARG:                                                              \
    assert(false);                                                             \
  }

class CounterAction {
public:
  CounterAction(const Argument &out_lhs, const Action_e &ee,
                const Argument &out_rhs)
      : lhs(out_lhs), action(ee), rhs(out_rhs) {}

  void to_real(const shared_ptr<TOReal> &convertor) {
    real_lhs = convertor->to_real(INT_T, lhs);
    real_rhs = convertor->to_real(INT_T, rhs);
  }

  void operator()(int *counter_value) const {
    if (action == CALL_ACTION) {
      getValue(real_lhs, counter_value);
      return;
    }
    int lhs_index = getIndex(real_lhs, counter_value);
    int rhs_v = getValue(real_rhs, counter_value);
    switch (action) {
    case ASSIGNMENT_ACTION:
      counter_value[lhs_index] = rhs_v;
      return;
    case SELF_INC_ACTION:
      counter_value[lhs_index] += rhs_v;
      return;
    case SELF_DEC_ACTION:
      counter_value[lhs_index] -= rhs_v;
      return;
    case CALL_ACTION:
      assert(false);
    }
  }

  friend ostream &operator<<(ostream &out, const CounterAction &act) {
    switch (act.action) {
    case CALL_ACTION:
      out << "call function point" << act.lhs_value;
      return out;
    case ASSIGNMENT_ACTION: {
      ACTION_TYPE_CASE_OUT("=");
    }
    case SELF_INC_ACTION: {
      ACTION_TYPE_CASE_OUT("+=");
    }
    case SELF_DEC_ACTION: {
      ACTION_TYPE_CASE_OUT("-=");
    }
    }
    return out;
  }

private:
  Argument lhs;
  Action_e action;
  Argument rhs;
  int_fast64_t lhs_value;
  int_fast64_t rhs_value;
  RealArgument real_lhs;
  RealArgument real_rhs;
};

} // namespace graphsat
#endif
