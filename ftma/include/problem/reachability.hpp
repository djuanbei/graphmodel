
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

#include "alg/ta_next_step.h"

namespace graphsat {

using std::map;
using std::set;
using std::vector;

template <typename SYS>
class Reachability {
  typedef typename SYS::State_t State_t;

 public:
  Reachability(SYS& s) : sys(s), nextS(s) {
    component_num = sys.getComponentNumber();
    manager = s.getStateManager();

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
  template <typename D>
  void computeAllReachableSet(D* data) {
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

  template <typename D>
  bool satisfy(D* data, const Property* prop) {
    return run(data, prop);
  }

  /**
   * @param prop The property which want to verify.
   *
   * @return true if ther is a reachable state which make prop ture,
   * false otherwise.
   */
  template <typename D>
  bool run(D* data, const Property* prop) {
    Check_State re = data->search(prop);

    if (re != UNKOWN) {
      if (TRUE == re) {
        return true;
      } else if (FALSE == re) {
        return false;
      }
    }
    data->setProperty(prop);

    // For given target find the source which change in last step

    while (!data->waitEmpty()) {
      typename SYS::State_t* state = data->next();

      if (oneDiscreteStep(data, state)) {
        manager->destroyState(state);
        return true;
      }
      manager->destroyState(state);
    }

    return false;
  }

 private:
  /**
   One discrete step
   */
  template <typename D>
  bool oneDiscreteStep(D* data, const State_t* const state) {
    //  data->incCurrentParent();

#ifdef PRINT_STATE
    manager->dump(state);
#endif
    //   manager->dump(state);
    std::vector<OneStep> re = nextS.getNextStep(const_cast<int*>(state));
    return doOneStep(data, manager.get(), state, re) == TRUE;

    /**
     freeze state the time can not delay
     */
    if (manager->isFreeze(state)) {
      for (int component = 0; component < component_num; component++) {
        // component is at  freeze location and component does not wait another
        // components. commit location go first.

        //        if (sys.isCommit(component, manager->getLocationID(component,
        //        state)) &&
        //            !manager->isBlock(component, state)) {
        //          return oneComponent(data, component, state);
        //        }
      }
    }

    // If  there has two out transition with match send and recive urgent
    // channel
    if (manager->hasMatchOutUrgentChan(state)) {
      vector<OneStep> re;
      nextS.doUrgant(const_cast<int*>(state), re);
      return doOneStep(data, manager.get(), state, re) == TRUE;
    }
    // If there has at less one out transition with breakcast sene channel
    if (manager->hasOutBreakcastChan(state)) {
      // TODO:
    }

    for (int component = 0; component < component_num; component++) {
      //      if (manager->isBlock(component, state)) {
      //        /**
      //         * Waiting for synchronize signal
      //         *
      //         */
      //        continue;
      //      }
      if (oneComponent(data, component, state)) {
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
  bool oneComponent(D* data, int component, const State_t* const state) {
    const int source = manager->getLocationID(component, state);

    const vector<int> out_ts = sys.getOutTransition(component, source);
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

      if (sys.hasChannel(component, link)) {
        const Channel& channel = sys.getChannel(component, link);
        // channel.id start from 1

        // TODO: URGENT_CH
        if (channel.getType() != URGENT_CH) {
          if (doSynchronize(data, component, state, link, channel)) {
            return true;
          }
          // TODO: URGENT_CH
        } else {  // URGENT channel

          if (oneTranision(data, component, link, state)) {
            return true;
          }
        }
      } else {
        if (oneTranision(data, component, link, state)) {
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
  bool unBlockOne(D* data, const int current_component,
                  const int block_component_id, const int link,
                  const State_t* const state, bool is_send) {
    manager->copy(next_state, state);
    // manager->unBlock(block_component_id, next_state);

    int block_link = next_state[block_component_id];
    // if (manager->isCommitComp(block_component_id, next_state)) {
    //   block_link = manager->getCommitLoc(block_component_id, next_state);
    // }
    assert(block_link >= 0);

    int block_source = sys.getSrc(block_component_id, block_link);

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
    manager->discretRun(send_component_id, send_link,
                        next_state);  // send part firstly update state

    manager->discretRun(receive_component_id, receive_link, next_state);

    int send_target = sys.getSnk(send_component_id, send_link);

    next_state[send_component_id] = send_target;

    bool is_send_commit = sys.isCommit(send_component_id, send_target);

    if (is_send_commit) {
      manager->incFreeze(next_state);
      // next_state[manager->getFreezeLocation()]++;
    }
    int receive_target = sys.getSnk(receive_component_id, receive_link);

    next_state[receive_component_id] = receive_target;

    bool is_receive_commit = sys.isCommit(receive_component_id, receive_target);

    if (is_receive_commit) {
      manager->incFreeze(next_state);
      // next_state[manager->getFreezeLocation()]++;
    }

    int source, target;
    source = sys.getSrc(send_component_id, send_link);
    target = sys.getSnk(send_component_id, send_link);

    if (sys.isFreezeLocation(send_component_id, source)) {
      manager->decFreeze(next_state);
      // next_state[manager->getFreezeLocation()]--;
      assert(manager->getFreezeComponentNumber(next_state) >= 0);
    }
    if (sys.isFreezeLocation(send_component_id, target)) {
      manager->incFreeze(next_state);
      // next_state[manager->getFreezeLocation()]++;
      assert(manager->getFreezeComponentNumber(next_state) <= component_num);
    }

    source = sys.getSrc(receive_component_id, receive_link);
    target = sys.getSnk(receive_component_id, receive_link);

    if (sys.isFreezeLocation(receive_component_id, source)) {
      manager->decFreeze(next_state);
      // next_state[manager->getFreezeLocation()]--;
      assert(manager->getFreezeComponentNumber(next_state) >= 0);
    }

    if (sys.isFreezeLocation(receive_component_id, target)) {
      manager->incFreeze(next_state);
      // next_state[manager->getFreezeLocation()]++;
      assert(manager->getFreezeComponentNumber(next_state) <= component_num);
    }

    // can not stay on current_target and block_target locations when
    // send_target or receive_target is a commit or urgent location.

    return delay(data, send_component_id, send_target, next_state);
  }

  /**
   * TODO: urgent chan
   *
   */
  template <typename D>
  bool doSynchronize(D* data, int component, const State_t* const state,
                     int link, const Channel& channel) {
    vector<int> wait_components;
    bool is_send = true;
    State_t* counter_value =
        const_cast<State_t*>(manager->getCounterValue(state));
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
                          is_send);
      } else if (channel.getType() == URGENT_CH) {
        // TODO:URGENT_CH
        assert(false && "Deal with urgent channel.");
        return true;
      } else if (channel.getType() == BROADCAST_CH) {
        for (auto id : wait_components) {
          int block_component_id = wait_components[id];
          if (unBlockOne(data, component, block_component_id, link, state,
                         is_send)) {
            return true;
          }
        }
      }

    } else {
      manager->copy(cache_state, state);
      State_t* counter_value =
          const_cast<State_t*>(manager->getCounterValue(state));
      assert(channel.getGlobalId(counter_value) > 0);  // chan it starts with 1
      if (channel.isSend()) {
        cache_state[component + component_num] =
            channel.getGlobalId(counter_value);  // send part
      } else if (channel.isRecive()) {
        cache_state[component + component_num] =
            -channel.getGlobalId(counter_value);  // receive part
      }
      // TODO: add commit property
      cache_state[component] = link;  // block link

      // if (manager->isCommitComp(component, state)) {
      //   manager->setCommitState(component,
      //                           cache_state); // save  commit property
      // }

      data->add(cache_state);
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
  bool oneTranision(D* data, const int component, const int link,
                    const State_t* const state) {
    manager->copy(next_state, state);
    int source = sys.getSrc(component, link);
    int target = sys.getSnk(component, link);

    if (sys.isFreezeLocation(component, source)) {
      manager->decFreeze(next_state);
      // next_state[manager->getFreezeLocation()]--;

      assert(manager->getFreezeComponentNumber(next_state) >= 0);
    }
    if (sys.isFreezeLocation(component, target)) {
      manager->incFreeze(next_state);
      // next_state[manager->getFreezeLocation()]++;
      assert(manager->getFreezeComponentNumber(next_state) <= component_num);
    }
    manager->discretRun(
        component, link,
        next_state);  // update counter state and reset clock state

    return delay(data, component, target, next_state);
  }

  template <typename D>
  bool delay(D* data, const int component, const int target, State_t* state) {
    if (!manager->isReachable(component, target, state)) {  // undefine state
      // TODO: undefine state
      return false;
    }
    state[component] = target;

    /**
     * First check there is no match  urgent channel
     Whether there is some component in freeze location
     */
    // TODO: Urgment chann
    if (!manager->hasMatchOutUrgentChan(state) && !manager->isFreeze(state)) {
      // if (!manager->isFreeze(state)) {
      manager->evolution(component, target, state);
    }

    return postDelay(data, component, target, state);
  }

  template <typename D>
  bool postDelay(D* data, const int component, const int target,
                 State_t* state) {
    // bool is_commit = sys.isCommit(component, target);

    bool re_bool = false;
    if (!manager->isFreeze(state)) {
      for (int component_id = 0; component_id < component_num; component_id++) {
        manager->employLocInvariants(component_id, state);
      }
    }
    if (manager->hasDiffCons()) {
      vector<int*> next_dbms;
      manager->norm(manager->getDBM(state), next_dbms);

      for (auto dbm : next_dbms) {
        manager->constructState(state, dbm, cache_state);
        if (data->add(cache_state) == TRUE) {
          re_bool = true;
          break;
        }
      }
      for (auto dbm : next_dbms) {
        manager->getClockManager().destroyDBM(dbm);
      }
    } else {
      manager->norm(manager->getDBM(state));

      if (data->add(state) == TRUE) {
        return true;
      }
    }
    return re_bool;
  }

  const SYS& sys;
  TANextStep nextS;

  shared_ptr<const typename SYS::StateManager_t> manager;
  int component_num;

  State_t* cache_state;
  State_t* next_state;
  std::default_random_engine generator;
};
}  // namespace graphsat

#endif
