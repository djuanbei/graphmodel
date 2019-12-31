/**
 * @file   TA.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:25:54 2019
 *
 * @brief  a model for timed automata. Corresponding support API to obtain
 * static information of system
 *
 *
 */

#ifndef __GRAPH_MODEL_
#define __GRAPH_MODEL_
#include <functional>
#include <vector>

#include "action/counteraction.h"
#include "channel.h"
#include "counter.h"

#include "domain/dbmset.hpp"

#include "parameter.h"
#include "state/discretestate.hpp"

#include "templatemodel.hpp"

#include "agentmodel.hpp"

#include "vardecl.h"

namespace graphsat {

using std::vector;

template <typename L, typename T>
class AgentSystem : public VarDecl {
 public:
  typedef typename T::State_t State_t;

  typedef typename T::StateManager_t StateManager_t;

  typedef StateSet<State_t> StateSet_t;
  typedef ClockConstraint CS_t;

  typedef L L_t;

  typedef T T_t;

  typedef Agent<L, T> Agent_t;
  typedef AgentTemplate<L, T> AgentTemplate_t;

  AgentSystem() {
    clock_max_value[0] = 0;
    counter_num = chan_num = 0;
    hasUrgentChan = hasBroadcaseChan = false;
  }
  virtual ~AgentSystem() {}

  shared_ptr<AgentTemplate_t> createTemplate(const string& n) {
    shared_ptr<AgentTemplate_t> re(new AgentTemplate_t(n, this));
    re->setParent(this);
    re->id = templates.size();
    templates.push_back(re);
    return re;
  }
  shared_ptr<Agent_t> createAgent(
      const shared_ptr<AgentTemplate_t>& template_arg, const Parameter& param) {
    shared_ptr<Agent_t> re(new Agent_t(template_arg, param));
    agents.push_back(re);
    manager=nullptr;//manager need update 
    return re;
  }

  void removeAgent() {
    agents.clear();
    for (auto& e : templates) {
      e->reset();
    }
    manager = nullptr;
  }

  int getComponentNumber() const { return (int)agents.size(); }

  int getInitialLoc(const int id) const { return agents[id]->getInitialLoc(); }

  bool hasChannel() const { return getTotalChanNumber() > 0; }
  bool hasChannel(const int component, const int link) const {
    return agents[component]->transitions[link].hasChannel();
  }

  bool isCommit(const int component, const int loc) const {
    return agents[component]->locations[loc].isCommit();
  }

  bool isFreezeLocation(const int component, const int loc) const {
    return agents[component]->locations[loc].isFreezeLocation();
  }

  const Channel& getChannel(const int component, const int link) const {
    return agents[component]->transitions[link].getChannel();
  }
  int getCompentId(const string& component_name) const {
    for (size_t i = 0; i < agents.size(); i++) {
      if (agents[i]->getName() == component_name) {
        return i;
      }
    }
    return NOT_FOUND;
  }

  TYPE_T getType(const int component, const string& name) const {
    TYPE_T re = agents[component]->getType(name);
    if (re != NO_T) {
      return re;
    }
    return VarDecl::getType(name);
  }
  TYPE_T getType(const string& component_name, const string& name) const {
    int component_id = getCompentId(component_name);
    return getType(component_id, name);
  }

  int getTotalChanNumber() const { return chan_num; }

  int getComponentCounterNumber(const int id) const {
    return agents[id]->getTemplate()->getCounterNumber();
  }

  int getComponentClockNumber(const int comp_id) const {
    return agents[comp_id]->getTemplate()->getClockNumber();
  }
  
  int getComponentClockStartID(const int id) const{
    int re = 1;
    for (int i = 0; i < id; i++) {
      re += getComponentClockNumber(i);
    }
    return re;    
  }
  
  int getKeyID( const int component,  const TYPE_T type,  const string & key) const{
    return agents[ component]->getKeyID( type, key);
  }
      
  bool hasUrgentCh(const int component, const int loc) const {
    return agents[component]->locations[loc].hasOutUrgentCh();
  }
  bool hasBroadcaseSendCh(const int component, const int loc) const {
    return agents[component]->locations[loc].hasOutBreakcastSendCh();
  }
  bool hasNormalCh(const int component, const int loc) const {
    return agents[component]->locations[loc].hasNormalCh();
  }

  bool hasUrgentCh() const { return hasUrgentChan; }

  bool hasBroadcaseCh() const { return hasBroadcaseChan; }

