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

  // int wait_cus_num=manager->getValue( 0, original_state,"wait_cus_len" );
  // int stop_lift_num=manager->getValue(0, original_state, "stop_lift_len" );
  // proj.push_back(wait_cus_num );
  // proj.push_back(stop_lift_num );
  return proj;
}

bool LiftCustomerProjector::contain(
    const vector<int>& one, const std::vector<std::vector<int>>& rhs) const {
  return find(rhs.begin(), rhs.end(), one) != rhs.end();
}

bool LiftCustomerProjector::include(
    const std::vector<std::vector<int>>& lhs,
    const std::vector<std::vector<int>>& rhs) const {
  if (lhs.empty()) {
    return rhs.empty();
  }
  if (rhs.empty()) {
    return false;
  }
  assert(lhs[0].size() == rhs[0].size());

  for (size_t i = 0; i < lhs.size(); i++) {
    if (!contain(lhs[i], rhs)) {
      return false;
    }
  }
  assert(lhs.size() == rhs.size());
  return true;
}

bool LiftCustomerProjector::projectEqualReach(
    const std::vector<std::vector<int>>& projs,
    const ReachableSet<TMStateManager>& reach_set) const {
  std::map<std::vector<int>, int> oneDimStateMap;
  std::vector<std::vector<int>> oneStataes;
  LiftCustomer generator;

  INT_TAS_t test_sys = generator.generate(4);
  TANextStep nextS(test_sys);

  shared_ptr<typename INT_TAS_t::StateManager_t> test_manager =
      test_sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(test_manager);

  int id = 0;
  for (auto& e : projs) {
    std::vector<int> state;
    state.push_back(e[0]);
    state.push_back(e[1]);
    if (oneDimStateMap.find(state) == oneDimStateMap.end()) {
      oneDimStateMap[state] = id++;
      oneStataes.push_back(state);
    }
  }

  Graph_t<int> graph;
  std::vector<int> srcs;
  std::vector<int> snks;
  std::set<std::pair<int, int>> ant_symmetry;
  std::map<int, int> link_map;
  for (int i = 0; i < (int)projs.size(); i++) {
    std::vector<int> e = projs[i];
    std::vector<int> A, B;
    A.push_back(e[0]);
    A.push_back(e[1]);

    B.push_back(e[0]);
    B.push_back(e[2]);

    int src = oneDimStateMap.at(A);
    int snk = oneDimStateMap.at(B);
    std::pair<int, int> dummy1(src, snk);
    std::pair<int, int> dummy2(snk, src);

    if (ant_symmetry.find(dummy2) == ant_symmetry.end()) {
      link_map[srcs.size()] = i;
      ant_symmetry.insert(dummy1);

      srcs.push_back(src);
      snks.push_back(snk);
      link_map[srcs.size()] = -i - 1;
      srcs.push_back(snk);
      snks.push_back(src);
    }
  }

  graph.initial(srcs, snks);
  std::set<std::vector<int>> check;
  // FullChoose fc(component_num, id);
  int* state = manager->newState();
  int link_num = graph.getLink_num();

  for (int i = 0; i < link_num; i++) {
    int src, snk;
    int ll = link_map[i];
    if (ll < 0) {
      continue;
    }

    graph.findSrcSnk(i, src, snk);

    for (int j = 0; j < link_num; j++) {
      int jj = link_map[j];
      if (jj < 0) {
        continue;
      }

      int src_2, snk_2;
      graph.findSrcSnk(j, src_2, snk_2);

      if (graph.directConnect(src, snk) && graph.directConnect(src, snk_2) &&
          graph.directConnect(snk, src_2) && graph.directConnect(snk, snk_2)) {
        vector<int> vertices;
        vertices.push_back(src);
        vertices.push_back(snk);
        vertices.push_back(src_2);
        vertices.push_back(snk_2);
        std::vector<std::vector<int>> links;
        std::vector<std::pair<int, int>> link_src_snk_map;
        links.push_back(graph.getDirectEdges(src, snk));
        link_src_snk_map.push_back(make_pair(0, 1));
        links.push_back(graph.getDirectEdges(src, src_2));
        link_src_snk_map.push_back(make_pair(0, 2));
        links.push_back(graph.getDirectEdges(src, snk_2));
        link_src_snk_map.push_back(make_pair(0, 3));
        links.push_back(graph.getDirectEdges(snk, src_2));
        link_src_snk_map.push_back(make_pair(1, 2));
        links.push_back(graph.getDirectEdges(snk, snk_2));
        link_src_snk_map.push_back(make_pair(1, 3));
        links.push_back(graph.getDirectEdges(src_2, snk_2));
        link_src_snk_map.push_back(make_pair(2, 3));

        std::vector<int> choose;
        for (auto& l : links) {
          choose.push_back(l.size());
        }
        FullChoose fc(choose);

        while (fc.next()) {
          vector<int> one_choose = *fc;
          for (size_t i = 0; i < choose.size(); i++) {
            one_choose[i] = links[i][one_choose[i]];
          }
          test_manager->reset(state);
          constructState(state, projs, oneStataes, vertices, one_choose,
                         link_src_snk_map, link_map);

          std::vector<OneStep> re = nextS.getNextStep(const_cast<int*>(state));
          data.clear();
          doOneStep(&data, test_manager.get(), state, re);
          for (size_t k = 0; k < data.size(); k++) {
            // cout << "======" << endl;

            data.getStateAt(state, k);
            // test_manager->dump(state);
            vector<int> dummy = to_vec(test_manager.get(), state);
            if (!contain(dummy, projs)) {
              test_manager->dump(state);
              return false;
            }
          }
        }
      }
    }
  }

  return true;
}

void LiftCustomerProjector::constructState(
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
}

}  // namespace graphsat
