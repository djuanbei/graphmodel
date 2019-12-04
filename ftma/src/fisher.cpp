
#include "benchmark/fisher.h"

namespace graphsat {
  FisherGenerator::FisherGenerator():k(2){
    vector<typename INT_TAS_t::T_t> es;
    vector<typename INT_TAS_t::L_t> ls;
    int                             k = 2;
    
    typename INT_TAS_t::L_t A( 0 );
    
    typename INT_TAS_t::L_t  req( 1 );
    typename INT_TAS_t::CS_t cs1( 1, 0, LE, k ); // x <= k
    req += cs1;
    
    typename INT_TAS_t::L_t wait( 2 );
    
    typename INT_TAS_t::L_t cs( 3 );
    
    typename INT_TAS_t::T_t A_req( 0, 1 );
    
    Argument first3( COUNTER_ARG, 0 );
    Argument second3( EMPTY_ARG, 0 );
    Argument rhs3( CONST_ARG, 0 );
    void *   ccs1 = createConstraint( first3, second3, EQ, rhs3 ); // id==0
    
    A_req.addCounterCons( ccs1 );
    
    pair<int, int> reset1( 1, 0 ); // x-->0
    A_req.addReset( reset1 );
    
    typename INT_TAS_t::T_t  req_wait( 1, 2 );
    typename INT_TAS_t::CS_t cs2( 1, 0, LE, k ); // x <= k
    req_wait += cs2;
    
    pair<int, int> reset2( 1, 0 ); // x-->0
    req_wait.addReset( reset2 );
    
    Argument       lhs( COUNTER_ARG, 0 );
    Argument       rhs( PARAMETER_ARG, 0 );
    CounterAction *caction =
    new CounterAction( lhs, ASSIGNMENT_ACTION, rhs ); // id=pid
    
    req_wait.addCounterAction( caction );
    
    typename INT_TAS_t::T_t wait_req( 2, 1 );
    
    pair<int, int> reset3( 1, 0 ); // x-->0
    wait_req.addReset( reset3 );
    wait_req.addCounterCons( ccs1 ); // id==0
    
    typename INT_TAS_t::T_t wait_cs( 2, 3 );
    
    Argument first4( COUNTER_ARG, 0 );
    Argument second4( PARAMETER_ARG, 0 );
    Argument rhs4( CONST_ARG, 0 );
    
    void *ccs2 = createConstraint( first4, second4, EQ, rhs4 ); // id==pid
    wait_cs.addCounterCons( ccs2 );
    typename INT_TAS_t::CS_t cs3( 1, 0, GT, k ); // x> k
    wait_cs += cs3;
    
    typename INT_TAS_t::T_t cs_A( 3, 0 );
    
    Argument       lhs2( COUNTER_ARG, 0 );
    Argument       rhs2( CONST_ARG, 0 );
    CounterAction *caction1 =
    new CounterAction( lhs2, ASSIGNMENT_ACTION, rhs2 ); // id=0;
    
    cs_A.addCounterAction( caction1 );
    
    ls.push_back( A );
    ls.push_back( req );
    ls.push_back( wait );
    ls.push_back( cs );
    
    es.push_back( A_req );
    es.push_back( req_wait );
    es.push_back( wait_req );
    es.push_back( wait_cs );
    es.push_back( cs_A );
    tmt=typename INT_TAS_t::AgentTemplate_t( ls, es, 0, 1 );
  }
  INT_TAS_t FisherGenerator::generate(int n) const{
    INT_TAS_t sys;
    for ( int i = 1; i <= n; i++ ) {
      Parameter param( 1 );
      param.setParameterMap( 0, i );
      typename INT_TAS_t::Agent_t tma( &tmt, param );
      
      sys += tma;
    }
    
    Counter counter( 0, 100 );
    sys.setCounterNum( 1 );
    sys.setCounter( 0, counter );
    return sys;
  }
}
