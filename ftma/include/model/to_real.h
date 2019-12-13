/**
 * @file   to_real.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec 13 10:12:04 2019
 *
 * @brief  Convert Argument to RealArgument
 *
 *
 */
#ifndef _TO_REAL_H
#define _TO_REAL_H
#include "util/typedef.h"
namespace graphsat {
class TOReal {
public:
  virtual RealArgument to_real(const TYPE_T &type,
                               const Argument &arg) const = 0;
};
} // namespace graphsat

#endif
