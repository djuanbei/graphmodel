#include "util/dbmutil.hpp"

#include <cassert>

#include "model/function.h"
namespace graphmodel {
extern std::string trim(std::string s);

std::string getOpStr(COMP_OPERATOR op) {
  switch (op) {
    ENUM_ITEM_STR(EQ);
    ENUM_ITEM_STR(LE);
    ENUM_ITEM_STR(GE);
    ENUM_ITEM_STR(LT);
    ENUM_ITEM_STR(GT);
    ENUM_ITEM_STR(NE);
  }
}

COMP_OPERATOR negation(COMP_OPERATOR op) {
  switch (op) {
    case EQ: {
      return NE;
    }
    case LE: {
      return GT;
    }
    case GE: {
      return LT;
    }
    case LT: {
      return GE;
    }
    case GT: {
      return LE;
    }
    case NE: {
      return EQ;
    }
  }
}

std::string getTypeStr(TYPE_T type) {
  switch (type) {
    TYPE_STR(INT_T);
    TYPE_STR(CLOCK_T);
    TYPE_STR(CHAN_T);
    TYPE_STR(URGENT_CHAN_T);
    TYPE_STR(BROADCAST_CHAN_T);

    ENUM_ITEM_STR(SYSTEM_T);

    ENUM_ITEM_STR(TEMPLATE_T);

    ENUM_ITEM_STR(AUTOMATA_T);

    ENUM_ITEM_STR(LOCATION_T);

    ENUM_ITEM_STR(FORMAL_PARAMETER_T);

    ENUM_ITEM_STR(CLOCK_CS_T);

    ENUM_ITEM_STR(INT_CS_T);

    ENUM_ITEM_STR(INT_UPDATE_T);

    ENUM_ITEM_STR(RESET_T);

    ENUM_ITEM_STR(SELF_DEF_T);

    ENUM_ITEM_STR(CHAN_ACTION_T);

    ENUM_ITEM_STR(REF_CHAN_ACTION_T);
    ENUM_ITEM_STR(FUN_T);
    ENUM_ITEM_STR(NO_T);
  }
}

TYPE_T baseType(TYPE_T type) {
  return (TYPE_T)((type / TYPE_FAMILY_LEN) * TYPE_FAMILY_LEN);
}

bool isRefChan(TYPE_T type) {
  TYPE_T base_t = baseType(type);
  if (base_t != CHAN_T && base_t != URGENT_CHAN_T &&
      base_t != BROADCAST_CHAN_T) {
    return false;
  }
  return isRefType(type);
}
TYPE_T get_formal_paramter_type(TYPE_T type) {
  if (INT_T == type || CONST_INT_T == type) {
    return PARAMETER_INT_T;
  }
  if (REF_INT_T == type || CONST_REF_INT_T == type) {
    return REF_PARAMETER_INT_T;
  }

  if (CLOCK_T == type || CONST_CLOCK_T == type) {
    return PARAMETER_CLOCK_T;
  }
  if (REF_CLOCK_T == type || CONST_REF_CLOCK_T == type) {
    return REF_PARAMETER_CLOCK_T;
  }

  if (CHAN_T == type || CONST_CHAN_T == type) {
    return PARAMETER_CHAN_T;
  }

  if (REF_CHAN_T == type || CONST_REF_CHAN_T == type) {
    return REF_PARAMETER_CHAN_T;
  }

  if (URGENT_CHAN_T == type || CONST_URGENT_CHAN_T == type) {
    return PARAMETER_CHAN_T;
  }

  if (REF_URGENT_CHAN_T == type || CONST_REF_URGENT_CHAN_T == type) {
    return REF_PARAMETER_CHAN_T;
  }

  if (BROADCAST_CHAN_T == type || CONST_BROADCAST_CHAN_T == type) {
    return PARAMETER_CHAN_T;
  }

  if (REF_BROADCAST_CHAN_T == type || CONST_BROADCAST_CHAN_T == type) {
    return REF_PARAMETER_CHAN_T;
  }

  if (SELF_DEF_T == type) {
    return PARAMETER_INT_T;
  }

  assert(false);
  return NO_T;
}

bool isRefType(const TYPE_T type) {
  if (type >= SYSTEM_T) {
    return false;
  }
  int m = type % TYPE_FAMILY_LEN;
  return m == 1 || m == 3 || m == 5;
}

int fromPidToChanId(int id) { return id + 1; }

int chanIdToFromPid(int id) { return id - 1; }
}  // namespace graphmodel
