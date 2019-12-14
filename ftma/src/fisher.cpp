
#include "benchmark/fisher.h"

namespace graphsat {
typedef typename INT_TAS_t::Agent_t Agent_t;
FisherGenerator::FisherGenerator() : k(2) {
  tmt = sys.createTemplate();
  tmt->addPara("pid");

  ADD_CLOCK((*tmt), x);
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;

  typename INT_TAS_t::L_t A(0);

  typename INT_TAS_t::L_t req(1);
  typename INT_TAS_t::CS_t cs1(x, LE, Argument(k)); // x <= k
  req += cs1;

  typename INT_TAS_t::L_t wait(2);

  typename INT_TAS_t::L_t cs(3);

  typename INT_TAS_t::T_t A_req(A, req);

  Argument first3(NORMAL_VAR_ARG, 0);
  Argument second3(EMPTY_ARG, 0);
  Argument rhs3(CONST_ARG, 0);
  void *ccs1 = createConstraint(first3, second3, EQ, rhs3); // id==0

  A_req.addCounterCons(ccs1);

  A_req.addReset(x, 0); // x-->0

  typename INT_TAS_t::T_t req_wait(req, wait);
  typename INT_TAS_t::CS_t cs2(x, LE, Argument(k)); // x <= k
  req_wait += cs2;

  req_wait.addReset(x, 0); // x-->0

  Argument lhs(NORMAL_VAR_ARG, 0);
  Argument rhs(PARAMETER_ARG, 0);
  CounterAction *caction =
      new CounterAction(lhs, ASSIGNMENT_ACTION, rhs); // id=pid

  req_wait.addCounterAction(caction);

  typename INT_TAS_t::T_t wait_req(wait, req);

  wait_req.addReset(x, 0);       // x-->0
  wait_req.addCounterCons(ccs1); // id==0

  typename INT_TAS_t::T_t wait_cs(wait, cs);

  Argument first4(NORMAL_VAR_ARG, 0);
  Argument second4(PARAMETER_ARG, 0);
  Argument rhs4(CONST_ARG, 0);

  void *ccs2 = createConstraint(first4, second4, EQ, rhs4); // id==pid
  wait_cs.addCounterCons(ccs2);
  typename INT_TAS_t::CS_t cs3(x, GT, Argument(k)); // x> k
  wait_cs += cs3;

  typename INT_TAS_t::T_t cs_A(cs, A);

  Argument lhs2(NORMAL_VAR_ARG, 0);
  Argument rhs2(CONST_ARG, 0);
  CounterAction *caction1 =
      new CounterAction(lhs2, ASSIGNMENT_ACTION, rhs2); // id=0;

  cs_A.addCounterAction(caction1);

  ls.push_back(A);
  ls.push_back(req);
  ls.push_back(wait);
  ls.push_back(cs);

  es.push_back(A_req);
  es.push_back(req_wait);
  es.push_back(wait_req);
  es.push_back(wait_cs);
  es.push_back(cs_A);
  tmt->initial(ls, es, 0);
}
INT_TAS_t FisherGenerator::generate(int n) const {
  INT_TAS_t re(sys);
  re.addType("id_t", 0, n);
  re.addInt("id", 1, 0, n);

  tmt->reset();
  for (int i = 1; i <= n; i++) {
    Parameter param = tmt->getParameter();
    param.setParameterMap(0, i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma(new Agent_t(tmt, param));

    re += tma;
  }
  re.build();

  return re;
}
} // namespace graphsat
