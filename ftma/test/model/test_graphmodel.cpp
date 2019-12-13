#include "gtest/gtest.h"


#include "model/graphmodel.hpp"
#include "model/location.h"
#include "model/transition.h"
#include "problem/reachability.hpp"
#include "state/reachableset.hpp"

#include "model/selecttransition.h"

#include "benchmark/train_gate.h"

#undef PRINT_STATE

using namespace graphsat;


typedef AgentSystem<Location, Transition> INT_TAS_t;

/*
 * For more details about this example, see 
 * "Automatic Verification of Real-Time Communicating Systems by Constraint Solving", 
 * by Wang Yi, Paul Pettersson and Mats Daniels. In Proceedings of the 7th International
 * Conference on Formal Description Techniques, pages 223-238, North-Holland. 1994.
 */

class GraphModelTest: public ::testing::Test{
 protected:
  GraphModelTest( ){
    int n=6;// 6 train
    train_tmt = sys.createTemplate();
    gate_tmt  = sys.createTemplate();
    train_tmt->addPara("id");
    sys.addConstant( "N", n ); // const N=n;
    sys.addType( "id_t", 0,  sys[ "N" ] - 1  ); // typedef int[ 0,N-1] id_t;
    sys.addChan( "appr", sys[ "N" ], ONE2ONE_CH ); // chan appr[ N]
    sys.addChan( "stop", sys[ "N" ], ONE2ONE_CH ); // chan stop[ N]
    sys.addChan( "leave", sys[ "N" ], ONE2ONE_CH ); // chan leave[ N]
    sys.addChan( "go", sys[ "N"],  URGENT_CH); // urgent chan go[ N]



    ADD_CLOCK( ( *train_tmt ), x );

  

    gate_tmt->addInt("list", sys[ "N" ] + 1 );
    gate_tmt->addInt( "len", 1 );

 
    gate_tmt->addFun("enqueue", shared_ptr<Enqueue_F>(new Enqueue_F( )) );
    
    
    gate_tmt->addFun( "dequeue", shared_ptr<Dequeue_F>(new Dequeue_F( )) );

    gate_tmt->addFun( "front", shared_ptr<Front_F>(new Front_F( ) ));
    
    gate_tmt->addFun( "tail", shared_ptr<Tail_F>(new Tail_F( ) ));

    
  }
  virtual ~GraphModelTest( ){
    
  }
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).

    
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }
  INT_TAS_t                                       sys;
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> train_tmt;
  shared_ptr<typename INT_TAS_t::AgentTemplate_t> gate_tmt;

  
};

TEST_F(GraphModelTest, getConstant) {
  EXPECT_EQ(sys[ "N"],6);
  EXPECT_EQ(sys.getChanNumber( ), 24 );

}

TEST_F( GraphModelTest, SELECT_VAR_ARG){
  sys.removeAgent( );
  
  int n=sys[ "N"];
  Parameter param=gate_tmt->getParameter( );
  shared_ptr< typename INT_TAS_t::Agent_t> tma( new INT_TAS_t::Agent_t(  gate_tmt, param ));
  sys+=tma;
  
  for( int i=0; i< n; i++){
    Parameter param=train_tmt->getParameter( );
    param.setParameterMap(0, i );
    shared_ptr<typename INT_TAS_t::Agent_t> tma( new INT_TAS_t::Agent_t(train_tmt, param ));
    sys+=tma;
  }
  sys.build();
  
  Argument arg(TEMPLATE_VAR_ARG,"list");
  shared_ptr<Argument> dummy( new Argument( TEMPLATE_VAR_ARG, "len"));
  arg.setIndex(dummy );
  
  RealArgument rarg=tma->to_real(INT_T, arg );
  shared_ptr<INT_TAS_t::StateManager_t> manager= sys.getStateManager();
  INT_TAS_t::State_t *state=manager->newState();

  INT_TAS_t::State_t *counters=manager->getCounterValue( state);
  
  EXPECT_EQ( getValue( rarg, counters), 0);
  
  
  Argument len_arg(TEMPLATE_VAR_ARG,"len");
  RealArgument rlen_arg=tma->to_real(INT_T, len_arg );
  
  shared_ptr<Function> enqueue_c =tma->getFun( "enqueue");
  shared_ptr<Function> dequeue_c=tma->getFun( "dequeue");
  
  shared_ptr<Function> front_c=tma->getFun( "front");

  shared_ptr<Function> tail_c=tma->getFun( "tail");

  

  for(int i=0; i< n; i++){
    (*enqueue_c)(counters, i );
    EXPECT_EQ( getValue( rlen_arg, counters), i+1);
    EXPECT_EQ( (*tail_c)(counters ), i);
    EXPECT_EQ( getValue( rlen_arg, counters), i+1);
  }
  
  for(int i=0; i< n; i++ ){
    EXPECT_EQ( getValue( rlen_arg, counters), n-i);
    EXPECT_EQ( (*front_c)(counters ), i);
    (*dequeue_c)( counters);
    if(getValue( rlen_arg, counters)>0){
      EXPECT_EQ( (*front_c)(counters ), i+1);
    }
    EXPECT_EQ( getValue( rlen_arg, counters), n-i-1);
  }
  
  manager->destroyState(state);
    
}

