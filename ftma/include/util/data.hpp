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
#include <cassert>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "macrodef.h"
#include "typedef.h"

namespace graphmodel {

inline static std::string arrayToVar(const std::string& name, size_t id) {
  std::stringstream ss;
  ss << id;
  return name + "#" + ss.str();
}

template <typename T>
class ValueData {
 public:
  void clear() { values.clear(); }
  void clear(const size_t type) { values[type].clear(); }
  void clear(const size_t type, const std::string& name) {
    for (size_t i = 0; i < values[type].size(); i++) {
      if (values[type][i].first == name) {
        values[type][i].second.clear();
      }
    }
  }
  void addValue(const size_t type, const std::string& name, T v = 0) {
    for (size_t i = 0; i < values[type].size(); i++) {
      if (values[type][i].first == name) {
        values[type][i].second.push_back(v);
        return;
      }
    }

    std::vector<T> vec_v;
    vec_v.push_back(v);
    values[type].push_back(make_pair(name, vec_v));
  }

  void setValue(const size_t type, const std::string& name, T v) {
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

  int getTypeNum(const size_t type) const {
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
  int getId(const size_t type, const std::string& name) const {
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

  const std::pair<std::string, std::vector<T>>& getValue(const size_t type,
                                                         int id) const {
    return values.at(type)[id];
  }

  std::vector<T> getValue(const size_t type, const std::string& name) const {
    if (values.find(type) == values.end()) {
      std::vector<T> dummy;
      return dummy;
    }

    for (size_t i = 0; i < values.at(type).size(); i++) {
      if (values.at(type)[i].first == name) {
        return values.at(type)[i].second;
      }
    }
    std::vector<T> dummy;
    return dummy;
  }

  std::vector<std::pair<std::string, std::vector<T>>> getValue(
      const size_t type) const {
    if (values.find(type) != values.end()) {
      return values.at(type);
    }

    return std::vector<std::pair<std::string, std::vector<T>>>();
  }

  bool hasValue(const size_t type, const std::string& name) const {
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

  bool hasValue(const size_t type) const {
    return values.find(type) != values.end();
  }

  T getValue(const size_t type, const std::string& name, int id) const {
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

  /**
   * @param name
   *
   * @return  NOT_FOUND if can not find name.
   */
  int getType(const std::string& name) const {
    for (typename std::map<
             size_t, std::vector<std::pair<std::string, std::vector<T>>>>::
             const_iterator it = values.begin();
         it != values.end(); it++) {
      if (hasValue(it->first, name)) {
        return it->first;
      }
    }
    return NOT_FOUND;
  }

 private:
  std::map<size_t, std::vector<std::pair<std::string, std::vector<T>>>> values;
};

// template <> class ValueData<int> {
// public:
//   void clear() { values.clear(); }
//   void clear(const int type) { values[type].clear(); }
//   void clear(const int type, const string &name) {
//     for (size_t i = 0; i < values[type].size(); i++) {
//       if (values[type][i].first == name) {
//         values[type][i].second.clear();
//       }
//     }
//   }
//   void addValue(const int type, const string &name, int v = 0) {

//     for (size_t i = 0; i < values[type].size(); i++) {
//       if (values[type][i].first == name) {
//         values[type][i].second.push_back(v);
//         return;
//       }
//     }

//     vector<int> vec_v;
//     vec_v.push_back(v);
//     values[type].push_back(make_pair(name, vec_v));
//   }

//   void setValue(const int type, const string &name, int v) {

//     for (size_t i = 0; i < values[type].size(); i++) {
//       if (values[type][i].first == name) {
//         if (values[type][i].second.empty()) {
//           values[type][i].second.push_back(v);
//         } else {
//           values[type][i].second[0] = v;
//         }
//         return;
//       }
//     }
//     addValue(type, name, v);
//   }

//   int getTypeNum(const int type) const {
//     if (values.find(type) == values.end()) {
//       return 0;
//     }
//     return (int)values.at(type).size();
//   }

//   /**
//    *
//    *
//    * @param type Element type which wants to find
//    * @param name The name of element
//    *
//    * @return  @NOT_FOUND if name is not in values
//    */
//   int getId(const int type, const string &name) const {
//     if (values.find(type) == values.end()) {
//       return NOT_FOUND;
//     }

//     for (size_t i = 0; i < values.at(type).size(); i++) {
//       if (values.at(type)[i].first == name) {
//         return (int)i;
//       }
//     }
//     return NOT_FOUND;
//   }

//   const pair<string, vector<int>> &getValue(const int type, int id) const {
//     return values.at(type)[id];
//   }

//   vector<int> getValue(const int type, const string &name) const {
//     if (values.find(type) == values.end()) {
//       vector<int> dummy;
//       return dummy;
//     }

//     for (size_t i = 0; i < values.at(type).size(); i++) {
//       if (values.at(type)[i].first == name) {
//         return values.at(type)[i].second;
//       }
//     }
//     vector<int> dummy;
//     return dummy;
//   }

//   vector<pair<string, vector<int>>> getValue(const int type) const {

//     if (values.find(type) != values.end()) {
//       return values.at(type);
//     }

//     return vector<pair<string, vector<int>>>();
//   }

//   bool hasValue(const int type, const string &name) const {
//     if (values.find(type) == values.end()) {
//       return false;
//     }
//     for (size_t i = 0; i < values.at(type).size(); i++) {
//       if (values.at(type)[i].first == name) {
//         return !(values.at(type)[i].second.empty());
//       }
//     }
//     return false;
//   }

//   bool hasValue(const int type) const {
//     return values.find(type) != values.end();
//   }

//   int getValue(const int type, const string &name, int id) const {
//     if (values.find(type) == values.end()) {
//       return NOT_FOUND;
//     }
//     for (size_t i = 0; i < values.at(type).size(); i++) {
//       if (values.at(type)[i].first == name) {
//         return values.at(type)[i].second[id];
//       }
//     }
//     assert(false);
//     return NOT_FOUND;
//   }

// private:
//   map<int, vector<pair<string, vector<int>>>> values;
// };

// template <> class ValueData<void *> {
// public:
//   void clear() { values.clear(); }
//   void clear(const int type) { values[type].clear(); }
//   void clear(const int type, const string &name) {
//     for (size_t i = 0; i < values[type].size(); i++) {
//       if (values[type][i].first == name) {
//         values[type][i].second.clear();
//       }
//     }
//   }
//   void addValue(const int type, const string &name, void *v = 0) {

//     for (size_t i = 0; i < values[type].size(); i++) {
//       if (values[type][i].first == name) {
//         values[type][i].second.push_back(v);
//         return;
//       }
//     }

//     vector<void *> vec_v;
//     vec_v.push_back(v);
//     values[type].push_back(make_pair(name, vec_v));
//   }

//   void setValue(const int type, const string &name, void *v) {

//     for (size_t i = 0; i < values[type].size(); i++) {
//       if (values[type][i].first == name) {
//         if (values[type][i].second.empty()) {
//           values[type][i].second.push_back(v);
//         } else {
//           values[type][i].second[0] = v;
//         }
//         return;
//       }
//     }
//     addValue(type, name, v);
//   }

//   int getTypeNum(const int type) const {
//     if (values.find(type) == values.end()) {
//       return 0;
//     }
//     return (int)values.at(type).size();
//   }

//   /**
//    *
//    *
//    * @param type Element type which wants to find
//    * @param name The name of element
//    *
//    * @return  @NOT_FOUND if name is not in values
//    */
//   int getId(const int type, const string &name) const {
//     if (values.find(type) == values.end()) {
//       return NOT_FOUND;
//     }

//     for (size_t i = 0; i < values.at(type).size(); i++) {
//       if (values.at(type)[i].first == name) {
//         return (int)i;
//       }
//     }
//     return NOT_FOUND;
//   }

//   const pair<string, vector<void *>> &getValue(const int type, int id) const
//   {
//     return values.at(type)[id];
//   }

//   vector<void *> getValue(const int type, const string &name) const {
//     if (values.find(type) == values.end()) {
//       vector<void *> dummy;
//       return dummy;
//     }

//     for (size_t i = 0; i < values.at(type).size(); i++) {
//       if (values.at(type)[i].first == name) {
//         return values.at(type)[i].second;
//       }
//     }
//     vector<void *> dummy;
//     return dummy;
//   }

//   vector<pair<string, vector<void *>>> getValue(const int type) const {

//     if (values.find(type) != values.end()) {
//       return values.at(type);
//     }

//     return vector<pair<string, vector<void *>>>();
//   }

//   bool hasValue(const int type, const string &name) const {
//     if (values.find(type) == values.end()) {
//       return false;
//     }
//     for (size_t i = 0; i < values.at(type).size(); i++) {
//       if (values.at(type)[i].first == name) {
//         return !(values.at(type)[i].second.empty());
//       }
//     }
//     return false;
//   }

//   bool hasValue(const int type) const {
//     return values.find(type) != values.end();
//   }

//   void *getValue(const int type, const string &name, int id) const {
//     if (values.find(type) == values.end()) {
//       return (void *)NOT_FOUND;
//     }
//     for (size_t i = 0; i < values.at(type).size(); i++) {
//       if (values.at(type)[i].first == name) {
//         return values.at(type)[i].second[id];
//       }
//     }
//     assert(false);
//     return (void *)NOT_FOUND;
//   }

// private:
//   map<int, vector<pair<string, vector<void *>>>> values;
// };

typedef ValueData<void*> PointerData;

}  // namespace graphmodel

#endif
