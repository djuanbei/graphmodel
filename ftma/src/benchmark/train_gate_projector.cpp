#include "benchmark/train_gate_projector.h"

namespace graphsat {

TrainGateProjector::TrainGateProjector(
    const std::shared_ptr<TMStateManager>& out_manager, const int pro_d)
    : manager(out_manager) {
  component_num = manager->getComponentNumber();
  pro_dim = pro_d;
  clock_start = manager->getClockStart();
  pro_clock_start = pro_dim * (pro_dim + 1);
}

void TrainGateProjector::operator()(const int* original_state,
                                    vector<int>& proj) const {
  proj.clear();
  for (int i = 0; i < pro_dim; i++) {
    int loc = manager->getLocationID(i, original_state);

    assert(loc >= 0 && "The location must greater or equal to 0.");
    proj.push_back(loc);
  }
  vector<int> locs(pro_dim, -1);
  int len = original_state[2 * component_num];  // gate is a component
  assert((len >= 0 && len <= component_num) &&
         "len variable is a bounded in [0, component_num].");
  int N = component_num;

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

bool TrainGateProjector::contain(
    const vector<int>& one, const std::vector<std::vector<int>>& rhs) const {
  size_t n = one.size();

  size_t equal_size = (pro_dim + pro_dim * pro_dim) * sizeof(int);
  size_t equal_n = (pro_dim + pro_dim * pro_dim);
  size_t j = 0;
  for (; j < rhs.size(); j++) {
    size_t k = 0;
    if (0 == memcmp(&(one[0]), &(rhs[j][0]), equal_size)) {
      k = equal_n;
      for (; k < n; k++) {
        if (one[k] > rhs[j][k]) {
          break;
        }
      }
    }
    if (k == n) {
      return true;
    }
  }
  return false;
}

std::vector<int> TrainGateProjector::getSrc(
    const std::vector<int>& proj) const {
  std::vector<int> re;
  return re;
}

std::vector<int> TrainGateProjector::getSnk(
    const std::vector<int>& proj) const {
  std::vector<int> re;
  return re;
}

bool TrainGateProjector::constructState(
    int* state, const std::vector<std::vector<int>>& projs,
    const std::vector<std::vector<int>>& oneStataes,
    const std::vector<int>& vertices, const std::vector<int>& choose,
    const std::vector<std::pair<int, int>>& link_src_snk_map,
    const std::map<int, int>& link_map) const {
  return false;
}

std::vector<int> TrainGateProjector::to_vec(const TMStateManager* manager,
                                            const int* original_state) const {
  std::vector<int> re;
  return re;
}

std::ostream& TrainGateProjector::dump(const vector<int>& proj_e,
                                       std::ostream& out) const {
  for (int i = 0; i < pro_dim; i++) {
    out << std::setw(LOC_OUT_WIDTH) << manager->getLocationName(i, proj_e[i]);
  }
  out << endl;
  return DBMManager::dump(out, &(proj_e[0]) + pro_clock_start, pro_dim + 1);
}

}  // namespace graphsat