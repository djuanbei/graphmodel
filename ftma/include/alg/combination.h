/**
 * @file   combination.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Jan  5 09:56:38 2020
 *
 * @brief  combination algorithm
 *
 *
 */
#ifndef _COMBINATION_H
#define _COMBINATION_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
namespace graphsat {

struct C_unique {
  int current;
  C_unique() : current(0) {}
  int operator()() { return ++current; }
};

class Combination_n_k {
 public:
  Combination_n_k(int n, int k);
  bool next();
  const std::vector<int>& operator*() const { return data; }

 private:
  int n;
  int k;
  std::vector<int> data;
  std::vector<int>::iterator first, last;
  C_unique uniqueNumber;
};

class FullChoose {
 public:
  FullChoose(const std::vector<int>& f);

  FullChoose(int len, int base);

  bool next();

  const std::vector<int>& operator*() const { return data; }

 private:
  std::vector<int> perChoose;
  std::vector<int> data;
  size_t len = 0;
  size_t current;
};

}  // namespace graphsat

#endif
