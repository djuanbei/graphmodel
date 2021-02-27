//
// Created by yun on 2021/2/27.
//

#ifndef GMODEL_TRACE_H
#define GMODEL_TRACE_H
#include <time.h>

#include <cmath>
#include <string>
#include <vector>

#include "sim/constant.h"

namespace graphmodel {
namespace sim {
using std::string;

struct Point {
  Point(time_t t, float lon, float lat, string loc, string cont) {
    time = t;
    longitude = lon;
    latitude = lat;
    location = loc;
    content = cont;
  }
  Point(time_t time, std::pair<float, float> p, string loc, string cont) {
    this->time = time;
    longitude = p.first;
    latitude = p.second;
    location = loc;
    content = cont;
  }
  string toString();

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

  void setLoc(time_t time, float lon, float lat) {
    currentTime = time;
    longitude = lon;
    latitude = lat;
  }
  void nextLoc(time_t time, float lon, float lat);

  long getId() const { return id; }
  float getLongitude() const { return longitude; }
  float getLatitude() const { return latitude; }

  void save(time_t time, string loc, string content);

  string toJson() const;
  void parseFromJson(string& jsonStr);

 private:
  std::pair<float, float> travelLoc(time_t time) {
    float diff = difftime(time, currentTime);

    return std::make_pair(longitude + longMoveRateBySec * diff,
                          latitude + latMoveRateBySec * diff);
  }
  long id;
  time_t currentTime;
  float longitude;
  float latitude;
  float longMoveRateBySec = 0;
  float latMoveRateBySec = 0;
  string version = DefaultVersion;
  bool isOccur = false;
  std::vector<Point> traces;
};
}  // namespace sim
}  // namespace graphmodel
#endif  // GMODEL_TRACE_H
