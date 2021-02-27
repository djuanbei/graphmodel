
#include "state/ta_statemanager.h"

#include <cassert>
#include <unordered_set>

#include "alg/one_step.h"
#include "model/graphmodel.hpp"

namespace graphmodel {
TMStateManager::TMStateManager(const INT_TAS_t& s,
                               const vector<Counter>& ecounters,
                               int out_clock_num,
                               const vector<int>& oclock_upper_bounds,
                               const vector<int>& nodes,
                               const vector<int>& links)
    : sys(s) {
  clock_num = out_clock_num;
  assert((int)oclock_upper_bounds.size() == 2 * clock_num + 2);

  state_length = 0;
  component_num = s.getComponentNumber();

  clock_upper_bounds = oclock_upper_bounds;
  node_nums = nodes;
  link_nums = links;

  counter_start_loc = component_num;
  state_length = component_num + (int)ecounters.size();

  freeze_location_index = state_length;
  state_length++;

  clock_start_loc = state_length;

  state_length += (clock_num + 1) * (clock_num + 1);
  parent_location_index = state_length;

  state_length++;

  hasDiff = !sys.getDiffCons().empty();
  dbm_manager = DBMManager(clock_num, clock_upper_bounds, sys.getDiffCons());
  counters = ecounters;
  compress_state =
      StateConvert<int>(getClockStart(), getStateLen() - getClockStart(),
                        getHeadCompression(), getBodyCompression());
  int* state = newState();
  for (int i = 0; i < component_num; i++) {
    state[i] = sys.getInitialLoc(i);
    if (sys.getLocationNumber(i) > 0 && sys.isFreezeLocation(i, state[i])) {
      incFreeze(state);
    }
  }

  state[parent_location_index] = -1;  // initial state;
  if (getFreezeComponentNumber(state) == 0) {
    if (sys.getLocationNumber(0) > 0) {
      init_states = doEvolution(this, 0, state[0], state);
    }
  } else {
    init_states.push_back(state);
  }
  cache_state = newState();
}

int* TMStateManager::newState() const {
  int* re_state = new int[state_length];

  fill(re_state, re_state + clock_start_loc, 0);

  dbm_manager.init(re_state + clock_start_loc);

  return re_state;
}
void TMStateManager::reset(int* state) const {
  fill(state, state + clock_start_loc, 0);
  dbm_manager.init(state + clock_start_loc);
}

int* TMStateManager::randState() const {
  int* re = newState();
  for (int i = 0; i < component_num; i++) {
    re[i] = ::rand() % sys.getLocationNumber(i);
  }

  for (int i = component_num; i < clock_start_loc; i++) {
    re[i] = ::rand() % 100;
  }

  int* dbm = getClockManager().randomFeasiableDBM();
  memcpy(re + clock_start_loc, dbm,
         (state_length - clock_start_loc) * sizeof(int));
  getClockManager().destroyDBM(dbm);

  return re;
}

bool TMStateManager::contain(const int* const lhs, const int* const rhs) const {
  if (memcmp(lhs, rhs, clock_start_loc * sizeof(int)) == 0) {
    return getClockManager().include(getDBM(lhs), getDBM(rhs));
  }
  return false;
}

bool TMStateManager::transitionReady(const int component, const int link,
                                     const int* const state) const {
  return sys.agents[component]->transitions[link].ready(this, state);
}
bool TMStateManager::isReachable(const int component, const int loc,
                                 int* state) const {
  return sys.agents[component]->locations[loc].isReachable(getClockManager(),
                                                           getDBM(state));
}

int TMStateManager::getCompentId(const string& template_name,
                                 const int agent_id) const {
  return sys.getCompentId(template_name, agent_id);
}
std::vector<int> TMStateManager::getCommitComponents(
    const int* const state) const {
  std::vector<int> re;
  for (int i = 0; i < component_num; i++) {
    if (sys.isCommit(i, state[i])) {
      re.push_back(i);
    }
  }
  return re;
}

bool TMStateManager::hasMatchOutUrgentChan(const int* const state) const {
  if (!sys.hasUrgentCh()) {
    return false;
  }
  std::unordered_set<int> pass_part;

  for (int comp = 0; comp < component_num; comp++) {
    int loc = getLocationID(comp, state);
    if (sys.hasUrgentCh(comp, loc)) {
      std::unordered_set<int> dummy =
          getEnableOutUrgent(comp, loc, const_cast<int*>(state));

      for (auto e : dummy) {
        if (pass_part.find(-e) != pass_part.end()) {
          return true;
        }
      }

      pass_part.insert(dummy.begin(), dummy.end());
    }
  }
  return false;
}

bool TMStateManager::hasOutSendBroadcastChan(const int* const state) const {
  if (!sys.hasBroadcaseCh()) {
    return false;
  }
  // std::set<int> send_part, receive_part;
  for (int comp = 0; comp < component_num; comp++) {
    int loc = getLocationID(comp, state);
    if (sys.hasBroadcaseSendCh(comp, loc)) {
      std::unordered_set<int> dummy =
          getEnableOutSendBroadcast(comp, loc, const_cast<int*>(state));
      if (!dummy.empty()) {
        return true;
      }
    }
  }

  return false;
}

int TMStateManager::getTypeStart(const TYPE_T type) const {
  switch (type) {
    case INT_T:
      return counter_start_loc;
    case CLOCK_T:
      return clock_start_loc;
    default:
      assert(false);
      return 0;
  }
}

int TMStateManager::getTypeNumber(const TYPE_T type) const {
  switch (type) {
    case INT_T:
      return getCounterNumber();
    case CLOCK_T:
      return getClockNumber();
    default:
      assert(false);
      return 0;
  }
}

const int* TMStateManager::getValue(const int component, const string& key,
                                    const int* const state) const {
  TYPE_T type = sys.getType(component, key);

  if (type == NO_T) {
    assert(false);
    return 0;
  }
  int start = getTypeStart(type);
  return state + (start + sys.getKeyID(component, type, key));
}

void TMStateManager::setValue(const int component, const string& key, int value,
                              int* state) const {
  TYPE_T type = sys.getType(component, key);

  if (type == NO_T) {
    assert(false);
  }
  int start = getTypeStart(type);
  state[start + sys.getKeyID(component, type, key)] = value;
}

int* TMStateManager::getValue(const int component, const string& key,
                              int* state) const {
  TYPE_T type = sys.getType(component, key);

  if (type == NO_T) {
    assert(false);
    return nullptr;
  }
  int start = getTypeStart(type);
  return state + (start + sys.getKeyID(component, type, key));
}

MatrixValue TMStateManager::getClockLowerBound(const int component,
                                               const std::string& key,
                                               const int* const state) const {
  int id = sys.getKeyID(component, CLOCK_T, key);
  const int* const dbm = getDBM(state);
  MatrixValue re = getClockManager().getUpperBound(dbm, GLOBAL_CLOCK_ID, id);
  re.value *= -1;
  return re;
}

void TMStateManager::setClockLowerBound(const int component,
                                        const std::string& key, int* state,
                                        const MatrixValue& value) const {
  int id = sys.getKeyID(component, CLOCK_T, key);
  int* dbm = getDBM(state);
  MatrixValue temp(value);
  temp.value *= -1;
  getClockManager().setUppperBound(dbm, GLOBAL_CLOCK_ID, id, temp);
}

MatrixValue TMStateManager::getClockUpperBound(const int component,
                                               const string& key,
                                               const int* const state) const {
  int id = sys.getKeyID(component, CLOCK_T, key);
  const int* const dbm = getDBM(state);
  return getClockManager().getUpperBound(dbm, id, GLOBAL_CLOCK_ID);
}

void TMStateManager::setClockUpperBound(const int component,
                                        const std::string& key, int* state,
                                        const MatrixValue& value) const {
  int id = sys.getKeyID(component, CLOCK_T, key);
  int* dbm = getDBM(state);
  getClockManager().setUppperBound(dbm, id, GLOBAL_CLOCK_ID, value);
}

// keyA- keyB > ( >=) re
MatrixValue TMStateManager::getClockDiffLowerBound(
    const int componentA, const std::string& keyA, const int componentB,
    const std::string& keyB, const int* const state) const {
  int idA = sys.getKeyID(componentA, CLOCK_T, keyA);
  int idB = sys.getKeyID(componentB, CLOCK_T, keyB);
  const int* const dbm = getDBM(state);

  MatrixValue re = getClockManager().getUpperBound(dbm, idB, idA);
  re.value *= -1;
  return re;
}

void TMStateManager::setClockLowerBound(
    const int componentA, const std::string& keyA, const int componentB,
    const std::string& keyB, const MatrixValue& value, int* state) const {
  int idA = sys.getKeyID(componentA, CLOCK_T, keyA);
  int idB = sys.getKeyID(componentB, CLOCK_T, keyB);

  int* dbm = getDBM(state);
  MatrixValue temp(value);
  temp.value *= -1;
  getClockManager().setUppperBound(dbm, idB, idA, temp);
}

MatrixValue TMStateManager::getClockUpperBound(const int componentA,
                                               const std::string& keyA,
                                               const int componentB,
                                               const std::string& keyB,
                                               const int* const state) const {
  int idA = sys.getKeyID(componentA, CLOCK_T, keyA);
  int idB = sys.getKeyID(componentB, CLOCK_T, keyB);
  const int* const dbm = getDBM(state);

  return getClockManager().getUpperBound(dbm, idA, idB);
}

void TMStateManager::setClockUpperBound(
    const int componentA, const std::string& keyA, const int componentB,
    const std::string& keyB, const MatrixValue& value, int* state) const {
  int idA = sys.getKeyID(componentA, CLOCK_T, keyA);
  int idB = sys.getKeyID(componentB, CLOCK_T, keyB);
  int* dbm = getDBM(state);
  getClockManager().setUppperBound(dbm, idA, idB, value);
}

Compression<int> TMStateManager::getHeadCompression() const {
  Compression<int> re_comp(clock_start_loc);

  for (int component_id = 0; component_id < component_num; component_id++) {
    re_comp.setBound(component_id, -node_nums[component_id],
                     node_nums[component_id]);
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
  re_comp.setBound(freeze_location_index, 0, component_num);

  return re_comp;
}

Compression<int> TMStateManager::getBodyCompression() const {
  int body_len = state_length - clock_start_loc;
  Compression<int> re_comp(body_len);
  // return re_comp;
  /**
   * set the minimum and maximum for DBM matrix element
   * TODO: compress by the upper bound of clock constaint
   */

  int len = (int)clock_upper_bounds.size() / 2;
  assert(body_len == len * len + 1);

  for (int i = 0; i < body_len - 1; i++) {
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

std::vector<int> TMStateManager::getEnableOutLinks(const int component,
                                                   const int source,
                                                   int* state) const {
  std::vector<int> re;

  const vector<int>& out_ts = sys.getOutTransition(component, source);
  for (auto link : out_ts) {
    if (transitionReady(component, link, state)) {
      re.push_back(link);
    }
  }
  return re;
}

std::unordered_set<int> TMStateManager::getEnableOutBroadcast(
    const int component, const int loc, int* state) const {
  std::unordered_set<int> re;
  int* counter_value = getCounterValue(state);
  const vector<int>& outs = sys.getOutTransition(component, loc);
  for (auto link : outs) {
    if (sys.hasChannel(component, link)) {
      if (sys.agents[component]->transitions[link].getChannel().getType() ==
          BROADCAST_CH) {
        if (transitionReady(component, link, state)) {
          re.insert(sys.agents[component]
                        ->transitions[link]
                        .getChannel()
                        .getSiginGlobalId(counter_value));
        }
      }
    }
  }
  return re;
}

std::unordered_set<int> TMStateManager::getEnableOutSendBroadcast(
    const int component, const int loc, int* state) const {
  std::unordered_set<int> re;
  int* counter_value = getCounterValue(state);
  const vector<int>& outs = sys.getOutTransition(component, loc);
  for (auto link : outs) {
    if (sys.hasBroadcaseSendCh(component, link)) {
      if (transitionReady(component, link, state)) {
        re.insert(sys.agents[component]
                      ->transitions[link]
                      .getChannel()
                      .getSiginGlobalId(counter_value));
      }
    }
  }
  return re;
}

std::unordered_set<int> TMStateManager::getEnableOutUrgent(
    const int component, const int loc, State_t* state) const {
  std::unordered_set<int> re;
  int* counter_value = getCounterValue(state);
  const vector<int>& outs = sys.getOutTransition(component, loc);
  for (auto link : outs) {
    if (sys.hasChannel(component, link)) {
      if (sys.agents[component]->transitions[link].getChannel().getType() ==
          URGENT_CH) {
        if (transitionReady(component, link, state)) {
          re.insert(sys.agents[component]
                        ->transitions[link]
                        .getChannel()
                        .getSiginGlobalId(counter_value));
        }
      }
    }
  }
  return re;
}

std::unordered_set<int> TMStateManager::getEnableOutNormalChan(
    const int component, const int loc, int* state) const {
  std::unordered_set<int> re;
  const vector<int>& outs = sys.getOutTransition(component, loc);
  int* counter_value = getCounterValue(state);
  for (auto link : outs) {
    if (sys.hasChannel(component, link)) {
      const Channel& ch = sys.agents[component]->transitions[link].getChannel();
      if (ch.getType() == ONE2ONE_CH) {
        if (transitionReady(component, link, state)) {
          re.insert(ch.getSiginGlobalId(counter_value));
        }
      }
    }
  }
  return re;
}

bool TMStateManager::hasDiffCons() const { return hasDiff; }

int TMStateManager::getLocationID(const int component,
                                  const int* const state) const {
  return state[component];
}

string TMStateManager::getLocationName(const int component,
                                       const int loc_ID) const {
  return sys.getLocationName(component, loc_ID);
}

vector<int> TMStateManager::getChanLinks(const int component, const int source,
                                         int chid, int* state) const {
  // set<int> re;
  vector<int> outs = sys.agents[component]->graph.getAdj(source);
  int* counter_value = getCounterValue(state);
  vector<int> temp;

  for (auto link : outs) {
    if (sys.hasChannel(component, link) &&
        sys.getChannel(component, link).getSiginGlobalId(counter_value) ==
            chid) {
      temp.push_back(link);
    }
  }

  return temp;
}

vector<int> TMStateManager::blockComponents(const int chid,
                                            const int* const state) const {
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

int TMStateManager::getComponentCounterStartLoc(const int id) const {
  int re = counter_start_loc;
  for (int i = 0; i < id; i++) {
    re += sys.getComponentCounterNumber(i);
  }
  return re;
}

int TMStateManager::getComponentClockStartLoc(const int id) const {
  int re = clock_start_loc;
  for (int i = 0; i < id; i++) {
    re += sys.getComponentClockNumber(i);
  }
  return re;
}

void TMStateManager::swap(const int i, const int j, int* state) const {
  if (i == j) {
    return;
  }
  assert((sys.agents[i]->getTemplate()->getName() ==
          sys.agents[j]->getTemplate()->getName()) &&
         "Agent i and agent j must come from sane template.");
  int temp;
  temp = state[i];
  state[i] = state[j];
  state[j] = temp;
  int counter_a_loc = getComponentCounterStartLoc(i);
  int counter_b_loc = getComponentCounterStartLoc(j);
  int counter_num = sys.getComponentCounterNumber(i);
  for (int k = 0; k < counter_num; k++) {
    temp = state[counter_a_loc + k];
    state[counter_a_loc + k] = state[counter_b_loc + k];
    state[counter_b_loc + k] = temp;
  }
  // TODO: scaler variable
  if (state[component_num] == i + 1) {
    state[component_num] = j + 1;
  } else if (state[component_num] == j + 1) {
    state[component_num] = i + 1;
  }
  int* dbm = getDBM(state);

  int clock_a_start = sys.getComponentClockStartID(i);
  int clock_b_start = sys.getComponentClockStartID(j);
  int clock_num = sys.getComponentClockNumber(i);

  for (int k = 0; k < clock_num; k++) {
    getClockManager().swap(dbm, clock_a_start + k, clock_b_start + k);
  }
}

void TMStateManager::constructState(const int* const state,
                                    const int* const dbm, int* re_state) const {
  memcpy(re_state, state, clock_start_loc * sizeof(int));
  memcpy(re_state + clock_start_loc, dbm,
         (state_length - clock_start_loc) * sizeof(int));
}

void TMStateManager::employLocInvariants(const int component,
                                         int* state) const {
  sys.agents[component]
      ->locations[getLocationID(component, state)]
      .employInvariants(getClockManager(), getDBM(state));
}

void TMStateManager::discreteRun(const int component, const int link,
                                 int* state) const {
  sys.agents[component]->transitions[link](this, state);
  int source = sys.getSrc(component, link);
  int target = sys.getSnk(component, link);
  if (sys.isFreezeLocation(component, source)) {
    decFreeze(state);
    assert(getFreezeComponentNumber(state) >= 0);
  }
  if (sys.isFreezeLocation(component, target)) {
    incFreeze(state);
    assert(getFreezeComponentNumber(state) <= component_num);
  }

  state[component] = target;
}

vector<int*> TMStateManager::evolution(const int component, const int loc,
                                       int* state) const {
  vector<int*> re;
  state[component] = loc;
  sys.agents[component]->locations[loc](getClockManager(), getDBM(state));
  if (hasDiffCons()) {
    vector<int*> next_dbms;
    norm(getDBM(state), next_dbms);
    if (1 == next_dbms.size()) {
      re.push_back(state);
    } else {
      for (auto dbm : next_dbms) {
        int* dummy = newState();
        constructState(state, dbm, dummy);
        re.push_back(dummy);
      }
      destroyState(state);
    }
    for (auto dbm : next_dbms) {
      getClockManager().destroyDBM(dbm);
    }
  } else {
    norm(getDBM(state));
    re.push_back(state);
  }
  return re;
}
string TMStateManager::getLocDotLabel(const int* const state) const {
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

vector<string> TMStateManager::getCounterDotLabel(
    const int* const state) const {
  vector<string> re;
  const int* counter_value = getCounterValue(const_cast<int*>(state));
  vector<BaseDecl> vars = sys.getAllVar(INT_T);
  for (auto& e : vars) {
    string item = e.name;
    if (e.num == 1) {
      item += " = " + to_string(counter_value[e.start_loc]);
    } else {
      item += " = [";
      for (int i = 0; i < e.num; i++) {
        if (i + 1 < e.num) {
          item += to_string(counter_value[e.start_loc + i]) + ", ";
        } else {
          item += to_string(counter_value[e.start_loc + i]);
        }
      }
      item += "]";
    }
    re.push_back(item);
  }
  return re;
}

void TMStateManager::encode(UINT* now, int* original) const {
  // copy(cache_state, original);
  getClockManager().encode(getDBM(original));
  compress_state.encode(now, original);
}

void TMStateManager::decode(int* now, const UINT* const original) const {
  compress_state.decode(now, original);
  getClockManager().decode(getDBM(now));
}

ostream& TMStateManager::dump(ostream& out, const int* const state) const {
  for (int i = 0; i < component_num; i++) {
    int loc = getLocationID(i, state);

    out << setw(LOC_OUT_WIDTH) << getLocationName(i, loc);
  }
  out << endl;
  vector<string> couter_labels = getCounterDotLabel(state);
  for (auto& l : couter_labels) {
    out << l << endl;
  }

  return getClockManager().dump(out, getDBM(state)) << endl;
}

}  // namespace graphmodel
