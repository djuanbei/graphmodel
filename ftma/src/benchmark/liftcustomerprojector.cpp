#include "benchmark/liftcustomer.h"
#include "benchmark/liftcustomer_projector.h"

#include "alg/combination.h"

namespace graphsat {
LiftCustomerProjector::LiftCustomerProjector(
    const std::shared_ptr<TMStateManager>& out_manager, const int pro_d)
    : manager(out_manager) {
  component_num = manager->getComponentNumber();
}
void LiftCustomerProjector::operator()(const int* original_state,
                                       vector<int>& proj) const {
  proj = to_vec(manager.get(), original_state);
}
std::vector<int> LiftCustomerProjector::to_vec(
    const TMStateManager* manager, const int* original_state) const {
  std::vector<int> proj;
  proj.push_back(original_state[0]);
  proj.push_back(original_state[1]);
  proj.push_back(original_state[2]);
  int wait_cus_num = manager->getValue(0, original_state, "wait_cus_len");
  int stop_lift_num = manager->getValue(0, original_state, "stop_lift_len");
  proj.push_back(wait_cus_num);
  proj.push_back(stop_lift_num);

  return proj;
}

std::vector<int> LiftCustomerProjector::getSrc(
    const std::vector<int>& proj) const {
  std::vector<int> re;
  re.push_back(proj[0]);
  re.push_back(proj[1]);
  re.push_back(proj[3]);
  re.push_back(proj[4]);
  return re;
}

std::vector<int> LiftCustomerProjector::getSnk(
    const std::vector<int>& proj) const {
  std::vector<int> re;
  re.push_back(proj[0]);
  re.push_back(proj[2]);
  re.push_back(proj[3]);
  re.push_back(proj[4]);
  return re;
}

bool LiftCustomerProjector::contain(
    const vector<int>& one, const std::vector<std::vector<int>>& rhs) const {
  return find(rhs.begin(), rhs.end(), one) != rhs.end();
}

bool LiftCustomerProjector::constructState(
    int* state, const std::vector<std::vector<int>>& pre_projs,
    const std::vector<std::vector<int>>& oneStataes,
    const std::vector<int>& vertices, const std::vector<int>& links,
    const std::vector<std::pair<int, int>>& link_src_snk_map,
    const std::map<int, int>& link_map) const {
  int num = vertices.size();
  state[0] = oneStataes[vertices[0]][0];
  for (int i = 0; i < num; i++) {
    int vertex = vertices[i];
    int loc = oneStataes[vertex][1];
    state[i + 1] = loc;
  }
  state[num + 1] = oneStataes[vertices[0]][2];
  state[num + 2] = oneStataes[vertices[0]][3];
  return true;
}

}  // namespace graphsat