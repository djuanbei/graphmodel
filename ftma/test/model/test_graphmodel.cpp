#include "gtest/gtest.h"

#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"
#include "problem/reachability.hpp"
#include "state/reachableset.hpp"

#include "benchmark/train_gate.h"
#include "benchmark/train_gate_projector.h"

#include "problem/pmcp.hpp"

#undef PRINT_STATE

using namespace graphsat;

typedef AgentSystem<Location, Transition> INT_TAS_t;

/*
 * For more details about this example, see
 * "Automatic Verification of Real-Time Communicating Systems by Constraint
 * Solving", by Wang Yi, Paul Pettersson and Mats Daniels. In Proceedings of the
 * 7th International Conference on Formal Description Techniques, pages 223-238,
 * North-Holland. 1994.
 */

class GraphModelTest : public ::testing::Test {
 protected:
  GraphModelTest() {
    n = rand() % 10 + 2;  // 6 train
    train_tmt = sys.createTemplate("Train");
    gate_tmt = sys.createTemplate("Gate");
    train_tmt->addPara("id");
    sys.addConstant("N", n);                     // const N=n;
    sys.addType("id_t", 0, sys["N"] - 1);        // typedef int[ 0,N-1] id_t;
    sys.addChan("appr", sys["N"], ONE2ONE_CH);   // chan appr[ N]
    sys.addChan("stop", sys["N"], ONE2ONE_CH);   // chan stop[ N]
    sys.addChan("leave", sys["N"], ONE2ONE_CH);  // chan leave[ N]
    sys.addChan("go", sys["N"], URGENT_CH);      // urgent chan go[ N]

    ADD_CLOCK((*train_tmt), x);

    gate_tmt->addInt("list", sys["N"] + 1);
    gate_tmt->addInt("len", 1);

    gate_tmt->addFun("enqueue", shared_ptr<Enqueue_F>(new Enqueue_F()));

    gate_tmt->addFun("dequeue", shared_ptr<Dequeue_F>(new Dequeue_F()));

    gate_tmt->addFun("front", shared_ptr<Front_F>(new Front_F()));

    gate_tmt->addFun("tail", shared_ptr<Tail_F>(new Tail_F()));
  }
  virtual ~GraphModelTest() {}
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }
  INT_TAS_t sys;
  int n;
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> train_tmt;
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> gate_tmt;
};

TEST_F(GraphModelTest, getConstant) {
  EXPECT_EQ(sys["N"], n);
  EXPECT_EQ(sys.getChanNumber(), 4 * n);
}

