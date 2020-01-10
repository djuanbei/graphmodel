#include "benchmark/liftcustomer.h"

namespace graphsat {

INT_TAS_t LiftCustomer::generate(int n) const {
  INT_TAS_t sys;

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> lift_tmt =
      sys.createTemplate("Lift");
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> customer_tmt =
      sys.createTemplate("Costomer");
  string wait_cus_num = "wait_cus_len"; 
  string stop_lift_num = "stop_lift_len";
  
  sys.addChan("ask", 1, BROADCAST_CH);  //ask
  sys.addChan("enter", 1, ONE2ONE_CH);  //enter
  
  sys.addInt(wait_cus_num, 1);
  sys.addInt(stop_lift_num, 1);
  

  typename INT_TAS_t::L_t* Ie = lift_tmt->createLocation("Ie");

  //typename INT_TAS_t::L_t* near = lift_tmt->createLocation("near");

  typename INT_TAS_t::L_t* stop = lift_tmt->createLocation("stop");

  typename INT_TAS_t::L_t* one = lift_tmt->createLocation("one");

  //typename INT_TAS_t::L_t* two = lift_tmt->createLocation("two");

  typename INT_TAS_t::L_t* cross = lift_tmt->createLocation("cross");

  typename INT_TAS_t::L_t* overload = lift_tmt->createLocation("overload");
  
  

  //lift_tmt->createTransition(Ie, near);

  lift_tmt->createTransition(Ie, cross);
  
  
  

  typename INT_TAS_t::T_t* Ie_stop = lift_tmt->createTransition(Ie, stop);
  

  Argument lhs2(NORMAL_VAR_ARG, stop_lift_num);
  Argument rhs2(1);
  CounterAction action1(lhs2, SELF_INC_ACTION, rhs2);
  (*Ie_stop) += action1;  // stop_lift_num+=1;


  Argument ch_ask(NORMAL_VAR_ARG, "ask");

  Ie_stop->setChannel( Channel( ch_ask, CHANNEL_RECEIVE ));


  typename INT_TAS_t::T_t* Ie_stop2 = lift_tmt->createTransition(Ie, stop);
  (*Ie_stop2) += action1;  // stop_lift_num+=1;


  Argument first10(NORMAL_VAR_ARG, wait_cus_num);
  Argument second10(0);
  Argument rhs10(0);
  CounterConstraint cs10(first10, second10, GT, rhs10);
  (*Ie_stop2) += cs10;  // wait_cus_num>0;

  

  typename INT_TAS_t::T_t* stop_one= lift_tmt->createTransition( stop, one);
  Argument ch_enter(NORMAL_VAR_ARG, "enter");

  stop_one->setChannel( Channel( ch_enter, CHANNEL_RECEIVE));


  //typename INT_TAS_t::T_t* one_two=lift_tmt->createTransition( one, two);


  //one_two->setChannel(Channel( ch_enter, CHANNEL_RECEIVE));
  

  typename INT_TAS_t::T_t* one_overload=lift_tmt->createTransition(  one, overload);

  one_overload->setChannel(Channel( ch_enter, CHANNEL_RECEIVE));

  Argument lhs3(NORMAL_VAR_ARG, stop_lift_num);
  Argument rhs3(1);
  CounterAction action2(lhs2, SELF_DEC_ACTION, rhs3);
  (*one_overload) += action2;  // stop_lift_num-=1;

  typename INT_TAS_t::T_t* one_cross= lift_tmt->createTransition( one, cross);
  (*one_cross)+=action2;
  
  one_cross->setChannel( Channel( ch_ask, CHANNEL_RECEIVE ));


  typename INT_TAS_t::T_t* one_cross2=
  lift_tmt->createTransition( one, cross);
  (*one_cross2)+=action2;


  lift_tmt->createTransition( cross, Ie);


  lift_tmt->initial(0);

  


  
  typename INT_TAS_t::L_t* Ic = customer_tmt->createLocation("Ic");

  typename INT_TAS_t::L_t* wait = customer_tmt->createLocation("wait");

  typename INT_TAS_t::L_t* in = customer_tmt->createLocation("in");

  
  

  typename INT_TAS_t::T_t *Ic_wait =customer_tmt->createTransition(Ic, wait);

  
  Argument first7(NORMAL_VAR_ARG, wait_cus_num);
  Argument second7(0);
  Argument rhs7(0);
  CounterConstraint cs7(first7, second7, EQ, rhs7);
  ( *Ic_wait)+=cs7; //wait_cus_num==0
  Ic_wait->setChannel( Channel( ch_ask, CHANNEL_SEND ));
  
  


  Argument lhs5(NORMAL_VAR_ARG, wait_cus_num);

  Argument rhs5(1);
  
  CounterAction action5(lhs5, SELF_INC_ACTION, rhs5);

  (* Ic_wait)+=action5; //wait_cus_num++
  


  

  typename INT_TAS_t::T_t* wait_in = customer_tmt->createTransition(wait, in);

  
  CounterAction action6(lhs5, SELF_DEC_ACTION, rhs5);
  (*wait_in) += action6; //wait_cus_num--
  

  Argument first5(NORMAL_VAR_ARG, stop_lift_num);
  Argument second5(0);
  Argument rhs51(0);
  CounterConstraint cs5(first5, second5, GT, rhs51);
  (*wait_in) += cs5; //stop_lift_num>0

  wait_in->setChannel(Channel( ch_enter, CHANNEL_SEND));

  customer_tmt->initial(0);

  Parameter param = lift_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma =
      sys.createAgent(lift_tmt, param);
  for (int i = 0; i < n; i++) {
    Parameter param = customer_tmt->getParameter();
    shared_ptr<typename INT_TAS_t::Agent_t> tma =
        sys.createAgent(customer_tmt, param);
  }
  

  return sys;
}

}  // namespace graphsat
