//
// Created by yun on 2021/2/27.
//

#include <sim/data/trace.h>

#include <string>

namespace graphmodel {
namespace sim {
std::string Point::toString() {
  string re = "";
  return re;
}

void Trace::nextLoc(time_t time, float lon, float lat) {
  float diff = difftime(time, currentTime);
  longMoveRateBySec = 0.0;
  latMoveRateBySec = 0.0;
  if (fabs(diff) > 1e-6) {
    longMoveRateBySec = (lon - longitude) / diff;
    latMoveRateBySec = (lat - latitude) / diff;
  }
}

void Trace::save(time_t time, string loc, string content) {
  std::pair<float, float> nloc = travelLoc(time);

  currentTime = time;
  longitude = nloc.first;
  longitude = nloc.second;

  Point p = Point(time, nloc, loc, content);
  traces.push_back(p);
}
}  // namespace sim
}  // namespace graphmodel
