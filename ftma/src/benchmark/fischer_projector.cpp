#include <map>

#include "alg/combination.h"
#include "benchmark/fischer_projector.h"

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
  for (int i = 0; i < pro_dim; i++) {
    proj.push_back(id == (i + 1));
  }
  const int* dbm = manager->getDBM(original_state);
  for (int i = 0; i <= pro_dim; i++) {
    for (int j = 0; j <= pro_dim; j++) {
      proj.push_back(manager->getClockManager().at(dbm, i, j));
    }
  }
}

bool FischerProjector::projectEqualReach(
    const std::vector<std::vector<int>>& projs,
    const ReachableSet<TMStateManager>& next_reach_set) const {
  std::map<AbsOneDimState, int> oneDimStateMap;
  std::vector<AbsOneDimState> oneStataes;

  int id = 0;
  for (auto& e : projs) {
    AbsOneDimState state;
    state.loc = e[0];
    state.has_id = e[2];
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
  for (size_t i = 0; i < projs.size(); i++) {
    std::vector<int> e = projs[i];
    AbsOneDimState A;
    A.loc = e[0];
    A.has_id = e[2];
    A.clock_lower_bound = e[5];
    A.clock_upper_bound = e[7];
    int src = oneDimStateMap.at(A);

    AbsOneDimState B;
    B.loc = e[1];
    B.has_id = e[3];
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
    }
  }
  graph.initial(srcs, snks);
  std::set<std::vector<int>> check;
  FullChoose fc(manager->getComponentNumber(), id);

  int* state = manager->newState();
  while (fc.next()) {
    std::vector<int> vertices = *fc;

    std::sort(vertices.begin(), vertices.end());
    bool b = false;
    vector<vector<int>> links;
    if (check.find(vertices) == check.end()) {
      b = true;
      for (size_t j = 0; b && j < manager->getComponentNumber(); j++) {
        for (size_t k = j + 1; b && k < manager->getComponentNumber(); k++) {
          if (!graph.hasUndirectEdge(vertices[j], vertices[k])) {
            b = false;
          } else {
            links.push_back(graph.getConnectEdges(vertices[j], vertices[k]));
          }
        }
      }
      check.insert(vertices);

      if (b) {
        std::vector<int> choose;
        for (auto& l : links) {
          choose.push_back(l.size());
        }
        FullChoose fc(choose);
        while (fc.next()) {
          vector<int> choose = *fc;
          for (size_t k = 0; k < choose.size(); k++) {
            choose[k] = link_map[choose[k]];
          }
          constructState(state, projs, oneStataes, vertices, choose, graph);
          if (!next_reach_set.contain(state)) {
            return false;
          }
        }
      }
    }
  }
  manager->destroyState(state);

  return true;
}

static int findIndex(const std::vector<int> & values, int v ){
  std::vector<int>::const_iterator it=find( values.begin( ), values.end( ), v);
  assert( it!= values.end( ));
  return it-values.begin( );
}


void FischerProjector::constructState(
    int* state, const std::vector<std::vector<int>>& projs,
    const std::vector<AbsOneDimState>& oneStataes,
    const std::vector<int>& vertices, const std::vector<int>& links,
    const Graph_t<int>& graph) const {
  int component_num = manager->getComponentNumber();
  for (int i = 0; i < component_num; i++) {
    state[i] = oneStataes[vertices[i]].loc;
    if (oneStataes[vertices[i]].has_id == 1) {
      manager->setValue(0, state, "id", i + 1);
    }
    // loc  freeze  id  0         clock_1 ..
    // component_num+2         component_num+1
    state[i + component_num + 3] = oneStataes[vertices[i]].clock_lower_bound;

    int index = component_num + 2 + (i + 1) * (component_num + 1);
    state[index] = oneStataes[vertices[i]].clock_upper_bound;
  }
  for (auto e : links) {
    int src, snk;
    graph.findSrcSnk( e, src, snk);
    src=findIndex( vertices, src );
    snk=findIndex( vertices, snk);
    
    MatrixValue value(projs[ e][ 9] ); //src-snk
    manager->setClockUpperBound(src+1,"x", snk+1, "x", state, value  );
    
    MatrixValue value1(projs[ e][ 11] ); //snk-src
    
    manager->setClockUpperBound(snk+1,"x", src+1, "x", state, value1  );
        
  }
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

  size_t n = lhs[0].size();
  size_t equal_size = (pro_dim + 1) * sizeof(int);

  for (size_t i = 0; i < lhs.size(); i++) {
    size_t j = 0;
    for (; j < rhs.size(); j++) {
      size_t k = 0;
      if (0 == memcmp(&(lhs[i][0]), &(rhs[j][0]), equal_size)) {
        k = 2 * pro_dim;
      }

      if (k == (size_t)(2 * pro_dim)) {
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
