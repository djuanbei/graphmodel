
/**
 * @file   reachability.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Apr 27 08:49:40 2019
 *
 * @brief  reachability problem of timed automata
 *
 *
 */

#ifndef __REACHABILITY_HPP
#define __REACHABILITY_HPP

#include <cassert>
#include <map>
#include <random>
#include <set>
#include <vector>

#include "util/macrodef.h"
#include "util/parallel.h"

#include "model/channel.h"
#include "property/property.h"

namespace graphsat {

using std::map;
using std::set;
using std::vector;

#ifdef ONLINE_CHECK
#define isReach(P, S) false
#endif

template <typename SYS> class Reachability {
  typedef typename SYS::State_t State_t;

public:
  Reachability(const SYS &s) : sys(s) {
    component_num = sys.getComponentNumber();
    manager = sys.getStateManager();

    cache_state = manager->newState();
    next_state = manager->newState();
  }

  ~Reachability() {
    if (cache_state != nullptr) {
      manager->destroyState(cache_state);
      cache_state = nullptr;
    }
    if (next_state != nullptr) {
      manager->destroyState(next_state);
      next_state = nullptr;
    }
  }
  template <typename D> void computeAllReachableSet(D &data) {
    Property prop;

    run(data, &prop);
  }

  /**
   * @brief whether the sysyem satisfies the prop
   *
   * @param prop The property which want to verify.
   *
   * @return ture if there is a reachable state which makes prop ture,
   * false otherwise.
   */

  template <typename D> bool satisfy(D &data, const Property *prop) {
    return run(data, prop);
  }

  /**
   * @param prop The property which want to verify.
   *
   * @return true if ther is a reachable state which make prop ture,
   * false otherwise.
   */
  template <typename D> bool run(D &data, const Property *prop) {

    Check_State re = data.search(prop);

    if (re != UNKOWN) {
      if (TRUE == re) {
        return true;
      } else if (FALSE == re) {
        return false;
      }
    }

    // For given target find the source which change in last step

    while (!data.waitEmpty()) {

      typename SYS::State_t *state = data.next();

      if (oneDiscreteStep(data, prop, state)) {
        delete[] state;
        return true;
      }
      delete[] state;
    }

    return false;
  }

private:
  /**
   One discrete step
   */
  template <typename D>
  bool oneDiscreteStep(D &data, const Property *prop, State_t *state) {
#ifdef DRAW_GRAPH
    data.incCurrentParent();

#endif
#ifdef PRINT_STATE
    manager->dump(state);
#endif
    // PRINT_STATE_MACRO;
    /**
     freeze state the time can not delay
     */
    if (manager->isFreeze(state)) {
      for (int component = 0; component < component_num; component++) {
        // component is at  freeze location and component does not wait another
        // components.
        if (manager->isCommitComp(component, state) &&
            !manager->isBlock(state, component)) {
          return oneComponent(data, component, prop, state);
        }
      }
    }

    for (int component = 0; component < component_num; component++) {

      if (manager->isBlock(state, component)) {
        /**
         * Waiting for synchronize signal
         *
         */
        continue;
      }
      if (oneComponent(data, component, prop, state)) {

        return true;
      }
    }
    return false;
  }

  /**
   *
   * @param component one step component
   * @param prop  the verifies property
   * @param state  the current state
   *
   * @return ture if prop satisfied by child state of state
   */
  template <typename D>
  bool oneComponent(D &data, int component, const Property *prop,
                    State_t *state) {

    const int source = manager->getLocationID(component, state);

    const vector<int> out_ts = manager->getOutTransition(component, source);
    for (auto link : out_ts) {

      assert(link >= 0 &&
             "The value of link id requires greater or equal than 1.");
      /**
       * Whether the jump conditions satisfies except synchronize signal
       *
       */

      if (!manager->transitionReady(component, link, state)) {
        continue;
      }

      if (sys.agents[component]->transitions[link].hasChannel()) {
        const Channel &channel =
            sys.agents[component]->transitions[link].getChannel();
        // channel.id start from 1

        if (doSynchronize(data, component, prop, state, link, channel)) {
          return true;
        }

      } else {
        if (oneTranision(data, component, link, prop, state)) {
          return true;
        }
      }
    }
    return false;
  }

  /**
   * TODO: first do send counter action then do receive action
   * @param current_component current run component
   * @param block_component_id blocked  component id
   * @param link  the block transition
   * @param state the current state
   * @param prop the property
   * @param is_send ture if block_component_id is send part, false otherwise.
   *
   * @return  true the next state make prop true, false otherwise.
   */
  template <typename D>
  bool unBlockOne(D &data, const int current_component,
                  const int block_component_id, const int link, State_t *state,
                  const Property *prop, bool is_send) {

    manager->copy(next_state, state);
    manager->unBlock(next_state, block_component_id);

    int block_link = next_state[block_component_id];
    if (manager->isCommitComp(block_component_id, next_state)) {
      block_link = manager->getCommitLoc(block_component_id, next_state);
    }
    assert(block_link >= 0);

    int block_source = 0;
    sys.agents[block_component_id]->graph.findSrc(block_link, block_source);
    next_state[block_component_id] = block_source;
    int send_component_id = current_component;
    int send_link = link;
    int receive_component_id = block_component_id;
    int receive_link = block_link;

    if (!is_send) {
      send_component_id = block_component_id;
      send_link = block_link;
      receive_component_id = current_component;
      receive_link = link;
    }

    // send do firstly
    /**
     *  TDOO: has some problems
     */
    sys.agents[send_component_id]->transitions[send_link](
        send_component_id, manager,
        next_state); // send part firstly update state

    sys.agents[receive_component_id]->transitions[receive_link](
        receive_component_id, manager, next_state);

    int send_target = 0;
    sys.agents[send_component_id]->graph.findSnk(send_link, send_target);
    next_state[send_component_id] = send_target;

    bool is_send_commit =
        sys.agents[send_component_id]->locations[send_target].isCommit();

    if (is_send_commit) {
      manager->setCommitState(send_component_id, next_state);
    }
    int receive_target = 0;

    sys.agents[receive_component_id]->graph.findSnk(receive_link,
                                                    receive_target);
    next_state[receive_component_id] = receive_target;

    bool is_receive_commit =
        sys.agents[receive_component_id]->locations[receive_target].isCommit();

    if (is_receive_commit) {
      manager->setCommitState(receive_component_id, next_state);
    }

    int source, target;
    source = target = 0;
    sys.agents[send_component_id]->graph.findSrcSnk(send_link, source, target);

    if (sys.agents[send_component_id]->locations[source].isFreezeLocation()) {
      next_state[manager->getFreezeLocation()]--;
      assert(next_state[manager->getFreezeLocation()] >= 0);
    }
    if (sys.agents[send_component_id]->locations[target].isFreezeLocation()) {
      next_state[manager->getFreezeLocation()]++;
      assert(next_state[manager->getFreezeLocation()] <= component_num);
    }

    sys.agents[receive_component_id]->graph.findSrcSnk(receive_link, source,
                                                       target);

    if (sys.agents[receive_component_id]
            ->locations[source]
            .isFreezeLocation()) {
      next_state[manager->getFreezeLocation()]--;
      assert(next_state[manager->getFreezeLocation()] >= 0);
    }

    if (sys.agents[receive_component_id]
            ->locations[target]
            .isFreezeLocation()) {
      next_state[manager->getFreezeLocation()]++;
      assert(next_state[manager->getFreezeLocation()] <= component_num);
    }

    // can not stay on current_target and block_target locations when
    // send_target or receive_target is a commit or urgent location.

    return delay(data, send_component_id, send_target, prop, next_state);
  }

  /**
   * TODO: urgent chan
   *
   */
  template <typename D>
  bool doSynchronize(D &data, int component, const Property *prop,
                     State_t *state, int link, const Channel &channel) {

    vector<int> wait_components;
    bool is_send = true;
    State_t *counter_value = manager->getCounterValue(state);
    if (channel.isSend()) {

      wait_components =
          manager->blockComponents(-channel.getGlobalId(counter_value), state);
    } else if (channel.isRecive()) {
      is_send = false;
      wait_components =
          manager->blockComponents(channel.getGlobalId(counter_value), state);
    }
    if (!wait_components.empty()) {
      // TODO: check all the channel type
      if (channel.getType() == ONE2ONE_CH || channel.getType() == URGENT_CH) {
        std::uniform_int_distribution<int> distribution(
            0, (int)wait_components.size() - 1);
        int id = distribution(generator);
        int block_component_id = wait_components[id];
        return unBlockOne(data, component, block_component_id, link, state,
                          prop, is_send);
      } else if (channel.getType() == BROADCAST_CH) {
        for (auto id : wait_components) {
          int block_component_id = wait_components[id];
          if (unBlockOne(data, component, block_component_id, link, state, prop,
                         is_send)) {
            return true;
          }
        }
      }

    } else {
      manager->copy(cache_state, state);
      State_t *counter_value = manager->getCounterValue(state);
      assert(channel.getGlobalId(counter_value) > 0); // chan it start with 1
      if (channel.isSend()) {
        cache_state[component + component_num] =
            channel.getGlobalId(counter_value); // send part
      } else if (channel.isRecive()) {
        cache_state[component + component_num] =
            -channel.getGlobalId(counter_value); // receive part
      }
      // TODO: add commit property
      if (manager->isCommitComp(component, state)) {
        cache_state[component] = link; // block link
        manager->setCommitState(component,
                                cache_state); // save  commit property
      } else {
        cache_state[component] = link; // block link
      }

      data.add(cache_state);
    }
    return false;
  }

  /**
   *@brief The current location of TA is on src location of link, then
   * it wants to jump through link to snk location of link.
   *
   * @param component Current compoent id
   * @param link  Current transition id
   * @param prop  verifying property
   * @param state  Current state
   *
   * @return  true if find a state makes prop true, false otherwise.
   */
  template <typename D>
  bool oneTranision(D &data, const int component, const int link,
                    const Property *prop, const State_t *const state) {

    manager->copy(next_state, state);
    int source = 0;
    int target = 0;
    sys.agents[component]->graph.findSrcSnk(link, source, target);
    if (sys.agents[component]->locations[source].isFreezeLocation()) {
      next_state[manager->getFreezeLocation()]--;
      assert(next_state[manager->getFreezeLocation()] >= 0);
    }
    if (sys.agents[component]->locations[target].isFreezeLocation()) {
      next_state[manager->getFreezeLocation()]++;
      assert(next_state[manager->getFreezeLocation()] <= component_num);
    }

    sys.agents[component]->transitions[link](
        component, manager,
        next_state); // update counter state and reset clock state

    return delay(data, component, target, prop, next_state);
  }

  template <typename D>
  bool delay(D &data, const int component, const int target,
             const Property *prop, State_t *state) {

    if (!sys.agents[component]->locations[target].isReachable(
            manager->getClockManager(), manager->getDBM(state))) {
      return false;
    }
    state[component] = target;

    /**
     Whether there is some component in freeze location
     */
    if (!manager->isFreeze(state)) {

      sys.agents[component]->locations[target](manager->getClockManager(),
                                               manager->getDBM(state));
    }

    return postDelay(data, component, target, prop, state);
  }

  template <typename D>
  bool postDelay(D &data, const int component, const int target,
                 const Property *prop, State_t *state) {

    bool is_commit = sys.agents[component]->locations[target].isCommit();

    bool re_bool = false;
    if (!manager->isFreeze(state)) {
      for (int component_id = 0; component_id < component_num; component_id++) {

        if (manager->withoutChannel(component_id, state)) {
          sys.agents[component_id]
              ->locations[manager->getLocationID(component_id, state)]
              .employInvariants(manager->getClockManager(),
                                manager->getDBM(state));
        } else {
          int block_source;
          sys.agents[component_id]->graph.findSrc(state[component_id],
                                                  block_source);
          sys.agents[component_id]->locations[block_source].employInvariants(
              manager->getClockManager(), manager->getDBM(state));
        }
      }
    }
    if (manager->hasDiffCons()) {

      vector<int *> next_dbms;
      manager->norm(manager->getDBM(state), next_dbms);

      for (auto dbm : next_dbms) {
        manager->constructState(component, target, state, dbm, is_commit,
                                cache_state);
        if (data.add(cache_state)) {
          if (isReach(prop, cache_state)) {
            re_bool = true;
            break;
          }
        }
      }
      for (auto dbm : next_dbms) {
        manager->getClockManager().destroyDBM(dbm);
      }
    } else {
      manager->norm(manager->getDBM(state));
      manager->constructState(component, target, is_commit, state);
      if (data.add(state)) {
        if (isReach(prop, state)) {
          return true;
        }
      }
    }
    return re_bool;
  }

  /**
   *
   *
   * @param prop  verifies property
   * @param state one reachable state
   *
   * @return ture if prop is true under state, false otherwise.
   */
#ifndef ONLINE_CHECK
  inline bool isReach(const Property *prop, const State_t *const state) const {
    return (*prop)(manager.get(), state);
  }
#endif

  shared_ptr<const typename SYS::StateManager_t> manager;
  int component_num;
  const SYS &sys;
  State_t *cache_state;
  State_t *next_state;
  std::default_random_engine generator;
};
} // namespace graphsat

#endif
