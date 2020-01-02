/**
 * @file   dbmutil.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Mar 29 10:54:12 2019
 *
 * @brief   dbm util class
 *
 *
 */

#ifndef DBM_UTIL_HPP
#define DBM_UTIL_HPP
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "typedef.h"

#include "macrodef.h"

namespace graphsat {
// using std::map;
// using std::string;
// using std::vector;

template <typename T>
bool executeOp(const T& lhs, COMP_OPERATOR op, const T& rhs) {
  switch (op) {
    case EQ:
      return lhs == rhs;
    case LE:
      return lhs <= rhs;
    case LT:
      return lhs < rhs;
    case GE:
      return lhs >= rhs;
    case GT:
      return lhs > rhs;
    case NE:
      return lhs != rhs;
    default:
      return false;
  }
}

// int_fast64_t getMapValue( const RealArgument & arg, const vector<int>
// &id_map);

std::string getOpStr(COMP_OPERATOR op);

COMP_OPERATOR negation(COMP_OPERATOR op);

std::string getTypeStr(TYPE_T type);

TYPE_T baseType(TYPE_T type);

int fromPidToChanId(int id);

int chanIdToFromPid(int id);

bool isRefChan(TYPE_T type);

TYPE_T get_formal_paramter_type(TYPE_T type);

bool isRefType(const TYPE_T type);

/**
 *  Both have compare < and <=
 *
 */
template <typename C>
inline C getMAX_INT() {
  return std::numeric_limits<C>::max() / 2 - 2;
}

/**
 * <  c then the MatrixValue is 2*c+1
 * <= c then the MatrixValue is 2*c
 *
 */

template <typename C>
inline C getMatrixValue(C realRight, bool isStrct = true) {
  C right = realRight * 2;
  if (!isStrct) {
    right = right | 1;
  }
  return right;
}

template <typename C>
inline bool isStrict(const C c) {
  return (c & 1) == 0;
}

template <typename C>
std::string getComp(const C x) {
  if (x & 1) {
    return "<=";
  } else {
    return "< ";
  }
}

template <typename C>
inline C getRight(const C c) {
  return c >> 1;
}

template <typename C>
inline C negMatrixValue(C matrix_value) {
  bool strict = isStrict(matrix_value);

  return getMatrixValue(-getRight(matrix_value), strict);
}

}  // namespace graphsat

#endif
