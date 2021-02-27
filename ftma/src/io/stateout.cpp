#include "io/stateout.h"

namespace graphmodel {

void StateOutput::generatorDot(const ReachableSet<TMStateManager>& reachSet,
                               const string& filename) {
  const TMStateManager* manager = reachSet.getManager();
  int* cache_state = manager->newState();
  int* convert_C_t = manager->newState();
  int component_num = manager->getComponentNumber();
  ofstream fout(filename);
  fout << "digraph G {" << endl;

  int clock_num = manager->getSystemClockNumber();
  for (size_t i = 0; i < reachSet.size(); i++) {
    reachSet.getStateAt(cache_state, i);

    fout << i << " [ shape=none, label=<";
    fout << "<table border=\"1\" >" << endl;
    fout << "<tr><td COLSPAN=\"" << clock_num + 1 << "\"> <b>" << i << " : "
         << manager->getLocDotLabel(cache_state) << "</b></td> </tr> " << endl;
    vector<string> couter_labels = manager->getCounterDotLabel(cache_state);
    for (auto& l : couter_labels) {
      fout << "<tr><td COLSPAN=\"" << clock_num + 1
           << "\"> <font color=\"blue\">" << l << "</font></td> </tr> " << endl;
    }
    manager->getClockManager().dumpDot(fout, manager->getDBM(cache_state));
    fout << "</table>";
    fout << ">];" << endl;
  }

  for (size_t i = 1; i < reachSet.size(); i++) {
    int parent = reachSet.getParentId(i);
    reachSet.getStateAt(cache_state, parent);
    reachSet.getStateAt(convert_C_t, i);

    fout << "\t" << parent << " -> " << i << "  [label=\"";
    int diff_num = 0;
    for (int j = 0; j < component_num; j++) {
      if (manager->getLocationID(j, cache_state) !=
          manager->getLocationID(j, convert_C_t)) {
        if (0 == diff_num) {
          fout << j;
        } else {
          fout << ", " << j;
        }
        diff_num++;
      }
    }
    fout << "\"];" << endl;
  }
  const vector<pair<int, int>>& passed_pair = reachSet.getFixPoindTail();
  for (auto& p : passed_pair) {
    reachSet.getStateAt(cache_state, p.first);
    reachSet.getStateAt(convert_C_t, p.second);

    fout << "\t" << p.first << " -> " << p.second
         << "  [style=dotted color=red label=\"";
    int diff_num = 0;
    for (int j = 0; j < component_num; j++) {
      if (manager->getLocationID(j, cache_state) !=
          manager->getLocationID(j, convert_C_t)) {
        if (0 == diff_num) {
          fout << j;
        } else {
          fout << ", " << j;
        }
        diff_num++;
      }
    }
    fout << "\"];" << endl;
  }
  fout << "}";
  fout.close();
  manager->destroyState(cache_state);
  manager->destroyState(convert_C_t);
}

bool StateOutput::generatePath(const ReachableSet<TMStateManager>& reachSet,
                               const string& filename,
                               const int* const target) {
  const TMStateManager* manager = reachSet.getManager();
  int* cache_state = manager->newState();
  int* convert_C_t = manager->newState();
  int component_num = manager->getComponentNumber();

  int clock_num = manager->getSystemClockNumber();

  vector<int> path = reachSet.findReachPath(target);
  if (path.empty()) {
    return false;
  }

  ofstream fout(filename);
  fout << "digraph G {" << endl;
  for (auto i : path) {
    reachSet.getStateAt(cache_state, i);
    fout << i << " [ shape=none, label=<";
    fout << "<table border=\"1\" >" << endl;
    fout << "<tr><td COLSPAN=\"" << clock_num + 1 << "\"> <b>" << i << " : "
         << manager->getLocDotLabel(cache_state) << "</b></td> </tr> " << endl;
    vector<string> couter_labels = manager->getCounterDotLabel(cache_state);
    for (auto& l : couter_labels) {
      fout << "<tr><td COLSPAN=\"" << clock_num + 1
           << "\"> <font color=\"blue\">" << l << "</font></td> </tr> " << endl;
    }
    manager->getClockManager().dumpDot(fout, manager->getDBM(cache_state));
    fout << "</table>";
    fout << ">];" << endl;
  }
  for (auto i : path) {
    if (i == 0) {
      continue;
    }

    int parent = reachSet.getParentId(i);
    reachSet.getStateAt(cache_state, parent);

    reachSet.getStateAt(convert_C_t, i);

    fout << "\t" << parent << " -> " << i << "  [label=\"";
    int diff_num = 0;
    for (int j = 0; j < component_num; j++) {
      if (manager->getLocationID(j, cache_state) !=
          manager->getLocationID(j, convert_C_t)) {
        if (0 == diff_num) {
          fout << j;
        } else {
          fout << ", " << j;
        }
        diff_num++;
      }
    }
    fout << "\"];" << endl;
  }

  fout << "}" << endl;
  fout.close();
  manager->destroyState(cache_state);
  manager->destroyState(convert_C_t);

  return true;
}
}  // namespace graphmodel
