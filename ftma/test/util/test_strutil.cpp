#include "gtest/gtest.h"

#include "util/strutil.h"

using namespace graphsat;

TEST(STR_UTIL, getFunArg) {

  string fun = "get( x)";
  string arg = getFunArg(fun);
  EXPECT_EQ(arg, "x");
  string fun1 = "get()";
  string arg1 = getFunArg(fun1);
  EXPECT_EQ(arg1, "");
  string fun_name=getFunName( fun1);
  EXPECT_EQ(fun_name,"get" );
  fun_name=getFunName( fun);
  EXPECT_EQ(fun_name,"get" );
}

TEST(STR_UTIL, trim) {
  string test = "g g ";

  EXPECT_EQ(trim(test), "g g");
  test = " g g";
  EXPECT_EQ(trim(test), "g g");
  test = " g g ";
  EXPECT_EQ(trim(test), "g g");
}
