#include <random>
#include <vector>

#include <gtest/gtest.h>

#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"
#include "problem/reachability.hpp"
#include "state/discretestate.hpp"
#include "state/reachableset.hpp"

#include "domain/dbm.h"
#include "domain/dbmset.hpp"
#include "property/fisherprop.h"
#include "property/locreachprop.h"

#include "benchmark/fisher.h"
#include "benchmark/fisher_projector.h"

#include "problem/pmcp.hpp"

#include "alg/symmetry.h"

#include "alg/util.h"

using namespace graphsat;
using namespace std;
typedef AgentSystem<Location, Transition> INT_TAS_t;

// typedef ReachableSet<INT_TAS_t> R_t;

// ref:  GRAZ1.pdf  example
TEST(TA, UNREACH) {
  INT_TAS_t sys;
  shared_ptr<INT_TAS_t::AgentTemplate_t> tmt1 = sys.createTemplate("P");
  ADD_CLOCK((*tmt1), x);
  ADD_CLOCK((*tmt1), y);

  vector<INT_TAS_t::T_t> es;
  vector<INT_TAS_t::L_t> ls;
  INT_TAS_t::L_t L0(0);
  INT_TAS_t::L_t L1(1);

  INT_TAS_t::T_t E00a(L0, L0);

  ClockReset reset(y, Argument(0)); // y-->0
  E00a += reset;
  // E00a.addReset(y, 0);                     // y-->0
  INT_TAS_t::CS_t cs1(y, LE, Argument(2)); // y<=2
  E00a += cs1;

  INT_TAS_t::T_t E00b(L0, L0);

  ClockReset reset1(x, Argument(0)); // x-->0
  E00b += reset1;
  // E00b.addReset(x, 0);                     // x-->0
  INT_TAS_t::CS_t cs2(x, LE, Argument(2)); // x<=2
  E00b += cs2;

  INT_TAS_t::T_t E01(L0, L1);

  INT_TAS_t::CS_t cs3(y, LE, Argument(2)); // y<=2
  INT_TAS_t::CS_t cs4(x, GE, Argument(4)); // x>=4

  E01 += cs3;
  E01 += cs4;

  ls.push_back(L0);
  ls.push_back(L1);

  es.push_back(E00a);

  es.push_back(E00b);
  es.push_back(E01);
  tmt1->initial(ls, es, 0);

  Parameter param = tmt1->getParameter();

  shared_ptr<INT_TAS_t::Agent_t> tma1 = sys.createAgent(tmt1, param);

  sys.build();
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  // sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);
  vector<int> loc;
  loc.push_back(1);
  LocReachProperty prop(loc);

  EXPECT_TRUE(reacher.satisfy(&data, &prop));
}

// ref:  paper    "Timed Automata: Semantics, Algorithms and Tools"  Fig. 4
TEST(REACHSET, TERIMINAL) {
  INT_TAS_t sys;
  shared_ptr<INT_TAS_t::AgentTemplate_t> tmt1 = sys.createTemplate("P");
  ADD_CLOCK((*tmt1), x);
  ADD_CLOCK((*tmt1), y);

  vector<INT_TAS_t::T_t> es;
  vector<INT_TAS_t::L_t> ls;

  INT_TAS_t::L_t start(0, "start");

  INT_TAS_t::L_t loop(1, "loop");
  INT_TAS_t::CS_t cs1(x, LE, Argument(10)); // x <= 10
  loop += cs1;

  INT_TAS_t::L_t end(2, "end");

  INT_TAS_t::T_t start_loop(start, loop);

  ClockReset reset3(x, Argument(0)); // x-->0
  start_loop += reset3;
  ClockReset reset4(y, Argument(0));
  start_loop += reset4; // y-->0

  INT_TAS_t::T_t loop_loop(loop, loop);
  INT_TAS_t::CS_t cs2(x, LE, Argument(10)); // x <= 10
  INT_TAS_t::CS_t cs3(x, GE, Argument(10)); // x>=10 === 0-x <= -10
  loop_loop += cs2;
  loop_loop += cs3;

  loop_loop += ClockReset(x, Argument(0)); // x-->0
                                           // ClockReset reset5(x,)
                                           // loop_loop.addReset(x, 0); // x-->0

  INT_TAS_t::T_t loop_end(loop, end);
  INT_TAS_t::CS_t cs4(y, GE, Argument(20)); // y>=20=== 0-y <= -20
  loop_end += cs4;
  loop_end += ClockReset(x, Argument(0)); // x-->0
  loop_end += ClockReset(y, Argument(0)); // y--> 0

  ls.push_back(start);
  ls.push_back(loop);
  ls.push_back(end);

  es.push_back(start_loop);
  es.push_back(loop_loop);
  es.push_back(loop_end);

  tmt1->initial(ls, es, 0);
  Parameter param = tmt1->getParameter();

  shared_ptr<INT_TAS_t::Agent_t> tma1 = sys.createAgent(tmt1, param);

  sys.build();

  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  // sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);

  Property prop;
  EXPECT_FALSE(reacher.satisfy(&data, &prop));
}

