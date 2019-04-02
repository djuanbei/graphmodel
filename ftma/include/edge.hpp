/**
 * @file   edge.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:45:30 2019
 * 
 * @brief  edge class in timed automata
 * 
 * 
 */
#ifndef EDGE_HPP
#define EDGE_HPP
#include<vector>
namespace ftma{
using namespace std;
template<typename C, typename CS, typename A>
struct  edge{
  typedef dbm<C, CS > DBM;
  typedef dbmset< C, DBM > DSet;
  
  int source, target;  //source location and target location of this edge. The index of location in tma.locations
  vector<CS> cons;// set of constraint at this edge
  vector<A> actions;// set of actions at this edge
  vector<int> reset;// set of reset clock variables

  bool apply(const   DSet & Ds,  const DBM &dbmManager, DSet & next  ) const{
    next.deleteAll( );
    vector<C*> vecSet;
    Ds.toVector( vecSet);
    
    for( typename vector<C*> ::iterator it =  vecSet.begin( ); it!= vecSet.end( ); it++){
      C *D=*it;
      for(typename vector<CS>::iterator cit=cons.begin( ); cit!= cons.end( ); cit++ ){
        dbmManager.andImpl( D, *cit);
      }
      if( dbmManager.isConsistent( D)){
        for( vector<int>::iterator rit=reset.begin( ); rit!=reset.end( ); rit++){
          assert( *rit>0);// clock id start from 1
          dbmManager.resetImpl(D, *rit, 0 );
        }
        next.add( D);        
      }else{
        delete[ ] D;
      }
    }
    
    Ds.clear( );
    return next.size( )>0;
      
  }

  bool apply( C* D, const DBM & dbmManager) const{

    for(typename vector<CS>::iterator cit=cons.begin( ); cit!= cons.end( ); cit++ ){
      dbmManager.andImpl( D, *cit);
    }
    if( dbmManager.isConsistent( D)){
      for( vector<int>::iterator rit=reset.begin( ); rit!=reset.end( ); rit++){
        assert( *rit>0);// clock id start from 1
        dbmManager.resetImpl(D, *rit, 0 );
      }
      return true;
    }else{
       return false;
    }
    
  }
  
};
}

#endif
