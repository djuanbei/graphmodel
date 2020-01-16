#include "benchmark/train_gate_projector.h"

namespace graphsat {

TrainGateProjector::TrainGateProjector(
    const std::shared_ptr<TMStateManager>& out_manager, const int pro_d)
    : Projector(out_manager) {
  component_num = manager->getComponentNumber();
  assert(2 == pro_d);
  pro_dim = pro_d;
  clock_start = manager->getClockStart();
  pro_clock_start = pro_dim * (pro_dim + 1) + 1;
  comparae_data_len = pro_clock_start + (pro_dim + 1) * (pro_dim + 1);
}

std::vector<int> TrainGateProjector::to_vec(const TMStateManager* manager,
                                            const int* original_state) const {
  vector<int> proj;
  for (int i = 0; i < pro_dim; i++) {
    int loc = manager->getLocationID(i, original_state);

    assert(loc >= 0 && "The location must greater or equal to 0.");
    proj.push_back(loc);
  }  // pro_dim
  int component_num = manager->getComponentNumber();
  int gate_loc = manager->getLocationID(component_num - 1, original_state);
  proj.push_back(gate_loc);  // The gate locatio  pro_dim+1
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
  }  // pro_dim+1 +( pro_dim)*( pro_dim)

  for (int i = 0; i <= pro_dim; i++) {
    for (int j = 0; j <= pro_dim; j++) {
      proj.push_back(original_state[i * (component_num) + j + clock_start]);
    }
  }  // pro_dim+1 +( pro_dim)*( pro_dim) +( pro_dim+1)*( pro_dim+1)
  proj.insert(proj.end(), locs.begin(), locs.end());
  return proj;
}

bool TrainGateProjector::contain(
    const vector<int>& one, const std::vector<std::vector<int>>& rhs) const {
  size_t n = comparae_data_len;

  size_t equal_size = pro_clock_start * sizeof(int);
  size_t equal_n = pro_clock_start;
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
  re.push_back(proj[0]);                    // train location
  re.push_back(proj[2]);                    // gate location
  re.push_back(proj[pro_clock_start + 1]);  // 0-x ~
  re.push_back(proj[pro_clock_start + 3]);  // x-0 ~
  re.push_back(proj[comparae_data_len]);    // train index
  return re;
}

std::vector<int> TrainGateProjector::getSnk(
    const std::vector<int>& proj) const {
  std::vector<int> re;

  re.push_back(proj[1]);                      // train location
  re.push_back(proj[2]);                      // gate location
  re.push_back(proj[pro_clock_start + 2]);    // 0-x ~
  re.push_back(proj[pro_clock_start + 6]);    // x-0 ~
  re.push_back(proj[comparae_data_len + 1]);  // train index

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

static bool sortQ(const pair<int, int>& lhs, const pair<int, int>& rhs) {
  return lhs.first < rhs.first;
}
bool TrainGateProjector::constructState(
    TMStateManager* manager, int* state,
    const std::vector<std::vector<int>>& pre_projs,
    const std::vector<std::vector<int>>& vertices,
    const std::vector<int>& links,
    const std::vector<std::pair<int, int>>& link_src_snk_map) const {
  int num = vertices.size();
  assert(4 == num);
  state[num] = vertices[0][1];  // gate location
  for (int i = 0; i < num; i++) {
    state[i] = vertices[i][0];  // train location
  }
  int gate_id = manager->getCompentId("Gate", 0);

  int* list = manager->getValue(gate_id, "list", state);
  int* len = manager->getValue(gate_id, "len", state);
  vector<pair<int, int>> temp;
  for (int i = 0; i < num; i++) {
    if (vertices[i][4] > -1) {
      temp.push_back(make_pair(vertices[i][4], i));
    }
  }
  std::sort(temp.begin(), temp.end(), sortQ);
  *len = temp.size();
  for (size_t i = 0; i < temp.size(); i++) {
    list[i] = temp[i].second;
  }
  int *dbm=manager->getDBM( state);
  

  return true;
}

}  // namespace graphsat
