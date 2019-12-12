/**
 * @file   arraytype.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec  6 19:34:06 2019
 *
 * @brief  self type def
 *
 *
 */
#ifndef __ARRAY_TYEP_H
#define __ARRAY_TYEP_H

#include <string>

class TypeDefArray {

public:
  TypeDefArray() { low = high = 0; }
  TypeDefArray(const std::string &n) : name(n) { low = high = 0; }
  TypeDefArray(const std::string &n, int l, int h) : name(n), low(l), high(h) {}
  void setLow(const int l) { low = l; }
  void setHigh(const int h) { high = h; }
  void setName(const std::string &n) { name = n; }
  std::string getName() const { return name; }
  int getLow() const { return low; }
  int getHigh() const { return high; }

private:
  std::string name;
  int low, high;
};

#endif
