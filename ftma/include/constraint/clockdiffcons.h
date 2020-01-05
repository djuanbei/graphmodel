
/**
 * @file   linsimpcons.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Mar 29 10:47:56 2019
 *
 * @brief  linear simple constraint such clock_x-clock_y < ( <= ) right
 *
 *
 */
#ifndef LIN_SIMP_CONS_H
#define LIN_SIMP_CONS_H

#include <iomanip>
#include <iostream>
#include <random>

#include "model/clock.h"
#include "model/to_real.h"
#include "util/dbmutil.hpp"

namespace graphsat {

// using namespace std;

//const int GLOBAL_CLOCK_ID = 0;

/**
 *  clock_x -clock_y < ( <= ) realRight
 *
 */
class ClockConstraint {
 public:
  ClockConstraint(const Argument& clock_id1, COMP_OPERATOR eop,
                  const Argument& rhs);

  ClockConstraint(const Argument& clock_id1, const Argument& clock_id2,
                  COMP_OPERATOR eop, const Argument& rhs);

  void to_real(const TOReal* convertor);

  ClockConstraint neg(void) const;

  bool isSat(const ClockConstraint& cons) const;

  std::ostream& dump2Dot(std::ostream& out) const;

  friend std::ostream& operator<<(std::ostream& out,
                                  const ClockConstraint& cons);

 private:
  /**
   is_strict_ref  is true : <
   is_strict_ref  is true : <=
   */
  ClockConstraint(const int clock_id1, const int clock_id2, const int rhs,
                  bool is_strict_ref);

  void neg_impl(void);

  void init(const int clock_id1, const int clock_id2, COMP_OPERATOR eop,
            const int rhs);

 public:
  Argument clock_x_arg;
  Argument clock_y_arg;
  Argument rhs_arg;

  RealArgument real_clock_x_arg;
  RealArgument real_clock_y_arg;
  RealArgument real_rhs_arg;

  int clock_x;
  int clock_y;
  COMP_OPERATOR op;
  int matrix_value;

 private:
  friend ClockConstraint randConst(const int num, const int low, const int up);
  friend class DBMManager;
};
ClockConstraint randConst(const int num, const int low, const int up);

}  // namespace graphsat
#endif