TEST(REACHSET, FISHER) {
  int n = 2;
  INT_TAS_t sys;
  sys.addType("id_t", 0, n);
  sys.addInt("id", 1, 0, n);

  shared_ptr<INT_TAS_t::AgentTemplate_t> tmt1 = sys.createTemplate("P");
  tmt1->addPara("pid");

  ADD_CLOCK((*tmt1), x);
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;
  int k = 2;

  typename INT_TAS_t::L_t A(0);

  typename INT_TAS_t::L_t req(1);
  typename INT_TAS_t::CS_t cs1(x, LE, Argument(k)); // x <= k
  req += cs1;

  typename INT_TAS_t::L_t wait(2);

  typename INT_TAS_t::L_t cs(3);

  typename INT_TAS_t::T_t A_req(A, req);
  Argument first(NORMAL_VAR_ARG, "id");
  Argument second(EMPTY_ARG, 0);
  Argument rhs(CONST_ARG, 0);

  CounterConstraint ccs1(first, second, EQ, rhs); // id==0

  A_req += ccs1;

  A_req += ClockReset(x, Argument(0)); // x-->0
                                       //  A_req.addReset(x, 0); // x-->0

  typename INT_TAS_t::T_t req_wait(req, wait);
  typename INT_TAS_t::CS_t cs2(x, LE, Argument(k)); // x <= k
  req_wait += cs2;

  req_wait += ClockReset(x, Argument(0)); // x-->0
                                          // req_wait.addReset(x, 0); // x-->0
  Argument lhs(NORMAL_VAR_ARG, "id");
  Argument rhs0(PARAMETER_ARG, "pid");
  CounterAction action(lhs, ASSIGNMENT_ACTION, rhs0); // id=pid

  req_wait += action;

  typename INT_TAS_t::T_t wait_req(wait, req);

  wait_req += ClockReset(x, Argument(0)); // x-->0
  //  wait_req.addReset(x, 0);       // x-->0
  wait_req += ccs1; // id==0

  typename INT_TAS_t::T_t wait_cs(wait, cs);

  Argument first1(NORMAL_VAR_ARG, "id");
  Argument second1(PARAMETER_ARG, "pid");
  Argument rhs01(CONST_ARG, 0);
  CounterConstraint ccs2(first1, second1, EQ, rhs01); // id==pid
  wait_cs += ccs2;
  typename INT_TAS_t::CS_t cs3(x, GT, Argument(k)); // x> k
  wait_cs += cs3;

  typename INT_TAS_t::T_t cs_A(cs, A);

  Argument lhs1(NORMAL_VAR_ARG, 0);
  Argument rhs1(CONST_ARG, 0);
  CounterAction caction1(lhs1, ASSIGNMENT_ACTION, rhs1);

  cs_A += caction1;

  ls.push_back(A);

  ls.push_back(req);
  ls.push_back(wait);
  ls.push_back(cs);

  es.push_back(A_req);
  es.push_back(req_wait);
  es.push_back(wait_req);
  es.push_back(wait_cs);
  es.push_back(cs_A);

  tmt1->initial(ls, es, 0);

  for (int i = 1; i <= n; i++) {
    Parameter param = tmt1->getParameter();

    param.setParameterMap("pid",
                          i); // add relation between local id and global id
    shared_ptr<typename INT_TAS_t::Agent_t> tma1 = sys.createAgent(tmt1, param);
  }
  sys.build();
  EXPECT_EQ(sys.getTotalChanNumber(), 0);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  // sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);

  FischerMutual prop;

  EXPECT_FALSE(reacher.satisfy(&data, &prop));
}

