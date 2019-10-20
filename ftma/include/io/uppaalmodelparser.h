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

#include "io/xmlconf.h"
#include "model/ta.hpp"

#include "action/counteraction.h"
#include "constraint/clockdiffcons.hpp"
#include "location.hpp"

#include "problem/reachability.hpp"
#include "transition.hpp"

#include "util/dbmutil.hpp"

#include "domain/dbm.hpp"
#include "domain/dbmset.hpp"

#include "io/uppaaldata.h"

namespace graphsat {

class UppaalParser {

public:
  UppaalParser( const string &xmlfile );

  const INT_TAS_t &getSYS() const { return sys; }

  const Property &getProp() const { return prop; }

  /**
   * @brief the id of type variable code_name in template  current_data
   *
   * @param current_data
   * @param type
   * @param code_name
   *
   * @return
   */
  int getGlobalId( UppaalTemplateData *current_data, string type,
                   string code_name );

  int getParameterId( UppaalTemplateData *current_data, const string &name );

  void addClockConstraint( UppaalTemplateData *current_data, int clock1_id,
                           int clock2_id, COMP_OPERATOR op, int rhs,
                           int parameter_id = -10 );

private:
  map<TYPE_T, string> type_name_map;

  UppaalTemplateData              system_data;
  map<string, UppaalTemplateData> template_map;

  INT_TAS_t sys;
  Property  prop;

  int parseDeclaration( XML_P system );

  int parseTemplateDeclaration( child_type templates );

  int parseTemplate( child_type templates );

  int parseSystem( XML_P system );

  int parseQuery( child_type queries );

  int parseTemplateParamter( UppaalTemplateData &tempData, XML_P parameter );

  vector<typename INT_TAS_t::L_t> parseLocation( UppaalTemplateData &tempData,
                                                 child_type locations );

  vector<typename INT_TAS_t::T_t> parseTransition( UppaalTemplateData &tempData,
                                                   child_type transitions );

  void parseLabel( UppaalTemplateData &data, string guards );

  /**
   * @param current_data  current template
   * @param name the name in xml model
   * @param save_name  the name in code level
   * @return  the type of name
   */
  TYPE_T getType( UppaalTemplateData *current_data, const string &xml_name,
                  string &code_name );
};

} // namespace graphsat

#endif
