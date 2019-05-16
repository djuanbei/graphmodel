
/**
 * @file   reachability.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Apr 27 08:49:40 2019
 *
 * @brief  reachability problem of timed automata
 *
 *
 */

#ifndef __REACHABILITY_HPP
#define __REACHABILITY_HPP

#include <map>
#include <set>
#include <vector>
#include<model/ta.hpp>
#include "parallel.h"

namespace graphsat {

using namespace std;

template <typename ReachableSet> class Reachability {

private:

  ReachableSet &                        data;
  const TAS_t &ta;

public:
  Reachability( ReachableSet &outData )
      : data( outData )
      , ta( data.getTA() ) {

  }

  ~Reachability() {}

  void computeAllReachableSet() {
    vector< vector<CS_t> >  cons;

    int component_num=ta.getComponentNum( );
    vector<int> loc(component_num, -1 );
    
    run(loc, cons );
    
  }

  bool reachable( const vector<int> loc ) {
    vector< vector<CS_t> > cons;
    return run( loc, cons );
  }

  /**
   * @param L
   * @param TA
   * @param C
   * @param reachSet
   *
   * @return
   */
  bool run( const vector<int> & loc, const vector< vector<CS_t>> &cons ) {

    Check_State re = data.find( loc, cons );

    if ( re != UNKOWN ) {
      if ( TRUE == re ) {
        return true;
      } else if ( FALSE == re ) {
        return false;
      }
    }



    // For given target find the source which change in last step


    StateSet_t secondWaitSet;
    bool find=false;
    while ( !data.waitSet.empty( ) ) {

      /**
       * TODO:
       * parallel  section
       *
       */
      find=false;
      
      typename StateSet_t::iterator end1 = data.waitSet.end();

      for ( typename StateSet_t::iterator it = data.waitSet.begin(); !find &&(it != end1);
            ++it ) {

        if ( data.oneStep( loc, cons, *it,
                           secondWaitSet ) ) { 
          find = true;
        }
          
      }
      if ( find ) {
        return true;
      }
      data.update(  secondWaitSet );
    }
    return false;
  }
};
} // namespace graphsat

#endif
