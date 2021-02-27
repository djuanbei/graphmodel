#include "gtest/gtest.h"
#include "util/strutil.h"

using namespace graphmodel;

TEST(STR_UTIL, getFunArg) {
  std::string fun = "get( x)";
  std::string arg = getFunArg(fun);
  EXPECT_EQ(arg, "x");
  std::string fun1 = "get()";
  std::string arg1 = getFunArg(fun1);
  EXPECT_EQ(arg1, "");
  std::string fun_name = getFunName(fun1);
  EXPECT_EQ(fun_name, "get");
  fun_name = getFunName(fun);
  EXPECT_EQ(fun_name, "get");
}

TEST(STR_UTIL, trim) {
  std::string test = "g g ";

  EXPECT_EQ(trim(test), "g g");
  test = " g g";
  EXPECT_EQ(trim(test), "g g");
  test = " g g ";
  EXPECT_EQ(trim(test), "g g");
}
