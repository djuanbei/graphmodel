#include "benchmark/train_gate.h"
#include "model/selecttransition.h"


namespace graphsat {

INT_TAS_t TrainGate::generate( int n) const{

  INT_TAS_t                                       sys;

  shared_ptr<typename INT_TAS_t::AgentTemplate_t> train_tmt = sys.createTemplate();
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> gate_tmt  = sys.createTemplate();

  sys.addConstant( "N", n ); // const N=n;
  //  TypeDefArray id_t( "id_t", 0, sys[ "N" ] - 1 ); // typedef int[ 0,N-1] id_t;
  sys.addType( "id_t", 0,  sys[ "N" ] - 1  ); // typedef int[ 0,N-1] id_t;


  sys.addChan( "appr", sys[ "N" ], ONE2ONE_CH ); // chan appr[ N]


  sys.addChan( "stop", sys[ "N" ], ONE2ONE_CH ); // chan stop[ N]


  sys.addChan( "leave", sys[ "N" ], ONE2ONE_CH ); // chan leave[ N]


  sys.addChan( "go", sys[ "N"],  URGENT_CH); // urgent chan go[ N]

  ADD_CLOCK( ( *train_tmt ), x );
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;

  typename INT_TAS_t::L_t Safe( 0 );
  typename INT_TAS_t::L_t Appr( 1 );
  typename INT_TAS_t::L_t Stop( 2 );
  typename INT_TAS_t::L_t Start( 3 );
  typename INT_TAS_t::L_t Cross( 4 );

  typename INT_TAS_t::CS_t cs1( x,  LE, Argument(20) ); // x <= 20
  Appr += cs1;

  typename INT_TAS_t::CS_t cs0( x, LE, Argument(15) ); // x <= 15
  Start += cs0;

  typename INT_TAS_t::CS_t cs2( x, LE, Argument( 5) ); // x <= 5
  Cross += cs2;

  ls.push_back( Safe );
  ls.push_back( Appr );
  ls.push_back( Stop );
  ls.push_back( Start );
  ls.push_back( Cross );

  //pair<int, int> resetX( x, 0 ); // x-->0

  typename INT_TAS_t::T_t Safe_Appr( Safe, Appr );

  int id = train_tmt->addPara( "id" ) + 1;

  Safe_Appr.setChannel( new ArrayChannel( "appr", id, true ) );
  Safe_Appr.addReset( x, 0 );// x-->0

  typename INT_TAS_t::T_t  Appr_Stop( Appr, Stop );
  typename INT_TAS_t::CS_t cs3( x, LE, Argument(10) ); // x<=10
  Appr_Stop += cs3;
  Appr_Stop.setChannel( new ArrayChannel( "stop", -id, true ) );

  typename INT_TAS_t::T_t Stop_Start( Stop, Start );
  Stop_Start.addReset( x, 0 );
  Stop_Start.setChannel( new ArrayChannel( "go", -id, true ) );

  typename INT_TAS_t::T_t Start_Cross( Start, Cross );
  Start_Cross.addReset( x, 0 );
  typename INT_TAS_t::CS_t cs4( x,  GE, Argument( 7) ); // x>=7
  Start_Cross += cs4;

  typename INT_TAS_t::T_t  Cross_Safe( Cross, Safe );
  typename INT_TAS_t::CS_t cs5( x,  GE, Argument( 3) ); // x>=3
  Cross_Safe += cs5;
  Cross_Safe.setChannel( new ArrayChannel( "leave", id, true ) );

  typename INT_TAS_t::T_t Appr_Cross( Appr, Cross );
  Appr_Cross.addReset( x, 0 );
  typename INT_TAS_t::CS_t cs6( x,  GE, Argument( 10) ); // x>=10
  Appr_Cross += cs6;

  es.push_back( Safe_Appr );
  es.push_back( Appr_Stop );
  es.push_back( Stop_Start );
  es.push_back( Cross_Safe );
  es.push_back( Appr_Cross );
  train_tmt->initial( ls, es, 0 );

  // gate template

  gate_tmt->addInt("list", sys[ "N" ] + 1 );


  int len_id=gate_tmt->addInt( "len", 1 );

  vector<typename INT_TAS_t::T_t> ges;
  vector<typename INT_TAS_t::L_t> gls;

  typename INT_TAS_t::L_t Free( 0 );
  gls.push_back( Free);
  typename INT_TAS_t::L_t Occ( 1 );
  gls.push_back( Occ);
  typename INT_TAS_t::L_t Ok( 2, COMMIT_LOC );
  gls.push_back( Ok);

  typename INT_TAS_t::T_t Free_Occ1( Free, Occ );

  Argument first1( TEMPLATE_VAR_ARG, len_id );
  Argument second1( EMPTY_ARG, 0 );
  Argument rhs1( CONST_ARG, 0 );
  void *   ccs1 = createConstraint( first1, second1, GT, rhs1 ); // len >0
  Free_Occ1.addCounterCons( ccs1 );

  IndexChannel *ch1= new IndexChannel("go", CHANNEL_SEND );
  ch1->setFunName( "front");
  Free_Occ1.setChannel( ch1 );
  ges.push_back(Free_Occ1 );

  
  SelectTransition Free_Occ2( Free, Occ );
  Free_Occ2.setSelectVar( "e");
  Free_Occ2.setSelectCollect( "id_t");


  Argument first3( TEMPLATE_VAR_ARG, len_id);
  Argument second3( EMPTY_ARG, 0 );
  Argument rhs3( CONST_ARG, 0 );
  void *   ccs3 = createConstraint( first3, second3, EQ, rhs3 ); // len==0
  Free_Occ2.addCounterCons( ccs3);

  SelectChannel *ch2=new SelectChannel( "appr", CHANNEL_RECEIVE);
  ch2->setSelectVar( "e");
  Free_Occ2.setChannel( ch2 );
  
  Argument       lhs2( TEMPALTE_FUN_POINTER_ARG, 0 );
  lhs2.name="enqueue(e)";
  Argument       rhs2( EMPTY_ARG, 0 );
  CounterAction *caction2 =
      new CounterAction( lhs2, CALL_ACTION, rhs2 ); // enqueue( e)
  Free_Occ2.addCounterAction(caction2 );
  
  ges.push_back( Free_Occ2);
  
  SelectTransition Occ_OK( Occ, Ok );
  Occ_OK.setSelectVar( "e");
  Occ_OK.setSelectCollect( "id_t");
  
  SelectChannel *ch3=new SelectChannel( "appr", CHANNEL_RECEIVE);
  ch3->setSelectVar( "e");
  Occ_OK.setChannel( ch3);

  Argument       lhs4( TEMPALTE_FUN_POINTER_ARG, 0 );
  lhs4.name="enqueue( e)";
  Argument       rhs4( EMPTY_ARG, 0 );
  CounterAction *caction4 =
      new CounterAction( lhs4, CALL_ACTION, rhs4 ); // enqueue( e)
  Occ_OK.addCounterAction(caction4);

  ges.push_back(Occ_OK );

  typename INT_TAS_t::T_t Ok_Occ( Ok, Occ );

  IndexChannel * ch4=new IndexChannel( "stop", CHANNEL_SEND);
  ch4->setFunName( "tail");
  Ok_Occ.setChannel( ch4);
  ges.push_back( Ok_Occ);
  
  SelectTransition Occ_Free( Occ, Free );
  Occ_Free.setSelectVar( "e");
  Occ_Free.setSelectCollect( "id_t");
  
  Argument first5( SELECT_VAR_ARG, "e");
  Argument second5( EMPTY_ARG, 0 );
  Argument rhs5( TEMPALTE_FUN_POINTER_ARG, "front" );
  void *   ccs5 = createConstraint( first5, second5, EQ, rhs5 ); // e== front( )
  Occ_Free.addCounterCons( ccs5);
  
  SelectChannel *ch5=new SelectChannel( "leave", CHANNEL_RECEIVE);
  ch5->setSelectVar( "e");
  Occ_Free.setChannel( ch5);
  

  Argument  lhs6( TEMPALTE_FUN_POINTER_ARG, "dequeue" );
  Argument       rhs6( EMPTY_ARG, 0 );
  CounterAction *caction6 =
      new CounterAction( lhs6, CALL_ACTION, rhs6 ); // dequeue
  Occ_Free.addCounterAction(caction6 );
  ges.push_back(Occ_Free );
  
  gate_tmt->initial( gls, ges, 0 );
  
  Parameter param=gate_tmt->getParameter( );
  typename INT_TAS_t::Agent_t tma( gate_tmt, param );
  sys+=tma;

  for( int i=0; i< n; i++){
    Parameter param=train_tmt->getParameter( );
    param.setParameterMap(0, i );
    typename INT_TAS_t::Agent_t tma(train_tmt, param );
    sys+=tma;
  }
  
  return sys;
}


int Enqueue_F::operator( )( int * state...){
  va_list args;
  va_start(args, state);
  int element=  va_arg(args, int);
  int *list=state+(*this)["list" ];
  int *len=state+(*this)["len" ];
  
  list[ len[0]++]=element;
  
  va_end(args);
  return 0;
  
}

int Dequeue_F::operator( )( int * state...){
  int *list=state+(*this)["list" ];
  int *len=state+(*this)["len" ];
  int i=0;
  len[ 0]-=1;
  while( i< len[ 0]){
    list[ i]=list[ i+1];
    i++;
  }
  list[ i]=0;
  return 0;
}

int  Front_F::operator( )( int * state...){
  int *list=state+(*this)["list" ];
  return  list[ 0];
}


int  Tail_F::operator( )( int * state...){
  int *list=state+(*this)["list" ];
  int *len=state+(*this)["len" ];
  return list[ len[ 0]-1];
}


} // namespace graphsat
