
#include "log/logset.h"

namespace graphsat {
void logSet() {
//  FLAGS_alsologtostderr = 1;
//  google::SetStderrLogging(
//      google::
//          GLOG_INFO ); // 设置glog的输出级别，这里的含义是输出INFO级别以上的信息
  // 设置INFO级别以上的信息log文件的路径和前缀名
  //  google::SetLogDestination(google::GLOG_INFO, "~/log/INFO_");

  // 设置WARNING级别以上的信息log文件的路径和前缀名
  // google::SetLogDestination(google::GLOG_WARNING, "~/log/WARNING_");

  // 设置ERROR级别以上的信息log文件的路径和前缀名
  // google::SetLogDestination(google::GLOG_ERROR, "~/log/ERROR_");
}
} // namespace graphsat
