#include "alg/ta_next_step.h"
#include <unordered_map>

namespace graphsat {

std::vector<OneStep> TANextStep::getNextStep(void* s) const {
  int* state = (int*)s;
  std::vector<OneStep> re;

  if (manager->isFreeze(state)) {
    doCommit(state, re);  // commit first
  }
  if (re.empty()) {  // there is no enable  commit locations
    if (manager->hasMatchOutUrgentChan(state)) {
      doUrgant(state, re);
    }
  }

  if (re.empty()) {  // there is no force  locations or transitions
    if (manager->hasOutSendBroadcastChan(state)) {
      doBroadcast(state, re);
    }
    doNormal(state, re);
  }

  return re;
}

void TANextStep::doNormal(int* state, std::vector<OneStep>& re) const {
  vector<set<int>> enableoutNorChan;
  vector<int> has_enable_chan_components;
  for (int i = 0; i < component_num; i++) {
    const int loc_a = manager->getLocationID(i, state);
    if (sys.hasNormalCh(i, loc_a)) {
      set<int> dummy_a = manager->getEnableOutNormalChan(i, loc_a, state);
      if (!dummy_a.empty()) {
        enableoutNorChan.push_back(std::move(dummy_a));
        has_enable_chan_components.push_back(i);
      }
    }
  }

  // std::unordered_map<std::pair<int, int> , vector<int> > chanLinks;

  for (int ii = 0; ii + 1 < (int)has_enable_chan_components.size(); ii++) {
    set<int>& dummy_a = enableoutNorChan[ii];
    int i = has_enable_chan_components[ii];
    const int loc_a = manager->getLocationID(i, state);

    for (int jj = ii + 1; jj < (int)has_enable_chan_components.size(); jj++) {
      const set<int>& dummy_b = enableoutNorChan[jj];
      int j = has_enable_chan_components[jj];

      const int loc_b = manager->getLocationID(j, state);

      for (auto e : dummy_b) {
        if (dummy_a.find(-e) != dummy_a.end()) {
          vector<int> links_a = manager->getChanLinks(i, loc_a, -e, state);
          vector<int> links_b = manager->getChanLinks(j, loc_b, e, state);
          // b is send part
          if (e > 0) {
            for (auto link_b : links_b) {
              for (auto link_a : links_a) {
                vector<pair<int, int>> path;
                path.push_back(make_pair(j, link_b));
                path.push_back(make_pair(i, link_a));
                discrete(state, path, re);
              }
            }

          } else {  // a is send part
            for (auto link_b : links_b) {
              for (auto link_a : links_a) {
                vector<pair<int, int>> path;
                path.push_back(make_pair(i, link_a));
                path.push_back(make_pair(j, link_b));
                discrete(state, path, re);
              }
            }
          }
        }
      }
    }
  }
  for (int i = 0; i < component_num; i++) {
    const int source = manager->getLocationID(i, state);
    vector<int> out_ts = manager->getEnableOutLinks(i, source, state);
    for (auto link : out_ts) {
      if (!sys.hasChannel(i, link)) {
        vector<pair<int, int>> path;
        path.push_back(make_pair(i, link));
        discrete(state, path, re);
      }
    }
  }
}

void TANextStep::doCommit(int* state, std::vector<OneStep>& re) const {
  int* counter_value = manager->getCounterValue(state);
  vector<int> commit_components = manager->getCommitComponents(state);
  for (vector<int>::iterator it = commit_components.begin();
       it != commit_components.end(); it++) {
    int component = *it;

    int source = manager->getLocationID(component, state);

    std::vector<int> out_ts =
        manager->getEnableOutLinks(component, source, state);
    for (auto link : out_ts) {
      if (sys.hasChannel(component, link)) {
        const Channel& ch = sys.getChannel(component, link);
        int signed_chid = ch.getSiginGlobalId(counter_value);

        if (ch.getType() == BROADCAST_CH) {
          assert(false);
        } else {
          for (int i = 0; i < component_num; i++) {
            if ((i == component) || (find(commit_components.begin(),
                                          commit_components.end(), i) < it)) {
              continue;
            }
            int loc = state[i];

            vector<int> links =
                manager->getChanLinks(i, loc, -signed_chid, state);
            for (int link_b : links) {
              if (!manager->transitionReady(i, link_b, state)) {
                continue;
              }
              vector<pair<int, int>> path;
              if (ch.isSend()) {
                path.push_back(make_pair(component, link));
                path.push_back(make_pair(i, link_b));
              } else {
                path.push_back(make_pair(i, link_b));
                path.push_back(make_pair(component, link));
              }
              discrete(state, path, re);
            }
          }
        }

      } else {
        vector<pair<int, int>> path;
        path.push_back(make_pair(component, link));
        discrete(state, path, re);
      }
    }
  }
}

void TANextStep::doUrgant(int* state, std::vector<OneStep>& re) const {
  vector<int> hasUrgentCh_part;
  vector<set<int>> enableoutUrgantChan;
  for (int i = 0; i < component_num; i++) {
    const int loc_a = manager->getLocationID(i, state);
    std::set<int> dummy_a = manager->getEnableOutUrgent(i, loc_a, state);
    if (!dummy_a.empty()) {
      hasUrgentCh_part.push_back(i);
      enableoutUrgantChan.push_back(std::move(dummy_a));
    }
  }

  for (int ii = 0; ii < (int)hasUrgentCh_part.size(); ii++) {
    int i = hasUrgentCh_part[ii];
    std::set<int>& dummy_a = enableoutUrgantChan[ii];

    const int loc_a = manager->getLocationID(i, state);

    for (int jj = ii + 1; jj < (int)hasUrgentCh_part.size(); jj++) {
      int j = hasUrgentCh_part[jj];
      const std::set<int>& dummy_b = enableoutUrgantChan[jj];

      const int loc_b = manager->getLocationID(j, state);

      for (auto e : dummy_b) {
        if (dummy_a.find(-e) != dummy_a.end()) {
          vector<int> links_a = manager->getChanLinks(i, loc_a, -e, state);
          vector<int> links_b = manager->getChanLinks(j, loc_b, e, state);
          // b is send part
          if (e > 0) {
            for (auto link_b : links_b) {
              for (auto link_a : links_a) {
                vector<pair<int, int>> path;
                path.push_back(make_pair(j, link_b));
                path.push_back(make_pair(i, link_a));
                discrete(state, path, re);
              }
            }

          } else {  // a is send part
            for (auto link_b : links_b) {
              for (auto link_a : links_a) {
                vector<pair<int, int>> path;
                path.push_back(make_pair(i, link_a));
                path.push_back(make_pair(j, link_b));
                discrete(state, path, re);
              }
            }
          }
        }
        //}
      }
    }
    //    }
  }
}

void TANextStep::doBroadcast(int* state, std::vector<OneStep>& re) const {
  for (int i = 0; i < component_num; i++) {
    const int loc_a = manager->getLocationID(i, state);
    std::set<int> dummy_a = manager->getEnableOutBroadcast(i, loc_a, state);
    if (!dummy_a.empty()) {
      std::set<int> temp(dummy_a.begin(), dummy_a.end());
      for (auto chid : temp) {
        if (chid > 0) {  // send action
          std::vector<int> links_a =
              manager->getChanLinks(i, loc_a, chid, state);
          for (auto link_a : links_a) {
            vector<pair<int, int>> path;
            path.push_back(make_pair(i, link_a));  // send part first
            vector<vector<pair<int, int>>> paths;
            paths.push_back(path);

            for (int j = 0; j < component_num; j++) {
              if (i == j) {  // The send and receive part can not in the same
                             // component
                continue;
              }
              const int loc_b = manager->getLocationID(j, state);
              const std::set<int> dummy_b =
                  manager->getEnableOutBroadcast(j, loc_b, state);
              for (auto e : dummy_b) {
                if (e == -chid) {  // match recieve  action

                  std::vector<int> links_b =
                      manager->getChanLinks(j, loc_b, e, state);
                  if (links_b.size() == 1) {
                    for (auto& p : paths) {
                      p.push_back(make_pair(j, *links_b.begin()));
                    }

                  } else {
                    vector<vector<pair<int, int>>> copy_paths;
                    copy_paths.swap(paths);
                    for (auto& p : copy_paths) {
                      vector<pair<int, int>> tt(p);
                      for (auto link_b : links_b) {
                        tt.push_back(make_pair(j, link_b));
                        paths.push_back(tt);
                      }
                    }
                  }
                }
              }
            }
            for (auto& path : paths) {
              discrete(state, path, re);
            }
          }
        }
      }
    }
  }
}

void TANextStep::discrete(const int* const state,
                          std::vector<pair<int, int>>& path,
                          std::vector<OneStep>& re) const {
  assert(!path.empty());

  OneStep dummy;
  int commit_num = manager->getFreezeComponentNumber(state);
  for (auto& p : path) {
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
    dummy.addAction(std::move(delay_action));
  }

  re.push_back(std::move(dummy));
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

void TANextStep::doCommitComponent(int* state, int component,
                                   std::vector<OneStep>& re) const {}

}  // namespace graphsat