  const vector<ClockConstraint>& getDiffCons() const { return difference_cons; }

  shared_ptr<StateManager_t> getStateManager() {
    if (nullptr == manager) {
      build();
    }
    return manager;
  }

   vector<BaseDecl> getAllVar(const TYPE_T type) const {
    vector<BaseDecl> re = VarDecl::getAllVar(type);
    int id_start = 0;
    for (auto& e : re) {
      e.start_loc = id_start;
      id_start += e.num;
    }
    for (auto& e : agents) {
      vector<BaseDecl> dummy = e->agent_tempate->getAllVar(type);
      for (auto& ee : dummy) {
        ee.name = e->getName() + "." + ee.name;
        ee.start_loc = id_start;
        id_start += ee.num;
        re.push_back(ee);
      }
    }
    return re;
  }


   int getTypeStart(const TYPE_T type) const {
    // clock and channel id start with 1
    if (CLOCK_T == type || CHAN_T == type) {
      return 1;
    }
    return 0;
  }

   int getStartLoc(const TYPE_T type, const int template_id) const {
    int re = getTypeStart(type)+getTypeNumber( type);
    
    for (auto& agent : agents) {
      if (agent->getTemplate()->id < template_id) {
        re += agent->getTemplate()->getTypeNumber(type);
      }
    }
    return re;
  }

   Argument addClock(const string& n) {
    Argument dummy = VarDecl::addClock(n);
    dummy.type = NORMAL_VAR_ARG;
    return dummy;
  }

  int getSrc(const int component, const int link) const {
    int re = 0;
    agents[component]->graph.findSrc(link, re);
    return re;
  }

  int getSnk(const int component, const int link) const {
    int re = 0;
    agents[component]->graph.findSnk(link, re);
    return re;
  }

  string getLocationName(const int component, const int loc_ID) const {
    return agents[component]->getLocationName(loc_ID);
  }
  int getLocationNumber(const int component) const {
    return agents[component]->locations.size();
  }

  int getTransitionNumber(const int component) const {
    return agents[component]->transitions.size();
  }

  vector<int> getOutTransition(const int component, const int src) const {
    return agents[component]->graph.getAdj(src);
  }

  const Channel& getChan(const int component, const int link) const {
    return agents[component]->transitions[link].getChannel();
  }
  
  void buildManager(){
    int clock_num = 0;
    for (auto& e : clock_max_value) {
      if (e.first > clock_num) {
        clock_num = e.first;
      }
    }
    
    vector<int> temp_clock_upperbound(2 * clock_num + 2, 0);
    
    for (int i = 0; i < clock_num + 1; i++) {
      temp_clock_upperbound[i] = getMatrixValue(clock_max_value[i], false);
    }
    
    for (int i = 0; i < clock_num + 1; i++) {
      temp_clock_upperbound[i + clock_num + 1] =
      getMatrixValue(-clock_max_value[i], true);
    }
    vector<int> node_n;
    for (size_t i = 0; i < agents.size(); i++) {
      node_n.push_back(agents[i]->graph.getVertex_num());
    }
    vector<int> link_num;
    for (auto& e : agents) {
      link_num.push_back(e->graph.getLink_num());
    }
    vector<Counter> counters;
    vector<BaseDecl> sysCounts = getInts();
    for (auto& e : sysCounts) {
      Counter counter(e.low, e.high);
      counters.push_back(counter);
    }
    
    for (auto& e : agents) {
      vector<BaseDecl> counts = e->agent_tempate->getInts();
      for (auto& ee : counts) {
        Counter counter(ee.low, ee.high);
        counters.push_back(counter);
      }
    }
    
    manager.reset(new StateManager_t(*this, counters, clock_num,
                                     temp_clock_upperbound, node_n, link_num));
  }
  
  
  

