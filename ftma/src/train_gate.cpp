#include "benchmark/train_gate.h"

namespace graphsat {
typedef typename INT_TAS_t::Agent_t Agent_t;
INT_TAS_t TrainGate::generate(int n) const {

  INT_TAS_t sys;

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> train_tmt =
      sys.createTemplate();
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> gate_tmt =
      sys.createTemplate();

  sys.addConstant("N", n); // const N=n;

  sys.addType("id_t", 0, sys["N"] - 1); // typedef int[ 0,N-1] id_t;

  sys.addChan("appr", sys["N"], ONE2ONE_CH); // chan appr[ N]

  sys.addChan("stop", sys["N"], ONE2ONE_CH); // chan stop[ N]

  sys.addChan("leave", sys["N"], ONE2ONE_CH); // chan leave[ N]

  sys.addChan("go", sys["N"], URGENT_CH); // urgent chan go[ N]

  ADD_CLOCK((*train_tmt), x);
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;

  typename INT_TAS_t::L_t Safe(0, "Safe");
  typename INT_TAS_t::L_t Appr(1, "Appr");
  typename INT_TAS_t::L_t Stop(2, "Stop");
  typename INT_TAS_t::L_t Start(3, "Start");
  typename INT_TAS_t::L_t Cross(4, "Cross");

  typename INT_TAS_t::CS_t cs1(x, LE, Argument(20)); // x <= 20
  Appr += cs1;

  typename INT_TAS_t::CS_t cs0(x, LE, Argument(15)); // x <= 15
  Start += cs0;

  typename INT_TAS_t::CS_t cs2(x, LE, Argument(5)); // x <= 5
  Cross += cs2;

  ls.push_back(Safe);
  ls.push_back(Appr);
  ls.push_back(Stop);
  ls.push_back(Start);
  ls.push_back(Cross);

  typename INT_TAS_t::T_t Safe_Appr(Safe, Appr);

  train_tmt->addPara("id");

  Argument ch1_arg(NORMAL_VAR_ARG, "appr");
  shared_ptr<Argument> dummy1(new Argument(PARAMETER_ARG, "id"));
  ch1_arg.setIndex(dummy1);
  Safe_Appr.setChannel(Channel(ch1_arg, CHANNEL_SEND));

  Safe_Appr += ClockReset(x, Argument(0)); // x-->0
  //  Safe_Appr.addReset(x, 0); // x-->0

  typename INT_TAS_t::T_t Appr_Stop(Appr, Stop);
  typename INT_TAS_t::CS_t cs3(x, LE, Argument(10)); // x<=10
  Appr_Stop += cs3;

  Argument ch2_arg(NORMAL_VAR_ARG, "stop");
  shared_ptr<Argument> dummy2(new Argument(PARAMETER_ARG, "id"));
  ch2_arg.setIndex(dummy2);
  Appr_Stop.setChannel(Channel(ch2_arg, CHANNEL_RECEIVE));

  typename INT_TAS_t::T_t Stop_Start(Stop, Start);
  Stop_Start += ClockReset(x, Argument(0));
  //  Stop_Start.addReset(x, 0);
  Argument ch3_arg(NORMAL_VAR_ARG, "go");
  shared_ptr<Argument> dummy3(new Argument(PARAMETER_ARG, "id"));
  ch3_arg.setIndex(dummy3);
  Stop_Start.setChannel(Channel(ch3_arg, CHANNEL_RECEIVE));

  typename INT_TAS_t::T_t Start_Cross(Start, Cross);
  Start_Cross += ClockReset(x, Argument(0)); // x-->0
  // Start_Cross.addReset(x, 0);
  typename INT_TAS_t::CS_t cs4(x, GE, Argument(7)); // x>=7
  Start_Cross += cs4;

  typename INT_TAS_t::T_t Cross_Safe(Cross, Safe);
  typename INT_TAS_t::CS_t cs5(x, GE, Argument(3)); // x>=3
  Cross_Safe += cs5;

  Argument ch4_arg(NORMAL_VAR_ARG, "leave");
  shared_ptr<Argument> dummy4(new Argument(PARAMETER_ARG, "id"));
  ch4_arg.setIndex(dummy4);

  Cross_Safe.setChannel(Channel(ch4_arg, CHANNEL_SEND));

  typename INT_TAS_t::T_t Appr_Cross(Appr, Cross);
  Appr_Cross += ClockReset(x, Argument(0)); // x-->0
  // Appr_Cross.addReset(x, 0);
  typename INT_TAS_t::CS_t cs6(x, GE, Argument(10)); // x>=10
  Appr_Cross += cs6;

  es.push_back(Safe_Appr);
  es.push_back(Appr_Stop);
  es.push_back(Stop_Start);
  es.push_back(Cross_Safe);
  es.push_back(Appr_Cross);
  train_tmt->initial(ls, es, 0);

  // gate template

  gate_tmt->addInt("list", sys["N"] + 1);

  gate_tmt->addInt("len", 1);

  gate_tmt->addFun("enqueue", shared_ptr<Enqueue_F>(new Enqueue_F()));

  gate_tmt->addFun("dequeue", shared_ptr<Dequeue_F>(new Dequeue_F()));

  gate_tmt->addFun("front", shared_ptr<Front_F>(new Front_F()));

  gate_tmt->addFun("tail", shared_ptr<Tail_F>(new Tail_F()));

  vector<typename INT_TAS_t::T_t> ges;
  vector<typename INT_TAS_t::L_t> gls;

  typename INT_TAS_t::L_t Free(0, "Free");
  gls.push_back(Free);
  typename INT_TAS_t::L_t Occ(1, "Occ");
  gls.push_back(Occ);
  typename INT_TAS_t::L_t Ok(2, COMMIT_LOC);
  Ok.setName("Ok");
  gls.push_back(Ok);

  typename INT_TAS_t::T_t Free_Occ1(Free, Occ);

  Argument first1(NORMAL_VAR_ARG, "len");
  Argument second1(EMPTY_ARG, 0);
  Argument rhs1(CONST_ARG, 0);
  CounterConstraint ccs1(first1, second1, GT, rhs1); // len >0
  Free_Occ1 += ccs1;

  Argument ch5_arg(NORMAL_VAR_ARG, "go");
  shared_ptr<Argument> dummy5(new Argument(FUN_POINTER_ARG, "front"));
  ch5_arg.setIndex(dummy5);

  Free_Occ1.setChannel(Channel(ch5_arg, CHANNEL_SEND));
  ges.push_back(Free_Occ1);

  typename INT_TAS_t::T_t Free_Occ2(Free, Occ);
  Free_Occ2.setSelectVar("e");
  Free_Occ2.setSelectCollect("id_t");

  Argument first3(NORMAL_VAR_ARG, "len");
  Argument second3(EMPTY_ARG, 0);
  Argument rhs3(CONST_ARG, 0);
  CounterConstraint ccs3(first3, second3, EQ, rhs3); // len==0
  Free_Occ2 += ccs3;

  Argument ch6_arg(NORMAL_VAR_ARG, "appr");
  shared_ptr<Argument> dummy6(new Argument(SELECT_VAR_ARG, "e"));
  ch6_arg.setIndex(dummy6);
  Free_Occ2.setChannel(Channel(ch6_arg, CHANNEL_RECEIVE));

  Argument lhs2(FUN_POINTER_ARG, 0);
  lhs2.name = "enqueue(e)";
  Argument rhs2(EMPTY_ARG, 0);
  CounterAction caction2(lhs2, CALL_ACTION, rhs2); // enqueue( e)
  Free_Occ2 += caction2;

  ges.push_back(Free_Occ2);

  typename INT_TAS_t::T_t Occ_OK(Occ, Ok);
  Occ_OK.setSelectVar("e");
  Occ_OK.setSelectCollect("id_t");

  Argument ch7_arg(NORMAL_VAR_ARG, "appr");
  shared_ptr<Argument> dummy7(new Argument(SELECT_VAR_ARG, "e"));
  ch7_arg.setIndex(dummy7);
  Occ_OK.setChannel(Channel(ch7_arg, CHANNEL_RECEIVE));

  Argument lhs4(FUN_POINTER_ARG, 0);
  lhs4.name = "enqueue(e)";
  Argument rhs4(EMPTY_ARG, 0);
  CounterAction caction4(lhs4, CALL_ACTION, rhs4); // enqueue( e)
  Occ_OK += caction4;

  ges.push_back(Occ_OK);

  typename INT_TAS_t::T_t Ok_Occ(Ok, Occ);

  Argument ch8_arg(NORMAL_VAR_ARG, "stop");
  shared_ptr<Argument> dummy8(new Argument(FUN_POINTER_ARG, "tail"));
  ch8_arg.setIndex(dummy8);
  Ok_Occ.setChannel(Channel(ch8_arg, CHANNEL_SEND));

  ges.push_back(Ok_Occ);

  typename INT_TAS_t::T_t Occ_Free(Occ, Free);
  Occ_Free.setSelectVar("e");
  Occ_Free.setSelectCollect("id_t");

  Argument first5(SELECT_VAR_ARG, "e");
  Argument second5(EMPTY_ARG, 0);
  Argument rhs5(FUN_POINTER_ARG, "front");
  CounterConstraint ccs5(first5, second5, EQ, rhs5); // e== front( )
  Occ_Free += ccs5;

  Argument ch9_arg(NORMAL_VAR_ARG, "leave");
  shared_ptr<Argument> dummy9(new Argument(SELECT_VAR_ARG, "e"));
  ch9_arg.setIndex(dummy9);
  Occ_Free.setChannel(Channel(ch9_arg, CHANNEL_RECEIVE));

  Argument lhs6(FUN_POINTER_ARG, "dequeue");
  Argument rhs6(EMPTY_ARG, 0);
  CounterAction caction6(lhs6, CALL_ACTION, rhs6); // dequeue
  Occ_Free += caction6;
  ges.push_back(Occ_Free);

  gate_tmt->initial(gls, ges, 0);

  Parameter param = gate_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma=sys.createAgent(gate_tmt, param);
  //  sys += tma;

  for (int i = 0; i < n; i++) {
    Parameter param = train_tmt->getParameter();
    param.setParameterMap("id", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma=sys.createAgent(train_tmt, param);
    //sys += tma;
  }
  sys.build();

  return sys;
}

int Enqueue_F::operator()(int *state...) {
  va_list args;
  va_start(args, state);
  int element = va_arg(args, int);
  int *list = state + (*this)["list"];
  int *len = state + (*this)["len"];

  list[len[0]++] = element;

  va_end(args);
  return 0;
}

int Dequeue_F::operator()(int *state...) {
  int *list = state + (*this)["list"];
  int *len = state + (*this)["len"];
  int i = 0;
  len[0] -= 1;
  while (i < len[0]) {
    list[i] = list[i + 1];
    i++;
  }
  list[i] = 0;
  return 0;
}

int Front_F::operator()(int *state...) {
  int *list = state + (*this)["list"];
  return list[0];
}

int Tail_F::operator()(int *state...) {
  int *list = state + (*this)["list"];
  int *len = state + (*this)["len"];
  return list[len[0] - 1];
}

} // namespace graphsat
