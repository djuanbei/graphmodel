#include "benchmark/fischer_projector.h"

#include <map>

#include "alg/combination.h"
#include "alg/one_step.h"
#include "alg/ta_next_step.h"
#include "benchmark/fischer.h"

namespace graphmodel {

FischerProjector::FischerProjector(
    const std::shared_ptr<TMStateManager>& out_manager, const int pro_d)
    : Projector(out_manager) {
  component_num = manager->getComponentNumber();
  pro_dim = pro_d;
  clock_start = manager->getClockStart();
}

std::vector<int> FischerProjector::getSrc(const std::vector<int>& e) const {
  std::vector<int> re;
  re.push_back(e[0]);
  re.push_back(e[2]);  // id!=0
  re.push_back(e[3] == 1);
  re.push_back(e[5]);
  re.push_back(e[7]);

  return re;
}

std::vector<int> FischerProjector::getSnk(const std::vector<int>& e) const {
  std::vector<int> re;
  re.push_back(e[1]);
  re.push_back(e[2]);
  re.push_back(e[3] == 2);
  re.push_back(e[6]);
  re.push_back(e[10]);
  return re;
}

std::vector<int> FischerProjector::to_vec(const TMStateManager* manager,
                                          const int* original_state) const {
  std::vector<int> proj;
  for (int i = 0; i < pro_dim; i++) {
    proj.push_back(original_state[i]);
  }
  int id = *manager->getValue(0, "id", original_state);
  proj.push_back(id != 0);
  if (id == 1) {
    proj.push_back(1);
  } else if (id == 2) {
    proj.push_back(2);
  } else {
    proj.push_back(0);
  }

  const int* dbm = manager->getDBM(original_state);
  for (int i = 0; i <= pro_dim; i++) {
    for (int j = 0; j <= pro_dim; j++) {
      proj.push_back(manager->getClockManager().at(dbm, i, j));
    }
  }
  return proj;
}

bool FischerProjector::contain(const vector<int>& one,
                               const std::vector<std::vector<int>>& rhs) const {
  size_t j = 0;
  size_t n = rhs[0].size();
  size_t equal_size = (pro_dim + 1) * sizeof(int);
  size_t equal_n = 2 * pro_dim;
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

bool FischerProjector::constructState(
    TMStateManager* manager, int* state,
    const std::vector<std::vector<int>>& pre_projs,
    const std::vector<std::vector<int>>& vertices,
    const std::vector<int>& links,
    const std::vector<std::pair<int, int>>& link_src_snk_map) const {
  int num = vertices.size();

  if (vertices[0][1] == 1) {
    bool b = false;
    for (int i = 0; i < num; i++) {
      if (vertices[i][2] == 1) {
        b = true;
      }
    }
    if (!b) {
      state[num] = num + 1;
    }
  } else {
    bool b = true;
    for (int i = 0; i < num; i++) {
      if (vertices[i][2] == 1) {
        b = false;
      }
    }
    if (!b) {
      return false;
    }
  }

  for (int i = 0; i < num; i++) {
    int loc = vertices[i][0];
    state[i] = loc;

    if (vertices[i][2] == 1) {
      state[num] = i + 1;
    }

    state[i + num + 3] = vertices[i][3];

    int index = num + 2 + (i + 1) * (num + 1);
    state[index] = vertices[i][4];
  }

  for (size_t i = 0; i < links.size(); i++) {
    int link_id = links[i];
    int src = link_src_snk_map[i].first;
    int snk = link_src_snk_map[i].second;

    if (link_id < 0) {
      int temp = src;
      src = snk;
      snk = temp;
      link_id *= -1;
      link_id--;
    }

    //   MatrixValue value(pre_projs[link_id][9]);  // src-snk
    int clock_start = num + 2;
    int index = clock_start + DBMManager::getIndex(src + 1, snk + 1, num + 1);
    state[index] = pre_projs[link_id][9];  //// src-snk

    index = clock_start + DBMManager::getIndex(snk + 1, src + 1, num + 1);
    state[index] = pre_projs[link_id][11];  //// snk-src
  }
  return true;
}

}  // namespace graphmodel
