/**
 * @file   DBMset.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:58:00 2019
 *
 * @brief  A dbm matrix set class.
 *
 *
 */
#ifndef DBM_SET_H
#define DBM_SET_H
#include <algorithm>
#include <map>
#include <vector>

#include "domain/dbm.h"
#include "util/dbmutil.hpp"

namespace graphmodel {
// using namespace std;
template <typename C>
class DBMset {
 public:
  class iterator;

  class const_iterator;

  DBMset(DBMManager& manager) { dbm_manager = manager; }
  ~DBMset() { deleteAll(); }

  iterator begin() { return iterator(this); }

  const_iterator begin() const { return const_iterator(this); }

  iterator end() {
    return iterator(this, map_data.size() + recovery_data.size());
    /**/
  }

  const_iterator end() const {
    return const_iterator(this, map_data.size() + recovery_data.size());
    /**/
  }

  /**
   * @param DM  A dbm matrix
   *
   * @return true if real insert DBMFactory into set
   * false otherwise. DM does not destory in function
   */
  bool add(C* DM) {
    uint32_t hashValue = dbm_manager.getHashValue(DM);
    typename std::pair<typename std::map<uint32_t, int>::iterator, bool> ret;
    ret = passed_data.insert(
        std::pair<uint32_t, int>(hashValue, map_data.size()));
    DF_T featrue = dbm_manager.getIncludeFeature(DM);
    if (false == ret.second) {
      // hashValue has in passedD
      C* D1 = getD(hashValue);

      if (!dbm_manager.equal(DM, D1)) {
        bool have = false;
        for (typename std::vector<C*>::iterator it = recovery_data.begin();
             it != recovery_data.end(); it++) {
          if (dbm_manager.equal(DM, *it)) {
            have = true;
            break;
          }
        }
        if (have) {
          return false;
        }
        if (include(DM, featrue)) {
          return false;
        } else {
          recoveryDAdd(DM, featrue);
        }

      } else {
        return false;
      }
    } else {
      mapDAdd(DM, featrue);
    }

    return true;
  }
  bool contain(const C* const DM) const {
    DF_T featrue = dbm_manager.getIncludeFeature(DM);
    return include(DM, featrue);
  }

  size_t size() const { return passed_data.size() + recovery_data.size(); }

  void clear(void) {
    passed_data.clear();

    map_data.clear();
    map_data_feature.clear();

    recovery_data.clear();
    recovery_data_feature.clear();
  }

  void deleteAll(void) {
    for (typename std::vector<C*>::const_iterator it = map_data.begin();
         it != map_data.end(); it++) {
      delete[] * it;
    }

    for (typename std::vector<C*>::iterator it = recovery_data.begin();
         it != recovery_data.end(); it++) {
      delete[] * it;
    }
    clear();
  }

  void And(DBMset<C>& other) {
    for (typename std::vector<C*>::iterator it = other.map_data.begin();
         it != other.map_data.end(); it++) {
      add(dbm_manager, *it);
    }

    for (typename std::vector<C*>::iterator it = other.recovery_data.begin();
         it != other.recovery_data.end(); it++) {
      add(dbm_manager, *it);
    }
    other.clear();
  }

  class const_iterator {
   protected:
    const DBMset<C>* data;
    size_t index;

   public:
    const_iterator(const DBMset<C>* odata) : data(odata) { index = 0; }

    const_iterator(const DBMset<C>* odata, size_t oindex) : data(odata) {
      index = oindex;
    }

    const_iterator(const const_iterator& other) : data(other.data) {
      index = other.index;
    }

    const_iterator& operator++() {
      index++;
      return *this;
    }
    bool operator==(const const_iterator& other) const {
      return index == other.index;
    }
    bool operator!=(const const_iterator& other) const {
      return index != other.index;
    }

    const C* operator*() const {
      size_t dSize = data->mapD.size();
      if (index < dSize) {
        return data->mapD[index];
      }
      if (index >= dSize + data->recoveryD.size()) {
        return nullptr;
      }

      return data->recoveryD[index - dSize];
    }
  };

  class iterator {
   protected:
    const DBMset<C>* data;
    size_t index;

   public:
    iterator(DBMset<C>* odata) : data(odata) { index = 0; }

    iterator(DBMset<C>* odata, size_t oindex) : data(odata) { index = oindex; }

    iterator(const iterator& other) : data(other.data) { index = other.index; }
    iterator& operator++() {
      index++;
      return *this;
    }

    bool operator==(const iterator& other) const {
      return index == other.index;
    }
    bool operator!=(const iterator& other) const {
      return index != other.index;
    }

    C* operator*() {
      size_t dSize = data->mapD.size();
      if (index < dSize) {
        return data->mapD[index];
      }
      if (index >= dSize + data->recoveryD.size()) {
        return nullptr;
      }

      return data->recoveryD[index - dSize];
    }
  };

 private:
  DBMManager dbm_manager;
  std::map<uint32_t, int> passed_data;
  std::vector<C*> map_data;
  std::vector<DF_T> map_data_feature;

  std::vector<C*> recovery_data;
  std::vector<DF_T> recovery_data_feature;

  C* getD(uint32_t hash_value) { return map_data[passed_data[hash_value]]; }

  void mapDAdd(C* D, DF_T& value) {
    map_data.push_back(D);
    map_data_feature.push_back(value);
  }

  void recoveryDAdd(C* D, DF_T& value) {
    recovery_data.push_back(D);
    recovery_data_feature.push_back(value);
  }

  bool include(const C* const DM, DF_T& featrue) const {
    for (size_t i = 0; i < map_data.size(); i++) {
      if ((map_data_feature[i] >= featrue) &&
          dbm_manager.include(map_data[i], DM)) {
        return true;
      }
    }

    for (size_t i = 0; i < recovery_data.size(); i++) {
      if ((recovery_data_feature[i] >= featrue) &&
          dbm_manager.include(recovery_data[i], DM)) {
        return true;
      }
    }
    return false;
  }
};
}  // namespace graphmodel
#endif