 private:
  void transfrom(Agent_t* agent) {
    agent->initFuns();
    agent->locations = agent->agent_tempate->template_locations;
    for (auto& e : agent->locations) {
      e.to_real(agent);
    }

    agent->transitions.clear();
    for (size_t i = 0; i < agent->agent_tempate->template_transitions.size();
         i++) {
      if (agent->agent_tempate->template_transitions[i].isSelect()) {
        T dummy(agent->agent_tempate->template_transitions[i]);
        TypeDefArray select_domain =
            agent->getTypeDef(dummy.getSelectCollect());
        for (int i = select_domain.getLow(); i <= select_domain.getHigh();
             i++) {
          agent->setSelect(i);
          dummy.to_real(agent);
          agent->transitions.push_back(dummy);
        }
      } else {
        T dummy(agent->agent_tempate->template_transitions[i]);
        dummy.to_real(agent);
        agent->transitions.push_back(dummy);
      }
    }

    agent->initial();

    if (agent->hasUrgentChan) {
      hasUrgentChan = true;
    }
    if (agent->hasBroadcaseChan) {
      hasBroadcaseChan = true;
    }

    chan_num += agent->getChannelNumber();

    initial_loc.push_back(agent->getInitialLoc());
    map<int, int> temp_max = agent->getClockMaxValue();
    for (auto& e : temp_max) {
      if (clock_max_value[e.first] < e.second) {
        clock_max_value[e.first] = e.second;
      }
    }

    difference_cons.insert(difference_cons.end(),
                           agent->difference_cons.begin(),
                           agent->difference_cons.end());
  }

  struct AgentCMP {
    bool operator()(const shared_ptr<Agent_t>& lhs,
                    const shared_ptr<Agent_t>& rhs) const {
      if (lhs->agent_tempate->id < rhs->agent_tempate->id) {
        return true;
      }
      if (lhs->agent_tempate->id > rhs->agent_tempate->id) {
        return false;
      }
      return (lhs->id < rhs->id);
    }
  };
  
  void build() {
    AgentCMP cmp;
    sort(agents.begin(), agents.end(), cmp);
    counter_num = getTypeNumber(INT_T);
    chan_num = getTypeNumber(CHAN_T);
    difference_cons.clear();
    
    for (auto& e : agents) {
      transfrom(e.get());
    }
    
    buildManager();
  }

 

  /**
   * multi-components
   *
   */
  vector<shared_ptr<AgentTemplate_t>> templates;

  vector<shared_ptr<Agent_t>> agents;

  int counter_num;
  int chan_num;

  vector<int> initial_loc;

  map<int, int> clock_max_value;
  vector<ClockConstraint> difference_cons;

  shared_ptr<StateManager_t> manager;

