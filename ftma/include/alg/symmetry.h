/**
 * @file   symmetry.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Dec 26 15:05:11 2019
 *
 * @brief  state symmetry  property
 *
 *
 */

#ifndef _SYMMETRY_H
#define _SYMMETRY_H
#include <algorithm>
#include <vector>

#include "state/discretestate.hpp"
#include "state/reachableset.hpp"
#include "state/ta_statemanager.h"

namespace graphsat {
using namespace std;
class Symmetry {
public:
  Symmetry(int n) : len(n) {}

  virtual bool isSymmetry(const StateSet<UINT> &sets,
                          const ReachableSet<TMStateManager> &reachSet,
                          const TMStateManager *manager) const;

protected:
  void swap(int *source, const vector<int> &swap_map,
            const TMStateManager *manager) const;

private:
  int len;
};

} // namespace graphsat

#endif
