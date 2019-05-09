/**
 * @file   state.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu May  9 10:28:46 2019
 * 
 * @brief  state structure
 * 
 * 
 */
#ifndef __STATE_HH
#define __STATE_HH

namespace graphsat{


class StateElem{
 private:
  DBMset<int, DBMset<int, DBM<int> >>  clockState;
  map<int, int> couterState;
  
  
};


class State{
 private:
  vector<StateElem> states;
  
};




}
#endif

