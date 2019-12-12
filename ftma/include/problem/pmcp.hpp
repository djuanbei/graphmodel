/**
 * @file   pmcp.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Wed Dec  4 13:57:59 2019
 *
 * @brief  this file give algorithm to deal with Parametrized Model Checking
 * Problem ( PMCP)
 *
 *
 */
#ifndef PMCP_H
#define PMCP_H
#include "problem/reachability.hpp"
#include "property/property.h"
#include "state/reachableset.hpp"
#include <algorithm>
#include <vector>

namespace graphsat {
using namespace std;
bool element_cmp(const vector<int> &lhs, const vector<int> &rhs);

template <typename SYS, typename G> class IncrementalCheck {

public:
  IncrementalCheck() : start(2), end(5), project_dim(2) {}
  bool check(const G &g, const Property *prop) {
    SYS dummy = g.generate(start);
    shared_ptr<typename SYS::StateManager_t> manager = dummy.getStateManager();
    ReachableSet<typename SYS::StateManager_t> pre_data(manager);

    dummy.addInitState(pre_data);
    Reachability<SYS> reacher(dummy);
    if (reacher.satisfy(pre_data, prop)) {
      return false;
    }
    vector<vector<int>> pre_project;
    pre_data.project(project_dim, pre_project);
    //  sort(pre_project.begin(), pre_project.end(), element_cmp);

    for (int i = 3; i < 5; i++) {
      SYS dummy = g.generate(i);
      shared_ptr<typename SYS::StateManager_t> manager =
          dummy.getStateManager();
      ReachableSet<typename SYS::StateManager_t> data(manager);

      dummy.addInitState(data);
      Reachability<SYS> reacher(dummy);
      if (reacher.satisfy(data, prop)) {
        return false;
      }
      vector<vector<int>> project;
      data.project(project_dim, project);
      //  sort(project.begin(), project.end(), element_cmp);

      if (equal(project, pre_project)) {
        return true;
      }
      pre_project.swap(project);
    }
    return false;
  }

private:
  int start;
  int end;
  size_t project_dim;

  bool equal(const vector<vector<int>> &lhs,
             const vector<vector<int>> &rhs) const {
    if (lhs.empty()) {
      return rhs.empty();
    }
    if (rhs.empty()) {
      return false;
    }
    assert(lhs[0].size() == rhs[0].size());

    size_t n = lhs[0].size();

    for (size_t i = 0; i < lhs.size(); i++) {
      size_t j = 0;
      for (; j < rhs.size(); j++) {
        size_t k = 0;
        for (; k < project_dim; k++) {
          if (lhs[i][k] != rhs[j][k]) {
            break;
          }
        }

        if (k == project_dim) {
          for (; k < n; k++) {
            if (lhs[i][k] > rhs[j][k]) {
              break;
            }
          }
        }
        if (k == n) {
          break;
        }
      }
      if (j == rhs.size()) {
        return false;
      }
    }
    return true;
  }
};

} // namespace graphsat

#endif
