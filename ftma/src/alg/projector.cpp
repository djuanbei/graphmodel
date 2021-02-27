
#include "alg/projector.h"

#include "alg/combination.h"
#include "alg/one_step.h"
#include "alg/ta_next_step.h"

namespace graphmodel {
bool Projector::include(const std::vector<std::vector<int>>& lhs,

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

  return true;
}

bool Projector::projectStableCheck(
    const std::vector<std::vector<int>>& two_dim_projs,
    const ReachableSet<TMStateManager>& reach_set,
    const SystemGenerator* generator) const {
  std::map<std::vector<int>, int> oneDimStateMap;

  std::vector<std::vector<int>> oneStataes;

  int id = 0;
  for (auto& e : two_dim_projs) {
    vector<int> src = getSrc(e);

    if (oneDimStateMap.find(src) == oneDimStateMap.end()) {
      oneDimStateMap[src] = id++;
      oneStataes.push_back(src);
    }
  }

  Graph_t<int> graph;
  std::vector<int> srcs;
  std::vector<int> snks;
  std::set<std::pair<int, int>> ant_symmetry;
  std::map<int, int> link_map;
  for (int i = 0; i < (int)two_dim_projs.size(); i++) {
    std::vector<int> e = two_dim_projs[i];
    std::vector<int> A = getSrc(e);

    int src = oneDimStateMap.at(A);

    std::vector<int> B = getSnk(e);

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

  int link_num = graph.getLink_num();

  INT_TAS_t test_sys = generator->generate(4);

  TANextStep nextS(test_sys);

  shared_ptr<typename INT_TAS_t::StateManager_t> test_manager =
      test_sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(test_manager);

  int* state = test_manager->newState();
  for (int i = 0; i < link_num; i++) {
    int src, snk;
    if (link_map[i] < 0) {
      continue;
    }

    graph.findSrcSnk(i, src, snk);

    for (int j = i + 1; j < link_num; j++) {
      if (link_map[j] < 0) {
        continue;
      }
      int src_2, snk_2;
      graph.findSrcSnk(j, src_2, snk_2);

      if (graph.directConnect(src, src_2) && graph.directConnect(src, snk_2) &&
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
            one_choose[i] = link_map.at(links[i][one_choose[i]]);
          }
          test_manager->reset(state);
          vector<vector<int>> points;
          for (size_t k = 0; k < vertices.size(); k++) {
            points.push_back(oneStataes[vertices[k]]);
          }
          if (constructState(test_manager.get(), state, two_dim_projs, points,
                             one_choose, link_src_snk_map)) {
            std::vector<OneStep> re =
                nextS.getNextStep(const_cast<int*>(state));
            data.clear();
            doOneStep(&data, test_manager.get(), state, re);
            for (size_t k = 0; k < data.size(); k++) {
              // cout << "======" << endl;

              data.getStateAt(state, k);
              // test_manager->dump(state);
              vector<int> dummy = to_vec(test_manager.get(), state);
              if (!contain(dummy, two_dim_projs)) {
                test_manager->dump(state);
                return false;
              }
            }
          }
        }
      }
    }
  }

  test_manager->destroyState(state);

  return true;
}

}  // namespace graphmodel
