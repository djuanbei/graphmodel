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
const static int OP_OUT_WIDTH  = 3;

const static int VALUE_OUT_WIDTH = 5;

typedef long double DF_T;

enum Check_State { TRUE, FALSE, UNKOWN };

enum COMP_OPERATOR { EQ, LE, GE, LT, GT, NE };

enum ARGUMENT_TYPE {
  CONST_ARG,
  COUNTER_ARG,
  PARAMETER_ARG,
  REF_PARAMETER_ARG,
  EMPTY_ARG
};

struct Argument {
  ARGUMENT_TYPE type;
  int           value;
  Argument()
      : type( CONST_ARG )
      , value( 0 ) {}
  Argument( ARGUMENT_TYPE t, int v ) {
    type  = t;
    value = v;
  }
};
} // namespace graphsat

#endif
