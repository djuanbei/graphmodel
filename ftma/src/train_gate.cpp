#include "benchmark/train_gate.h"
#include "model/selecttransition.h"

namespace graphsat {
TrainGate::TrainGate()
    : n( 6 ) {
  train_tmt = sys.createTemplate();
  gate_tmt  = sys.createTemplate();

  sys.addConstant( "N", n );
  TypeDefArray id_t( "id_t", 0, sys[ "N" ] - 1 );
  sys.addType( "id_t", id_t );

  ChanDecl appr( "appr" );
  appr.num = sys[ "N" ];
  sys.addChan( appr );

  ChanDecl stop( "stop" );
  stop.num = sys[ "N" ];
  sys.addChan( stop );

  ChanDecl leave( "leave" );
  leave.num = sys[ "N" ];
  sys.addChan( leave );

  ChanDecl go( "go" );
  go.num  = sys[ "N" ];
  go.type = URGENT_CH;
  sys.addChan( go );

  ADD_CLOCK( ( *train_tmt ), x );
  vector<typename INT_TAS_t::T_t> es;
  vector<typename INT_TAS_t::L_t> ls;

  typename INT_TAS_t::L_t Safe( 0 );
  typename INT_TAS_t::L_t Appr( 1 );
  typename INT_TAS_t::L_t Stop( 2 );
  typename INT_TAS_t::L_t Start( 3 );
  typename INT_TAS_t::L_t Cross( 4 );

  typename INT_TAS_t::CS_t cs1( x, 0, LE, 20 ); // x <= 20
  Appr += cs1;

  typename INT_TAS_t::CS_t cs0( x, 0, LE, 15 ); // x <= 15
  Start += cs0;

  typename INT_TAS_t::CS_t cs2( x, 0, LE, 5 ); // x <= 5
  Cross += cs2;

  ls.push_back( Safe );
  ls.push_back( Appr );
  ls.push_back( Stop );
  ls.push_back( Start );
  ls.push_back( Cross );

  pair<int, int> resetX( x, 0 ); // x-->0

  typename INT_TAS_t::T_t Safe_Appr( Safe, Appr );

  int id = train_tmt->addPara( "id" ) + 1;

  Safe_Appr.setChannel( new ArrayChannel( "appr", id, true ) );
  Safe_Appr.addReset( resetX );

  typename INT_TAS_t::T_t  Appr_Stop( Appr, Stop );
  typename INT_TAS_t::CS_t cs3( x, 0, LE, 10 ); // x<=10
  Appr_Stop += cs3;
  Appr_Stop.setChannel( new ArrayChannel( "stop", -id, true ) );

  typename INT_TAS_t::T_t Stop_Start( Stop, Start );
  Stop_Start.addReset( resetX );
  Stop_Start.setChannel( new ArrayChannel( "go", -id, true ) );

  typename INT_TAS_t::T_t Start_Cross( Start, Cross );
  Start_Cross.addReset( resetX );
  typename INT_TAS_t::CS_t cs4( x, 0, GE, 7 ); // x>=7
  Start_Cross += cs4;

  typename INT_TAS_t::T_t  Cross_Safe( Cross, Safe );
  typename INT_TAS_t::CS_t cs5( x, 0, GE, 3 ); // x>=3
  Cross_Safe += cs5;
  Cross_Safe.setChannel( new ArrayChannel( "leave", id, true ) );

  typename INT_TAS_t::T_t Appr_Cross( Appr, Cross );
  Appr_Cross.addReset( resetX );
  typename INT_TAS_t::CS_t cs6( x, 0, GE, 10 ); // x>=10
  Appr_Cross += cs6;

  es.push_back( Safe_Appr );
  es.push_back( Appr_Stop );
  es.push_back( Stop_Start );
  es.push_back( Cross_Safe );
  es.push_back( Appr_Cross );
  train_tmt->initial( ls, es, 0 );

  // gate
  BaseDecl list( "list" );
  list.num = sys[ "N" ] + 1;
  gate_tmt->addInt( list );

  BaseDecl len( "len" );
  gate_tmt->addInt( len );

  vector<typename INT_TAS_t::T_t> ges;
  vector<typename INT_TAS_t::L_t> gls;

  typename INT_TAS_t::L_t Free( 0 );
  typename INT_TAS_t::L_t Occ( 1 );
  typename INT_TAS_t::L_t Ok( 2, COMMIT_LOC );

  typename INT_TAS_t::T_t Free_Occ1( Free, Occ );

  Argument first1( COUNTER_ARG, 0 );
  Argument second1( EMPTY_ARG, 0 );
  Argument rhs1( CONST_ARG, 0 );
  void *   ccs1 = createConstraint( first1, second1, LT, rhs1 ); // len >0
  Free_Occ1.addCounterCons( ccs1 );

  IndexChannel *ch1= new IndexChannel("go", 1, false );
  ch1->setFunName( "front");
  Free_Occ1.setChannel( ch1 );
  
  

  SelectTransition Free_Occ2( Free, Occ );
  Free_Occ2.setSelectVar( "e");
  Free_Occ2.setSelectCollect( "id_t");
  SelectChannel *ch2=new SelectChannel( "appr", -1, false);
  ch2->setSelectVar( "e");
  Free_Occ2.setChannel( ch2 );
  
  
  Argument       lhs2( FUN_POINTER_ARG, 0 );
  lhs2.name="enqueue";
  
  Argument       rhs2( CONST_ARG, 0 );
  

  typename INT_TAS_t::T_t Occ_OK( Occ, Ok );

  typename INT_TAS_t::T_t Ok_Occ( Ok, Occ );

  typename INT_TAS_t::T_t Occ_Free( Occ, Free );
  gate_tmt->initial( gls, ges, 0 );
}

// void enqueue( typename INT_TAS_t::Agent_t &agent, int *state,
//               const int element ) {

//   int *list          = agent.getValue( state, "list" );
//   int *len           = agent.getValue( state, "len" );
//   list[ ( *len )++ ] = element;
// }

// void dequeue( typename INT_TAS_t::Agent_t &agent, int *state ) {
//   int *len  = agent.getValue( state, "len" );
//   int *list = agent.getValue( state, "list" );
//   int  i    = 0;
//   *len -= 1;
//   while ( i < *len ) {
//     list[ i ] = list[ i - 1 ];
//   }
//   list[ i ] = 0;
// }


// int front(  void  *agent, int *state ) {
  
//   int *list = ((typename INT_TAS_t::Agent_t*)agent)->getValue( state, "list" );
//   return list[ 0 ];
// }

// int tail( typename INT_TAS_t::Agent_t &agent, int *state ) {
//   int *list = agent.getValue( state, "list" );
//   int *len  = agent.getValue( state, "len" );
//   return list[ *len - 1 ];
// }

} // namespace graphsat
