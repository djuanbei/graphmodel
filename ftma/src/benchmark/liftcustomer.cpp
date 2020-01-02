#include "benchmark/liftcustomer.h"

namespace graphsat {

INT_TAS_t LiftCustomer::generate(int n) const {
  INT_TAS_t sys;

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> lift_tmt =
      sys.createTemplate("Lift");
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> customer_tmt =
      sys.createTemplate("Costomer");
  string wait_counter = "wait_lift_len";
  string stop_counter = "stop_cus_len";
  sys.addInt(wait_counter, 1);
  sys.addInt(stop_counter, 1);

  vector<typename INT_TAS_t::T_t> lift_es;
  vector<typename INT_TAS_t::L_t> lift_ls;

  typename INT_TAS_t::L_t Ie(0, "Ie");

  typename INT_TAS_t::L_t near(1, "near");

  typename INT_TAS_t::L_t stop(2, "stop");
  typename INT_TAS_t::L_t cross(3, "cross");

  lift_ls.push_back(Ie);

  lift_ls.push_back(near);
  lift_ls.push_back(stop);
  lift_ls.push_back(cross);

  typename INT_TAS_t::T_t Ie_near(Ie, near);

  typename INT_TAS_t::T_t near_stop(near, stop);
  Argument first1(NORMAL_VAR_ARG, wait_counter);
  Argument second1(0);
  Argument rhs1(0);

  CounterConstraint cs1(first1, second1, GT, rhs1);
  near_stop += cs1;  // wait_cus_num >0
  Argument lhs2(NORMAL_VAR_ARG, stop_counter);
  Argument rhs2(1);
  CounterAction action1(lhs2, SELF_INC_ACTION, rhs2);
  near_stop += action1;  // stop_lift_num+=1;

  typename INT_TAS_t::T_t stop_cross(stop, cross);

  Argument first2(NORMAL_VAR_ARG, wait_counter);
  Argument second2(0);
  Argument rhs21(0);

  CounterConstraint cs2(first2, second2, EQ, rhs21);
  stop_cross += cs2;  // wait_cus_num==0

  Argument lhs3(NORMAL_VAR_ARG, stop_counter);
  Argument rhs3(1);
  CounterAction action2(lhs2, SELF_DEC_ACTION, rhs3);
  stop_cross += action2;  // stop_lift_num-=1;

  typename INT_TAS_t::T_t near_cross(near, cross);
  near_cross += cs2;

  lift_es.push_back(Ie_near);
  lift_es.push_back(near_stop);
  lift_es.push_back(stop_cross);
  lift_es.push_back(near_cross);
  lift_tmt->initial(lift_ls, lift_es, 0);

  vector<typename INT_TAS_t::T_t> cus_es;
  vector<typename INT_TAS_t::L_t> cus_ls;
  typename INT_TAS_t::L_t Ic(0, "Ic");

  // typename INT_TAS_t::L_t out(1, "out");

  typename INT_TAS_t::L_t wait(1, "wait");

  typename INT_TAS_t::L_t in(2, "in");
  cus_ls.push_back(Ic);

  cus_ls.push_back(wait);
  cus_ls.push_back(in);

  typename INT_TAS_t::T_t Ic_wait(Ic, wait);
  //  typename INT_TAS_t::T_t out_wait(out, wait);

  Argument lhs5(NORMAL_VAR_ARG, wait_counter);

  Argument rhs5(1);
  CounterAction action5(lhs5, SELF_INC_ACTION, rhs5);
  //  out_wait+= action5;

  typename INT_TAS_t::T_t wait_in(wait, in);
  CounterAction action6(lhs5, SELF_DEC_ACTION, rhs5);
  wait_in += action6;

  Argument first5(NORMAL_VAR_ARG, stop_counter);
  Argument second5(0);
  Argument rhs51(0);
  CounterConstraint cs5(first5, second5, GT, rhs51);
  wait_in += cs5;

  cus_es.push_back(Ic_wait);
  //  cus_es.push_back(out_wait) ;
  cus_es.push_back(wait_in);

  customer_tmt->initial(cus_ls, cus_es, 0);

  Parameter param = lift_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma =
      sys.createAgent(lift_tmt, param);
  for (int i = 0; i < n; i++) {
    Parameter param = customer_tmt->getParameter();
    shared_ptr<typename INT_TAS_t::Agent_t> tma =
        sys.createAgent(customer_tmt, param);
  }
  //  sys.build();
  return sys;
}

}  // namespace graphsat
