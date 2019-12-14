#include "gtest/gtest.h"

#include "constraint/clockdiffcons.h"

using namespace graphsat;

TEST(LIN_SIMP_CONS_H, ClockConstraint) {
  Argument c1(NORMAL_VAR_ARG, 1);
  Argument c2(NORMAL_VAR_ARG, 2);

  ClockConstraint cs1(c1, LE, Argument(10));
  ClockConstraint cs2(c2, LE, Argument(10));
  EXPECT_TRUE(cs1.isSat(cs2));
  ClockConstraint cs3 = cs2.neg();
  EXPECT_FALSE(cs2.isSat(cs3));
  ClockConstraint cs4(c1, c2, LE, Argument(10));
  ClockConstraint cs5(c2, c1, LE, Argument(-10));
  EXPECT_TRUE(cs4.isSat(cs5));
  EXPECT_TRUE(cs5.isSat(cs4));
  ClockConstraint cs6(c2, c1, LT, Argument(-10));

  EXPECT_FALSE(cs4.isSat(cs6));
  for (int i = 0; i < 20; i++) {
    ClockConstraint cs1 = randConst(2, 0, 20);
    ClockConstraint cs2 = randConst(2, 0, 20);
    EXPECT_EQ(cs1.isSat(cs2), cs2.isSat(cs1));
  }
}
