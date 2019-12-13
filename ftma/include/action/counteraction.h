/**
 * @file   Action.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:39:22 2019
 *
 * @brief  Action class which  do x=x+c where x is a counter
 *        variable and c is a constant integer
 *
 *
 */
#ifndef ACTION_HPP
#define ACTION_HPP

#include <cassert>
#include <cstdint>
#include <vector>

#include "model/function.h"
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
  case TEMPLATE_VAR_ARG:                                                       \
    counter_value[lhs_value] op counter_value[rhs_value];                      \
    return;                                                                    \
  case SYSTEM_VAR_ARG:                                                         \
    counter_value[lhs_value] op counter_value[rhs_value];                      \
    return;                                                                    \
  case PARAMETER_ARG:                                                          \
    counter_value[lhs_value] op rhs_value;                                     \
    return;                                                                    \
  case REF_PARAMETER_ARG:                                                      \
    counter_value[lhs_value] op counter_value[rhs_value];                      \
    return;                                                                    \
  case TEMPALTE_FUN_POINTER_ARG:                                               \
    counter_value[lhs_value] op (*((Function *)rhs_value))(counter_value);     \
    return;                                                                    \
  case SYSTEM_FUN_POINTER_ARG:                                                 \
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
  case TEMPLATE_VAR_ARG:                                                       \
    out << "counter_" << act.lhs_value << setw(OP_OUT_WIDTH) << op_str         \
        << "counter_" << act.rhs_value;                                        \
    return out;                                                                \
  case SYSTEM_VAR_ARG:                                                         \
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
  case TEMPALTE_FUN_POINTER_ARG:                                               \
    out << "counter_" << act.lhs_value << setw(OP_OUT_WIDTH) << op_str         \
        << setw(VALUE_OUT_WIDTH) << "function *" << act.rhs_value;             \
    return out;                                                                \
  case SYSTEM_FUN_POINTER_ARG:                                                 \
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

  void operator()(int *counter_value) const {

    switch (action) {
    case CALL_ACTION:
      ((IndexFun_t)lhs_value)(counter_value);
      return;
    case ASSIGNMENT_ACTION: {
      ACTION_TYPE_CASE(=);
    }
    case SELF_INC_ACTION: {
      ACTION_TYPE_CASE(+=);
    }
    case SELF_DEC_ACTION: {
      ACTION_TYPE_CASE(-=);
    }
    }
  }
  CounterAction *copy() const { return new CounterAction(lhs, action, rhs); }
  void globalUpdate(const vector<int> &counter_map,
                    const vector<int> &parameter_value) {
    lhs_value = lhs.value;
    rhs_value = rhs.value;

    switch (lhs.type) {
    case CONST_ARG:
      assert(false);
      break;
    case TEMPLATE_VAR_ARG:
      break;
    case SYSTEM_VAR_ARG:
      break;
    case PARAMETER_ARG:
      assert(false);
      break;
    case REF_PARAMETER_ARG:
      lhs_value = counter_map[lhs.value];
      break;
    case TEMPALTE_FUN_POINTER_ARG:
      break;
    case SYSTEM_FUN_POINTER_ARG:
      break;
    case SELECT_VAR_ARG:
      break;
    case EMPTY_ARG:
      assert(false);
    }
    switch (rhs.type) {
    case CONST_ARG:
      break;
    case TEMPLATE_VAR_ARG:
      break;
    case SYSTEM_VAR_ARG:
      break;
    case PARAMETER_ARG:
      rhs_value = parameter_value[rhs.value];
      break;
    case REF_PARAMETER_ARG:
      rhs_value = counter_map[rhs.value];
      break;
    case TEMPALTE_FUN_POINTER_ARG:
      break;
    case SYSTEM_FUN_POINTER_ARG:
      break;
    case SELECT_VAR_ARG:
      break;
    case EMPTY_ARG:
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
  }

private:
  Argument lhs;
  Action_e action;
  Argument rhs;
  int_fast64_t lhs_value;
  int_fast64_t rhs_value;
};

} // namespace graphsat
#endif
