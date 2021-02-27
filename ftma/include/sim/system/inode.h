//
// Created by yun on 2021/2/27.
//

#ifndef GMODEL_INODE_H
#define GMODEL_INODE_H
class INode {
 public:
  virtual run(IEvent event) const;
};
#endif  // GMODEL_INODE_H
