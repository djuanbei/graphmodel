
#include "example.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>

#include "action/counteraction.h"
#include "alg/symmetry.h"
#include "alg/util.h"
#include "benchmark/fischer.h"
#include "benchmark/fischer_projector.h"
#include "benchmark/liftcustomer.h"
#include "benchmark/liftcustomer_projector.h"
#include "benchmark/liftcustomerproperty.h"
#include "benchmark/train_gate.h"
#include "benchmark/train_gate_projector.h"
#include "constraint/clockdiffcons.h"
#include "domain/dbm.h"
#include "domain/dbmset.hpp"
#include "io/stateout.h"
#include "io/uppaalmodelparser.h"
#include "log/logset.h"
#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"
#include "problem/pmcp.hpp"
#include "problem/reachability.hpp"
#include "property/fisherprop.h"
#include "property/locreachprop.h"
#include "state/discretestate.hpp"
#include "state/reachableset.hpp"
#include "util/datacompression.h"
#include "util/dbmutil.hpp"

using std::vector;
using namespace graphmodel;

typedef AgentSystem<Location, Transition> INT_TAS_t;
typedef typename INT_TAS_t::Agent_t Agent_t;
// typename INT_TAS_t::AgentTemplate_t                       tat;
typedef ReachableSet<INT_TAS_t::StateManager_t> R_t;

typedef Reachability<INT_TAS_t> RS_t;

namespace graphmodel {

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, 1000);

void example2(void) {
  INT_TAS_t sys;
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> tmt1 =
      sys.createTemplate("P");
  Argument x = tmt1->addClock("x");
  Argument y = tmt1->addClock("y");
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;
  typename INT_TAS_t::L_t* L0 = tmt1->createLocation(0);
  typename INT_TAS_t::L_t* L1 = tmt1->createLocation(1);

  typename INT_TAS_t::T_t* E00a = tmt1->createTransition(L0, L0);
  Clock ZERO;
  (*E00a) += ClockReset(y, Argument(0));  // y-->0
  // E00a.addReset(y, 0);                              // y-->0
  typename INT_TAS_t::CS_t cs1(y, LE, Argument(2));  // y<=2
  (*E00a) += cs1;

  typename INT_TAS_t::T_t* E00b = tmt1->createTransition(L0, L0);
  // pair<int, int>          reset2( x, 0 );
  (*E00b) += ClockReset(x, Argument(0));  // x-->0
  // E00b.addReset(x, 0);                              // x-->0
  typename INT_TAS_t::CS_t cs2(x, LE, Argument(2));  // x<=2
  (*E00b) += cs2;

  typename INT_TAS_t::T_t* E01 = tmt1->createTransition(L0, L1);

  typename INT_TAS_t::CS_t cs3(y, LE, Argument(2));  // y<=2
  typename INT_TAS_t::CS_t cs4(x, GE, Argument(4));  // x>=4

  (*E01) += cs3;
  (*E01) += cs4;

  tmt1->initial(0);

  Parameter param = tmt1->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma1 = sys.createAgent(tmt1, param);

  // sys += tma1;
  //  sys.build();
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  R_t data(manager);
  // sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);
  Property prop;

  if (reacher.satisfy(&data, &prop)) {
    cout << "right" << endl;
  } else {
    cout << "there is something wrong" << endl;
  }
}

int* randV(int n, int s) {
  int* v = new int[n];

  for (int i = 0; i < n; i++) {
    v[i] = distribution(generator);
  }

  return v;
}

// void example5(void) {
//   StateSet<UINT> sets;
//   vector<UINT *> vecs;
//   int n = 20;
//   int s = 4;
//   int num = 1000;
//   for (int i = 0; i < num; i++) {
//     int *temp = randV(n, s);
//     if (sets.add(temp)) {
//       vecs.push_back(temp);
//     } else {
//       delete temp;
//     }
//   }
//   for (auto e : vecs) {
//     sets.contain(e);
//   }
// }

void example6() {
  vector<int*> vecs;
  int n = 10;
  DBMManager manager(n);
  DBMset<int> sets(manager);
  int num = 1000;

  for (int i = 0; i < num; i++) {
    int* dbm = manager.randomFeasiableDBM();
    if (sets.add(dbm)) {
      vecs.push_back(dbm);
    } else {
      manager.destroyDBM(dbm);
    }
  }
  for (auto e : vecs) {
    sets.contain(e);
  }
}

