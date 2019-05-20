/**
 * @file   Action.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:39:22 2019
 *
 * @brief  Action class which  do x=x+c where x is a counter
 *        variable and c is a constant integer
 *
 *
 */
#ifndef ACTION_HPP
#define ACTION_HPP
#include "counter.hpp"
#include "parameter.h"

namespace graphsat {
class CounterAction {
public:
  virtual void operator()( const Parameter &p, int *value ) const = 0;
};

} // namespace graphsat
#endif
