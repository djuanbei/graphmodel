/**
 * @file   timer.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Jan 13 09:21:27 2020
 *
 * @brief  timer class
 *
 *
 */

#ifndef _TIMER_H
#define _TIMER_H
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
namespace graphsat {

class timer {
 public:
  typedef std::chrono::high_resolution_clock clock;
  typedef clock::time_point time_point;
  typedef clock::duration duration;

 public:
  timer() { reset(); }

  void reset() { _starttime = clock::now(); }

  duration elapsed() const { return clock::now() - _starttime; }

 protected:
  time_point _starttime;
};

// make sure the clock and this_thread::sleep_for is precise within diff
// millisecond (or at least in agreement as to how inaccurate they are)
bool test_timer(int diff = 10);

// The volatile_write function is a simple function to force the system to
// actually write data somewhere in memory. In this case, it prevents the
// optimizer from seeing that we are not doing anything with the result of func
// in run_test.

template <typename T>
void volatile_write(const T& x) {
  volatile T* p = new T;
  *p = x;
  delete p;
}

template <typename Function>
void run_time(const std::string& name, Function func) {
  std::cout << name;
  timer t;
  volatile_write(func());
  timer::duration duration = t.elapsed();
  std::cout << '\t' << duration.count() << std::endl;
}

}  // namespace graphsat
#endif
