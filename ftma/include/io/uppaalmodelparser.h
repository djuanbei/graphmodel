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
#include "constraint/clockdiffcons.h"
#include "model/location.hpp"

#include "model/transition.hpp"
#include "problem/reachability.hpp"

#include "util/dbmutil.hpp"

#include "domain/dbm.h"
#include "domain/dbmset.hpp"

#include "io/uppaaldata.h"

namespace graphsat {

static const TYPE_T gloabl_variable_types[] = {INT_T, CLOCK_T, CHAN_T};

class UppaalParser {

public:
  UppaalParser( const string &xmlfile );

  const INT_TAS_t &getSYS() const { return sys; }

  const Property &getProp() const { return prop; }

private:
  UppaalData              system_data;
  map<string, UppaalData> template_map;

  INT_TAS_t sys;
  Property  prop;

  int parseDeclaration( XML_P system );

  int parseTemplate( child_type templates );

  int parseSystem( XML_P system );

  int parseQuery( child_type queries );

  int parseTemplateParamter( UppaalData &tempData, XML_P parameter );

  int setCounter();
  int setChannel();

  vector<typename INT_TAS_t::L_t> parseLocation( UppaalData &tempData,
                                                 child_type  locations );

  vector<typename INT_TAS_t::T_t> parseTransition( UppaalData &tempData,
                                                   child_type  transitions );

  void parseLabel( UppaalData &data, string guards );

  /**
   * @param current_data  current template
   * @param name the name in xml model
   * @param save_name  the name in code level
   * @return  the type of name
   */
  TYPE_T getType( UppaalData *current_data, const string &xml_name,
                  string &code_name );
};

} // namespace graphsat

#endif
