//
// Created by yun on 2021/2/27.
//

#ifndef GMODEL_IENVIRONMENT_H
#define GMODEL_IENVIRONMENT_H

namespace graphmodel {
namespace sim {
class IEnvironment {
 public:
  virtual int getInt(string version, string key) = 0;
  virtual void setInt(String version, string key) = 0;
  virtual double getDouble(string version, string key) = ;
  virtual void setDouble(string version, string key) = 0;
  virtual vector<int> getVec(string version, string key) = 0;
  virtual void getVec(string version, string key);
};
}  // namespace sim
}  // namespace graphmodel
#endif  // GMODEL_IENVIRONMENT_H
