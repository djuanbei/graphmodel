/**
 * @file   parameter.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat May 18 11:36:23 2019
 *
 * @brief
 *
 *
 */

#ifndef __PARAMETER_H
#define __PARAMETER_H

namespace graphsat {
class Parameter {

public:
  Parameter()
      : value( NULL ) {}
  ~Parameter() { delete[] value; }

private:
  // Parameter(const Parameter& p){

  // }

  Parameter &operator=( const Parameter &p ) { return *this; }
  int *      value;
};
} // namespace graphsat

#endif
