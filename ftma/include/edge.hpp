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
template<typename C, typename A>
struct  edge{
  int source, target;  //source location and target location of this edge. The index of location in tma.locations
  vector<C> cons;// set of constraint at this edge
  vector<A> actions;// set of actions at this edge
  vector<int> reset;// set of reset clock variables
};
}

#endif
