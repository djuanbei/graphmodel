#include "util/timer.h"

namespace graphsat {

bool test_timer(int diff) {
  using std::chrono::milliseconds;
  typedef timer::duration duration;

  const milliseconds sleep_time(1000);

  timer t;
  std::this_thread::sleep_for(sleep_time);
  duration recorded = t.elapsed();

  // make sure the clock and this_thread::sleep_for is precise within one
  // millisecond (or at least in agreement as to how inaccurate they are)
  return (recorded - milliseconds(diff) < sleep_time) &&
         (recorded + milliseconds(diff) > sleep_time);
}
}  // namespace graphsat
