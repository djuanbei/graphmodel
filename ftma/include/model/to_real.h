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
#include "util/argument.h"
#include "util/typedef.h"
namespace graphmodel {
class TOReal {
 public:
  virtual RealArgument to_real(const TYPE_T& type,
                               const Argument& arg) const = 0;
  virtual CHANNEL_TYPE getChanType(const std::string& chan_name) const {
    return ONE2ONE_CH;
  }
};

}  // namespace graphmodel

#endif
