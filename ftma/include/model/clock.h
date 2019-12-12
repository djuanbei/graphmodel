/**
 * @file   clock.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Wed Dec 11 16:04:23 2019
 * 
 * @brief  clock data structure
 * 
 * 
 */

#ifndef TYPE_CLOCK_H
#define TYPE_CLOCK_H
#include <cassert>

namespace graphsat{


struct Clock{
  explicit Clock( const int d):id( d){
    assert( id>=0 &&"The clock id must greater or equal than 0.");
  }
  Clock( ):id( 0){
    
  }
  int id;
  const static Clock ZERO;
};

}

#endif 