  bool hasUrgentChan;
  bool hasBroadcaseChan;
  friend class TMStateManager;
  // template <typename TT> friend class Reachability;
};

// template <> class AgentSystem<Location, Transition> : public VarDecl {

// public:
//   typedef typename Transition::State_t State_t;

//   typedef typename Transition::StateManager_t StateManager_t;

//   typedef StateSet<State_t> StateSet_t;
//   typedef ClockConstraint CS_t;

//   typedef Location L_t;

//   typedef Transition T_t;

//   typedef Agent<Location, Transition> Agent_t;
//   typedef AgentTemplate<Location, Transition> AgentTemplate_t;

//   AgentSystem() {
//     clock_max_value[0] = 0;
//     counter_num = chan_num = 0;
//     hasUrgentChan = hasBroadcaseChan = false;
//   }
//   virtual ~AgentSystem() {}

//   shared_ptr<AgentTemplate_t> createTemplate(const string &n) {
//     shared_ptr<AgentTemplate_t> re(new AgentTemplate_t(n, this));
//     re->setParent(this);
//     re->id = templates.size();
//     templates.push_back(re);
//     return re;
//   }
//   shared_ptr<Agent_t>
//   createAgent(const shared_ptr<AgentTemplate_t> &template_arg,
//               const Parameter &param) {
//     shared_ptr<Agent_t> re(new Agent_t(template_arg, param));
//     agents.push_back(re);
//     return re;
//   }

//   void removeAgent() {
//     agents.clear();
//     for (auto &e : templates) {
//       e->reset();
//     }
//   }

//   int getComponentNumber() const { return (int)agents.size(); }

//   bool hasChannel(const int component, const int link) const {
//     return agents[component]->transitions[link].hasChannel();
//   }
//   bool isCommit(const int component, const int loc) const {
//     return agents[component]->locations[loc].isCommit();
//   }
//   bool isFreezeLocation(const int component, const int loc) const {
//     return agents[component]->locations[loc].isFreezeLocation();
//   }
//   const Channel &getChannel(const int component, const int link) const {
//     return agents[component]->transitions[link].getChannel();
//   }

//   int getChanNum() const { return chan_num; }

//   bool hasUrgentCh(const int component, const int loc) const {
//     return agents[component]->locations[loc].hasOutUrgentCh();
//   }
//   bool hasBroadcaseSendCh(const int component, const int loc) const {
//     return agents[component]->locations[loc].hasOutBreakcastSendCh();
//   }

//   bool hasUrgentCh() const { return hasUrgentChan; }
//   vector<int> getChanLinks(const int component, const int source,
//                            const int chid, int *counter_value) const {
//     vector<int> re;
//     vector<int> outs = agents[component]->graph.getAdj(source);
//     if (chid > 0) {
//       for (auto link : outs) {
//         if (agents[component]->transitions[link].hasChannel() &&
//             agents[component]->transitions[link].getChannel().isSend() &&
//             agents[component]->transitions[link].getChannel().getGlobalId(
//                 counter_value) == chid) {
//           re.push_back(link);
//         }
//       }
//     } else {
//       for (auto link : outs) {
//         if (agents[component]->transitions[link].hasChannel() &&
//             agents[component]->transitions[link].getChannel().isRecive() &&
//             agents[component]->transitions[link].getChannel().getGlobalId(
//                 counter_value) == -chid) {
//           re.push_back(link);
//         }
//       }
//     }

//     return re;
//   }

//   bool hasBroadcaseCh() const { return hasBroadcaseChan; }

//   const vector<ClockConstraint> &getDiffCons() const { return
//   difference_cons; }

//   shared_ptr<StateManager_t> getStateManager() const { return stateManager; }

//   struct AgentCMP {
//     bool operator()(const shared_ptr<Agent_t> &lhs,
//                     const shared_ptr<Agent_t> &rhs) const {
//       if (lhs->agent_tempate->id < rhs->agent_tempate->id) {
//         return true;
//       }
//       if (lhs->agent_tempate->id > rhs->agent_tempate->id) {
//         return false;
//       }
//       return (lhs->id < rhs->id);
//     }
//   };

//   void build() {
//     AgentCMP cmp;
//     sort(agents.begin(), agents.end(), cmp);
//     counter_num = getTypeNumber(INT_T);
//     chan_num = getTypeNumber(CHAN_T);
//     difference_cons.clear();

//     for (auto &e : agents) {
//       transfrom(e);
//     }
//     int clock_num = 1;
//     for (auto &e : clock_max_value) {
//       if (e.first > clock_num) {
//         clock_num = e.first;
//       }
//     }

//     vector<int> temp_clock_upperbound(2 * clock_num + 2, 0);

//     for (int i = 0; i < clock_num + 1; i++) {
//       temp_clock_upperbound[i] = getMatrixValue(clock_max_value[i], false);
//     }

//     for (int i = 0; i < clock_num + 1; i++) {
//       temp_clock_upperbound[i + clock_num + 1] =
//           getMatrixValue(-clock_max_value[i], true);
//     }
//     vector<int> node_n;
//     for (size_t i = 0; i < agents.size(); i++) {
//       node_n.push_back(agents[i]->graph.getVertex_num());
//     }
//     vector<int> link_num;
//     for (auto &e : agents) {
//       link_num.push_back(e->graph.getLink_num());
//     }
//     vector<Counter> counters;
//     vector<BaseDecl> sysCounts = getInts();
//     for (auto &e : sysCounts) {
//       Counter counter(e.low, e.high);
//       counters.push_back(counter);
//     }

//     for (auto &e : agents) {
//       vector<BaseDecl> counts = e->agent_tempate->getInts();
//       for (auto &ee : counts) {
//         Counter counter(ee.low, ee.high);
//         counters.push_back(counter);
//       }
//     }

//     stateManager.reset(new StateManager_t(
//         *this, counters, clock_num, temp_clock_upperbound, node_n,
//         link_num));
//   }

//   template <typename D> void addInitState(D &data) const {
//     State_t *state = stateManager->newState();
//     int component_num = (int)agents.size();
//     bool withoutCommit = true;
//     for (int component = 0; component < component_num; component++) {
//       state[component] = initial_loc[component];
//       if (agents[component]->isCommit(state[component])) {
//         stateManager->setCommitState(component, state);
//         withoutCommit = false;
//       }
//     }
//     if (withoutCommit) {
//       for (int component = 0; component < component_num; component++) {
//         agents[component]->locationRun(initial_loc[component],
//                                        stateManager->getClockManager(),
//                                        stateManager->getDBM(state));
//       }
//     }

//     for (int component = 0; component < component_num; component++) {
//       agents[component]
//           ->locations[stateManager->getLocationID(component, state)]
//           .employInvariants(stateManager->getClockManager(),
//                             stateManager->getDBM(state));
//     }
//     if (stateManager->getClockManager().isConsistent(
//             stateManager->getDBM(state))) {
//       stateManager->norm(stateManager->getDBM(state));
//       data.add(state);
//     }
//     stateManager->destroyState(state);
//   }

//   virtual vector<BaseDecl> getAllVar(const TYPE_T type) const {
//     vector<BaseDecl> re = VarDecl::getAllVar(type);
//     int id_start = 0;
//     for (auto &e : re) {
//       e.start_loc = id_start;
//       id_start += e.num;
//     }
//     for (auto &e : agents) {
//       vector<BaseDecl> dummy = e->agent_tempate->getAllVar(type);
//       for (auto &ee : dummy) {
//         ee.name=e->getName( )+"."+ee.name;
//         ee.start_loc = id_start;
//         id_start += ee.num;
//         re.push_back(ee);
//       }
//     }
//     return re;
//   }

//   virtual int getTypeStart(const TYPE_T type) const {
//     // clock and channel id start with 1
//     if (CLOCK_T == type || CHAN_T == type) {
//       return 1;
//     }
//     return 0;
//   }

//   virtual int getStartLoc(const TYPE_T type, const int template_id) const {
//     int re = getTypeStart(type);
//     for (auto &agent : agents) {
//       if (agent->getTemplate()->id < template_id) {
//         re += agent->getTemplate()->getTypeNumber(type);
//       }
//     }
//     return re;
//   }

//   virtual Argument addClock(const string &n) {
//     Argument dummy = VarDecl::addClock(n);
//     dummy.type = NORMAL_VAR_ARG;
//     return dummy;
//   }

//   int getSrc(const int component, const int link) const {
//     int re = 0;
//     agents[component]->graph.findSrc(link, re);
//     return re;
//   }

//   int getSnk(const int component, const int link) const {
//     int re = 0;
//     agents[component]->graph.findSnk(link, re);
//     return re;
//   }

//   string getLocationName(const int component, const int loc_ID) const {
//     return agents[component]->getLocationName(loc_ID);
//   }

//   vector<int> getOutTransition(const int component, const int src) const {
//     return agents[component]->graph.getAdj(src);
//   }

//   const Channel &getChan(const int component, const int link) const {
//     return agents[component]->transitions[link].getChannel();
//   }

// private:
//   void transfrom(shared_ptr<Agent_t> &agent) {

//     agent->initFuns();
//     agent->locations = agent->agent_tempate->template_locations;
//     for (auto &e : agent->locations) {
//       e.to_real(agent);
//     }

//     agent->transitions.clear();
//     for (size_t i = 0; i < agent->agent_tempate->template_transitions.size();
//          i++) {
//       if (agent->agent_tempate->template_transitions[i].isSelect()) {
//         Transition dummy(agent->agent_tempate->template_transitions[i]);
//         TypeDefArray select_domain =
//         agent->getType(dummy.getSelectCollect()); for (int i =
//         select_domain.getLow(); i <= select_domain.getHigh();
//              i++) {
//           agent->setSelect(i);
//           dummy.to_real(agent);
//           agent->transitions.push_back(dummy);
//         }
//       } else {
//         Transition dummy(agent->agent_tempate->template_transitions[i]);
//         dummy.to_real(agent);
//         agent->transitions.push_back(dummy);
//       }
//     }

//     agent->initial();

//     if (agent->hasUrgentChan) {
//       hasUrgentChan = true;
//     }
//     if (agent->hasBroadcaseChan) {
//       hasBroadcaseChan = true;
//     }

//     chan_num += agent->getChannelNumber();

//     initial_loc.push_back(agent->getInitialLoc());
//     map<int, int> temp_max = agent->getClockMaxValue();
//     for (auto &e : temp_max) {
//       if (clock_max_value[e.first] < e.second) {
//         clock_max_value[e.first] = e.second;
//       }
//     }

//     difference_cons.insert(difference_cons.end(),
//                            agent->difference_cons.begin(),
//                            agent->difference_cons.end());
//   }

//   /**
//    * multi-components
//    *
//    */
//   vector<shared_ptr<AgentTemplate_t>> templates;

//   vector<shared_ptr<Agent_t>> agents;

//   int counter_num;
//   int chan_num;

//   vector<int> initial_loc;

//   map<int, int> clock_max_value;
//   vector<ClockConstraint> difference_cons;

//   shared_ptr<StateManager_t> stateManager;

//   bool hasUrgentChan;
//   bool hasBroadcaseChan;
//   friend class TMStateManager;
// };

}  // namespace graphsat

#endif
