
#include "model/location.h"

namespace graphsat {
bool Location::operator()(const DBMFactory &dbm_manager, const int *const dbm,
                          vector<int *> &re_vec) const {
  int *newDBM = dbm_manager.createDBM(dbm);
  bool re = isReachable(dbm_manager, newDBM);
  if (!re) {
    dbm_manager.destroyDBM(newDBM);
    return false;
  }

  dbm_manager.norm(newDBM, re_vec);

  return (!re_vec.empty());
}

Location &Location::operator+=(const ClockConstraint &cs) {
  invariants.push_back(cs);
  return *this;
}
void Location::to_real(const TOReal *convertor) {
  for (auto &e : invariants) {
    e.to_real(convertor);
  }
}
} // namespace graphsat
