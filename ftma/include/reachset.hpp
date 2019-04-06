
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
  
  int initial_loc; // the initial location of timed automata
  int vertex_num; // number of locations in 


  tma<L,E> &TMA;
  DBM dbmManager;
  vector< DSet > reachSet;
  graph_t <int> graph;
  C * ks;
  
  vector<Constraint<C> > differenceCons;
  
 public:
  reach(tma<L,E> &oTMA ):TMA(oTMA),dbmManager(oTMA.clock_num ){
    
      
    initial_loc= TMA.initial_loc;
   
    vector<int> srcs;
    vector<int> snks;
    
    for(size_t i=0; i< TMA.edges.size(); i++ ){
      srcs.push_back(TMA.edges[i].source);
      snks.push_back(TMA.edges[i].target);
    }

    graph.initial(srcs, snks);

    vertex_num=graph.getVertex_num();

    reachSet.resize(vertex_num);
      
    ks=new C[2*(TMA.clock_num+1) ];
    fill(ks, ks+2*(TMA.clock_num+1), LTEQ_ZERO);

    for(typename vector<L>::iterator it=TMA.locations.begin() ; it!=TMA.locations.end(); it++ ){
      for(typename vector<CS>::iterator cit =it->invariants.begin(); cit!=it->invariants.end(); cit++ ){
        
        if(cit->x >0 && cit->y > 0){
          differenceCons.push_back(*cit);
        }else{
          if(cit->x >0){
            if(cit->matrix_value> ks[cit->x] ){
              ks[cit->x]=cit->matrix_value;
            }else{
              CS temp= cit->neg();
              if(temp.matrix_value> ks[temp.x]){
                ks[temp.x]=temp.matrix_value;
              }
            }
          }
        }
      }
    }
    for(int i=1; i<= TMA.clock_num+1; i++){
      ks[i]=getMatrixValue(getRight(ks[i]), false);
    }
    
    /**
     *      x-y<= k_i
     *      x-y< -k_{i+n+1}
     * 
     */

    for(int i=1; i< TMA.clock_num+1; i++){
      ks[i+TMA.clock_num+1]=LTEQ_ZERO-ks[i];
    }
    
  }
  ~reach(){
    delete[] ks;
    deleteData();
  }
  
    
  bool reachableSet()  {
        
    run();

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
  bool run()  {
    vector<int> changed;
    set<int>  secondChanged;
    
    //For given target find the source which change in last step
    map<int, vector<int> > relatedLinks;
    
    //There are no edges connect with  initial location
    if(initial_loc>= vertex_num){
      return true;
    }
    //    ASSERT(initial_loc>=0, "The initial location must greater or equal to 0" );
    assert(initial_loc>=0);
    
    vector< dbmset<C, DBM > > waitSet(vertex_num);
    vector< dbmset<C, DBM > > secondWaitSet(vertex_num );
      
    C* D= dbmManager.newMatrix();
    TMA.locations[initial_loc ].apply(dbmManager, D);
    waitSet[initial_loc].add(dbmManager, D);
    reachSet[initial_loc].add(dbmManager, D);
    changed.push_back(initial_loc);

    while(!changed.empty()){
      relatedLinks.clear( );
      for(size_t i=0; i< changed.size(); i++){
        int source=changed[i];
        int link, outDegree, target;
        outDegree=graph.getOutDegree(source);
        for(int j=0; j<  outDegree; j++){
          link=graph.getAdj(source, j);

          graph.findRhs(link, source, target);
          relatedLinks[ target].push_back( link);
        }
      }

      vector<int> targets;
      vector<vector<int> > vecRelatedLinks;
      for(map<int, vector<int> >::iterator it=relatedLinks.begin( ); it!=relatedLinks.end( ); it++){
        targets.push_back( it->first);
        vecRelatedLinks.push_back( it->second);
      }

      /**
       * TODO:
       * parallel  section
       * 
       */

      for( int i=0; i< (int)targets.size( ); i++){
        
        int source, target, link;
        target=targets[i];
        
        for( vector<int>::iterator lit=vecRelatedLinks[i].begin( ); lit!=vecRelatedLinks[i].end( ); lit++){
          link=*lit;
          graph.findRhs(link, target, source);

          DSet next;
          if(TMA.edges[link].apply(dbmManager, waitSet[source],  next)){
            DSet next1;
            graph.findRhs(link, source, target);
            if(TMA.locations[target].apply(dbmManager, next, next1)){
              vector<C*> vec;
              next1.toVector( vec);
              for(size_t k=0; k< vec.size( ); k++ ){
                vector<C*> reVec;
                dbmManager.norm(vec[k], ks, differenceCons,  reVec);
                
                for(size_t h=0; h< reVec.size( ); h++ ){
                  if(reachSet[target].add(dbmManager, reVec[h])){
                    secondWaitSet[target].add(dbmManager, reVec[h]);                    
                  }
                }
                
              }
              secondChanged.insert(target);                              
            }
          }
        }
      }

      changed.clear();
      changed.insert(changed.begin(), secondChanged.begin(), secondChanged.end());
      secondChanged.clear();

      waitSet=secondWaitSet;
      for(size_t i=0; i< changed.size(); i++){
        int source=changed[i];
        secondWaitSet[source].clear();
      }

    }
    return true;
  }

  void deleteData(){
    for(typename vector< dbmset<C, DBM > >::iterator it =reachSet.begin() ; it!= reachSet.end(); it++){
      it->deleteAll();
    }
    reachSet.clear();
  }
  
};
}

#endif
