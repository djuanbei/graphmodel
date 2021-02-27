#include "model/transition.h"

#include <cassert>

namespace graphmodel {

bool Transition::ready(const TMStateManager* manager,
                       const int* const state) const {
  if (!guards.empty()) {
    const DBMManager& dbm_manager = manager->getClockManager();
    const int* source_DBM = manager->getDBM(state);
    assert(dbm_manager.isConsistent(source_DBM));
    int* copy_DBM = dbm_manager.createDBM(source_DBM);

    for (auto& cs : guards) {
      dbm_manager.andImpl(copy_DBM, cs);
    }

    if (!dbm_manager.isConsistent(copy_DBM)) {
      dbm_manager.destroyDBM(copy_DBM);
      return false;
    }
    dbm_manager.destroyDBM(copy_DBM);
  }

  if (!counter_cons.empty()) {
    const int* counter_value = manager->getCounterValue(state);

    for (auto& cs : counter_cons) {
      if (!cs(const_cast<int*>(counter_value))) {
        return false;
      }
    }
  }
  return true;
}

void Transition::operator()(const TMStateManager* const manager,
                            int* re_state) const {
  assert(ready(manager, re_state));

  const DBMManager& dbm_manager = manager->getClockManager();

  int* source_DBM = manager->getDBM(re_state);

  /**
   * the state which statisfied the guards can jump this transition
   *
   */
  for (auto& cs : guards) {
    dbm_manager.andImpl(source_DBM, cs);
  }

  for (auto& reset : resets) {
    assert(reset.first > 0);    // clock id start from 1
    assert(reset.second >= 0);  // clock value must positive
    dbm_manager.resetImpl(source_DBM, reset.first, reset.second);
  }

  if (!actions.empty()) {
    int* counterValue = manager->getCounterValue(re_state);

    for (auto& action : actions) {
      action(counterValue);
    }
  }
}

void Transition::to_real(const TOReal* convertor) {
  if (has_channel) {
    channel.to_real(convertor);
  }
  for (auto& e : guards) {
    e.to_real(convertor);
  }
  for (auto& e : counter_cons) {
    e.to_real(convertor);
  }
  for (auto& e : actions) {
    e.to_real(convertor);
  }
  resets.clear();
  for (auto& e : reset_arg) {
    e.to_real(convertor);
    resets.push_back(e.getValue());
  }
}
std::ostream& Transition::dump2Dot(std::ostream& out) const {
  out << "\t" << source << " -> " << target << " [label=<";
  out << "<table border=\"0\" >" << endl;
  out << "<tr><td>" << source << "-" << target << "</td></tr>" << endl;
  for (auto& e : guards) {
    e.dump2Dot(out);
  }

  out << "</table>";
  out << ">]" << endl;

  return out;
}

}  // namespace graphmodel
