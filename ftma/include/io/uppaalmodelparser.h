/**
 * @file   modelparser.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Apr 25 18:31:08 2019
 *
 * @brief  xml model parser
 *
 *
 */
#ifndef __MODEL_PARSER_HPP
#define __MODEL_PARSER_HPP

#include "model/ta.hpp"
#include "io/xmlconf.h"

#include "action.hpp"
#include "constraint/linsimpcons.hpp"
#include "location.hpp"

#include "problem/reachability.hpp"
#include "transition.hpp"

#include "util/dbmutil.hpp"

#include "domain/dbm.hpp"
#include "domain/dbmset.hpp"



namespace graphsat {

const static  string  NTA_STR="nta";

const static  string  DECLARATION_STR="declaration";

const static  string TEMPLATE_STR="template";

const static string NAME_STR="name";

const static string PARAMETER_STR="parameter";

const static string LOCATION_STR="location";

const static string INIT_STR="init";

const static string TRANSITION_STR="transition";

const static string SOURCE_STR="source";

const static string TARGET_STR="target";

const static string REF_STR="ref";

const static string LABEL_STR="label";

const static string  KIND_STR="kind";

const static string NAIL_STR="nail";

const static string  GUARD_STR="guard";

const static string  ASSIGNMENT_STR="assignment";


const static string SYNCHRONISATION_STR="synchronisation";


const static string SYSTEM_STR="system";

const static string QUERIES_STR="queries";

const static string QUERY_STR="query";

const static string FORMULA_STR="formula";

const static string COMMENT_STR="comment";


typedef int C;

typedef Constraint<C>                                   CS;
typedef DBM<C, CS>                                      DManager_t;
typedef DBMset<C, DManager_t>                           DBMSet_t;
typedef Location<C, CS, DManager_t, DBMSet_t>           L;
typedef Transition<C, CS, DManager_t, DBMSet_t, Action> T;
typedef TA<C, L, T>        TA_t;


class  UppaalParser{
 public:
  UppaalParser( ){
  }
  
  UppaalParser( const string& xmlfile);
  

 private:
  TA_t model;
  
  
  
};

}

#endif
