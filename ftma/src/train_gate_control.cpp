#include "benchmark/train_gate_controll.h"

namespace graphsat {
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

  typename INT_TAS_t::L_t far(0, "far");
  typename INT_TAS_t::L_t near(1, "near");
  typename INT_TAS_t::L_t in(2, "in");

  train_ls.push_back(far);
  train_ls.push_back(near);
  train_ls.push_back(in);

  typename INT_TAS_t::T_t far_near(far, near);
  Argument ch1_arg(NORMAL_VAR_ARG, "approach");
  far_near.setChannel(Channel(ch1_arg, CHANNEL_SEND));

  typename INT_TAS_t::T_t near_in(near, in);

  Argument ch2_arg(NORMAL_VAR_ARG, "enter");

  near_in.setChannel(Channel(ch2_arg, CHANNEL_SEND));

  typename INT_TAS_t::T_t in_far(in, far);
  Argument ch3_arg(NORMAL_VAR_ARG, "exit");
  near_in.setChannel(Channel(ch3_arg, CHANNEL_SEND));
  train_es.push_back(far_near);
  train_es.push_back(near_in);
  train_es.push_back(in_far);

  train_tmt->initial(train_ls, train_es, 0);

  vector<typename INT_TAS_t::T_t> control_es;
  vector<typename INT_TAS_t::L_t> control_ls;

  typename INT_TAS_t::L_t zero(0, "0");
  typename INT_TAS_t::L_t one(1, "1");
  typename INT_TAS_t::L_t two(2, "2");
  typename INT_TAS_t::L_t three(3, "3");

  control_ls.push_back(zero);
  control_ls.push_back(one);
  control_ls.push_back(two);
  control_ls.push_back(three);

  typename INT_TAS_t::T_t zero_one(zero, one);
  Argument ch4_arg(NORMAL_VAR_ARG, "approach");
  zero_one.setChannel(Channel(ch4_arg, CHANNEL_RECEIVE));

  typename INT_TAS_t::T_t one_two(one, two);
  Argument ch5_arg(NORMAL_VAR_ARG, "lower");
  one_two.setChannel(Channel(ch5_arg, CHANNEL_RECEIVE));

  typename INT_TAS_t::T_t two_three(two, three);
  Argument ch6_arg(NORMAL_VAR_ARG, "exit");
  two_three.setChannel(Channel(ch6_arg, CHANNEL_SEND));

  typename INT_TAS_t::T_t three_zero(three, zero);

  Argument ch7_arg(NORMAL_VAR_ARG, "raise");
  three_zero.setChannel(Channel(ch7_arg, CHANNEL_SEND));

  control_es.push_back(zero_one);
  control_es.push_back(one_two);
  control_es.push_back(two_three);
  control_es.push_back(three_zero);
  control_tmt->initial(control_ls, control_es, 0);

  vector<typename INT_TAS_t::T_t> gate_es;
  vector<typename INT_TAS_t::L_t> gate_ls;
  typename INT_TAS_t::L_t up(0, "up");
  typename INT_TAS_t::L_t down(0, "down");

  gate_ls.push_back(up);
  gate_ls.push_back(down);

  typename INT_TAS_t::T_t up_down(up, down);
  Argument ch8_arg(NORMAL_VAR_ARG, "lower");
  up_down.setChannel(Channel(ch8_arg, CHANNEL_RECEIVE));

  typename INT_TAS_t::T_t down_up(down, up);
  Argument ch9_arg(NORMAL_VAR_ARG, "raise");
  down_up.setChannel(Channel(ch8_arg, CHANNEL_SEND));

  gate_es.push_back(up_down);
  gate_es.push_back(down_up);

  gate_tmt->initial(gate_ls, gate_es, 0);

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
}  // namespace graphsat
