
#include "model/location.h"

namespace graphsat {
bool Location::operator()(const DBMManager& dbm_manager, const int* const dbm,
                          vector<int*>& re_vec) const {
  int* newDBM = dbm_manager.createDBM(dbm);
  bool re = isReachable(dbm_manager, newDBM);
  if (!re) {
    dbm_manager.destroyDBM(newDBM);
    return false;
  }

  dbm_manager.norm(newDBM, re_vec);

  return (!re_vec.empty());
}

Location& Location::operator+=(const ClockConstraint& cs) {
  invariants.push_back(cs);
  return *this;
}
void Location::to_real(const TOReal* convertor) {
  for (auto& e : invariants) {
    e.to_real(convertor);
  }
}
// TODO: add name
std::ostream& Location::dump2Dot(std::ostream& out) const {
  out << location_id;
  switch (type) {
    case NORMOAL_LOC:
      out << " [ shape = circle, label=<";
      break;
    case INIT_LOC:
      out << " [ shape = doublecircle, label=<";
      break;
    case URGENT_LOC:
      out << " [ shape = octagon, label=<";
      break;
    case COMMIT_LOC:
      out << " [ shape = doubleoctagon, label=<";
      break;
  }

  out << "<table border=\"0\" >" << std::endl;
  out << "<tr><td>" << name << "</td></tr>" << std::endl;
  for (auto cs : invariants) {
    cs.dump2Dot(out);
  }
  out << "</table>";

  out << ">];" << std::endl;
  return out;
}

}  // namespace graphsat
