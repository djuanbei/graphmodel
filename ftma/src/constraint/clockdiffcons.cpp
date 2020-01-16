#include "constraint/clockdiffcons.h"
#include <cassert>

namespace graphsat {

ClockConstraint::ClockConstraint(const Argument& clock_id1, COMP_OPERATOR eop,
                                 const Argument& rhs) {
  clock_x_arg = clock_id1;

  clock_y_arg.type = CONST_ARG;
  clock_y_arg.value = 0;
  rhs_arg = rhs;

  if (clock_id1.type == NORMAL_VAR_ARG && rhs.type == CONST_ARG) {
    init(clock_id1.value, 0, eop, rhs.value);
    return;
  }
  assert(false && "At now the program has not supprt other cases.");
}

ClockConstraint::ClockConstraint(const Argument& clock_id1,
                                 const Argument& clock_id2, COMP_OPERATOR eop,
                                 const Argument& rhs) {
  clock_x_arg = clock_id1;
  clock_y_arg = clock_id2;
  rhs_arg = rhs;
  if (clock_id1.type == NORMAL_VAR_ARG && clock_id2.type == NORMAL_VAR_ARG) {
    init(clock_id1.value, clock_id2.value, eop, rhs.value);
    return;
  }

  assert(false && "At now the program has not supprt other cases.");
}

void ClockConstraint::to_real(const TOReal* convertor) {
  real_clock_x_arg = convertor->to_real(CLOCK_T, clock_x_arg);
  real_clock_y_arg = convertor->to_real(CLOCK_T, clock_y_arg);
  real_rhs_arg = convertor->to_real(CLOCK_T, rhs_arg);
  init(real_clock_x_arg.getValue(), real_clock_y_arg.getValue(), op,
       real_rhs_arg.getValue());
}

void ClockConstraint::setSelectValue(const int select_value) {}

// void ClockConstraint::globalUpdate(const vector<int> &parameter_value) {
//   if (rhs_arg.type == CONST_ARG) {
//     return;
//   } else if (rhs_arg.type == PARAMETER_ARG) {
//     int rhs = parameter_value[rhs_arg.value];
//     int id1 = clock_x;
//     int id2 = clock_y;
//     init(id1, id2, op, rhs);
//   }
//}

// void ClockConstraint::clockShift(int shift) {
//   if (clock_x > 0) {
//     clock_x += shift;
//   }
//   if (clock_y > 0) {
//     clock_y += shift;
//   }
// }

ClockConstraint ClockConstraint::neg() const {
  ClockConstraint re(*this);
  re.neg_impl();
  return re;
}

bool ClockConstraint::isSat(const ClockConstraint& cons) const {
  if ((cons.clock_x == clock_x) && (cons.clock_y == clock_y)) {
    return true;
  } else if ((cons.clock_x == clock_y) && (cons.clock_y == clock_x)) {
    ClockConstraint negCons = cons.neg();
    return negCons.matrix_value < matrix_value;
  }

  if ((clock_x > 0 && clock_y > 0) && (cons.clock_x > 0 && cons.clock_y > 0)) {
    return true;
  }
  if (clock_y > 0 && cons.clock_y > 0) {
    return true;
  }

  if (0 == clock_y) {
    if (clock_x != cons.clock_y) {
      return true;
    }

    return matrix_value + cons.matrix_value > LTEQ_ZERO;

  } else {
    if (clock_y != cons.clock_x) {
      return true;
    }
    return matrix_value + cons.matrix_value > LTEQ_ZERO;
  }

  return true;
}

std::ostream& operator<<(std::ostream& out, const ClockConstraint& cons) {
  if (cons.clock_x >= 0 && cons.clock_y >= 0) {
    if (isStrict<int>(cons.matrix_value)) {
      out << CLOCK_NAME_PRE << cons.clock_x << " - " << CLOCK_NAME_PRE
          << cons.clock_y << std::setw(2) << "<" << std::setw(5)
          << getRight(cons.matrix_value);
    } else {
      out << CLOCK_NAME_PRE << cons.clock_x << " - " << CLOCK_NAME_PRE
          << cons.clock_y << std::setw(2) << "<=" << std::setw(5)
          << getRight(cons.matrix_value);
    }
  } else if (cons.clock_x < 0) {
    if (isStrict<int>(cons.matrix_value)) {
      out << "0     - " << CLOCK_NAME_PRE << cons.clock_y << std::setw(2) << "<"
          << std::setw(5) << getRight(cons.matrix_value);
    } else {
      out << "0     - "
          << " - " << CLOCK_NAME_PRE << cons.clock_y << std::setw(2)
          << "<=" << std::setw(5) << getRight(cons.matrix_value);
    }
  } else {
    if (isStrict<int>(cons.matrix_value)) {
      out << CLOCK_NAME_PRE << cons.clock_x << "          <  "
          << getRight(cons.matrix_value);
    } else {
      out << CLOCK_NAME_PRE << cons.clock_x << "          <= " << std::setw(5)
          << getRight(cons.matrix_value);
    }
  }
  return out;
}

ClockConstraint::ClockConstraint(const int clock_id1, const int clock_id2,
                                 const int rhs, bool is_strict_ref) {
  clock_x_arg.type = NORMAL_VAR_ARG;
  clock_x_arg.value = clock_id1;

  clock_y_arg.type = NORMAL_VAR_ARG;
  clock_y_arg.value = NORMAL_VAR_ARG;
  op = LE;
  if (is_strict_ref) {
    op = LT;
  }
  rhs_arg.type = CONST_ARG;
  rhs_arg.value = rhs;

  clock_x = clock_id1;
  clock_y = clock_id2;
  matrix_value = getMatrixValue(rhs, is_strict_ref);
}

void ClockConstraint::neg_impl(void) {
  int temp = clock_x;
  clock_x = clock_y;
  clock_y = temp;
  matrix_value = 1 - matrix_value;
}

void ClockConstraint::init(const int clock_id1, const int clock_id2,
                           COMP_OPERATOR eop, const int rhs) {
  op = eop;
  assert(op != NE);
  assert(op != EQ);
  if (LE == op) {
    clock_x = clock_id1;
    clock_y = clock_id2;
    matrix_value = getMatrixValue(rhs, false);
  } else if (LT == op) {
    clock_x = clock_id1;
    clock_y = clock_id2;
    matrix_value = getMatrixValue(rhs, true);
  } else if (GE == op) {
    clock_x = clock_id2;
    clock_y = clock_id1;
    matrix_value = getMatrixValue(-rhs, false);  // clock_y-clock_x <= -rhs
  } else if (GT == op) {
    clock_x = clock_id2;
    clock_y = clock_id1;
    matrix_value = getMatrixValue(-rhs, true);  // clock_y-clock_x < -rhs
  }
}

std::ostream& ClockConstraint::dump2Dot(std::ostream& out) const {
  out << "<tr><td>";
  if (clock_x_arg.type != EMPTY_ARG) {
    out << clock_x_arg.to_string();
  } else if (clock_y_arg.type != EMPTY_ARG) {
    out << " - " << clock_y_arg.to_string();
  }
  out << " " << getOpStr(op) << " " << rhs_arg.to_string();
  out << "</td></tr>" << std::endl;

  return out;
}

ClockConstraint randConst(const int num, const int low, const int up) {
  std::uniform_int_distribution<int> distribution(0, num);
  std::default_random_engine generator;
  int xx = distribution(generator);
  int yy = distribution(generator);
  while (xx == yy) {
    yy = distribution(generator);
  }

  std::uniform_int_distribution<int> distribution1(low, up);

  int vv = distribution1(generator);
  if (distribution1(generator) % 2) {
    return ClockConstraint(Argument(NORMAL_VAR_ARG, xx),
                           Argument(NORMAL_VAR_ARG, yy), LT, Argument(vv));
  } else {
    return ClockConstraint(Argument(NORMAL_VAR_ARG, xx),
                           Argument(NORMAL_VAR_ARG, yy), LE, Argument(vv));
  }
}

}  // namespace graphsat
