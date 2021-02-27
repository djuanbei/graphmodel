#include "alg/symmetry.h"

#include <cassert>
#include <map>

#include "io/stateout.h"

namespace graphmodel {

bool Symmetry::isSymmetry(const StateSet<UINT>& sets,
                          const ReachableSet<TMStateManager>& reachSet,
                          const TMStateManager* manager) const {
  vector<int> dummy(len);

  int* temp = manager->newState();
  UINT* comTemp = new UINT[manager->getCompressionSize()];
  for (int i = 0; i < len; i++) {
    dummy[i] = i;
  }
  bool re = true;
  for (auto e : sets) {
    sort(dummy.begin(), dummy.end());

    while (next_permutation(dummy.begin(), dummy.end())) {
      manager->decode(temp, e);
      // cout << "=====original=====" << endl;
      //   manager->dump(temp);
      swap(temp, dummy, manager);
      //  cout << "=====swap======" << endl;
      // manager->dump(temp);
      //  cout << "===========" << endl;
      manager->encode(comTemp, temp);

      if (sets.contain(comTemp) == NOT_FOUND) {
        manager->decode(temp, e);
        StateOutput::generatePath(reachSet, "onepath.gv", temp);
        vector<vector<int>> path_state = reachSet.getPath(temp);
        size_t i = 0;
        for (; i < path_state.size(); i++) {
          swap(&(path_state[i][0]), dummy, manager);
          cout << "************************" << endl;
          manager->dump(&(path_state[i][0]));
          manager->encode(comTemp, &(path_state[i][0]));

          // manager->getClockManager().decode(manager->getDBM(&(path_state[ i][
          // 0])));
          if (!sets.contain(comTemp)) {
            break;
          }
          manager->dump(&(path_state[i][0]));
          manager->decode(temp, comTemp);
          manager->dump(temp);
          assert(reachSet.contain(temp));
          assert(reachSet.contain(&(path_state[i][0])));
        }
        if (i > 0) {
          manager->dump(&(path_state[i - 1][0]));

          StateOutput::generatePath(reachSet, "onepath1.gv",
                                    &(path_state[i - 1][0]));
        }

        assert(false);
        re = false;
        break;
      }
    }
    if (!re) {
      break;
    }
  }
  manager->destroyState(temp);
  delete[] comTemp;
  return re;
}
void Symmetry::swap(int* source, const vector<int>& swap_map,
                    const TMStateManager* manager) const {
  vector<int> index(len);
  for (int i = 0; i < len; i++) {
    index[i] = i;
  }

  for (int i = 0; i < len; i++) {
    if (swap_map[i] != index[i]) {
      int j = i;
      for (; j < len; j++) {
        if (index[j] == swap_map[i]) {
          break;
        }
      }
      assert(j < len);
      manager->swap(j, i, source);
      int tt = index[i];
      index[i] = index[j];
      index[j] = tt;
    }
  }
}

}  // namespace graphmodel
