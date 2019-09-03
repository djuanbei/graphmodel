/**
 * @file   Counter.hpp
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
    
const int MAX_COUNTER_VALUE=255;
class Counter {
private:
  int value;
  int lb, up; // lb<= value<= up
public:
  Counter() { value = lb = up = 0; }
  Counter( int l, int u ) {
    value = l;
    lb    = l;
    up    = u;
  }
  void setValue( int v ) { value = v; }
};
} // namespace graphsat

#endif
