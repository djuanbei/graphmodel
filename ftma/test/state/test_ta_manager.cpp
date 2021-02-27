#include <gtest/gtest.h>

#include <random>
#include <vector>

#include "benchmark/fischer.h"
#include "benchmark/train_gate.h"
#include "benchmark/train_gate_projector.h"
#include "domain/dbm.h"
#include "domain/dbmset.hpp"
#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"
#include "problem/pmcp.hpp"
#include "problem/reachability.hpp"
#include "property/fisherprop.h"
#include "state/discretestate.hpp"
#include "state/reachableset.hpp"

using namespace graphmodel;
using namespace std;

TEST(STATE_MANAGER_H, getStateLen) {
  FischerGenerator F;
  for (int i = 1; i < 10; i++) {
    INT_TAS_t fs = F.generate(i);
    shared_ptr<typename INT_TAS_t::StateManager_t> manager =
        fs.getStateManager();
    EXPECT_EQ(manager->getStateLen(), i + 3 + (i + 1) * (i + 1));
  }
  TrainGate TG;
  for (int i = 2; i < 10; i++) {
    INT_TAS_t tg_sys = TG.generate(i);
    shared_ptr<typename INT_TAS_t::StateManager_t> manager =
        tg_sys.getStateManager();
    EXPECT_EQ(manager->getStateLen(),
              i + 1 + 2 + i + 1 + 1 + (i + 1) * (i + 1));
  }
}

TEST(STATE_MANAGER_H, getCounterNumber) {
  FischerGenerator F;
  int n = 10;
  INT_TAS_t fs = F.generate(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager = fs.getStateManager();
  EXPECT_EQ(manager->getComponentNumber(), n);

  for (int i = 0; i < n; i++) {
    EXPECT_EQ(fs.getComponentCounterNumber(i), 0);
    EXPECT_EQ(fs.getComponentClockNumber(i), 1);
  }

  TrainGate TG;
  INT_TAS_t tg_sys = TG.generate(n);

  for (int i = 0; i < n; i++) {
    EXPECT_EQ(tg_sys.getComponentCounterNumber(i), 0);
    EXPECT_EQ(tg_sys.getComponentClockNumber(i), 1);
  }
}
TEST(STATE_MANAGER_H, swap) {
  FischerGenerator F;
  int n = 10;
  INT_TAS_t fs = F.generate(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager = fs.getStateManager();
  for (int i = 0; i < 1000; i++) {
    int* state = manager->randState();
    int* state1 = manager->newState();
    manager->copy(state1, state);
    int ii = rand() % n;
    int jj = rand() % n;
    manager->swap(ii, jj, state);
    if (!manager->equal(state1, state)) {
      manager->swap(ii, jj, state);
      EXPECT_TRUE(manager->equal(state1, state));
    }
    manager->destroyState(state1);
    manager->destroyState(state);
  }
}
TEST(STATE_MANAGER_H, CONTAIN) {
  int n = 3;
  FischerGenerator F;
  INT_TAS_t sys = F.generate(n);

  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);

  Reachability<INT_TAS_t> reacher(sys);
  reacher.computeAllReachableSet(&data);
  const StateSet<UINT>& states = data.getStates();
  int* s = manager->newState();
  for (auto e : states) {
    manager->decode(s, e);
    EXPECT_TRUE(data.contain(s));
  }
  vector<UINT> ss = data.getProcess_states();
  int len = manager->getCompressionSize();
  for (size_t i = 0; i < data.size(); i++) {
    EXPECT_TRUE(states.exists(&(ss[i * len])));
  }

  manager->destroyState(s);
}
TEST(STATE_MANAGER_H, ENCODE) {
  int n = 3;
  FischerGenerator F;
  INT_TAS_t sys = F.generate(n);

  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);

  Reachability<INT_TAS_t> reacher(sys);
  reacher.computeAllReachableSet(&data);
  const StateSet<UINT>& states = data.getStates();
  int len = manager->getCompressionSize();
  UINT* cs1 = new UINT[len];
  int* s = manager->newState();
  int* s1 = manager->newState();
  for (auto e : states) {
    manager->decode(s, e);
    manager->copy(s1, s);

    manager->encode(cs1, s);
    manager->decode(s, cs1);
    EXPECT_TRUE(memcmp(cs1, e, len * sizeof(UINT)) == 0);
    EXPECT_TRUE(memcmp(s1, s, manager->getStateLen() * sizeof(int)) == 0);
  }

  delete[] cs1;
  manager->destroyState(s);
  manager->destroyState(s1);
}