TEST_F(GraphModelTest, getType) {
  sys.removeAgent();

  int n = sys["N"];
  Parameter param = gate_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma =
      sys.createAgent(gate_tmt, param);

  for (int i = 0; i < n; i++) {
    Parameter param = train_tmt->getParameter();
    param.setParameterMap("id", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma =
        sys.createAgent(train_tmt, param);
  }

  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  for (int i = 0; i < n + 1; i++) {
    EXPECT_EQ(sys.getType(i, "appr"), CHAN_T);
    EXPECT_EQ(sys.getType(i, "stop"), CHAN_T);
    EXPECT_EQ(sys.getType(i, "leave"), CHAN_T);
    EXPECT_EQ(sys.getType(i, "go"), CHAN_T);
  }
  EXPECT_EQ(sys.getType(n, "list"), INT_T);
  for (int i = 0; i < n; i++) {
    EXPECT_EQ(sys.getType(i, "list"), NO_T);
  }
}

TEST_F(GraphModelTest, getCompentId) {
  sys.removeAgent();

  int n = sys["N"];
  Parameter param = gate_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma =
      sys.createAgent(gate_tmt, param);

  for (int i = 0; i < n; i++) {
    Parameter param = train_tmt->getParameter();
    param.setParameterMap("id", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma =
        sys.createAgent(train_tmt, param);
  }

  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  EXPECT_EQ(sys.getCompentId("Gate"), n);
  for (int i = 0; i < n; i++) {
    EXPECT_EQ(sys.getCompentId("Train[" + to_string(i) + "]"), i);
  }
}
TEST_F(GraphModelTest, getTypeStart) {
  sys.removeAgent();

  int n = sys["N"];
  Parameter param = gate_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma =
      sys.createAgent(gate_tmt, param);

  for (int i = 0; i < n; i++) {
    Parameter param = train_tmt->getParameter();
    param.setParameterMap("id", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma =
        sys.createAgent(train_tmt, param);
  }

  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  EXPECT_EQ(manager->getTypeStart(INT_T), n + 1);

  EXPECT_EQ(manager->getTypeStart(CLOCK_T), 2 * (n + 1) + 2);
}

TEST_F(GraphModelTest, TEMPALTE_FUN_POINTER_ARG) {
  sys.removeAgent();

  int n = sys["N"];
  Parameter param = gate_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma =
      sys.createAgent(gate_tmt, param);

  for (int i = 0; i < n; i++) {
    Parameter param = train_tmt->getParameter();
    param.setParameterMap("id", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma =
        sys.createAgent(train_tmt, param);
  }

  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  Argument arg(NORMAL_VAR_ARG, "list");
  shared_ptr<Argument> dummy(new Argument(NORMAL_VAR_ARG, "len"));
  arg.setIndex(dummy);

  RealArgument rarg = tma->to_real(INT_T, arg);

  INT_TAS_t::State_t* state = manager->newState();

  INT_TAS_t::State_t* counters = manager->getCounterValue(state);

  EXPECT_EQ(getValue(rarg, counters), 0);

  Argument len_arg(NORMAL_VAR_ARG, "len");
  RealArgument rlen_arg = tma->to_real(INT_T, len_arg);

  shared_ptr<Function> enqueue_c = tma->getFun("enqueue");
  shared_ptr<Function> dequeue_c = tma->getFun("dequeue");

  shared_ptr<Function> front_c = tma->getFun("front");

  shared_ptr<Function> tail_c = tma->getFun("tail");

  for (int i = 0; i < n; i++) {
    (*enqueue_c)(counters, i);
    EXPECT_EQ(getValue(rlen_arg, counters), i + 1);
    EXPECT_EQ((*tail_c)(counters), i);
    EXPECT_EQ(getValue(rlen_arg, counters), i + 1);
  }

  for (int i = 0; i < n; i++) {
    EXPECT_EQ(getValue(rlen_arg, counters), n - i);
    EXPECT_EQ((*front_c)(counters), i);
    (*dequeue_c)(counters);
    if (getValue(rlen_arg, counters) > 0) {
      EXPECT_EQ((*front_c)(counters), i + 1);
    }
    EXPECT_EQ(getValue(rlen_arg, counters), n - i - 1);
  }

  manager->destroyState(state);
}

TEST_F(GraphModelTest, constraint) {
  sys.removeAgent();
  int n = sys["N"];
  Parameter param = gate_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma =
      sys.createAgent(gate_tmt, param);
  for (int i = 0; i < n; i++) {
    Parameter param = train_tmt->getParameter();
    param.setParameterMap("id", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma =
        sys.createAgent(train_tmt, param);
  }

  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  Argument arg(NORMAL_VAR_ARG, "list");
  shared_ptr<Argument> dummy(new Argument(NORMAL_VAR_ARG, "len"));
  arg.setIndex(dummy);

  RealArgument rarg = tma->to_real(INT_T, arg);

  INT_TAS_t::State_t* state = manager->newState();

  INT_TAS_t::State_t* counters = manager->getCounterValue(state);

  EXPECT_EQ(getValue(rarg, counters), 0);

  Argument len_arg(NORMAL_VAR_ARG, "len");

  RealArgument rlen_arg = tma->to_real(INT_T, len_arg);

  shared_ptr<Function> enqueue_c = tma->getFun("enqueue");
  shared_ptr<Function> dequeue_c = tma->getFun("dequeue");

  shared_ptr<Function> front_c = tma->getFun("front");

  shared_ptr<Function> tail_c = tma->getFun("tail");

  Argument second1(EMPTY_ARG, 0);
  Argument rhs1(CONST_ARG, 0);
  CounterConstraint* ccs1 =
      new CounterConstraint(len_arg, second1, GT, rhs1);  // len >0
  ccs1->to_real(tma.get());

  EXPECT_FALSE((*ccs1)(counters));

  for (int i = 0; i < n; i++) {
    (*enqueue_c)(counters, i);
    EXPECT_TRUE((*ccs1)(counters));
  }
  for (int i = 0; i < n; i++) {
    EXPECT_TRUE((*ccs1)(counters));
    (*dequeue_c)(counters);
  }
  EXPECT_FALSE((*ccs1)(counters));

  manager->destroyState(state);
}

TEST_F(GraphModelTest, Channel) {
  sys.removeAgent();
  int n = sys["N"];
  Parameter param = gate_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma =
      sys.createAgent(gate_tmt, param);

  for (int i = 0; i < n; i++) {
    Parameter param = train_tmt->getParameter();
    param.setParameterMap("id", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma =
        sys.createAgent(train_tmt, param);
  }

  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  Argument arg(NORMAL_VAR_ARG, "list");
  shared_ptr<Argument> dummy(new Argument(NORMAL_VAR_ARG, "len"));
  arg.setIndex(dummy);

  RealArgument rarg = tma->to_real(INT_T, arg);

  INT_TAS_t::State_t* state = manager->newState();

  INT_TAS_t::State_t* counters = manager->getCounterValue(state);

  Argument ch1_arg(NORMAL_VAR_ARG, "go");
  shared_ptr<Argument> dummy1(new Argument(FUN_POINTER_ARG, "front"));
  ch1_arg.setIndex(dummy1);

  Channel ch1(ch1_arg);
  ch1.setAction(CHANNEL_SEND);
  ch1.to_real(tma.get());

  shared_ptr<Function> enqueue_c = tma->getFun("enqueue");
  shared_ptr<Function> dequeue_c = tma->getFun("dequeue");

  shared_ptr<Function> front_c = tma->getFun("front");

  shared_ptr<Function> tail_c = tma->getFun("tail");

  EXPECT_EQ(3 * n + 1, tma->getKeyID(CHAN_T, "go"));
  for (int i = 0; i < n; i++) {
    (*enqueue_c)(counters, i);
    int ffid = ch1(counters);
    EXPECT_EQ(3 * n + 1, ffid);
  }

  for (int i = 0; i < n; i++) {
    int ffid = ch1(counters);
    EXPECT_EQ(3 * n + 1 + i, ffid);
    (*dequeue_c)(counters);
  }
  manager->destroyState(state);
}

TEST_F(GraphModelTest, SELECT_VAR_ARG) {
  sys.removeAgent();
  int n = sys["N"];
  Parameter param = gate_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma =
      sys.createAgent(gate_tmt, param);

  for (int i = 0; i < n; i++) {
    Parameter param = train_tmt->getParameter();
    param.setParameterMap("id", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma =
        sys.createAgent(train_tmt, param);
  }

  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  shared_ptr<Function> enqueue_c = tma->getFun("enqueue");
  shared_ptr<Function> dequeue_c = tma->getFun("dequeue");
  shared_ptr<Function> front_c = tma->getFun("front");
  shared_ptr<Function> tail_c = tma->getFun("tail");

  TypeDefArray tt = tma->getTypeDef("id_t");
  EXPECT_EQ(tt.getLow(), 0);
  EXPECT_EQ(tt.getHigh(), n - 1);

  Argument ch1_arg(NORMAL_VAR_ARG, "appr");
  shared_ptr<Argument> dummy1(new Argument(SELECT_VAR_ARG, "e"));
  ch1_arg.setIndex(dummy1);

  Channel ch1(ch1_arg);

  ch1.setAction(CHANNEL_RECEIVE);
  EXPECT_EQ(1, tma->getKeyID(CHAN_T, "appr"));  // the channel id start with 1

  INT_TAS_t::State_t* state = manager->newState();
  INT_TAS_t::State_t* counters = manager->getCounterValue(state);

  for (int i = tt.getLow(); i <= tt.getHigh(); i++) {
    tma->setSelect(i);
    ch1.to_real(tma.get());
    int ffid = ch1(counters);
    EXPECT_EQ(i + 1, ffid);
  }

  Argument lhs2(FUN_POINTER_ARG, "enqueue(e)");
  Argument rhs2(EMPTY_ARG, 0);
  CounterAction caction2(lhs2, CALL_ACTION, rhs2);  // enqueue( e)

  for (int i = tt.getLow(); i <= tt.getHigh(); i++) {
    tma->setSelect(i);
    caction2.to_real(tma.get());
    caction2(counters);
    EXPECT_EQ((*tail_c)(counters), i);
  }

  Argument lhs1(SELECT_VAR_ARG, "e");

  Argument second1(EMPTY_ARG, 0);
  Argument rhs1(FUN_POINTER_ARG, "front");
  CounterConstraint ccs1(lhs1, second1, EQ, rhs1);  // e==front()

  for (int i = tt.getLow(); i <= tt.getHigh(); i++) {
    tma->setSelect(i);
    (*enqueue_c)(counters, i);
    ccs1.to_real(tma.get());
    EXPECT_TRUE(ccs1(counters));
    (*dequeue_c)(counters);
  }

  manager->destroyState(state);
}

TEST(TRAIN_GATE_H, generate) {
  TrainGate TG;
  int n = 2;
  INT_TAS_t tg_sys = TG.generate(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      tg_sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);

  Reachability<INT_TAS_t> reacher(tg_sys);

  TrainGatePro prop(n);
  prop.setCS(4);

  EXPECT_FALSE(reacher.satisfy(&data, &prop));
}

TEST(PMCP, train_gate) {
  TrainGate TG;
  IncrementalCheck<INT_TAS_t, TrainGate, TrainGateProjector> check;
  TrainGatePro prop(2);
  prop.setCS(4);
  //  EXPECT_TRUE(check.check(TG, &prop));
}
