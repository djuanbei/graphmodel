
/**
 * @file   reachset.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:24:17 2019
 * 
 * @brief  reachable analysis of an TMA
 * 
 * 
 */

#ifndef __REACH_SET_HPP
#define __REACH_SET_HPP
#include<vector>
#include "graph/graph.hpp"
#include "dbm.hpp"
#include "dbmset.hpp"

namespace ftma{

using namespace std;
using namespace raptor;
using namespace std;

template< typename C, typename L, typename E>
class reach{
 private:
  typedef dbm<C, Constraint<C> > D;
  typedef dbmset< C, D > DSet;
  tma<L,E> &TMA;

  D dbmManager;
  
  vector< DSet > reachSet;
  graph_t <int> graph;
  C * ks;
  
  vector<Constraint<C> > differenceCons;
  
 public:
  reach( tma<L,E> &oTMA ):TMA( oTMA),dbmManager(oTMA.clock_num ){

    vector<int>  srcs;
    vector<int> snks;
    for(size_t i=0; i< TMA.edges.size( ); i++  ){
      srcs.push_back(TMA.edges[i].source);
      snks.push_back(TMA.edges[i].target);
    }
    reachSet.resize(TMA.locations.size( ) );
    graph.initial( srcs, snks);

    ks=new C[ 2*(oTMA.clock_num+1) ];
    fill( ks, ks+2*(oTMA.clock_num+1), dbmUTIL<C>::LTEQ_ZERO);
    
    
  }
  ~reach( ){
    delete[ ] ks;
    deleteData( );
  }
  
  bool reachableSet(const tma<L,E> &TMA, vector< dbmset<C, D > > &reachSet, D & d ) const {
        
    run(TMA, graph,  reachSet );

    return true;
    
  }
  /** 
   * 
   * 
   * @param L 
   * @param TMA 
   * @param C 
   * @param reachSet 
   * 
   * @return 
   */
  bool run(const tma<L,E> & TMA, const graph_t<int> & graph, vector< dbmset<C, D > > &reachSet) const {
    int initial_loc=    TMA.initial_loc;
    int vertex_num=graph.getVertex_num( );
    //There are no edges connect with  initial location
    if( initial_loc>= vertex_num){
      return true;
    }
    //    ASSERT(initial_loc>=0, "The initial location must greater or equal to 0" );
    assert(initial_loc>=0 );
    
    vector< dbmset<C, D > > waitSet;
    waitSet.resize(TMA.locations.size( ));
    C* DM= dbmManager.newMatrix( );
    waitSet[ initial_loc].add(DM );
        
  }

  void deleteData(  ){
    for(typename vector< dbmset<C, D > >::iterator it =reachSet.begin( ) ; it!= reachSet.end( ); it++ ){
      it->deleteAll();
    }
    reachSet.clear();
  }
  
};
}

#endif