TEST_F(GraphModelTest, constraint){
  sys.removeAgent( );
  int n=sys[ "N"];
  Parameter param=gate_tmt->getParameter( );
  shared_ptr<typename INT_TAS_t::Agent_t> tma( new INT_TAS_t::Agent_t( gate_tmt, param ));
  sys+=tma;
  
  for( int i=0; i< n; i++){
    Parameter param=train_tmt->getParameter( );
    param.setParameterMap(0, i );
    shared_ptr<typename INT_TAS_t::Agent_t> tma( new INT_TAS_t::Agent_t(  train_tmt, param ));
    sys+=tma;
  }
  sys.build();
  
  Argument arg(TEMPLATE_VAR_ARG,"list");
  shared_ptr<Argument> dummy( new Argument( TEMPLATE_VAR_ARG, "len"));
  arg.setIndex(dummy );
  
  RealArgument rarg=tma->to_real(INT_T, arg );
  shared_ptr<INT_TAS_t::StateManager_t> manager= sys.getStateManager();
  INT_TAS_t::State_t *state=manager->newState();

  INT_TAS_t::State_t *counters=manager->getCounterValue( state);
  
  EXPECT_EQ( getValue( rarg, counters), 0);
  
  
  Argument len_arg(TEMPLATE_VAR_ARG,"len");
  
  RealArgument rlen_arg=tma->to_real(INT_T, len_arg );


  shared_ptr<Function> enqueue_c =tma->getFun( "enqueue");
  shared_ptr<Function> dequeue_c=tma->getFun( "dequeue");

  shared_ptr<Function> front_c=tma->getFun( "front");

  shared_ptr<Function> tail_c=tma->getFun( "tail");


  Argument second1( EMPTY_ARG, 0 );
  Argument rhs1( CONST_ARG, 0 );
  nCounterConstraint *   ccs1 =new  nCounterConstraint( len_arg, second1, GT, rhs1 ); // len >0
  ccs1->to_real(tma );

  EXPECT_FALSE((*ccs1 )(counters ) );

  for(int i=0; i< n; i++){
    (*enqueue_c)(counters, i );
     EXPECT_TRUE((*ccs1 )(counters ) );
  }
  for( int i=0; i< n; i++){
    EXPECT_TRUE((*ccs1 )(counters ) );
    (*dequeue_c)(counters );
  }
  EXPECT_FALSE((*ccs1 )(counters ) );

  manager->destroyState(state);
  
}

TEST_F(GraphModelTest, IndexChannel ){

  sys.removeAgent( );
  int n=sys[ "N"];
  Parameter param=gate_tmt->getParameter( );
  shared_ptr<typename INT_TAS_t::Agent_t> tma( new INT_TAS_t::Agent_t (gate_tmt, param ));
  sys+=tma;
  
  for( int i=0; i< n; i++){
    Parameter param=train_tmt->getParameter( );
    param.setParameterMap(0, i );
    shared_ptr<typename INT_TAS_t::Agent_t> tma( new INT_TAS_t::Agent_t (train_tmt, param ));
    sys+=tma;
  }
  sys.build();
  
  Argument arg(TEMPLATE_VAR_ARG,"list");
  shared_ptr<Argument> dummy( new Argument( TEMPLATE_VAR_ARG, "len"));
  arg.setIndex(dummy );
  
  RealArgument rarg=tma->to_real(INT_T, arg );
  shared_ptr<INT_TAS_t::StateManager_t> manager= sys.getStateManager();
  INT_TAS_t::State_t *state=manager->newState();

  INT_TAS_t::State_t *counters=manager->getCounterValue( state);



  
  Argument ch1_arg(SYSTEM_VAR_ARG, "go" );
  shared_ptr<Argument> dummy1( new Argument( TEMPALTE_FUN_POINTER_ARG, "front"));
  ch1_arg.setIndex(dummy1 );
  
  n_Channel ch1(ch1_arg  );
  ch1.setAction(CHANNEL_SEND );
  ch1.to_real( tma);
  
  shared_ptr<Function> enqueue_c =tma->getFun( "enqueue");
  shared_ptr<Function> dequeue_c=tma->getFun( "dequeue");
  
  shared_ptr<Function> front_c=tma->getFun( "front");
  
  shared_ptr<Function> tail_c=tma->getFun( "tail");
  
  EXPECT_EQ(18, tma->getStart(CHAN_T, "go"));
  for( int i=0; i< n; i++){
    (*enqueue_c)(counters, i );
    int ffid=ch1(counters);
    EXPECT_EQ(18, ffid);
  }
  
  for( int i=0; i< n; i++){
   
    int ffid=ch1(counters);
    EXPECT_EQ(18+i, ffid);
    (*dequeue_c)(counters);
  }



  


  
}

TEST(GRAPH_MODEL, SELECT_VAR_ARG ){
  
  
}




