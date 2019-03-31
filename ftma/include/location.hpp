/**
 * @file   location.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:33:47 2019
 * 
 * @brief  Timed automata location class
 * 
 * 
 */
#ifndef __LOCATION_HPP
#define __LOCATION_HPP
#include<vector>
namespace ftma{
using namespace std;
template<typename C>
struct location{
  vector<C> cons; //set of invariants  in this location
};
}

#endif
