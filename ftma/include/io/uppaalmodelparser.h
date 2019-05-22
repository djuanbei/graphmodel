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
  UppaalParser() {}

  UppaalParser( const string &xmlfile );

  const TAS_t &getSYS() const { return sys; }

  const Property &getProp() const { return prop; }

private:
  UppaalData data;

  TAS_t    sys;
  Property prop;

  int parserDeclaration( child_type declarations );

  int parserTemplate( child_type templates );

  int parserSystem( child_type system );

  int parserQuery( child_type queries );

  vector<L_t> parserLocation( UppaalData &tempData, child_type locations );

  vector<T_t> parserTransition( UppaalData &tempData, child_type transitions );

  void parserLabel( UppaalData &data, string guards );
};

} // namespace graphsat

#endif
