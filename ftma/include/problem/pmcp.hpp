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

#include <algorithm>
#include <vector>

#include "alg/util.h"
#include "problem/reachability.hpp"
#include "property/property.h"
#include "state/reachableset.hpp"

namespace graphsat {
using namespace std;
bool element_cmp(const vector<int>& lhs, const vector<int>& rhs);

template <typename SYS, typename G, typename PROJ>
class IncrementalCheck {
 public:
  IncrementalCheck() : start(2), end(8), project_dim(2) {}

  bool check(const G& g, const Property* prop) {
    SYS dummy = g.generate(start);
    shared_ptr<typename SYS::StateManager_t> manager = dummy.getStateManager();
    ReachableSet<typename SYS::StateManager_t> pre_data(manager);

    // dummy.addInitState(pre_data);
    Reachability<SYS> reacher(dummy);
    if (reacher.satisfy(&pre_data, prop)) {
      return false;
    }
    vector<vector<int>> pre_project;
    PROJ proj(manager, project_dim);

    pre_data.project(proj, pre_project);
    /// sort(pre_project.begin(), pre_project.end(), element_cmp);
    deleteRepeat(pre_project);

    for (int i = start + 1; i < end; i++) {
      std::cout << i << " instances" << std::endl;
      SYS dummy = g.generate(i);
      shared_ptr<typename SYS::StateManager_t> manager =
          dummy.getStateManager();
      ReachableSet<typename SYS::StateManager_t> data(manager);

      //  dummy.addInitState(data);
      Reachability<SYS> reacher(dummy);
      if (reacher.satisfy(&data, prop)) {
        return false;
      }
      vector<vector<int>> project;
      PROJ proj(manager, project_dim);
      data.project(proj, project);
      deleteRepeat(project);
      //  sort(project.begin(), project.end(), element_cmp);

      if (proj.include(project, pre_project)) {
        return true;
      }

      pre_project.swap(project);
    }
    return false;
  }

 private:
  void deleteRepeat(vector<vector<int>>& pre_project) const {
    std::vector<vector<int>>::iterator it;
    sort(pre_project.begin(), pre_project.end(), vect_cmp<int>);
    it = std::unique(pre_project.begin(), pre_project.end());
    pre_project.resize(std::distance(pre_project.begin(), it));
  }
  int start;
  int end;
  size_t project_dim;
};

}  // namespace graphsat

#endif
