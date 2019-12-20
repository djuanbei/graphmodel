/**
 * @file   next_step.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec 20 13:35:47 2019
 *
 * @brief  The util class for generation next step
 *
 *
 */
#include <vector>

#include "one_step.h"
namespace graphsat {

class NextStep {
public:
  virtual std::vector<OneStep> getNextStep(void *state) const = 0;
};

} // namespace graphsat
