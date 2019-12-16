/**
 * @file   critical_section.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Dec 16 19:06:18 2019
 *
 * @brief  Some location is the critical section location. The same time only
 * allow at most one component enter it.
 *
 *
 */
#ifndef CRITICAL_SECTION_PROPERTY_H
#define CRITICAL_SECTION_PROPERTY_H
#include "property/property.h"
namespace graphsat {

class CriticalSection : public Property {
public:
  virtual bool operator()(const void *manager, const int *const state) const {
    int component_num = ((ComponentInfo *)manager)->getComponentNum();
    int cs_num = 0;
    for (int i = 0; i < component_num; i++) {
      if (isLookComponent(i) && (state[i] == CS)) {
        cs_num++;
      }
    }
    return cs_num >= 2;
  }
  void setCS(const int cs) { CS = cs; }

protected:
  virtual bool isLookComponent(const int component) const { return true; }

  int CS;
};
} // namespace graphsat

#endif
