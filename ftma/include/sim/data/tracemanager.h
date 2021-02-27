//
// Created by yun on 2021/2/27.
//

#ifndef GMODEL_TRACEMANAGER_H
#define GMODEL_TRACEMANAGER_H
#include <map>
#include <vector>

#include "sim/data/trace.h"
namespace graphmodel {
namespace sim {
class TraceManager {
 private:
  std::map<string, std::vector<Trace>> data;

 public:
  TraceManager(string jsonListFileName);

  void readExcel(string jsonListFileName);

  void getDatas() { return; }
};
}  // namespace sim
}  // namespace graphmodel
#endif  // GMODEL_TRACEMANAGER_H