TEST(PMCP, FISHER) {
  FisherGenerator F;
  IncrementalCheck<INT_TAS_t, FisherGenerator, FisherProjector> check;
  FischerMutual prop;
  EXPECT_TRUE(check.check(F, &prop));
}

TEST(FISHER, SYMMETRY) {
  int n = 3;
  FisherGenerator F;
  INT_TAS_t sys = F.generate(n);
  Symmetry symm(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  // sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);
  reacher.computeAllReachableSet(&data);

  // EXPECT_TRUE(symm.isSymmetry(data.getStates(), data, manager.get()));
}

TEST(FISHER, equal2) {
  int n = 3;
  FisherGenerator F;
  INT_TAS_t sys = F.generate(n);
  Symmetry symm(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  // sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);
  reacher.computeAllReachableSet(&data);

  StateSet<UINT> states = data.getStates();
  int len = manager->getCompressionSize();
  // UINT *cs1 = new UINT[len];
  int *s = manager->newState();

  vector<UINT> ss = data.getProcess_states();
  vector<vector<int>> vecs1, vecs2;
  int slen = manager->getStateLen();

  for (size_t i = 0; i < data.size(); i++) {
    manager->decode(s, &(ss[i * len]));
    vector<int> dummy(s, s + slen);
    vecs1.push_back(dummy);
    EXPECT_TRUE(data.contain(s));
    EXPECT_TRUE(states.exists(&(ss[i * len])));
  }

  for (auto e : states) {

    manager->decode(s, e);
    vector<int> dummy(s, s + slen);
    vecs2.push_back(dummy);
    EXPECT_TRUE(data.contain(s));
  }
  manager->destroyState(s);

  std::sort(vecs2.begin(), vecs2.end(), vect_cmp<int>);
  std::sort(vecs1.begin(), vecs1.end(), vect_cmp<int>);

  EXPECT_EQ(vecs1.size(), vecs2.size());

  for (size_t i = 0; i < vecs1.size(); i++) {
    EXPECT_EQ(vecs1[i], vecs2[i]);
  }
}

TEST(FISHER, equal3) {
  int n = 3;
  FisherGenerator F;
  INT_TAS_t sys = F.generate(n);
  Symmetry symm(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  // sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);
  reacher.computeAllReachableSet(&data);

  StateSet<UINT> states = data.getStates();
  int len = manager->getCompressionSize();
  // UINT *cs1 = new UINT[len];
  int *s = manager->newState();

  vector<UINT> ss = data.getProcess_states();
  vector<vector<int>> vecs1, vecs2;
  int slen = manager->getStateLen();

  for (size_t i = 0; i < data.size(); i++) {
    manager->decode(s, &(ss[i * len]));
    vector<int> dummy(s, s + slen);
    vecs1.push_back(dummy);
    EXPECT_TRUE(data.contain(s));
    EXPECT_TRUE(states.exists(&(ss[i * len])));
  }

  for (auto e : states) {

    manager->decode(s, e);
    vector<int> dummy(s, s + slen);
    vecs2.push_back(dummy);
    EXPECT_TRUE(data.contain(s));
  }
  manager->destroyState(s);

  std::sort(vecs2.begin(), vecs2.end(), vect_cmp<int>);
  std::sort(vecs1.begin(), vecs1.end(), vect_cmp<int>);

  EXPECT_EQ(vecs1.size(), vecs2.size());

  for (size_t i = 0; i < vecs1.size(); i++) {
    EXPECT_EQ(vecs1[i], vecs2[i]);
  }

  // vector<vector<int>> vecs3 = data.getTest();
  // std::sort(vecs3.begin(), vecs3.end(), vect_cmp<int>);

  // EXPECT_EQ(vecs1.size(), vecs3.size());

  // for (size_t i = 0; i < vecs1.size(); i++) {
  //   EXPECT_EQ(vecs1[i], vecs3[i]);
  // }
}
