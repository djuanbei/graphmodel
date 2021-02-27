/**
 * @file   typedef.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Dec  5 09:55:57 2019
 *
 * @brief  type info
 *
 *
 */

#ifndef TYEP_DEF_H
#define TYEP_DEF_H

#include <cstdarg>
#include <cstdint>
#include <memory>
#include <string>

#include "macrodef.h"

namespace graphmodel {
// using std::std::string;
typedef unsigned int UINT;

const static int zero_clock_index = 0;

const static std::string NTA_STR = "nta";

const static std::string DECLARATION_STR = "declaration";

const static std::string TEMPLATE_STR = "template";

const static std::string NAME_STR = "name";

const static std::string PARAMETER_STR = "parameter";

const static std::string LOCATION_STR = "location";

const static std::string INIT_STR = "init";

const static std::string TRANSITION_STR = "transition";

const static std::string SOURCE_STR = "source";

const static std::string TARGET_STR = "target";

const static std::string ID_STR = "id";

const static std::string REF_STR = "ref";

const static std::string LABEL_STR = "label";

const static std::string KIND_STR = "kind";

const static std::string NAIL_STR = "nail";

const static std::string INVARIANT_STR = "invariant";

const static std::string GUARD_STR = "guard";

const static std::string ASSIGNMENT_STR = "assignment";

const static std::string SYNCHRONISATION_STR = "synchronisation";

const static std::string SYSTEM_STR = "system";

const static std::string QUERIES_STR = "queries";

const static std::string QUERY_STR = "query";

const static std::string FORMULA_STR = "formula";

const static std::string COMMENT_STR = "comment";

const static int LOC_OUT_WIDTH = 7;
const static int OP_OUT_WIDTH = 3;

const static int VALUE_OUT_WIDTH = 5;

typedef long double DF_T;

enum Check_State { TRUE, FALSE, UNKOWN };

enum COMP_OPERATOR { EQ, LE, GE, LT, GT, NE };

enum Action_e {
  CALL_ACTION,        // invoke function
  ASSIGNMENT_ACTION,  //=
  SELF_INC_ACTION,    //+=
  SELF_DEC_ACTION     //-=
};

enum TYPE_T {
  TYPE_TYPE(INT_T),
  TYPE_TYPE(CLOCK_T),

  TYPE_TYPE(CHAN_T),
  TYPE_TYPE(URGENT_CHAN_T),
  TYPE_TYPE(BROADCAST_CHAN_T),

  SYSTEM_T,
  TEMPLATE_T,
  AUTOMATA_T,
  LOCATION_T,
  FORMAL_PARAMETER_T,

  CLOCK_CS_T,
  INT_CS_T,
  INT_UPDATE_T,
  RESET_T,
  SELF_DEF_T,
  CHAN_ACTION_T,
  REF_CHAN_ACTION_T,
  FUN_T,
  NO_T

};

class VariableMap {
 public:
  virtual int getKeyID(const TYPE_T type, const std::string& key) const = 0;
  virtual int* getValue(const TYPE_T type, int* state,
                        const std::string& key) const = 0;
};

typedef int (*IndexFun_t)(int*, ...);
typedef int (*ConstraintFun_t)(const int*, ...);

const static int NOT_FOUND = -1;

enum CHANNEL_TYPE { ONE2ONE_CH, BROADCAST_CH, URGENT_CH };

enum CHANNEL_ACTION { CHANNEL_SEND, CHANNEL_RECEIVE };

const int NO_CHANNEL = 0;

const int DEFAULT_COUNTER_LOWER = 0;

const int DEFAULT_COUNTER_UPPER = 100;

const static int NO_DEF = -1;

const static std::string LOC_NAME_PRE = "Loc_";
const static std::string TRANSITION_NAME_PRE = "Tran_";

const static std::string CLOCK_NAME_PRE = "c_";

}  // namespace graphmodel

#endif
