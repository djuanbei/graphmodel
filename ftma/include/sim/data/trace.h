//
// Created by yun on 2021/2/27.
//

#ifndef GMODEL_TRACE_H
#define GMODEL_TRACE_H
#include <time.h>

#include <string>

#include "sim/constant.h"

namespace Sim {
using std::string;

struct Point {
  Point(time_t t, float lon, float lat, string loc, string cont) {
    time = t;
    longitude = lon;
    latitude = lat;
    location = loc;
    content = cont;
  }
  time_t time;
  float longitude;
  float latitude;
  string location;
  string content;
};

class Trace {
 public:
  Trace() {}
  Trace(long id, float lon, float lat, float longSpeed, float latSpeed,
        string version) {
    this->id = id;

    longitude = lon;
    latitude = lat;
    latMoveRateBySec = longSpeed;
    latMoveRateBySec = latSpeed;
    this->version = version;
  }

  long getId() const { return id; }
  float getLongitude() const { return longitude; }
  float getLatitude() const { return latitude; }

  void save(time_t time, string name, string content) { currentTime = time; }

 private:
  long id;
  time_t currentTime;
  float longitude;
  float latitude;
  float longMoveRateBySec = 0;
  float latMoveRateBySec = 0;
  string version = DefaultVersion;
  bool isOccur = false;
};
}  // namespace Sim
#endif  // GMODEL_TRACE_H
