/**
 * @file   train_gate_projector.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Dec 16 20:15:30 2019
 *
 * @brief  projection for train-gate example
 *
 *
 */
#ifndef TRAIN_GATE_PROJECTIOR_H
#define TRAIN_GATE_PROJECTIOR_H
#include "alg/projector.h"

namespace graphsat {
template <typename StateManager_t>
class TrainGateProjector : public Projector<StateManager_t> {
public:
  TrainGateProjector(const shared_ptr<StateManager_t> &manager,
                     const int pro_d) {
    component_num = manager->getComponentNum();
    pro_dim = pro_d;
    clock_start = manager->getClockStart();
  }
  virtual void operator()(const int *original_state, vector<int> &proj) const {
    proj.clear();
    for (int i = 0; i < pro_dim; i++) {
      assert(original_state[i] >= 0 &&
             "The location must greater or equal to 0.");
      proj.push_back(original_state[i]);
    }
    vector<int> locs(pro_dim, -1);
    int len = original_state[3 * component_num]; // gate is a component
    assert((len >= 0 && len <= component_num) &&
           "len variable is a bounded in [0, component_num].");
    int N = 2 * component_num;

    for (int i = 0; i < len; i++) {
      assert((original_state[i + N] >= 0 &&
              original_state[i + N] < component_num - 1) &&
             "The element of list is the id of train.");
      if (original_state[i + N] < pro_dim) {
        locs[original_state[i + N]] = i;
      }
    }

    for (int i = 0; i < pro_dim; i++) {
      for (int j = 0; j < pro_dim; j++) {
        if (i == j) {
          proj.push_back(EQUAL);
          continue;
        }
        if (locs[i] > -1 && locs[j] > -1) {
          assert(locs[i] != locs[j]);
          if (locs[i] < locs[j]) {
            proj.push_back(FRONTER);
          } else {
            proj.push_back(LATER);
          }
        } else if (locs[i] > -1) {
          proj.push_back(FRONTER);
        } else if (locs[j] > -1) {
          proj.push_back(LATER);
        } else {
          proj.push_back(UNKOWN);
        }
      }
    }

    for (int i = 0; i <= pro_dim; i++) {
      for (int j = 0; j <= pro_dim; j++) {
        proj.push_back(original_state[i * (component_num) + j + clock_start]);
      }
    }
  }

  bool include(const vector<vector<int>> &lhs,
               const vector<vector<int>> &rhs) const {
    if (lhs.empty()) {
      return rhs.empty();
    }
    if (rhs.empty()) {
      return false;
    }
    assert(lhs[0].size() == rhs[0].size());

    size_t n = lhs[0].size();

    size_t equal_size = pro_dim + pro_dim * pro_dim;

    for (size_t i = 0; i < lhs.size(); i++) {
      size_t j = 0;
      for (; j < rhs.size(); j++) {
        size_t k = 0;
        for (; k < equal_size; k++) {
          if (lhs[i][k] != rhs[j][k]) {
            break;
          }
        }

        if (k == equal_size) {
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
  void setLocName(const int id, const string &n) { loc_name_map[id] = n; }

  virtual ostream &dump(const vector<int> &proj_e, ostream &out) const {
    for (int i = 0; i < pro_dim; i++) {
    }
    return out;
  }

  virtual ostream &dump(const vector<vector<int>> &proj, ostream &out) const {
    return out;
  }

private:
  int component_num;
  int pro_dim;
  int clock_start;
  map<int, string> loc_name_map;

  enum Order { UNKOWN = 0, EQUAL = 1, FRONTER = 2, LATER = 3 };
};

} // namespace graphsat
#endif
