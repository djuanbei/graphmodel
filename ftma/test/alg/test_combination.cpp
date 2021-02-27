#include "alg/combination.h"
#include "gtest/gtest.h"

using namespace graphmodel;

TEST(Combination_n_k, Combination_n_k) {
  Combination_n_k com(5, 3);
  std::vector<int> data = *com;
  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(data[i], i + 1);
  }
  EXPECT_TRUE(com.next());
  data = *com;
  EXPECT_EQ(data[0], 1);
  EXPECT_EQ(data[1], 2);
  EXPECT_EQ(data[2], 4);
}
