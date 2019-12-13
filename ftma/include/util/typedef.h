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
#include "macrodef.h"
#include <cstdarg>
#include <cstdint>
#include <string>

namespace graphsat {
using std::string;
typedef unsigned int UINT;

const static int zero_clock_index = 0;

const static string NTA_STR = "nta";

const static string DECLARATION_STR = "declaration";

const static string TEMPLATE_STR = "template";

const static string NAME_STR = "name";

const static string PARAMETER_STR = "parameter";

const static string LOCATION_STR = "location";

const static string INIT_STR = "init";

const static string TRANSITION_STR = "transition";

const static string SOURCE_STR = "source";

const static string TARGET_STR = "target";

const static string ID_STR = "id";

const static string REF_STR = "ref";

const static string LABEL_STR = "label";

const static string KIND_STR = "kind";

const static string NAIL_STR = "nail";

const static string INVARIANT_STR = "invariant";

const static string GUARD_STR = "guard";

const static string ASSIGNMENT_STR = "assignment";

const static string SYNCHRONISATION_STR = "synchronisation";

const static string SYSTEM_STR = "system";

const static string QUERIES_STR = "queries";

const static string QUERY_STR = "query";

const static string FORMULA_STR = "formula";

const static string COMMENT_STR = "comment";

const static int LOC_OUT_WIDTH = 7;
const static int OP_OUT_WIDTH = 3;

const static int VALUE_OUT_WIDTH = 5;

typedef long double DF_T;

enum Check_State { TRUE, FALSE, UNKOWN };

enum COMP_OPERATOR { EQ, LE, GE, LT, GT, NE };

enum ARGUMENT_TYPE {
  CONST_ARG,         // constant value
  TEMPLATE_VAR_ARG,  // template variable id
  SYSTEM_VAR_ARG,    // system variable id
  PARAMETER_ARG,     // template argument id (pass value)
  REF_PARAMETER_ARG, // temmplate argument if (pass reference)
  FUN_POINTER_ARG,   // function
  SELECT_VAR_ARG,    // select variable
  EMPTY_ARG          // otherwise
};

struct RealArgument {
  ARGUMENT_TYPE type;
  int_fast64_t value;
  string name;
  shared_ptr<RealArgument> index;
  RealArgument() : type(EMPTY_ARG), value(0) {}
  RealArgument(ARGUMENT_TYPE t, int v) : type(t), value(v) {}
  RealArgument(ARGUMENT_TYPE t, const string &n) : type(t), value(0), name(n) {}
};

struct Argument {
  ARGUMENT_TYPE type;
  int_fast64_t value;
  string name;
  shared_ptr<Argument> index;
  Argument() : type(EMPTY_ARG), value(0) {}
  explicit Argument(int v) : type(CONST_ARG), value(v) {}
  Argument(ARGUMENT_TYPE t, int v) : type(t), value(v) {}
  Argument(ARGUMENT_TYPE t, const string &n) : type(t), value(0), name(n) {}
  void setIndex(const shared_ptr<Argument> &out_index) { index = out_index; }
};

enum Action_e {
  CALL_ACTION,       // invoke function
  ASSIGNMENT_ACTION, //=
  SELF_INC_ACTION,   //+=
  SELF_DEC_ACTION    //-=
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
  NO_T

};

class VariableMap {
public:
  virtual int getStart(const TYPE_T type, const string &key) const = 0;
  virtual int *getValue(const TYPE_T type, int *state,
                        const string &key) const = 0;
};

typedef int (*IndexFun_t)(int *, ...);
typedef int (*ConstraintFun_t)(const int *, ...);

const static int NOT_FOUND = -1;

enum CHANNEL_TYPE { ONE2ONE_CH, BROADCAST_CH, URGENT_CH };

enum CHANNEL_ACTION { CHANNEL_SEND, CHANNEL_RECEIVE };

const int NO_CHANNEL = 0;

const int DEFAULT_COUNTER_LOWER = 0;

const int DEFAULT_COUNTER_UPPER = 100;

const static int NO_DEF = -1;

const static string LOC_NAME_PRE = "Loc_";
const static string TRANSITION_NAME_PRE = "Tran_";

const static string CLOCK_NAME_PRE = "c_";

} // namespace graphsat

#endif
