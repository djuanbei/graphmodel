/**
 * @file   templatemodel.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec  6 19:58:40 2019
 *
 * @brief  template model
 *
 *
 */

#ifndef TEMPLATE_MODEL_H
#define TEMPLATE_MODEL_H

#include "graph/graph.hpp"
#include "graphmodel.hpp"
#include "vardecl.h"

#include "location.h"
#include "transition.h"

namespace graphsat {
using namespace raptor;

/**
 *
 *  TA has one-component
 *
 */

template <typename L, typename T> class Agent;

// template <typename L, typename T> class AgentSystem;

template <typename L, typename T> class AgentTemplate : public VarDecl {

private:
  typedef ClockConstraint CS_t;

  typedef Agent<L, T> Agent_t;

public:
  virtual ~AgentTemplate() {}
  void initial(vector<L> &locs, vector<T> &es, int init) {
    template_locations = locs;

    template_transitions = es;

    initial_loc = init;
    initial();
  }

  void findRhs(const int link, const int lhs, int &rhs) const {
    graph.findRhs(link, lhs, rhs);
  }

  int getClockNumber() const { return data.getTypeNum(CLOCK_T); }

  int getChannelNumber() const { return data.getTypeNum(CHAN_T); }

  void setInitialLoc(int loc) { initial_loc = loc; }

  int getInitialLoc() const { return initial_loc; }

  int addPara(const string &p) {
    int re = parameters.size();
    parameters.push_back(p);
    return re;
  }
  Parameter getParameter() const {
    Parameter re(parameters);
    return re;
  }

  int getParaId(const string &p) const {
    for (size_t i = 0; i < parameters.size(); i++) {
      if (p == parameters[i]) {
        return i;
      }
    }
    return NOT_FOUND;
  }
  void reset() { agents.clear(); }
  virtual Argument addClock(const string &n) {
    Argument dummy = VarDecl::addClock(n);
    dummy.type = NORMAL_VAR_ARG;
    return dummy;
  }

private:
  AgentTemplate(const string &n, const VarDecl *s) : name(n), sys(s) {
    initial_loc = 0;
  }
  AgentTemplate(const AgentTemplate &other) : sys(nullptr) { assert(false); }

  AgentTemplate &operator=(const AgentTemplate &other) {
    assert(false);
    return *this;
  }

  int getStart(const TYPE_T type) const { return sys->getStartLoc(type, id); }

  int getParentKeyID(const TYPE_T type, const string &key) const {
    return sys->getLocalKeyID(type, key) + sys->getTypeStart(type);
  }
  shared_ptr<Function> getSYSFun(const string &name) const {
    return sys->getFun(name);
  }

  void initial() {

    vector<int> srcs;
    vector<int> snks;

    for (auto t : template_transitions) {
      srcs.push_back(t.getSource());
      snks.push_back(t.getTarget());
    }

    graph.initial(srcs, snks);

    int vertex_num = graph.getVertex_num();

    // // There are no edges connect with  initial location
    assert(initial_loc >= 0 && initial_loc < vertex_num);
  }
  string name;
  const VarDecl *sys;
  int id;
  int number_children;

  vector<L> template_locations;
  vector<T> template_transitions;
  int initial_loc;

  Graph_t<int> graph;

  vector<string> parameters;
  vector<Agent<L, T> *> agents;
  map<string, shared_ptr<Function>> fun_map;

  template <typename R1> friend class Reachability;

  friend class Agent<L, T>;
  template <typename L1, typename T1> friend class AgentSystem;
};

template <> class AgentTemplate<Location, Transition> : public VarDecl {

private:
  typedef ClockConstraint CS_t;

  typedef Agent<Location, Transition> Agent_t;

public:
  virtual ~AgentTemplate() {}
  void initial(vector<Location> &locs, vector<Transition> &es, int init) {
    template_locations = locs;

    template_transitions = es;

    initial_loc = init;
    initial();
  }

  void findRhs(const int link, const int lhs, int &rhs) const {
    graph.findRhs(link, lhs, rhs);
  }

  int getClockNumber() const { return data.getTypeNum(CLOCK_T); }

  int getChannelNumber() const { return data.getTypeNum(CHAN_T); }

  void setInitialLoc(int loc) { initial_loc = loc; }

  int getInitialLoc() const { return initial_loc; }

  int addPara(const string &p) {
    int re = parameters.size();
    parameters.push_back(p);
    return re;
  }
  Parameter getParameter() const {
    Parameter re(parameters);
    return re;
  }

  int getParaId(const string &p) const {
    for (size_t i = 0; i < parameters.size(); i++) {
      if (p == parameters[i]) {
        return i;
      }
    }
    return NOT_FOUND;
  }
  void reset() { agents.clear(); }
  virtual Argument addClock(const string &n) {
    Argument dummy = VarDecl::addClock(n);
    dummy.type = NORMAL_VAR_ARG;
    return dummy;
  }

private:
  AgentTemplate(const string &n, const VarDecl *s) : name(n), sys(s) {
    initial_loc = 0;
  }
  AgentTemplate(const AgentTemplate &other) : sys(nullptr) { assert(false); }

  AgentTemplate &operator=(const AgentTemplate &other) {
    assert(false);
    return *this;
  }

  int getStart(const TYPE_T type) const { return sys->getStartLoc(type, id); }

  int getParentKeyID(const TYPE_T type, const string &key) const {
    return sys->getLocalKeyID(type, key) + sys->getTypeStart(type);
  }
  shared_ptr<Function> getSYSFun(const string &name) const {
    return sys->getFun(name);
  }

  void initial() {

    vector<int> srcs;
    vector<int> snks;

    for (auto t : template_transitions) {
      srcs.push_back(t.getSource());
      snks.push_back(t.getTarget());
    }

    graph.initial(srcs, snks);

    int vertex_num = graph.getVertex_num();

    // // There are no edges connect with  initial location
    assert(initial_loc >= 0 && initial_loc < vertex_num);
  }
  string name;

  const VarDecl *sys;
  int id;

  int number_children;

  vector<Location> template_locations;
  vector<Transition> template_transitions;
  int initial_loc;

  Graph_t<int> graph;

  vector<string> parameters;
  vector<Agent<Location, Transition> *> agents;
  map<string, shared_ptr<Function>> fun_map;

  template <typename R1> friend class Reachability;

  friend class Agent<Location, Transition>;
  template <typename L1, typename T1> friend class AgentSystem;
};

} // namespace graphsat

#endif
