/**
 * @file   function.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Dec 12 21:06:18 2019
 *
 * @brief  The function data structure in model
 *
 *
 */
#ifndef __FUNCTION_H
#define __FUNCTION_H

#include <map>
#include <string>
#include <vector>

namespace graphsat {

// using namespace std;

class Function {
 private:
  std::map<std::string, int> value_map;

 public:
  virtual ~Function() {}
  virtual int operator()(int* state...) = 0;

  // virtual int operator()(const int *state...) { return 0; }

  virtual std::vector<std::string> getKeys() const {
    std::vector<std::string> re;
    for (auto& e : value_map) {
      re.push_back(e.first);
    }
    return re;
  }
  virtual int& operator[](const std::string& key) { return value_map[key]; }
  virtual Function* copy() const = 0;
};

}  // namespace graphsat

#endif
