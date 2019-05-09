/**
 * @file   counter.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:37:15 2019
 *
 * @brief  Counter class
 *
 *
 */
#ifndef COUNTER_HPP
#define COUNTER_HPP
namespace graphsat {
class counter {
 private:
  int value; 
  int lb, up; //lb<= value<= up
 public:
  counter( ){
    value=lb=up=0;
  }
 
};
} // namespace graphsat

#endif