void fischer(int n) {
  INT_TAS_t sys;
  sys.addType("id_t", 0, n);
  sys.addInt("id", 1, 0, n);

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> tmt1 =
      sys.createTemplate("P");
  tmt1->addPara("pid");

  Argument x = tmt1->addClock("x");

  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;
  int k = 3;

  typename INT_TAS_t::L_t* A = tmt1->createLocation("A");

  typename INT_TAS_t::L_t* req = tmt1->createLocation("req");
  typename INT_TAS_t::CS_t cs1(x, LE, Argument(k));  // x <= k
  (*req) += cs1;

  typename INT_TAS_t::L_t* wait = tmt1->createLocation("wait");

  typename INT_TAS_t::L_t* cs = tmt1->createLocation("cs");

  typename INT_TAS_t::T_t* A_req = tmt1->createTransition(A, req);
  Argument first(NORMAL_VAR_ARG, "id");
  Argument second;
  Argument rhs(0);

  CounterConstraint ccs1(first, second, EQ, rhs);  // id==0

  (*A_req) += ccs1;

  (*A_req) += ClockReset(x, Argument(0));  // x-->0

  typename INT_TAS_t::T_t* req_wait = tmt1->createTransition(req, wait);
  typename INT_TAS_t::CS_t cs2(x, LE, Argument(k));  // x <= k
  (*req_wait) += cs2;

  (*req_wait) +=
      ClockReset(x, Argument(0));  // x-->0
                                   //  req_wait.addReset(x, 0); // x-->0
  Argument lhs(NORMAL_VAR_ARG, "id");
  Argument rhs0(PARAMETER_ARG, "pid");
  CounterAction action(lhs, ASSIGNMENT_ACTION, rhs0);  // id=pid

  (*req_wait) += action;

  typename INT_TAS_t::T_t* wait_req = tmt1->createTransition(wait, req);

  (*wait_req) += ClockReset(x, Argument(0));  // x-->0
  // wait_req.addReset(x, 0);       // x-->0
  (*wait_req) += ccs1;  // id==0

  typename INT_TAS_t::T_t* wait_cs = tmt1->createTransition(wait, cs);

  Argument first1(NORMAL_VAR_ARG, "id");
  Argument second1(PARAMETER_ARG, "pid");
  Argument rhs01(CONST_ARG, 0);
  CounterConstraint ccs2(first1, second1, EQ, rhs01);  // id==pid
  (*wait_cs) += ccs2;
  typename INT_TAS_t::CS_t cs3(x, GT, Argument(k));  // x> k
  (*wait_cs) += cs3;

  typename INT_TAS_t::T_t* cs_A = tmt1->createTransition(cs, A);

  Argument lhs1(NORMAL_VAR_ARG, "id");
  Argument rhs1(CONST_ARG, 0);
  CounterAction caction1(lhs1, ASSIGNMENT_ACTION, rhs1);

  (*cs_A) += caction1;

  tmt1->initial(0);
  tmt1->dump2Dot("fischer.gv");

  for (int i = 1; i <= n; i++) {
    Parameter param = tmt1->getParameter();

    param.setParameterMap("pid", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma1 = sys.createAgent(tmt1, param);
  }

  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  R_t data(manager);

  Reachability<INT_TAS_t> reacher(sys);
  FischerMutual prop;

  if (reacher.satisfy(&data, &prop)) {
    cout << "There is something wrong" << endl;
  } else {
    cout << "fischer mutual exclusion property check right" << endl;
  }

  cout << "reach data size: " << data.size() << endl;
  StateOutput::generatorDot(data, "test.gv");

  int* state = manager->newState();
  vector<int> clock_ids;
  clock_ids.push_back(1);
  vector<vector<int>> one_states;
  for (size_t i = 0; i < data.size(); i++) {
    vector<int> dummy;

    data.getStateAt(state, i);
    dummy.push_back(state[0]);
    dummy.push_back(*manager->getValue(0, "id", state) == 1);
    dummy.push_back(*manager->getValue(0, "id", state) > 0);
    int* dbm = manager->getDBM(state);
    manager->getClockManager().encode(dbm);
    dummy.push_back(manager->getClockManager().at(dbm, 0, 0));
    dummy.push_back(manager->getClockManager().at(dbm, 0, 1));
    dummy.push_back(manager->getClockManager().at(dbm, 1, 0));
    dummy.push_back(manager->getClockManager().at(dbm, 1, 1));

    one_states.push_back(dummy);
  }

  std::vector<vector<int>>::iterator it;
  sort(one_states.begin(), one_states.end(), vect_cmp<int>);
  it = std::unique(one_states.begin(), one_states.end());
  one_states.resize(std::distance(one_states.begin(), it));
  cout << "size: " << one_states.size() << endl;
  manager->destroyState(state);

  for (size_t i = 0; i < one_states.size(); i++) {
    int* state = &(one_states[i][0]);

    cout << i + 1 << ": " << sys.getLocationName(0, state[0])
         << ", ";  // << state[1]<<", " <<state[2] << endl;
    if (state[1] == 1) {
      cout << "true"
           << ", ";
    } else {
      cout << "false"
           << ", ";
    }
    if (state[2] == 1) {
      cout << "true"
           << ", ";
    } else {
      cout << "false"
           << ", ";
    }
    cout << endl;

    //<< sys.getLocationName(0, state[0]) << ", " << state[1]<<", " <<state[2]
    //<< endl;

    int* dbm = state + 3;
    manager->getClockManager().dump(cout, dbm, 2);
  }
}

void testIsConsistent() {
  int n = 12;

  DBMManager df(n);
  for (int i = 0; i < 5; i++) {
    int* d = df.randomFeasiableDBM();
    ClockConstraint cons = df.getCons(d, 2, 3);
    cout << cons << endl;
    ClockConstraint cons1 = cons.neg();

    cout << cons1 << endl;
    cout << "============" << endl;
    df.andImpl(d, cons1);

    assert(!df.isConsistent(d));

    df.destroyDBM(d);
  }
}

void incrementalTestFS() {
  FischerGenerator F;
  IncrementalCheck<INT_TAS_t, FischerGenerator, FischerProjector> check;
  FischerMutual prop;

  if (check.check(F, &prop)) {
    cout << "ok" << endl;
  } else {
    cout << "fail" << endl;
  }
}
void incrementalTestTG() {
  TrainGate TG;
  IncrementalCheck<INT_TAS_t, TrainGate, TrainGateProjector> check;
  TrainGatePro prop(2);
  prop.setCS(4);
  if (check.check(TG, &prop)) {
    cout << "ok" << endl;
  } else {
    cout << "no" << endl;
  }
}

void incrementalTestLC() {
  LiftCustomer generator;
  IncrementalCheck<INT_TAS_t, LiftCustomer, LiftCustomerProjector> check;
  INT_TAS_t sys = generator.generate(1);
  int overload_loc = sys.getLocationID(0, "overload");
  std::vector<int> locs;
  locs.push_back(overload_loc);

  LiftCustomerProperty prop(locs);

  if (check.check(generator, &prop)) {
    cout << "ok" << endl;
  } else {
    cout << "no" << endl;
  }
}

void incrementalTest() {
  INT_TAS_t sys;
  int n = 3;
  sys.addType("id_t", 0, n);
  sys.addInt("id", 1, 0, n);

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> tmt1 =
      sys.createTemplate("P");
  tmt1->addPara("pid");

  ADD_CLOCK((*tmt1), x);

  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;
  int k = 2;

  typename INT_TAS_t::L_t* A = tmt1->createLocation();

  typename INT_TAS_t::L_t* req = tmt1->createLocation();
  typename INT_TAS_t::CS_t cs1(x, LE, Argument(k));  // x <= k
  (*req) += cs1;

  typename INT_TAS_t::L_t* wait = tmt1->createLocation();

  typename INT_TAS_t::L_t* cs = tmt1->createLocation();

  typename INT_TAS_t::T_t* A_req = tmt1->createTransition(A, req);

  Argument first3(NORMAL_VAR_ARG, 0);
  Argument second3(EMPTY_ARG, 0);
  Argument rhs3(CONST_ARG, 0);
  CounterConstraint ccs1(first3, second3, EQ, rhs3);  // id==0

  (*A_req) += ccs1;

  (*A_req) += ClockReset(x, Argument(0));  // x-->0
  // A_req.addReset(x, 0); // x-->0

  typename INT_TAS_t::T_t* req_wait = tmt1->createTransition(req, wait);
  typename INT_TAS_t::CS_t cs2(x, LE, Argument(k));  // x <= k
  (*req_wait) += cs2;

  (*req_wait) +=
      ClockReset(x, Argument(0));  // x-->0
                                   // req_wait.addReset(x, 0); // x-->0

  Argument lhs(NORMAL_VAR_ARG, 0);
  Argument rhs(PARAMETER_ARG, 0);
  CounterAction caction(lhs, ASSIGNMENT_ACTION, rhs);  // id=pid

  (*req_wait) += caction;

  typename INT_TAS_t::T_t* wait_req = tmt1->createTransition(wait, req);

  (*wait_req) += ClockReset(x, Argument(0));  // x-->0
  // wait_req.addReset(x, 0);       // x-->0
  (*wait_req) += ccs1;  // id==0

  typename INT_TAS_t::T_t* wait_cs = tmt1->createTransition(wait, cs);

  Argument first4(NORMAL_VAR_ARG, 0);
  Argument second4(PARAMETER_ARG, 0);
  Argument rhs4(CONST_ARG, 0);

  CounterConstraint ccs2(first4, second4, EQ, rhs4);  // id==pid
  (*wait_cs) += ccs2;
  typename INT_TAS_t::CS_t cs3(x, GT, Argument(k));  // x> k
  (*wait_cs) += cs3;

  typename INT_TAS_t::T_t* cs_A = tmt1->createTransition(cs, A);

  Argument lhs2(NORMAL_VAR_ARG, 0);
  Argument rhs2(CONST_ARG, 0);
  CounterAction caction1(lhs2, ASSIGNMENT_ACTION, rhs2);  // id=0;

  (*cs_A) += caction1;

  tmt1->initial(0);

  for (int i = 1; i <= n; i++) {
    Parameter param = tmt1->getParameter();
    param.setParameterMap("pid", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma1 = sys.createAgent(tmt1, param);
  }

  // Counter counter( 0, 100 );
  // sys.setCounterNum( 1 );
  // sys.setCounter( 0, counter );
  //  sys += counter;

  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  R_t data(manager);
  // sys.addInitState(data);

  Reachability<INT_TAS_t> reacher(sys);
  FischerMutual prop;

  if (reacher.satisfy(&data, &prop)) {
    cout << "There is something wrong" << endl;
  } else {
    cout << "fischer mutual exclusion property check right" << endl;
  }

  cout << "reach data size: " << data.size() << endl;
  vector<vector<int>> project;
  FischerProjector proj(manager, 2);
  data.project(proj, project);

  INT_TAS_t sys1;
  for (int i = 1; i <= 2; i++) {
    Parameter param = tmt1->getParameter();
    param.setParameterMap("pid", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma1 =
        sys1.createAgent(tmt1, param);

    // sys1 += tma1;
  }
  // sys1.setCounterNum( 1 );
  // sys1.setCounter( 0, counter );
  //  sys1 += counter;

  shared_ptr<INT_TAS_t::StateManager_t> manager1 = sys1.getStateManager();
  R_t data1(manager1);
  // sys.addInitState(data1);

  Reachability<INT_TAS_t> reacher1(sys1);
  FischerMutual prop1;

  if (reacher1.satisfy(&data1, &prop1)) {
    cout << "There is something wrong" << endl;
  } else {
    cout << "fischer mutual exclusion property check right" << endl;
  }

  cout << "reach data size: " << data1.size() << endl;
  vector<vector<int>> project1;
  FischerProjector proj1(manager1, 2);
  data1.project(proj1, project1);
  int num = 0;
  size_t mm = project[0].size();
  for (size_t i = 0; i < project.size(); i++) {
    size_t j = 0;
    for (; j < project1.size(); j++) {
      size_t k = 0;
      for (; k < 2; k++) {
        if (project[i][k] != project1[j][k]) {
          //   cout<<k<<" "<<project[ i][ k]<<" "<<project1[ j][ k]<<endl;
          break;
        }
      }
      if (k == 2) {
        for (; k < mm; k++) {
          if (project[i][k] > project1[j][k]) {
            //   cout<<k<<" "<<project[ i][ k]<<" "<<project1[ j][ k]<<endl;
            break;
          }
        }
      }
      if (k == mm) {
        break;
      }
    }
    if (j == project1.size()) {
      cout << num++ << endl;
      cout << "Can not hold" << endl;
    }
  }
  cout << "ok" << endl;
}

void fischer1() {
  UppaalParser parser(
      "/Users/yunyun/mycode/c++/graphmodel/ftma/example/2doors.xml");
  //  UppaalParser parser( "/Users/yun/mycode/c++/ftma/ftma/example/test1.xml"
  //  );
  INT_TAS_t sys = parser.getSYS();
  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  R_t data(manager);
  // sys.addInitState(data);

  Reachability<INT_TAS_t> reacher(sys);
  //  FischerMutual     prop;
  //  if ( reacher.satisfy( &prop ) ) {
  //   cout << "There is something wrong" << endl;
  // } else {

  //   cout << "fischer mutual exclusion property check right" << endl;
  // }
  reacher.computeAllReachableSet(&data);
  //  cout << "reach data size: " << data.size() << endl;
  StateOutput::generatorDot(data, "test.gv");
  // data.generatorDot("test.gv");
}

// void runxml( const string &file_name ) {

//   UppaalParser              parser( file_name );
//   INT_TAS_t                 sys     = parser.getSYS();
//   INT_TAS_t::StateManager_t manager = sys.getStateManager();
//   R_t                       data( manager );

//   sys.addInitState( data, manager );

//   Reachability<INT_TAS_t> reacher( sys );

//   reacher.computeAllReachableSet( data );
//   cout << "reach data size: " << data.size() << endl;
//   data.generatorDot( "test.gv" );
// }

void testOP() {
  for (int i = 0; i < 10; i++) {
    cout << "<  " << i - 5 << " " << getMatrixValue(i - 5, true) << endl;
    cout << "<= " << i - 5 << " " << getMatrixValue(i - 5, false) << endl
         << endl;
    ;
  }
}

void testcompression() {
  const int len = 10;
  Compression<int> data(len);
  int low = -6;
  int up = 10;
  for (int i = 0; i < len; i++) {
    data.setBound(i, low, up);
  }
  UINT cd[len];
  int dd[len];
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(low, up - 1);

  int d[len];
  for (int i = 0; i < 10000; i++) {
    for (int j = 0; j < len; j++) {
      d[j] = distribution(generator);
    }
    data.encode(cd, d);
    data.decode(dd, cd);
    for (int j = 0; j < len; j++) {
      assert(dd[j] == d[j]);
    }
  }
}
void train_gate(const int n) {
  TrainGate TG;

  INT_TAS_t tg_sys = TG.generate(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      tg_sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  //  tg_sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(tg_sys);

  TrainGatePro prop(n);
  prop.setCS(4);

  if (reacher.satisfy(&data, &prop)) {
    cout << "some thing is wrong!" << endl;
  }
  StateOutput::generatorDot(data, "test.gv");
  // data.generatorDot("test.gv");
}

void lift_customer(int n) {
  LiftCustomer liftc;
  INT_TAS_t sys = liftc.generate(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);

  Reachability<INT_TAS_t> reacher(sys);
  int overload_loc = sys.getLocationID(0, "overload");
  std::vector<int> locs;
  locs.push_back(overload_loc);

  LiftCustomerProperty prop(locs);

  if (reacher.satisfy(&data, &prop)) {
    cout << "There is something wrong" << endl;
  } else {
    cout << "Lift costomer without overload property check right" << endl;
  }

  //  reacher.computeAllReachableSet(&data);
  int* state = manager->newState();
  set<vector<int>> proj2;
  for (size_t i = 0; i < data.size(); i++) {
    data.getStateAt(state, i);
    vector<int> dummy;
    dummy.push_back(state[0]);
    dummy.push_back(state[1]);
    dummy.push_back(state[2]);

    proj2.insert(dummy);
    manager->dump(state);
  }
  manager->destroyState(state);
  cout << data.size() << endl;
  StateOutput::generatorDot(data, "test.gv");
  cout << "2 proj size: " << proj2.size() << endl;
  for (auto e : proj2) {
    cout << sys.getLocationName(0, e[0]) << ", " << sys.getLocationName(1, e[1])
         << ", " << sys.getLocationName(1, e[2]) << endl;
  }
}

void fischerSymmetry(int n) {
  FischerGenerator F;
  INT_TAS_t sys = F.generate(n);
  Symmetry symm(n);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  // sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);
  reacher.computeAllReachableSet(&data);

  if (symm.isSymmetry(data.getStates(), data, manager.get())) {
    cout << "ok" << endl;
  } else {
    cout << "error" << endl;
  }
}

}  // namespace graphmodel
