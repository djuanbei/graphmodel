
#include <random>
#include <vector>

#include <gtest/gtest.h>

#include "state/discretestate.hpp"

#include "domain/dbm.h"
#include "domain/dbmset.hpp"

using namespace graphsat;
using namespace std;

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, 10000);

int *randV(int n) {

  int *v = new int[n];

  for (int i = 0; i < n; i++) {
    v[i] = distribution(generator);
  }

  return v;
}

TEST(StateSet1, add_contain) {
  StateSet<int> sets;

  vector<int *> vecs;
  int n = 10;
  int s = 4;
  int num = 1000;
  sets.setParam(n, s);
  for (int i = 0; i < num; i++) {
    int *temp = randV(n);
    if (sets.add(temp)) {
      vecs.push_back(temp);
    } else {
      delete[] temp;
    }
  }
  for (auto e : vecs) {
    EXPECT_TRUE(sets.contain(e));
  }

  for (auto e : vecs) {
    delete[] e;
  }
}

TEST(DBMSET1, add) {

  vector<int *> vecs;
  int n = 10;
  DBMFactory manager(n);
  DBMset<int> sets(manager);
  int num = 10000;

  for (int i = 0; i < num; i++) {
    int *dbm = manager.randomFeasiableDBM();
    if (sets.add(dbm))
      vecs.push_back(dbm);
    else
      manager.destroyDBM(dbm);
  }
  for (auto e : vecs) {
    EXPECT_TRUE(sets.contain(e));
  }
}
TEST(DBM1, include) {
  int MAX_INT = 100000;
  std::default_random_engine generator;
  distribution =
      std::uniform_int_distribution<int>(-MAX_INT / 4 + 1, MAX_INT / 4);
  int clock_num = 20;
  DBMFactory manager(clock_num);
  int num = 100;
  for (int i = 0; i < num; i++) {
    int *dbm = manager.randomFeasiableDBM();
    int x = abs(distribution(generator)) % (clock_num + 1);
    int y = abs(distribution(generator)) % (clock_num + 1);
    if (x != y) {
      int rhs = distribution(generator);
      ClockConstraint cs(Argument(NORMAL_VAR_ARG, x),
                         Argument(NORMAL_VAR_ARG, y), LE, Argument(rhs));
      int *dbm1 = manager.And(dbm, cs);
      EXPECT_TRUE(manager.include(dbm, dbm1));
      manager.destroyDBM(dbm1);
    }
    manager.destroyDBM(dbm);
  }
}

bool cmp(const vector<int> &a, const vector<int> &b) {
  for (size_t i = 0; i < a.size(); i++) {
    if (a[i] < b[i]) {
      return true;
    } else if (a[i] > b[i]) {
      return false;
    }
  }
  return false;
}
TEST(StateSet1, equal) {

  StateSet<int> sets;

  vector<vector<int>> vecs;
  int n = 11;
  int s = 4;
  int num = 10000;
  sets.setParam(n, s);
  for (int i = 0; i < num; i++) {
    int *temp = randV(n);
    if (sets.add(temp)) {
      vector<int> dummy(temp, temp + n);
      vecs.push_back(dummy);
    }
    delete[] temp;
  }
  vector<vector<int>> getRe;
  for (auto state : sets) {
    vector<int> dummy(state, state + n);
    getRe.push_back(dummy);
  }
  EXPECT_TRUE(getRe.size() == vecs.size());
  sort(getRe.begin(), getRe.end(), cmp);
  sort(vecs.begin(), vecs.end(), cmp);
  for (size_t i = 0; i < vecs.size(); i++) {
    EXPECT_EQ(vecs[i], getRe[i]);
  }
}
