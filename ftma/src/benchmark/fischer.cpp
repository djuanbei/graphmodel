
#include "benchmark/fischer.h"

namespace graphmodel {
typedef typename INT_TAS_t::Agent_t Agent_t;
FischerGenerator::FischerGenerator() : k(3) {}
void FischerGenerator::initial(
    shared_ptr<typename INT_TAS_t::AgentTemplate_t>& tmt) const {
  tmt->addPara("pid");

  ADD_CLOCK((*tmt), x);
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;

  typename INT_TAS_t::L_t* A = tmt->createLocation("A");

  typename INT_TAS_t::L_t* req = tmt->createLocation("req");

  typename INT_TAS_t::CS_t cs1(x, LE, Argument(k));  // x <= k
  (*req) += cs1;

  typename INT_TAS_t::L_t* wait = tmt->createLocation("wait");

  typename INT_TAS_t::L_t* cs = tmt->createLocation("cs");

  typename INT_TAS_t::T_t* A_req = tmt->createTransition(A, req);

  Argument first3(NORMAL_VAR_ARG, "id");
  Argument second3;
  Argument rhs3(0);
  CounterConstraint ccs1(first3, second3, EQ, rhs3);  // id==0

  (*A_req) += ccs1;
  ClockReset reset(x, Argument(0));
  (*A_req) += reset;  // x-->0

  typename INT_TAS_t::T_t* req_wait = tmt->createTransition(req, wait);
  typename INT_TAS_t::CS_t cs2(x, LE, Argument(k));  // x <= k
  (*req_wait) += cs2;

  (*req_wait) += ClockReset(x, Argument(0));  // x-->0

  Argument lhs(NORMAL_VAR_ARG, "id");
  Argument rhs(PARAMETER_ARG, "pid");
  CounterAction caction(lhs, ASSIGNMENT_ACTION, rhs);  // id=pid

  (*req_wait) += caction;

  typename INT_TAS_t::T_t* wait_req = tmt->createTransition(wait, req);

  (*wait_req) += ClockReset(x, Argument(0));  // x-->0

  (*wait_req) += ccs1;  // id==0

  typename INT_TAS_t::T_t* wait_cs = tmt->createTransition(wait, cs);

  Argument first4(NORMAL_VAR_ARG, "id");
  Argument second4(PARAMETER_ARG, "pid");
  Argument rhs4(0);

  CounterConstraint ccs2(first4, second4, EQ, rhs4);  // id==pid
  (*wait_cs) += ccs2;
  typename INT_TAS_t::CS_t cs3(x, GT, Argument(k));  // x> k
  (*wait_cs) += cs3;

  typename INT_TAS_t::T_t* cs_A = tmt->createTransition(cs, A);

  Argument lhs2(NORMAL_VAR_ARG, "id");
  Argument rhs2(0);
  CounterAction caction1(lhs2, ASSIGNMENT_ACTION, rhs2);  // id=0;

  (*cs_A) += caction1;

  tmt->initial(0);
}
INT_TAS_t FischerGenerator::generate(int n) const {
  INT_TAS_t re(sys);

  re.addType("id_t", 0, n);
  re.addInt("id", 1, 0, n);

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> tmt = re.createTemplate("P");
  initial(tmt);

  for (int i = 1; i <= n; i++) {
    Parameter param = tmt->getParameter();
    param.setParameterMap("pid", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma = re.createAgent(tmt, param);
    // re += tma;
  }
  //  re.build();
  re.getStateManager();
  return re;
}
}  // namespace graphmodel
