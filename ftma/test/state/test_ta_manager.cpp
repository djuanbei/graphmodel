#include <random>
#include <vector>

#include <gtest/gtest.h>

#include "state/discretestate.hpp"

#include "domain/dbm.h"
#include "domain/dbmset.hpp"

#include "property/fisherprop.h"

#include "benchmark/fisher.h"

#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"
#include "problem/reachability.hpp"
#include "state/reachableset.hpp"

#include "benchmark/train_gate.h"
#include "benchmark/train_gate_projector.h"

#include "problem/pmcp.hpp"

using namespace graphsat;
using namespace std;

TEST(STATE_MANAGER_H, getCounterNumber) {
  FisherGenerator F;
  int n = 10;
  INT_TAS_t fs = F.generate(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager = fs.getStateManager();

  for (int i = 0; i < n; i++) {
    EXPECT_EQ(fs.getCounterNumber(i), 0);
    EXPECT_EQ(fs.getClockNumber(i), 1);
  }

  TrainGate TG;
  INT_TAS_t tg_sys = TG.generate(n);

  for (int i = 0; i < n; i++) {
    EXPECT_EQ(tg_sys.getCounterNumber(i), 0);
    EXPECT_EQ(tg_sys.getClockNumber(i), 1);
  }
}
TEST(STATE_MANAGER_H, swap) {
  FisherGenerator F;
  int n = 10;
  INT_TAS_t fs = F.generate(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager = fs.getStateManager();
  for (int i = 0; i < 1000; i++) {
    int *state = manager->rand();
    int *state1 = manager->newState();
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
  FisherGenerator F;
  INT_TAS_t sys = F.generate(n);
  // Symmetry symm(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);
  reacher.computeAllReachableSet(&data);
  StateSet<UINT> states = data.getStates();
  int *s = manager->newState();
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
  FisherGenerator F;
  INT_TAS_t sys = F.generate(n);

  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);
  reacher.computeAllReachableSet(&data);
  StateSet<UINT> states = data.getStates();
  int len = manager->getCompressionSize();
  UINT *cs1 = new UINT[len];
  int *s = manager->newState();
  int *s1 = manager->newState();
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
