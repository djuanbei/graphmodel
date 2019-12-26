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
