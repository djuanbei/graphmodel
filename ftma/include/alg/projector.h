/**
 * @file   projector.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Dec 16 19:33:24 2019
 *
 * @brief  state projector
 *
 *
 */

#ifndef PROJECTOR_H
#define PROJECTOR_H
#include <iostream>
#include <vector>

namespace graphsat {
using std::cout;
using std::endl;
using std::ostream;
using std::vector;

class Projector {
 public:
  virtual void operator()(const int* original_state,
                          vector<int>& proj) const = 0;
  virtual bool include(const vector<vector<int>>& lhs,
                       const vector<vector<int>>& rhs) const = 0;

  virtual ostream& dump(const vector<int>& proj_e, ostream& out) const = 0;

  virtual ostream& dump(const vector<vector<int>>& proj, ostream& out) const {
    for (auto& e : proj) {
      dump(e, out);
    }
    return out;
  }

  void dump_D(const vector<int>& proj_e) const { dump(proj_e, cout); }

  void dump_D(const vector<vector<int>>& proj) const { dump(proj, cout); }
};

}  // namespace graphsat

#endif
