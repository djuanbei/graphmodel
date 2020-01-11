#include <map>

#include "alg/combination.h"
#include "benchmark/fischer.h"
#include "benchmark/fischer_projector.h"

#include "alg/one_step.h"
#include "alg/ta_next_step.h"

namespace graphsat {

FischerProjector::FischerProjector(
    const std::shared_ptr<TMStateManager>& out_manager, const int pro_d)
    : manager(out_manager) {
  component_num = manager->getComponentNumber();
  pro_dim = pro_d;
  clock_start = manager->getClockStart();
}

void FischerProjector::operator()(const int* original_state,
                                  vector<int>& proj) const {
  for (int i = 0; i < pro_dim; i++) {
    proj.push_back(original_state[i]);
  }
  int id = manager->getValue(0, original_state, "id");
  proj.push_back( id!=0);
  if( id==1 ){
    proj.push_back(1);
  }else if( id==2){
    proj.push_back(2);
  }else{
    proj.push_back(0);
  }
  

  const int* dbm = manager->getDBM(original_state);
  for (int i = 0; i <= pro_dim; i++) {
    for (int j = 0; j <= pro_dim; j++) {
      proj.push_back(manager->getClockManager().at(dbm, i, j));
    }
  }
}

static bool directConnect(const Graph_t<int>& graph, int src, int snk) {
  return (graph.hasDirectEdge(src, snk)) || (graph.hasDirectEdge(snk, src));
}

std::vector<int> FischerProjector::to_vec(const TMStateManager* manager,
                                          const int* original_state) const {
  std::vector<int> proj;
  for (int i = 0; i < pro_dim; i++) {
    proj.push_back(original_state[i]);
  }
  int id = manager->getValue(0, original_state, "id");
  proj.push_back( id!=0);
  if( id==1 ){
    proj.push_back(1);
  }else if( id==2){
    proj.push_back(2);
  }else{
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


bool FischerProjector::projectEqualReach(
    const std::vector<std::vector<int>>& pre_projs,
    const ReachableSet<TMStateManager>& reach_set) const {
  std::map<AbsOneDimState, int> oneDimStateMap;
  std::vector<AbsOneDimState> oneStataes;
  FischerGenerator generator;

  INT_TAS_t test_sys = generator.generate(4);
  TANextStep nextS(test_sys);

  shared_ptr<typename INT_TAS_t::StateManager_t> test_manager =
      test_sys.getStateManager();
  ReachableSet<typename INT_TAS_t::StateManager_t> data(test_manager);

  int id = 0;
  for (auto& e : pre_projs) {
    AbsOneDimState state;
    state.loc = e[0];
    state.has_id = (e[3]==1);
    state.clock_lower_bound = e[5];
    state.clock_upper_bound = e[7];
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
  for (int i = 0; i < (int)pre_projs.size(); i++) {
    std::vector<int> e = pre_projs[i];
    AbsOneDimState A;
    A.loc = e[0];
    A.has_id = e[3]==1;
    A.clock_lower_bound = e[5];
    A.clock_upper_bound = e[7];
    int src = oneDimStateMap.at(A);

    AbsOneDimState B;
    B.loc = e[1];
    B.has_id = e[3]==2;
    B.clock_lower_bound = e[6];
    B.clock_upper_bound = e[10];
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
  FullChoose fc(component_num, id);

  int* state = manager->newState();
  int link_num = graph.getLink_num();

  for (int i = 0; i < link_num; i++) {
    int src, snk;

    graph.findSrcSnk(i, src, snk);

    for (int j = 0; j < link_num; j++) {
      int src_2, snk_2;
      graph.findSrcSnk(j, src_2, snk_2);
      if (directConnect(graph, src, src_2) &&
          directConnect(graph, src, snk_2) &&
          directConnect(graph, snk, src_2) &&
          directConnect(graph, snk, snk_2)) {
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
          constructState(state, pre_projs, oneStataes, vertices, one_choose,
                         link_src_snk_map, link_map);
          test_manager->dump(state);
          std::vector<OneStep> re = nextS.getNextStep(const_cast<int*>(state));
          data.clear();
          doOneStep(&data, test_manager.get(), state, re);
          for (size_t k = 0; k < data.size(); k++) {
           // cout << "======" << endl;

            data.getStateAt(state, k);
            //test_manager->dump(state);
            vector<int> dummy=to_vec( test_manager.get( ),state );
            if (!contain(dummy, pre_projs)) {
              test_manager->dump(state);
              return false;
            }
          }
        }
      }
    }
  }

  // while (fc.next()) {
  //   std::vector<int> vertices = *fc;

  //   std::sort(vertices.begin(), vertices.end());
  //   bool b = false;
  //   std::vector<std::vector<int>> links;
  //   std::vector<std::pair<int, int>> link_src_snk_map;
  //   if (check.find(vertices) == check.end()) {
  //     b = true;
  //     for (size_t j = 0; b && j < component_num; j++) {
  //       for (size_t k = j + 1; b && k < component_num; k++) {
  //         b = false;
  //         if (graph.hasDirectEdge(vertices[j], vertices[k])) {
  //           links.push_back(graph.getDirectEdges(vertices[j], vertices[k]));
  //           link_src_snk_map.push_back(make_pair(j, k));
  //           b = true;
  //         } else if (graph.hasDirectEdge(vertices[k], vertices[j])) {
  //           links.push_back(graph.getDirectEdges(vertices[k], vertices[j]));
  //           link_src_snk_map.push_back(make_pair(k, j));
  //           b = true;
  //         }
  //       }
  //     }
  //     check.insert(vertices);

  //     if (b) {
  //       std::vector<int> choose;
  //       for (auto& l : links) {
  //         choose.push_back(l.size());
  //       }
  //       FullChoose fc(choose);
  //       while (fc.next()) {
  //         vector<int> choose = *fc;
  //         for (size_t i = 0; i < choose.size(); i++) {
  //           choose[i] = links[i][choose[i]];
  //         }
  //         manager->reset(state);
  //         manager->dump(state);

  //         constructState(state, pre_projs, oneStataes, vertices, choose,
  //                        link_src_snk_map, link_map);
  //         if (!reach_set.contain(state)) {
  //           manager->dump(state);
  //           return false;
  //         }
  //       }
  //     }
  //   }
  //}
  manager->destroyState(state);

  return true;
}

void FischerProjector::constructState(
    int* state, const std::vector<std::vector<int>>& pre_projs,
    const std::vector<AbsOneDimState>& oneStataes,
    const std::vector<int>& vertices, const std::vector<int>& links,
    const std::vector<std::pair<int, int>>& link_src_snk_map,
    const std::map<int, int>& link_map) const {
  // manager->dump(state);
  int num = vertices.size();
  for (int i = 0; i < num; i++) {
    int vertex = vertices[i];
    int loc = oneStataes[vertex].loc;
    state[i] = loc;

    if (oneStataes[vertex].has_id == 1) {
      state[num] = i + 1;
    }
    // loc    id freeze 0         clock_1 ..
    // component_num+2         component_num+1
    state[i + num + 3] = oneStataes[vertex].clock_lower_bound;

    int index = num + 2 + (i + 1) * (num + 1);
    state[index] = oneStataes[vertex].clock_upper_bound;
  }
  // manager->dump(state);
  for (size_t i = 0; i < links.size(); i++) {
    int e = links[i];
    int src = link_src_snk_map[i].first;
    int snk = link_src_snk_map[i].second;

    int link_id = link_map.at(e);

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

    // manager->setClockUpperBound(src, "x", snk, "x", state, value);
    // manager->dump(state);
    // MatrixValue value1(pre_projs[link_id][11]);  // snk-src

    // manager->setClockUpperBound(snk, "x", src, "x", state, value1);
  //  manager->dump(state);
  }
}

bool FischerProjector::contain(const vector<int>& one,
                               const std::vector<std::vector<int>>& rhs) const {
  size_t j = 0;
  size_t n = rhs[0].size();
  size_t equal_size = (pro_dim + 1) * sizeof(int);
  for (; j < rhs.size(); j++) {
    size_t k = 0;
    if (0 == memcmp(&(one[0]), &(rhs[j][0]), equal_size)) {
      k = 2 * pro_dim;
    }
    if (k == (size_t)(2 * pro_dim)) {
      for (; k < n; k++) {
        if (one[k] > rhs[j][k]) {
          break;
        }
      }
    }
    if (k == n) {
      break;
    }
  }
  return j != rhs.size();
}

bool FischerProjector::include(const vector<vector<int>>& lhs,
                               const vector<vector<int>>& rhs) const {
  if (lhs.empty()) {
    return rhs.empty();
  }
  if (rhs.empty()) {
    return false;
  }
  assert(lhs[0].size() == rhs[0].size());

  //  size_t n = lhs[0].size();
  //  size_t equal_size = (pro_dim + 1) * sizeof(int);

  for (size_t i = 0; i < lhs.size(); i++) {
    if (!contain(lhs[i], rhs)) {
      return false;
    }

    // size_t j = 0;
    // for (; j < rhs.size(); j++) {
    //   size_t k = 0;
    //   if (0 == memcmp(&(lhs[i][0]), &(rhs[j][0]), equal_size)) {
    //     k = 2 * pro_dim;
    //   }

    //   if (k == (size_t)(2 * pro_dim)) {
    //     for (; k < n; k++) {
    //       if (lhs[i][k] > rhs[j][k]) {
    //         break;
    //       }
    //     }
    //   }
    //   if (k == n) {
    //     break;
    //   }
    // }
    // if (j == rhs.size()) {
    //   return false;
    // }
  }
  return true;
}

FischerProjector::BetaElement FischerProjector::beta(
    const std::vector<int>& one) const {
  BetaElement re;
  re.A_loc = one[0];
  re.B_loc = one[1];
  re.A_has_id = one[2];
  re.B_has_id = one[3];
  return re;
}

bool operator<(const FischerProjector::BetaElement& lhs,
               const FischerProjector::BetaElement& rhs) {
  if (lhs.A_loc < rhs.A_loc) {
    return true;
  }

  if (lhs.A_loc > rhs.A_loc) {
    return false;
  }

  if (lhs.B_loc < rhs.B_loc) {
    return true;
  }

  if (lhs.B_loc > rhs.B_loc) {
    return false;
  }

  if (lhs.A_has_id < rhs.A_has_id) {
    return true;
  }

  if (lhs.A_has_id > rhs.A_has_id) {
    return false;
  }
  if (lhs.B_has_id < rhs.B_has_id) {
    return true;
  }
  if (lhs.B_has_id > rhs.B_has_id) {
    return false;
  }
  for (int i = 0; i < 9; i++) {
    if (lhs.clock_dbm[i] < rhs.clock_dbm[i]) {
      return true;
    }
    if (lhs.clock_dbm[i] > rhs.clock_dbm[i]) {
      return false;
    }
  }
  return false;
}

bool operator<(const FischerProjector::AbsOneDimState& lhs,
               const FischerProjector::AbsOneDimState& rhs) {
  if (lhs.loc < rhs.loc) {
    return true;
  }
  if (lhs.loc > rhs.loc) {
    return false;
  }
  if (lhs.has_id < rhs.has_id) {
    return true;
  }
  if (lhs.has_id > rhs.has_id) {
    return false;
  }
  if (lhs.clock_lower_bound < rhs.clock_lower_bound) {
    return true;
  }
  if (lhs.clock_lower_bound > rhs.clock_lower_bound) {
    return false;
  }

  return lhs.clock_upper_bound < rhs.clock_upper_bound;
}

}  // namespace graphsat
