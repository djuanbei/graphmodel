#include "benchmark/train_gate_controll.h"

namespace graphmodel {
typedef typename INT_TAS_t::Agent_t Agent_t;
TrainGateControl::TrainGateControl() {}
INT_TAS_t TrainGateControl::generate(int n) const {
  INT_TAS_t sys;

  sys.addChan("approach", 1, ONE2ONE_CH);
  sys.addChan("enter", 1, ONE2ONE_CH);
  sys.addChan("exit", 1, ONE2ONE_CH);
  sys.addChan("lower", 1, ONE2ONE_CH);
  sys.addChan("raise", 1, ONE2ONE_CH);

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> train_tmt =
      sys.createTemplate("Train");

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> gate_tmt =
      sys.createTemplate("Gate");

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> control_tmt =
      sys.createTemplate("Control");

  vector<typename INT_TAS_t::T_t> train_es;
  vector<typename INT_TAS_t::L_t> train_ls;

  typename INT_TAS_t::L_t* far = train_tmt->createLocation("far");
  typename INT_TAS_t::L_t* near = train_tmt->createLocation("near");
  typename INT_TAS_t::L_t* in = train_tmt->createLocation("in");

  typename INT_TAS_t::T_t* far_near = train_tmt->createTransition(far, near);
  Argument ch1_arg(NORMAL_VAR_ARG, "approach");
  far_near->setChannel(Channel(ch1_arg, CHANNEL_SEND));

  typename INT_TAS_t::T_t* near_in = train_tmt->createTransition(near, in);

  Argument ch2_arg(NORMAL_VAR_ARG, "enter");

  near_in->setChannel(Channel(ch2_arg, CHANNEL_SEND));

  train_tmt->createTransition(in, far);
  Argument ch3_arg(NORMAL_VAR_ARG, "exit");
  near_in->setChannel(Channel(ch3_arg, CHANNEL_SEND));

  train_tmt->initial(0);

  vector<typename INT_TAS_t::T_t> control_es;
  vector<typename INT_TAS_t::L_t> control_ls;

  typename INT_TAS_t::L_t* zero = control_tmt->createLocation("0");
  typename INT_TAS_t::L_t* one = control_tmt->createLocation("1");
  typename INT_TAS_t::L_t* two = control_tmt->createLocation("2");
  typename INT_TAS_t::L_t* three = control_tmt->createLocation("3");

  typename INT_TAS_t::T_t* zero_one = control_tmt->createTransition(zero, one);
  Argument ch4_arg(NORMAL_VAR_ARG, "approach");
  zero_one->setChannel(Channel(ch4_arg, CHANNEL_RECEIVE));

  typename INT_TAS_t::T_t* one_two = control_tmt->createTransition(one, two);
  Argument ch5_arg(NORMAL_VAR_ARG, "lower");
  one_two->setChannel(Channel(ch5_arg, CHANNEL_RECEIVE));

  typename INT_TAS_t::T_t* two_three =
      control_tmt->createTransition(two, three);
  Argument ch6_arg(NORMAL_VAR_ARG, "exit");
  two_three->setChannel(Channel(ch6_arg, CHANNEL_SEND));

  typename INT_TAS_t::T_t* three_zero =
      control_tmt->createTransition(three, zero);

  Argument ch7_arg(NORMAL_VAR_ARG, "raise");
  three_zero->setChannel(Channel(ch7_arg, CHANNEL_SEND));

  control_tmt->initial(0);

  vector<typename INT_TAS_t::T_t> gate_es;
  vector<typename INT_TAS_t::L_t> gate_ls;
  typename INT_TAS_t::L_t* up = gate_tmt->createLocation("up");
  typename INT_TAS_t::L_t* down = gate_tmt->createLocation("down");

  typename INT_TAS_t::T_t* up_down = gate_tmt->createTransition(up, down);
  Argument ch8_arg(NORMAL_VAR_ARG, "lower");
  up_down->setChannel(Channel(ch8_arg, CHANNEL_RECEIVE));

  typename INT_TAS_t::T_t* down_up = gate_tmt->createTransition(down, up);
  Argument ch9_arg(NORMAL_VAR_ARG, "raise");
  down_up->setChannel(Channel(ch8_arg, CHANNEL_SEND));

  gate_tmt->initial(0);

  Parameter param = gate_tmt->getParameter();
  sys.createAgent(gate_tmt, param);
  param = control_tmt->getParameter();
  sys.createAgent(control_tmt, param);
  for (int i = 0; i < n; i++) {
    Parameter param = train_tmt->getParameter();
    sys.createAgent(train_tmt, param);
  }

  //  sys.build();

  return sys;
}
}  // namespace graphmodel
