/**
 * @file   data.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu May 23 10:21:13 2019
 *
 * @brief
 *
 *
 */

#ifndef __DATA_H
#define __DATA_H
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include<cassert>

#include "macrodef.h"

#include "typedef.h"

namespace graphsat {
using std::map;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

inline static string arrayToVar(const string &name, int id) {
  stringstream ss;
  ss << id;
  return name + "#" + ss.str();
}

template <typename T> class ValueData {
public:
  void clear() { values.clear(); }
  void clear(const int type) { values[type].clear(); }
  void clear(const int type, const string &name) {
    for (size_t i = 0; i < values[type].size(); i++) {
      if (values[type][i].first == name) {
        values[type][i].second.clear();
      }
    }
  }
  void addValue(const int type, const string &name, T v = 0) {

    for (size_t i = 0; i < values[type].size(); i++) {
      if (values[type][i].first == name) {
        values[type][i].second.push_back(v);
        return;
      }
    }

    vector<T> vec_v;
    vec_v.push_back(v);
    values[type].push_back(make_pair(name, vec_v));
  }

  void setValue(const int type, const string &name, T v) {

    for (size_t i = 0; i < values[type].size(); i++) {
      if (values[type][i].first == name) {
        if (values[type][i].second.empty()) {
          values[type][i].second.push_back(v);
        } else {
          values[type][i].second[0] = v;
        }
        return;
      }
    }
    addValue(type, name, v);
  }

  int getTypeNum(const int type) const {
    if (values.find(type) == values.end()) {
      return 0;
    }
    return (int)values.at(type).size();
  }

  /**
   *
   *
   * @param type Element type which wants to find
   * @param name The name of element
   *
   * @return  @NOT_FOUND if name is not in values
   */
  int getId(const int type, const string &name) const {
    if (values.find(type) == values.end()) {
      return NOT_FOUND;
    }

    for (size_t i = 0; i < values.at(type).size(); i++) {
      if (values.at(type)[i].first == name) {
        return (int)i;
      }
    }
    return NOT_FOUND;
  }

  const pair<string, vector<T>> &getValue(const int type, int id) const {
    return values.at(type)[id];
  }

  vector<T> getValue(const int type, const string &name) const {
    if (values.find(type) == values.end()) {
      vector<T> dummy;
      return dummy;
    }

    for (size_t i = 0; i < values.at(type).size(); i++) {
      if (values.at(type)[i].first == name) {
        return values.at(type)[i].second;
      }
    }
    vector<T> dummy;
    return dummy;
  }

  vector<pair<string, vector<T>>> getValue(const int type) const {

    if (values.find(type) != values.end()) {
      return values.at(type);
    }

    return vector<pair<string, vector<T>>>();
  }

  bool hasValue(const int type, const string &name) const {
    if (values.find(type) == values.end()) {
      return false;
    }
    for (size_t i = 0; i < values.at(type).size(); i++) {
      if (values.at(type)[i].first == name) {
        return !(values.at(type)[i].second.empty());
      }
    }
    return false;
  }

  bool hasValue(const int type) const {
    return values.find(type) != values.end();
  }

  T getValue(const int type, const string &name, int id) const {
    if (values.find(type) == values.end()) {
      return (T)NOT_FOUND;
    }
    for (size_t i = 0; i < values.at(type).size(); i++) {
      if (values.at(type)[i].first == name) {
        return values.at(type)[i].second[id];
      }
    }
    assert(false);
    return (T)NOT_FOUND;
  }

private:
  map<int, vector<pair<string, vector<T>>>> values;
};

typedef ValueData<void *> PointerData;
} // namespace graphsat

#endif
