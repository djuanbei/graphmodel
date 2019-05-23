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
#include <vector>
namespace graphsat {
using std::vector;
class Parameter {

public:
  Parameter() {}
  void       addValue( int v ) { value.push_back( v ); }
  void       setValue( int id, const int v ) { value[ id ] = v; }
  int        getValue( int id ) const { return value[ id ]; }
  const int *getValue() const { return &( value[ 0 ] ); }
  bool       empty() const { return value.empty(); }

private:
  vector<int> value;
};
} // namespace graphsat

#endif
