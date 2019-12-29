#include "alg/ta_next_step.h"

namespace graphsat {

std::vector<OneStep> TANextStep::getNextStep(void *s) const {
  int *state = (int *)s;
  std::vector<OneStep> re;

  if (manager->isFreeze(state)) {
    doCommit(state, re);
  }
  if (re.empty()) { // there is no enable  commit locations
    if (manager->hasMatchOutUrgentChan(state)) {
      doUrgant(state, re);
    }
  }

  if (re.empty()) { // there is no force  locations or transitions
    if (manager->hasOutBreakcastChan(state)) {
      doBreakcast(state, re);
    } else {
      doNormal(state, re);
    }
  }

  return re;
}

void TANextStep::doNormal(int *state, std::vector<OneStep> &re) const {
  // int *counter_value = manager->getCounterValue(state);

  for (int i = 0; i < component_num; i++) {
    const int loc_a = manager->getLocationID(i, state);
    if (sys.hasNormalCh(i, loc_a)) {
      vector<int> dummy_a = manager->getEnableOutNormalChan(i, loc_a, state);
      if (!dummy_a.empty()) {
        const set<int> temp(dummy_a.begin(), dummy_a.end());
        for (int j = i + 1; j < component_num; j++) {
          const int loc_b = manager->getLocationID(j, state);
          if (sys.hasNormalCh(j, loc_b)) {
            const vector<int> dummy2 =
                manager->getEnableOutNormalChan(j, loc_b, state);
            for (auto e : dummy2) {
              if (temp.find(-e) != temp.end()) {
                vector<int> links_a =
                    manager->getChanLinks(i, loc_a, -e, state);
                vector<int> links_b = manager->getChanLinks(j, loc_b, e, state);
                // b is send part
                if (e > 0) {
                  for (auto link_b : links_b) {
                    for (auto link_a : links_a) {
                      vector<pair<int, int>> path;
                      path.push_back(make_pair(j, link_b));
                      path.push_back(make_pair(i, link_a));
                      discret(state, path, re);
                    }
                  }

                } else { // a is send part
                  for (auto link_b : links_b) {
                    for (auto link_a : links_a) {
                      vector<pair<int, int>> path;
                      path.push_back(make_pair(i, link_a));
                      path.push_back(make_pair(j, link_b));
                      discret(state, path, re);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  for (int i = 0; i < component_num; i++) {
    const int source = manager->getLocationID(i, state);
    const vector<int> out_ts = sys.getOutTransition(i, source);

    for (auto link : out_ts) {
      if (!sys.hasChannel(i, link)) {
        if (!manager->transitionReady(i, link, state)) {
          continue;
        }
        vector<pair<int, int>> path;
        path.push_back(make_pair(i, link));
        discret(state, path, re);
      }
    }
  }
}

void TANextStep::doCommit(int *state, std::vector<OneStep> &re) const {
  int *counter_value = manager->getCounterValue(state);
  for (int component = 0; component < component_num; component++) {
    const int source = manager->getLocationID(component, state);
    if (sys.isCommit(component, source)) {
      const vector<int> out_ts = sys.getOutTransition(component, source);
      for (auto link : out_ts) {
        if (!manager->transitionReady(component, link, state)) {
          continue;
        }
        if (sys.hasChannel(component, link)) {
          const Channel &ch = sys.getChannel(component, link);
          int chid = ch.getGlobalId(counter_value);

          if (ch.getType() == BROADCAST_CH) {

          } else {
            for (int i = 0; i < component_num; i++) {
              int loc = state[i];
              if (i == component) {
                continue;
              }

              if (ch.isSend()) {
                vector<int> links = manager->getChanLinks(i, loc, -chid, state);
                for (int link_b : links) {
                  if (!manager->transitionReady(i, link_b, state)) {
                    continue;
                  }
                  vector<pair<int, int>> path;
                  path.push_back(make_pair(component, link));
                  path.push_back(make_pair(i, link_b));
                  discret(state, path, re);
                }
              } else {
                vector<int> links = manager->getChanLinks(i, loc, chid, state);

                for (int link_b : links) {
                  if (!manager->transitionReady(i, link_b, state)) {
                    continue;
                  }
                  vector<pair<int, int>> path;
                  path.push_back(make_pair(i, link_b));
                  path.push_back(make_pair(component, link));
                  discret(state, path, re);
                }
              }
            }
          }

        } else {
          vector<pair<int, int>> path;
          path.push_back(make_pair(component, link));
          discret(state, path, re);
        }
      }
    }
  }
}

void TANextStep::doUrgant(int *state, std::vector<OneStep> &re) const {

  //  int *counter_value = manager->getCounterValue(state);
  for (int i = 0; i < component_num; i++) {
    const int loc_a = manager->getLocationID(i, state);
    vector<int> dummy_a = manager->getEnableOutUrgent(i, loc_a, state);
    if (!dummy_a.empty()) {
      const set<int> temp(dummy_a.begin(), dummy_a.end());
      for (int j = i + 1; j < component_num; j++) {
        const int loc_b = manager->getLocationID(j, state);
        const vector<int> dummy2 = manager->getEnableOutUrgent(j, loc_b, state);
        for (auto e : dummy2) {
          if (temp.find(-e) != temp.end()) {
            vector<int> links_a = manager->getChanLinks(i, loc_a, -e, state);
            vector<int> links_b = manager->getChanLinks(j, loc_b, e, state);
            // b is send part
            if (e > 0) {
              for (auto link_b : links_b) {
                for (auto link_a : links_a) {
                  vector<pair<int, int>> path;
                  path.push_back(make_pair(j, link_b));
                  path.push_back(make_pair(i, link_a));
                  discret(state, path, re);
                }
              }

            } else { // a is send part
              for (auto link_b : links_b) {
                for (auto link_a : links_a) {
                  vector<pair<int, int>> path;
                  path.push_back(make_pair(i, link_a));
                  path.push_back(make_pair(j, link_b));
                  discret(state, path, re);
                }
              }
            }
          }
        }
      }
    }
  }
}

void TANextStep::doBreakcast(int *state, std::vector<OneStep> &re) const {}

void TANextStep::discret(int *state, std::vector<pair<int, int>> &path,
                         std::vector<OneStep> &re) const {
  assert(!path.empty());

  OneStep dummy;
  int commit_num = manager->getFreezeComponentNumber(
      state); //  state[manager->getFreezeLocation()];
  for (auto &p : path) {
    OneStep::Action action(p.first, -1, p.second);
    dummy.addAction(action);
    int source = sys.getSrc(p.first, p.second);
    int target = sys.getSnk(p.first, p.second);
    if (sys.isCommit(p.first, source)) {
      commit_num--;
    }
    if (sys.isCommit(p.first, target)) {
      commit_num++;
    }
  }

  if (0 == commit_num) {
    pair<int, int> last = path.back();
    int target = sys.getSnk(last.first, last.second);
    OneStep::Action delay_action(last.first, target, -1);
    delay_action.action = OneStep::CONTINUED_EVOLUTION;
    dummy.addAction(delay_action);
  }

  re.push_back(dummy);
}
int TANextStep::getCommitCount(const int component, const int link,
                               int count) const {

  int source = sys.getSrc(component, link);
  int target = sys.getSnk(component, link);

  if (sys.isFreezeLocation(component, source)) {
    count--;
    assert(count >= 0 && "This value at least 0.");
  }
  if (sys.isFreezeLocation(component, target)) {
    count++;

    assert(count <= component_num && "This value at most component_num.");
  }
  return count;
}

void TANextStep::doCommitComponent(int *state, int component,
                                   std::vector<OneStep> &re) const {}

} // namespace graphsat