TEST(STATE_MANAGER_H, getTypeNumber) {
  TrainGate TG;
  int n = rand() % 10 + 3;
  INT_TAS_t tg_sys = TG.generate(n);
  shared_ptr<INT_TAS_t::StateManager_t> manager = tg_sys.getStateManager();
  EXPECT_EQ(manager->getTypeNumber(INT_T), n + 2);
  EXPECT_EQ(manager->getTypeNumber(CLOCK_T), (n + 1) * (n + 1));
}

TEST(STATE_MANAGER_H, getValue) {
  TrainGate TG;

  int n = rand() % 10 + 3;
  INT_TAS_t tg_sys = TG.generate(n);
  tg_sys.addInt("x", 1);
  tg_sys.buildManager();
  shared_ptr<INT_TAS_t::StateManager_t> manager = tg_sys.getStateManager();

  int* state = manager->newState();
  int index = 2 * (n + 1) + 1;
  for (int i = 0; i < 10; i++) {
    int v = rand() % 20;
    state[index] = v;
    EXPECT_EQ(*manager->getValue(n, "len", state), v);
  }

  manager->destroyState(state);
}

TEST(STATE_MANAGER_H, setValue) {
  TrainGate TG;

  int n = rand() % 10 + 3;
  INT_TAS_t tg_sys = TG.generate(n);
  tg_sys.addInt("x", 1);
  tg_sys.buildManager();
  shared_ptr<INT_TAS_t::StateManager_t> manager = tg_sys.getStateManager();

  int* state = manager->newState();
  // int index = 2 * (n + 1) + 1;
  for (int i = 0; i < 10; i++) {
    int v = rand() % 20;
    manager->setValue(n, "len", v, state);
    EXPECT_EQ(*manager->getValue(n, "len", state), v);
  }

  manager->destroyState(state);
}

TEST(STATE_MANAGER_H, getClockBound) {
  FischerGenerator F;
  for (int i = 1; i < 10; i++) {
    INT_TAS_t fs = F.generate(i);

    shared_ptr<typename INT_TAS_t::StateManager_t> manager =
        fs.getStateManager();
    int* state = manager->randState();
    int v = rand() % 1000;
    MatrixValue value(v, false);  //<= 10
    manager->setClockLowerBound(0, "x", state, value);

    EXPECT_EQ(manager->getClockLowerBound(0, "x", state), value);
    value.value *= -1;
    EXPECT_EQ(state[i + 3], value.matrixValue());

    MatrixValue value1(v, true);  //<= 10
    manager->setClockLowerBound(0, "x", state, value1);

    EXPECT_EQ(manager->getClockLowerBound(0, "x", state), value1);
    value1.value *= -1;
    EXPECT_EQ(state[i + 3], value1.matrixValue());
  }

  for (int i = 1; i < 10; i++) {
    INT_TAS_t fs = F.generate(i);

    shared_ptr<typename INT_TAS_t::StateManager_t> manager =
        fs.getStateManager();
    int* state = manager->randState();
    int v = rand() % 1000;
    MatrixValue value(v, false);  //<= 10
    manager->setClockUpperBound(0, "x", state, value);

    EXPECT_EQ(manager->getClockUpperBound(0, "x", state), value);

    EXPECT_EQ(state[2 * i + 3], value.matrixValue());

    MatrixValue value1(v, true);  //<= 10

    manager->setClockUpperBound(0, "x", state, value1);

    EXPECT_EQ(manager->getClockUpperBound(0, "x", state), value1);

    EXPECT_EQ(state[2 * i + 3], value1.matrixValue());
  }
}
