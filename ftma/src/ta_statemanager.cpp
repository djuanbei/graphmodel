
#include <cassert>

#include "state/ta_statemanager.h"

#include "model/graphmodel.hpp"

namespace graphsat {
TMStateManager::TMStateManager(const INT_TAS_t &s,
                               const vector<Counter> &ecounters,
                               int out_clock_num,
                               const vector<int> &oclock_upper_bounds,
                               const vector<int> &nodes,
                               const vector<int> &links)
    : sys(s) {
  clock_num = out_clock_num;
  assert((int)oclock_upper_bounds.size() == 2 * clock_num + 2);

  state_length = 0;
  component_num = s.getComponentNumber();

  clock_upper_bounds = oclock_upper_bounds;
  node_nums = nodes;
  link_nums = links;
  chan_num = sys.getChanNum();

  if (chan_num > 0) {
    counter_start_loc = 2 * component_num;
    state_length = 2 * component_num + (int)ecounters.size();
  } else {
    counter_start_loc = component_num;
    state_length = component_num + (int)ecounters.size();
  }
  freeze_location_index = state_length;

  state_length++;
  clock_start_loc = state_length;

  state_length += (clock_num + 1) * (clock_num + 1);
  hasDiff = !sys.getDiffCons().empty();
  dbm_manager = DBMFactory(clock_num, clock_upper_bounds, sys.getDiffCons());
  counters = ecounters;
}

int *TMStateManager::newState() const {

  int *re_state = new int[state_length];

  fill(re_state, re_state + clock_start_loc, 0);

  dbm_manager.init(re_state + clock_start_loc);

  return re_state;
}

bool TMStateManager::transitionReady(const int component, const int link,
                                     const int *const state) const {
  return sys.agents[component]->transitions[link].ready(component, this, state);
}
bool TMStateManager::isReachable(const int component, const int loc,
                                 int *state) const {
  return sys.agents[component]->locations[loc].isReachable(getClockManager(),
                                                           getDBM(state));
}

bool TMStateManager::hasMatchOutUrgentChan(const int *const state) const {
  if (sys.hasUrgentCh()) {
    const int *counter_value = getCounterValue(state);
    set<int> send_part, receive_part;
    for (int comp = 0; comp < component_num; comp++) {
      int loc = getLocationID(comp, state);
      if (sys.hasUrgentCh(comp, loc)) {
        vector<int> dummy =
            getOutUrgent(comp, loc, const_cast<int *>(counter_value));
        for (auto e : dummy) {
          if (e > 0) {
            if (receive_part.find(-e) != receive_part.end()) {
              return true;
            }

          } else {
            if (send_part.find(-e) != send_part.end()) {
              return true;
            }
          }
        }
        for (auto e : dummy) {
          if (e > 0) {
            send_part.insert(e);
          } else {
            receive_part.insert(e);
          }
        }
      }
    }
  }
  return false;
}

bool TMStateManager::hasOutBreakcastChan(const int *const state) const {

  for (int comp = 0; comp < component_num; comp++) {
    int loc = getLocationID(comp, state);
  }

  return false;
}

Compression<int> TMStateManager::getHeadCompression() const {
  Compression<int> re_comp(clock_start_loc);
  if (chan_num > 0) {
    for (int component_id = 0; component_id < component_num; component_id++) {

      // the value contain link id
      // TODO:
      int m = max(node_nums[component_id], link_nums[component_id]);
      re_comp.setBound(component_id, -m, m);
    }
  } else {
    for (int component_id = 0; component_id < component_num; component_id++) {
      re_comp.setBound(component_id, -node_nums[component_id],
                       node_nums[component_id]);
    }
  }

  if (chan_num > 0) {
    for (int component_id = 0; component_id < component_num; component_id++) {
      re_comp.setBound(component_id + component_num, -chan_num, chan_num);
    }
  }
  int k = 0;
  for (int i = counter_start_loc; i < freeze_location_index; i++) {
    re_comp.setBound(i, counters[k].getLB(), counters[k].getUP());
    k++;
  }

  /**
   * At most all the component in freeze location
   *
   */
  re_comp.setBound(freeze_location_index, 0, component_num - 1);

  return re_comp;
}

Compression<int> TMStateManager::getBodyCompression() const {
  int body_len = state_length - clock_start_loc;
  Compression<int> re_comp(body_len);
  return re_comp;
  /**
   * set the minimum and maximum for DBM matrix element
   * TODO: compress by the upper bound of clock constaint
   */

  int len = (int)clock_upper_bounds.size() / 2;
  assert(body_len == len * len);

  for (int i = 0; i < body_len; i++) {
    int row = (i) / len;
    int col = (i) % len;
    if (row == col) {
      re_comp.setBound(i, LTEQ_ZERO, LTEQ_ZERO);
    } else if (row == 0) {
      re_comp.setBound(i, clock_upper_bounds[col + len], LTEQ_ZERO);
    } else if (col == 0) {
      re_comp.setBound(i, LTEQ_ZERO, clock_upper_bounds[row] + 1);
    } else {
      re_comp.setBound(i, clock_upper_bounds[col + len],
                       clock_upper_bounds[row] + 1);
    }
  }
  return re_comp;
}

vector<int> TMStateManager::getOutUrgent(const int component, const int loc,
                                         State_t *state) const {

  vector<int> re;
  vector<int> outs = sys.getOutTransition(component, loc);
  for (auto link : outs) {
    if (sys.hasChannel(component, link)) {
      if (sys.agents[component]->transitions[link].getChannel().getType() ==
          URGENT_CH) {
        if (transitionReady(component, link, state)) {
          int chid =
              sys.agents[component]->transitions[link].getChannel().getGlobalId(
                  state);
          if (sys.agents[component]->transitions[link].getChannel().isSend()) {
            re.push_back(chid);
          } else {
            re.push_back(-chid);
          }
        }
      }
    }
  }
  return re;
}

bool TMStateManager::hasDiffCons() const { return hasDiff; }

int TMStateManager::getLocationID(const int component,
                                  const int *const state) const {
  int re = state[component];

  if (isCommitComp(component, state)) { // commit location
    re = getCommitLoc(component, state);
  }

  if (isBlock(component, state)) {
    re = sys.getSrc(component, re);
  }
  assert(re >= 0 && "The location id must greater or equal to then 0.");
  return re;
}

string TMStateManager::getLocationName(const int component,
                                       const int loc_ID) const {
  return sys.getLocationName(component, loc_ID);
}

bool TMStateManager::hasChannel() const { return sys.getChanNum() > 0; }

vector<int> TMStateManager::blockComponents(const int chid,
                                            const int *const state) const {
  vector<int> re_block_components;
  for (int i = 0; i < component_num; i++) {

    /**
     * return entire componments which waits for this signal chid
     *
     */

    if (state[i + component_num] == chid) {
      re_block_components.push_back(i);
    }
  }
  return re_block_components;
}

void TMStateManager::constructState(const int component_id, const int target,
                                    const int *const state, int *dbm,
                                    bool isCommit, int *re_state) const {

  memcpy(re_state, state, clock_start_loc * sizeof(int));

  re_state[component_id] = target;
  memcpy(re_state + clock_start_loc, dbm,
         (state_length - clock_start_loc) * sizeof(int));
  if (isCommit) {
    setCommitState(component_id, re_state);
  }
}

void TMStateManager::employLocInvariants(const int component,
                                         int *state) const {
  sys.agents[component]
      ->locations[getLocationID(component, state)]
      .employInvariants(getClockManager(), getDBM(state));
}

void TMStateManager::discretRun(const int component, const int link,
                                int *state) const {
  sys.agents[component]->transitions[link](component, this, state);
}

void TMStateManager::evolution(const int component, const int loc,
                               int *state) const {
  sys.agents[component]->locations[loc](getClockManager(), getDBM(state));
}
string TMStateManager::getLocDotLabel(const int *const state) const {
  string re = "";
  for (int i = 0; i < component_num; i++) {
    int loc = getLocationID(i, state);
    if (i != 0) {
      re += ", " + getLocationName(i, loc);
    } else {
      re += getLocationName(i, loc);
    }
  }
  return re;
}

vector<string>
TMStateManager::getCounterDotLabel(const int *const state) const {

  vector<string> re;
  const int *counter_value = getCounterValue(const_cast<int *>(state));
  vector<BaseDecl> vars = sys.getAllVar(INT_T);
  for (auto &e : vars) {
    string item = e.name;
    if (e.num == 1) {
      item += " = " + to_string(counter_value[e.start_loc]);
    } else {
      item += " = [";
      for (int i = 0; i < e.num; i++) {
        if (i > 0) {
          item += ",";
        }
        item += to_string(counter_value[e.start_loc + i]) + " ";
      }
      item += "]";
    }
    re.push_back(item);
  }
  return re;
}

void TMStateManager::constructState(const int component_id, const int target,
                                    bool isCommit, int *state) const {
  state[component_id] = target;
  if (isCommit) {
    setCommitState(component_id, state);
  }
}

ostream &TMStateManager::dump(const int *const state, ostream &out) const {

  for (int i = 0; i < component_num; i++) {

    int loc = getLocationID(i, state);

    out << setw(LOC_OUT_WIDTH)
        << getLocationName(i, loc); // agents[i]->getLocationName(loc);
  }
  out << endl;
  return getClockManager().dump(out, getDBM(state)) << endl;
}

} // namespace graphsat
