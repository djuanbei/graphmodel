/**
 * @file   fisherprop.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Dec  5 09:50:53 2019
 *
 * @brief  fisher property
 *
 *
 */

#include "property/property.h"
namespace graphsat {

class FischerMutual : public Property {

public:
  virtual bool operator()(const void *manager, const int *const state) const {

    int component_num = ((ComponentInfo *)manager)->getComponentNum();
    int cs_num = 0;
    for (int i = 0; i < component_num; i++) {
      if (state[i] == 3) {
        cs_num++;
      }
    }
    return cs_num >= 2;
  }
};

} // namespace graphsat
