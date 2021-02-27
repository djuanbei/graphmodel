/**
 * @file   path.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec 20 13:28:12 2019
 *
 * @brief  one path
 *
 *
 */
#ifndef RUN_PATH_H
#define RUN_PATH_H

namespace graphmodel {

class Path {
 public:
  /**
   * @return  the next step, null_ptr if reaches the terminal
   */
  virtual const OneStep* next();
  /**
   * reset the next to the first step
   */
  virtual void reset();
};

}  // namespace graphmodel

#endif
