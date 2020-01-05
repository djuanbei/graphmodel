

/**
 * @file   main.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Mar 30 18:10:24 2019
 *
 * @brief  a test main
 *
 *
 */
//#define ONLINE_CHECK

//#define PRINT_STATE

//#define YYDEBUG 1

#include "example.h"
#include "log/logset.h"

#include "benchmark/train_gate.h"
#include "io/uppaalmodelparser.h"

#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"
#include "problem/reachability.hpp"
#include "state/reachableset.hpp"

#include "benchmark/train_gate.h"

#include "benchmark/train_gate_projector.h"

#include "problem/pmcp.hpp"

#include "benchmark/fischer.h"

#include "benchmark/fischer_projector.h"

#include "alg/symmetry.h"

#include "benchmark/liftcustomer.h"

#include <iostream>

extern int yy_flex_debug;

using std::vector;
using namespace graphsat;

void test() {
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
void test1() {
  int n = 3;
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

void lift_customer() {
  LiftCustomer liftc;
  INT_TAS_t sys = liftc.generate(2);
  shared_ptr<typename INT_TAS_t::StateManager_t> manager =
      sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(manager);
  // sys.addInitState(data);
  Reachability<INT_TAS_t> reacher(sys);
  reacher.computeAllReachableSet(&data);
  int* state = manager->newState();
  for (size_t i = 0; i < data.size(); i++) {
    data.getStateAt(state, i);
    manager->dump(state);
  }
  manager->destroyState(state);
  cout << data.size() << endl;
}

int main(int argc, const char* argv[]) {
  // test();
  // return 0;
  incrementalTest1();
  return 0;
  // lift_customer();
  //  return 0;
  // test1();
  // return 0;
  // train_gate(3);
  fischer(2);
  fischer(3);
  fischer(4);
  fischer(5);
  return 0;

  // test();
  // return 0;

  // google::InitGoogleLogging( argv[ 0 ] );
  // logSet();
  // TrainGate tg;
  // tg.generate( 2);
  // return 0;
  // set_log_dir_2("./log");
  // google::InitGoogleLogging("testlog");
  // LOG( ERROR ) << "Found " << 2 << " cookies";
  incrementalTest1();
  //  google::ShutdownGoogleLogging();
  return 0;

  runxml("/Users/yunyun/mycode/c++/graphmodel/ftma/example/2doors.xml");
  return 0;
  if (argc > 1) {
    runxml(argv[1]);
    return 0;
  }
  fischer(2);
  return 0;
  testIsConsistent();

  return 0;
  yy_flex_debug = 1;
  //    incrementalTest( );
  // return 0;
  //  testcompression( );
  //  return 0;
  //  testOP( );
  //  return 9;
  //  example2( );
  //  return 0;

  //  fischer( 2 );
  // return 0;
  //  example5();
  //  return 0;
  //  State<int> s;
  //  cout<<"xml"<<endl;
  fisher1();

  return 0;

  // example1();
  // example2();
  // return 0;
  // typename INT_TAS_t::CS_t cons( 1, 2, LE, 2 ); // x-y <= 2

  // cout << "constrain: " << cons << endl;

  // cout << "negation constraint: " << cons.neg() << endl;
  // // insert code here...
  // DBMFactory exampleDBM( 4 );
  // int *      D = exampleDBM.randomDBM();
  // cout << "matrix dump :\n";
  // exampleDBM.dump( cout, D ) << endl;

  // cout << "========================" << endl;
  // exampleDBM.canonicalForm( D );
  // cout << "matrix dump :\n";
  // exampleDBM.dump( cout, D ) << endl;

  // cout << "========================" << endl;
  // exampleDBM.canonicalForm( D );
  // cout << "matrix dump :\n";
  // exampleDBM.dump( cout, D ) << endl;
  // std::cout << "Hello, World!\n";

  // int *D1 = exampleDBM.createDBM();
  // cout << "matrix dump :\n";
  // exampleDBM.dump( cout, D1 ) << endl;
  // int *D2 = exampleDBM.reset( D1, 1, (int) 10 );
  // cout << "matrix dump :\n";
  // exampleDBM.dump( cout, D2 ) << endl;

  // // cout<<"constrain: "<<cons<<endl;

  // // //C_t*  D3=exampleDBM.And( D2, cons.neg( ));
  // int *D3 = exampleDBM.reset( D2, 2, (int) 10 );

  // cout << "matrix dump :\n";
  // exampleDBM.dump( cout, D3 ) << endl;
  /*
   vector<L_t> locs;

   vector<T_t> es;

   tma<L_t, T_t> tma1( locs, es, 0, 3 );

   reach<C_t, L_t, T_t> RETMA( tma1 );
   */
  return 0;
}
