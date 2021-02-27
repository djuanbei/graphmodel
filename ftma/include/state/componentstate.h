/**
 * @file   componentstate.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Dec  5 09:44:18 2019
 *
 * @brief  provide compoent info
 *
 *
 */

#ifndef COMPONENT_STATE_H
#define COMPONENT_STATE_H
namespace graphmodel {

class ComponentInfo {
 public:
  virtual int getComponentNumber() const { return 0; }
};

}  // namespace graphmodel

#endif
