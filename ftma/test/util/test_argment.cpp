#include "gtest/gtest.h"

#include "util/strutil.h"
#include "util/typedef.h"

using namespace graphsat;

TEST(Argument, to_string) {
  Argument h;
  EXPECT_EQ(h.to_string(), "");
  for (int i = -100; i < 100; i++) {
    Argument h(i);
    EXPECT_EQ(h.to_string(), to_string(i));
  }
  string p = "ss";
  Argument hh(NORMAL_VAR_ARG, "ss");
  EXPECT_EQ(hh.to_string(), p);
  p = rand_str();
  Argument hh1(NORMAL_VAR_ARG, p);
  EXPECT_EQ(hh1.to_string(), p);

  p = rand_str();
  Argument hh2(PARAMETER_ARG, p);
  EXPECT_EQ(hh2.to_string(), p);

  p = rand_str();
  Argument hh3(REF_PARAMETER_ARG, p);
  EXPECT_EQ(hh3.to_string(), p);

  p = rand_str();
  Argument hh4(FUN_POINTER_ARG, p);
  EXPECT_EQ(hh4.to_string(), p);

  p = rand_str();
  Argument hh5(SELECT_VAR_ARG, p);
  EXPECT_EQ(hh5.to_string(), p);
}
