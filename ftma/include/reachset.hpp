
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
  typedef Constraint<C> CS;
  typedef dbm<C, CS > DBM;
  typedef dbmset< C, DBM > DSet;
  tma<L,E> &TMA;
  DBM dbmManager;
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

    ks=new C[ 2*(TMA.clock_num+1) ];
    fill( ks, ks+2*(TMA.clock_num+1), LTEQ_ZERO);

    for(typename vector<L>::iterator it=TMA.locations.begin( ) ; it!=TMA.locations.end( ); it++ ){
      for( typename vector<CS>::iterator cit =it->invariants.begin( ); cit!=it->invariants.end( ); cit++ ){
        
        if( cit->x >0 && cit->y > 0){
          differenceCons.push_back( *cit);
        }else{
          if( cit->x >0){
            if( cit->matrix_value> ks[ cit->x] ){
              ks[ cit->x]=cit->matrix_value;
            }else{
              CS temp= cit->neg( );
              if( temp.matrix_value> ks[ temp.x]){
                ks[ temp.x]=temp.matrix_value;
              }
            }
          }
        }
      }
    }
    /**
     *      x-y<= k_i
     *      x-y< -k_{i+n+1}
     * 
     */


    for( int i=1; i< TMA.clock_num+1; i++){
      ks[i+TMA.clock_num+1]=LTEQ_ZERO-ks[i];
    }
    
  }
  ~reach( ){
    delete[ ] ks;
    deleteData( );
  }
  
  bool reachableSet(const tma<L,E> &TMA, vector< dbmset<C, DBM > > &reachSet, DBM & d ) const {
        
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
  bool run(const tma<L,E> & TMA, const graph_t<int> & graph, vector< dbmset<C, DBM > > &reachSet) const {
    
    int initial_loc= TMA.initial_loc;
    int vertex_num=graph.getVertex_num( );
    vector<int> changed;
    set<int>  secondChanged;

        
    //There are no edges connect with  initial location
    if( initial_loc>= vertex_num){
      return true;
    }
    //    ASSERT(initial_loc>=0, "The initial location must greater or equal to 0" );
    assert(initial_loc>=0 );
    
    vector< dbmset<C, DBM > > waitSet;
    waitSet.resize(TMA.locations.size( ));
    vector< dbmset<C, DBM > > secondWaitSet;
    secondWaitSet.resize(TMA.locations.size( ));
    
    C* D= dbmManager.newMatrix( );
    TMA.locations[initial_loc ].apply( D);
    waitSet[ initial_loc].add(D );
    reachSet[ initial_loc].add( D);
    changed.push_back( initial_loc);

    while( !changed.empty( )){

      for( size_t i=0; i< changed.size( ); i++){
        int v=changed[ i];

        int link, outDegree, tempSnk;
        outDegree=graph.getOutDegree(v);
        for( int j=0; j<  outDegree; j++){
          link=graph.getAdj(v, j);
          DSet next;
          if(TMA.edges[link].apply(waitSet[ v], dbmManager, next )){
            DSet next1;
            graph.findRhs( link, v, tempSnk);
            if(TMA.locations[ tempSnk].apply( dbmManager, next, next1)){
              secondWaitSet[ tempSnk].And( next1);
              reachSet[ tempSnk].And( next1);
              secondChanged.insert( tempSnk);
            }
          }
        }

      }
      changed.clear( );
      changed.insert(changed.begin( ), secondChanged.begin( ), secondChanged.end( ) );
      secondChanged.clear( );

      waitSet=secondChanged;
      for( int j=0; j< vertex_num; j++){
        secondWaitSet[ j].clear( );
      }
      
    }
        
  }

  void deleteData(  ){
    for(typename vector< dbmset<C, DBM > >::iterator it =reachSet.begin( ) ; it!= reachSet.end( ); it++ ){
      it->deleteAll();
    }
    reachSet.clear();
  }
  
};
}

#endif
