#include "benchmark/train_gate.h"

namespace graphsat {
typedef typename INT_TAS_t::Agent_t Agent_t;
INT_TAS_t TrainGate::generate(int n) const {
  INT_TAS_t sys;

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> train_tmt =
      sys.createTemplate("Train");
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> gate_tmt =
      sys.createTemplate("Gate");

  sys.addConstant("N", n);  // const N=n;

  sys.addType("id_t", 0, sys["N"] - 1);  // typedef int[ 0,N-1] id_t;

  sys.addChan("appr", sys["N"], ONE2ONE_CH);  // chan appr[ N]

  sys.addChan("stop", sys["N"], ONE2ONE_CH);  // chan stop[ N]

  sys.addChan("leave", sys["N"], ONE2ONE_CH);  // chan leave[ N]

  sys.addChan("go", sys["N"], URGENT_CH);  // urgent chan go[ N]

  ADD_CLOCK((*train_tmt), x);

  typename INT_TAS_t::L_t* Safe = train_tmt->createLocation("Safe");
  typename INT_TAS_t::L_t* Appr = train_tmt->createLocation("Appr");
  typename INT_TAS_t::L_t* Stop = train_tmt->createLocation("Stop");
  typename INT_TAS_t::L_t* Start = train_tmt->createLocation("Start");
  typename INT_TAS_t::L_t* Cross = train_tmt->createLocation("Cross");

  typename INT_TAS_t::CS_t cs1(x, LE, Argument(20));  // x <= 20
  (*Appr) += cs1;

  typename INT_TAS_t::CS_t cs0(x, LE, Argument(15));  // x <= 15
  (*Start) += cs0;

  typename INT_TAS_t::CS_t cs2(x, LE, Argument(5));  // x <= 5
  (*Cross) += cs2;

  typename INT_TAS_t::T_t* Safe_Appr = train_tmt->createTransition(Safe, Appr);

  train_tmt->addPara("id");

  Argument ch1_arg(NORMAL_VAR_ARG, "appr");

  ch1_arg.setIndex(Argument(PARAMETER_ARG, "id"));
  Safe_Appr->setChannel(Channel(ch1_arg, CHANNEL_SEND));

  (*Safe_Appr) += ClockReset(x, Argument(0));  // x-->0

  typename INT_TAS_t::T_t* Appr_Stop = train_tmt->createTransition(Appr, Stop);
  typename INT_TAS_t::CS_t cs3(x, LE, Argument(10));  // x<=10
  (*Appr_Stop) += cs3;

  Argument ch2_arg(NORMAL_VAR_ARG, "stop");

  ch2_arg.setIndex(Argument(PARAMETER_ARG, "id"));
  Appr_Stop->setChannel(Channel(ch2_arg, CHANNEL_RECEIVE));

  typename INT_TAS_t::T_t* Stop_Start =
      train_tmt->createTransition(Stop, Start);
  (*Stop_Start) += ClockReset(x, Argument(0));

  Argument ch3_arg(NORMAL_VAR_ARG, "go");

  ch3_arg.setIndex(Argument(PARAMETER_ARG, "id"));
  Stop_Start->setChannel(Channel(ch3_arg, CHANNEL_RECEIVE));

  typename INT_TAS_t::T_t* Start_Cross =
      train_tmt->createTransition(Start, Cross);
  (*Start_Cross) += ClockReset(x, Argument(0));  // x-->0

  typename INT_TAS_t::CS_t cs4(x, GE, Argument(7));  // x>=7
  (*Start_Cross) += cs4;

  typename INT_TAS_t::T_t* Cross_Safe =
      train_tmt->createTransition(Cross, Safe);
  typename INT_TAS_t::CS_t cs5(x, GE, Argument(3));  // x>=3
  (*Cross_Safe) += cs5;

  Argument ch4_arg(NORMAL_VAR_ARG, "leave");

  ch4_arg.setIndex(Argument(PARAMETER_ARG, "id"));

  Cross_Safe->setChannel(Channel(ch4_arg, CHANNEL_SEND));

  typename INT_TAS_t::T_t* Appr_Cross =
      train_tmt->createTransition(Appr, Cross);
  (*Appr_Cross) += ClockReset(x, Argument(0));  // x-->0

  typename INT_TAS_t::CS_t cs6(x, GE, Argument(10));  // x>=10
  (*Appr_Cross) += cs6;

  train_tmt->initial(0);

  // gate template

  gate_tmt->addInt("list", sys["N"] + 1, 0, sys["N"] + 2);

  gate_tmt->addInt("len", 1, 0, sys["N"] + 1);

  gate_tmt->addFun("enqueue", shared_ptr<Enqueue_F>(new Enqueue_F()));

  gate_tmt->addFun("dequeue", shared_ptr<Dequeue_F>(new Dequeue_F()));

  gate_tmt->addFun("front", shared_ptr<Front_F>(new Front_F()));

  gate_tmt->addFun("tail", shared_ptr<Tail_F>(new Tail_F()));

  vector<typename INT_TAS_t::T_t> ges;
  vector<typename INT_TAS_t::L_t> gls;

  typename INT_TAS_t::L_t* Free = gate_tmt->createLocation("Free");

  typename INT_TAS_t::L_t* Occ = gate_tmt->createLocation("Occ");

  typename INT_TAS_t::L_t* Ok = gate_tmt->createLocation(COMMIT_LOC);
  Ok->setName("Ok");

  typename INT_TAS_t::T_t* Free_Occ1 = gate_tmt->createTransition(Free, Occ);

  Argument first1(NORMAL_VAR_ARG, "len");
  Argument second1(EMPTY_ARG, 0);
  Argument rhs1(CONST_ARG, 0);
  CounterConstraint ccs1(first1, second1, GT, rhs1);  // len >0
  (*Free_Occ1) += ccs1;

  Argument ch5_arg(NORMAL_VAR_ARG, "go");

  ch5_arg.setIndex(Argument(FUN_POINTER_ARG, "front"));

  Free_Occ1->setChannel(Channel(ch5_arg, CHANNEL_SEND));

  typename INT_TAS_t::T_t* Free_Occ2 = gate_tmt->createTransition(Free, Occ);
  Free_Occ2->setSelectVar("e");
  Free_Occ2->setSelectCollect("id_t");

  Argument first3(NORMAL_VAR_ARG, "len");
  Argument second3(EMPTY_ARG, 0);
  Argument rhs3(CONST_ARG, 0);
  CounterConstraint ccs3(first3, second3, EQ, rhs3);  // len==0
  (*Free_Occ2) += ccs3;

  Argument ch6_arg(NORMAL_VAR_ARG, "appr");

  ch6_arg.setIndex(Argument(SELECT_VAR_ARG, "e"));
  Free_Occ2->setChannel(Channel(ch6_arg, CHANNEL_RECEIVE));

  Argument lhs2(FUN_POINTER_ARG, 0);
  lhs2.setName("enqueue(e)");
  Argument rhs2(EMPTY_ARG, 0);
  CounterAction caction2(lhs2, CALL_ACTION, rhs2);  // enqueue( e)
  (*Free_Occ2) += caction2;

  typename INT_TAS_t::T_t* Occ_OK = gate_tmt->createTransition(Occ, Ok);
  Occ_OK->setSelectVar("e");
  Occ_OK->setSelectCollect("id_t");

  Argument ch7_arg(NORMAL_VAR_ARG, "appr");

  ch7_arg.setIndex(Argument(SELECT_VAR_ARG, "e"));
  Occ_OK->setChannel(Channel(ch7_arg, CHANNEL_RECEIVE));

  Argument lhs4(FUN_POINTER_ARG, 0);
  lhs4.setName("enqueue(e)");
  Argument rhs4(EMPTY_ARG, 0);
  CounterAction caction4(lhs4, CALL_ACTION, rhs4);  // enqueue( e)
  (*Occ_OK) += caction4;

  typename INT_TAS_t::T_t* Ok_Occ = gate_tmt->createTransition(Ok, Occ);

  Argument ch8_arg(NORMAL_VAR_ARG, "stop");

  ch8_arg.setIndex(Argument(FUN_POINTER_ARG, "tail"));
  Ok_Occ->setChannel(Channel(ch8_arg, CHANNEL_SEND));

  typename INT_TAS_t::T_t* Occ_Free = gate_tmt->createTransition(Occ, Free);
  Occ_Free->setSelectVar("e");
  Occ_Free->setSelectCollect("id_t");

  Argument first5(SELECT_VAR_ARG, "e");
  Argument second5(EMPTY_ARG, 0);
  Argument rhs5(FUN_POINTER_ARG, "front");
  CounterConstraint ccs5(first5, second5, EQ, rhs5);  // e== front( )
  (*Occ_Free) += ccs5;

  Argument ch9_arg(NORMAL_VAR_ARG, "leave");

  ch9_arg.setIndex(Argument(SELECT_VAR_ARG, "e"));
  Occ_Free->setChannel(Channel(ch9_arg, CHANNEL_RECEIVE));

  Argument lhs6(FUN_POINTER_ARG, "dequeue");
  Argument rhs6(EMPTY_ARG, 0);
  CounterAction caction6(lhs6, CALL_ACTION, rhs6);  // dequeue
  (*Occ_Free) += caction6;

  gate_tmt->initial(0);

  Parameter param = gate_tmt->getParameter();
  shared_ptr<typename INT_TAS_t::Agent_t> tma =
      sys.createAgent(gate_tmt, param);

  for (int i = 0; i < n; i++) {
    Parameter param = train_tmt->getParameter();
    param.setParameterMap("id", i);
    shared_ptr<typename INT_TAS_t::Agent_t> tma =
        sys.createAgent(train_tmt, param);
  }

  sys.getStateManager();
  return sys;
}

int Enqueue_F::operator()(int* state...) {
  va_list args;
  va_start(args, state);
  int element = va_arg(args, int);
  int* list = state + (*this)["list"];
  int* len = state + (*this)["len"];

  list[len[0]++] = element;

  va_end(args);
  return 0;
}

int Dequeue_F::operator()(int* state...) {
  int* list = state + (*this)["list"];
  int* len = state + (*this)["len"];
  int i = 0;
  len[0] -= 1;
  while (i < len[0]) {
    list[i] = list[i + 1];
    i++;
  }
  list[i] = 0;
  return 0;
}

int Front_F::operator()(int* state...) {
  int* list = state + (*this)["list"];
  return list[0];
}

int Tail_F::operator()(int* state...) {
  int* list = state + (*this)["list"];
  int* len = state + (*this)["len"];
  return list[len[0] - 1];
}

}  // namespace graphsat
