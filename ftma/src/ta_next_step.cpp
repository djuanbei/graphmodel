#include "alg/ta_next_step.h"

namespace graphsat {

std::vector<OneStep> TANextStep::getNextStep(void *s) const {
  int *state = (int *)s;
  std::vector<OneStep> re;

  if (manager->isFreeze(state)) {
    doFreeze(state, re);
  }
  if (!re.empty()) {
    if (manager->hasMatchOutUrgentChan(state)) {
      doUrgant(state, re);
    } else if (manager->hasOutBreakcastChan(state)) {
      doBreakcast(state, re);
    } else {

      for (int i = 0; i < component_num; i++) {
        const int source_a = manager->getLocationID(i, state);
        for (int j = i + 1; i < component_num; i++) {
          const int source_b = manager->getLocationID(j, state);
        }
      }
      for (int i = 0; i < component_num; i++) {
        const int source = manager->getLocationID(i, state);
        const vector<int> out_ts = sys.getOutTransition(i, source);
        for (auto link : out_ts) {

          if (!manager->transitionReady(i, link, state)) {
            continue;
          }
          OneStep dummy;
          OneStep::Action action(i, source, link);
          dummy.addAction(action);
          re.push_back(dummy);
        }
      }
    }
  }

  return re;
}

void TANextStep::doFreeze(int *state, std::vector<OneStep> &re) const {

  for (int i = 0; i < component_num; i++) {
    if (manager->isCommitComp(i, state)) {
      const int source = manager->getLocationID(i, state);
      const vector<int> out_ts = sys.getOutTransition(i, source);
      for (auto link : out_ts) {
        if (!manager->transitionReady(i, link, state)) {
          continue;
        }
        OneStep dummy;
        OneStep::Action action(i, source, link);
        dummy.addAction(action);
        re.push_back(dummy);
      }
    }
  }
}

void TANextStep::doUrgant(int *state, std::vector<OneStep> &re) const {

  int *counter_value = manager->getCounterValue(state);
  for (int i = 0; i < component_num; i++) {
    const int loc_a = manager->getLocationID(i, state);
    vector<int> dummy_a = manager->getOutUrgent(i, loc_a, counter_value);
    if (!dummy_a.empty()) {
      const set<int> temp(dummy_a.begin(), dummy_a.end());
      for (int j = i + 1; j < component_num; j++) {
        const int loc_b = manager->getLocationID(j, state);
        const vector<int> dummy2 =
            manager->getOutUrgent(j, loc_b, counter_value);
        for (auto e : dummy2) {
          if (temp.find(-e) != temp.end()) {
            vector<int> links_a = sys.getChanLinks(i, loc_a, -e, counter_value);
            vector<int> links_b = sys.getChanLinks(j, loc_b, e, counter_value);
            // b is send part
            if (e > 0) {
              for (auto l_b : links_b) {
                for (auto l_a : links_a) {
                  OneStep dummy;
                  OneStep::Action send_action(j, loc_b, l_b);
                  dummy.addAction(send_action);
                  OneStep::Action recive_action(i, loc_a, l_a);
                  dummy.addAction(recive_action);
                  OneStep::Action delay_action(i, loc_a, l_a);
                  delay_action.action = OneStep::CONTINUED_EVOLUTION;
                  dummy.addAction(delay_action);
                  re.push_back(dummy);
                }
              }

            } else { // a is send part
              for (auto l_b : links_b) {
                for (auto l_a : links_a) {
                  OneStep dummy;
                  OneStep::Action send_action(i, loc_a, l_a);
                  dummy.addAction(send_action);

                  OneStep::Action recive_action(j, loc_b, l_b);
                  dummy.addAction(recive_action);

                  OneStep::Action delay_action(i, loc_a, l_a);
                  delay_action.action = OneStep::CONTINUED_EVOLUTION;
                  dummy.addAction(delay_action);
                  re.push_back(dummy);
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

} // namespace graphsat
