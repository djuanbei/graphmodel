#include "io/uppaalmodelparser.h"
#include "util/dbmutil.hpp"
#include <cassert>

namespace graphsat {

UppaalParser::UppaalParser( const string &xmlfile ) {

  XmlConfig  xmldoc( xmlfile );
  XML_P      declaration = xmldoc.getOneChild( DECLARATION_STR );
  child_type templates   = xmldoc.getChild( TEMPLATE_STR );
  XML_P      system      = xmldoc.getOneChild( SYSTEM_STR );
  child_type queries     = xmldoc.getChild( QUERIES_STR );

  parseDeclaration( declaration );

  parseTemplateDeclaration( templates );

  parseTemplate( templates );
  parseSystem( system );
  parseQuery( queries );
}

int UppaalParser::parseDeclaration( XML_P declaration ) {
  if ( NULL == declaration ) {
    return 0;
  }

  string content = declaration->getValue();
  if ( content.length() > 0 ) {
    parseProblem( content, this, &system_data, &system_data );
  }
  int num = 0;
  for ( auto key : gloabl_variable_types ) {
    num += system_data.getTypeNum( key );
  }

  system_data.setGlobalVarNum( num );

  return 0;
}

int UppaalParser::parseTemplateDeclaration( child_type templates ) {
  assert( NULL != templates );

  for ( child_iterator it = templates->begin(); it != templates->end(); it++ ) {
    UppaalTemplateData template_data;
    XML_P              nameConf = ( *it )->getOneChild( NAME_STR );
    template_data.setName( nameConf->getValue() );

    XML_P declaration = ( *it )->getOneChild( DECLARATION_STR );

    if ( NULL != declaration ) {
      string dec_content = declaration->getValue();
      parseProblem( dec_content, this, &system_data, &template_data );
    }
    template_map[ template_data.name ] = template_data;
  }
  int num = 0;
  for ( auto key : gloabl_variable_types ) {
    num += system_data.getTypeNum( key );
  }

  system_data.setGlobalVarNum( num );

  return 0;
}

int UppaalParser::parseTemplate( child_type templates ) {

  if ( NULL == templates ) {
    return 0;
  }

  for ( child_iterator it = templates->begin(); it != templates->end(); it++ ) {

    XML_P nameConf = ( *it )->getOneChild( NAME_STR );

    UppaalTemplateData &template_data = template_map[ nameConf->getValue() ];

    system_data.addValue( TEMPLATE_STR, template_data.name );

    XML_P parameter = ( *it )->getOneChild( PARAMETER_STR );

    if ( NULL != parameter ) {
      parseTemplateParamter( template_data, parameter );
    }

    child_type location_comps = ( *it )->getChild( LOCATION_STR );

    vector<typename INT_TAS_t::L_t> locations =
        parseLocation( template_data, location_comps );

    XML_P init_conf = ( *it )->getOneChild( INIT_STR );

    if ( NULL != init_conf ) {
      string loc = init_conf->getAttrValue( REF_STR );
      template_data.setInitialLoc( template_data.getId( LOCATION_STR, loc ) );
    }

    child_type transition_comps = ( *it )->getChild( TRANSITION_STR );

    vector<typename INT_TAS_t::T_t> transitions =
        parseTransition( template_data, transition_comps );
    template_data.tat =
        INT_TAS_t::TAT_t( locations, transitions, template_data.getInitialLoc(),
                          template_data.getTypeNum( CLOCK_STR ) );
  }

  return 0;
}

int UppaalParser::parseSystem( XML_P system ) {
  assert( NULL != system );

  string content = system->getValue();

  parseProblem( content, this, &system_data, &system_data );

  int counter_num = system_data.getTypeNum( INT_STR );
  for ( int i = 0; i < counter_num; i++ ) {
    Counter                          counter( 0, MAX_COUNTER_VALUE );
    const pair<string, vector<int>> &hh = system_data.getValue( INT_STR, i );
    counter.setValue( hh.second[ 0 ] );
    sys += counter;
  }

  SystemDec *sys_dec =
      (SystemDec *) system_data.getPointer( SYSTEM_STR, SYSTEM_STR );
  /**
   Exactly one system declaration
   */
  assert( NULL != sys_dec );

  for ( size_t i = 0; i < sys_dec->timed_automata_list.size(); i++ ) {
    /**
     * System declaration the corresponding  automation without parameter.
     */
    if ( sys_dec->timed_automata_list[ i ]->no_parameter ) {

      string template_name = sys_dec->timed_automata_list[ i ]->name;

      vector<pair<string, vector<void *>>> template_parameter_vec =
          template_map[ template_name ].getPoints( PARAMETER_STR );

      Parameter parameter_template;
      int       global_num = system_data.getGlobalVarNum();
      int       param_num  = template_parameter_vec.size();
      for ( int i = 0; i < global_num; i++ ) {
        parameter_template.setCounterMap( i + param_num, i );
      }

      if ( template_parameter_vec.empty() ) {
        Parameter parameter = parameter_template;

        typename INT_TAS_t::TA_t tma( &template_map[ template_name ].tat,
                                      parameter );
        sys += tma;

      } else {
        // If template has paramters, then the number of parameters is exactly
        // one.
        assert( 1 == template_parameter_vec.size() );

        const vector<int> &iarray = system_data.getIntArray(
            ( (ParaElement *) template_parameter_vec[ 0 ].second[ 0 ] )
                ->type_name );
        for ( auto e : iarray ) {
          Parameter parameter = parameter_template;

          parameter.addParameterValue( e );
          typename INT_TAS_t::TA_t tma( &template_map[ template_name ].tat,
                                        parameter );
          sys += tma;
        }
      }
    } else {
    }
  }

  return 0;
}

int UppaalParser::parseQuery( child_type queries ) { return 0; }

vector<INT_TAS_t::L_t>
    UppaalParser::parseLocation( UppaalTemplateData &template_data,
                                 child_type          locations ) {
  vector<INT_TAS_t::L_t> return_locations;

  for ( child_iterator lit = locations->begin(); lit != locations->end();
        lit++ ) {

    string id_str = ( *lit )->getAttrValue( ID_STR );
    assert( id_str != "" );
    template_data.addValue( LOCATION_STR, id_str );

    int location_id = template_data.getId( LOCATION_STR, id_str );

    INT_TAS_t::L_t location( location_id );
    string location_name = ( *lit )->getOneChild( NAME_STR )->getValue();
    location.setName( location_name );

    child_type labels = ( *lit )->getChild( LABEL_STR );
    if ( NULL != labels ) {
      for ( child_iterator llit = labels->begin(); llit != labels->end();
            llit++ ) {
        string kind = "";
        kind        = ( *llit )->getAttrDefault( KIND_STR, kind );

        if ( kind == INVARIANT_STR ) {
          string invariants = ( *llit )->getValue();
          parseLabel( template_data, invariants );
          vector<void *> cons = template_data.getPoints( CLOCK_CS, CLOCK_CS );
          for ( auto cs : cons ) {
            location += *( (INT_TAS_t::CS_t *) ( cs ) );
          }
          template_data.clearPoints( CLOCK_CS );
        }
      }
    }
    return_locations.push_back( location );
  }
  return return_locations;
}

int UppaalParser::parseTemplateParamter( UppaalTemplateData &template_data,
                                         XML_P               parameter ) {

  string         para_content = parameter->getValue();
  vector<string> terms        = splitStr( para_content, "," );
  for ( auto str : terms ) {
    ParaElement *temp_param = new ParaElement();
    bool         is_ref     = false;
    size_t       start      = str.find( PARAMETER_REF_STR );

    if ( start != std::string::npos ) {
      str                = deleteChar( str, start, '&' );
      is_ref             = true;
      temp_param->is_ref = true;
    }

    vector<string> parts = splitStr( str, " " );

    string name = parts.back();
    if ( is_ref ) {
      name = name.substr( 1 );
    }
    temp_param->name      = name;
    temp_param->type_name = parts[ parts.size() - 2 ];

    if ( find( parts.begin(), parts.end(), BOOL_STR ) != parts.end() ) {
      if ( is_ref ) {
        temp_param->type = PARAM_BOOL_REF_T;
      } else {
        temp_param->type = PARAM_BOOL_T;
      }
    } else if ( find( parts.begin(), parts.end(), CHAN_STR ) != parts.end() ) {
      if ( is_ref ) {
        temp_param->type = PARAM_CHANNEL_REF_T;
      } else {
        temp_param->type = PARAM_CHANNEL_T;
      }

    } else if ( find( parts.begin(), parts.end(), INT_STR ) != parts.end() ) {
      if ( is_ref ) {
        temp_param->type = PARAM_INT_REF_T;
      } else {
        temp_param->type = PARAM_INT_T;
      }
    } else {
      temp_param->type = PARAM_SCALAR_T;
    }

    template_data.addPointer( PARAMETER_STR, name, temp_param );
  }
  return 0;
}

vector<INT_TAS_t::T_t>
    UppaalParser::parseTransition( UppaalTemplateData &template_data,
                                   child_type          transitions ) {
  vector<INT_TAS_t::T_t> return_transitions;
  for ( child_iterator tit = transitions->begin(); tit != transitions->end();
        tit++ ) {

    XML_P  source     = ( *tit )->getOneChild( SOURCE_STR );
    string source_ref = source->getAttrValue( REF_STR );

    XML_P  target     = ( *tit )->getOneChild( TARGET_STR );
    string target_ref = target->getAttrValue( REF_STR );

    int source_id = template_data.getId( LOCATION_STR, source_ref );
    int target_id = template_data.getId( LOCATION_STR, target_ref );

    INT_TAS_t::T_t transition( source_id, target_id );

    child_type labels = ( *tit )->getChild( LABEL_STR );
    if ( NULL != labels ) {

      for ( child_iterator llit = labels->begin(); llit != labels->end();
            llit++ ) {
        string kind = "";
        kind        = ( *llit )->getAttrValue( KIND_STR );
        if ( GUARD_STR == kind ) {
          string guard = ( *llit )->getValue();
          parseLabel( template_data, guard );
          vector<void *> cons = template_data.getPoints( CLOCK_CS, CLOCK_CS );
          for ( auto cs : cons ) {
            transition += *( (INT_TAS_t::CS_t *) ( cs ) );
          }

          template_data.clearPoints( CLOCK_CS );

          vector<void *> counterCs = template_data.getPoints( INT_CS, INT_CS );
          for ( auto cs : counterCs ) {
            transition.addCounterCons( (CounterConstraint *) cs );
          }

          template_data.clearPoints( INT_CS );

        } else if ( ASSIGNMENT_STR == kind ) {
          string assign_statement = ( *llit )->getValue();
          parseLabel( template_data, assign_statement );
          vector<void *> updates =
              template_data.getPoints( INT_UPDATE, INT_UPDATE );
          for ( auto update : updates ) {
            transition.addCounterAction( (CounterAction *) update );
          }
          template_data.clearPoints( INT_UPDATE );

          vector<void *> resets =
              template_data.getPoints( RESET_STR, RESET_STR );
          for ( auto reset : resets ) {
            transition.addReset( *( (pair<int, int> *) reset ) );
            delete (pair<int, int> *) reset;
          }
          template_data.clearPoints( RESET_STR );

        } else if ( SYNCHRONISATION_STR == kind ) {
          cout << kind << ": " << ( *llit )->getValue() << endl;
        }
      }
    }
    return_transitions.push_back( transition );
  }
  return return_transitions;
}

void UppaalParser::parseLabel( UppaalTemplateData &template_data,
                               string              guards ) {
  // template_data.clearPoints();
  parseProblem( guards, this, &system_data, &template_data );
}

void UppaalParser::addClockConstraint( UppaalTemplateData *current_data,
                                       int clock1_id, int clock2_id,
                                       COMP_OPERATOR op, int rhs,
                                       int parameter_id ) {
  if ( EQ == op ) {
    void *cs = new INT_TAS_t::CS_t( clock1_id, clock2_id, GE, rhs,
                                    parameter_id ); // x-y<= c
    current_data->addPointer( CLOCK_CS, CLOCK_CS, cs );

    cs = new INT_TAS_t::CS_t( clock1_id, clock2_id, LE, rhs,
                              parameter_id ); // x-y>= c
    current_data->addPointer( CLOCK_CS, CLOCK_CS, cs );

  } else {
    void *cs = new INT_TAS_t::CS_t( clock1_id, clock2_id, op, rhs,
                                    parameter_id ); // x op c
    current_data->addPointer( CLOCK_CS, CLOCK_CS, cs );
  }
}

/**
 * @brief the id of type variable code_name in template  current_data
 *
 * @param current_data
 * @param type
 * @param code_name
 *
 * @return
 */

int UppaalParser::getGlobalId( UppaalTemplateData *current_data, string type,
                               string code_name ) {

  int id = system_data.getId( type, code_name );

  if ( type == INT_STR ) {
    return id + current_data->getPointNum( PARAMETER_STR );
  } else if ( type == BOOL_STR ) {
    return id + system_data.getTypeNum( INT_STR ) +
           current_data->getPointNum( PARAMETER_STR );
  } else if ( type == CHAN_STR ) {
    return id + system_data.getTypeNum( INT_STR ) +
           system_data.getTypeNum( BOOL_STR ) +
           current_data->getPointNum( PARAMETER_STR );
  }

  assert( false );
  return -1;
}

int UppaalParser::getParameterId( UppaalTemplateData *current_data,
                                  const string &      name ) {

  return current_data->getPointerId( PARAMETER_STR, name );
}

TYPE_T UppaalParser::getType( UppaalTemplateData *current_data,
                              const string &xml_name, string &code_name ) {
  code_name = xml_name;
  if ( current_data->hasPointer( PARAMETER_STR, code_name ) ) {
    return PARAMETER_T;
  }
  if ( system_data.hasValue( TEMPLATE_STR, code_name ) ) {
    return TEMPLATE_T;
  }
  /**
   * The clock variable only declare in template section and can not declare as
   * global variable
   *
   */

  if ( current_data->hasValue( CLOCK_STR, code_name ) ) {
    return CLOCK_T;
  }

  code_name = current_data->getVarFullName( xml_name );

  if ( system_data.hasValue( INT_STR, code_name ) ) {
    return INT_T;
  }
  if ( system_data.hasValue( BOOL_STR, code_name ) ) {
    return BOOL_T;
  }
  if ( system_data.hasValue( CHAN_STR, code_name ) ) {
    return CHAN_T;
  }

  code_name = xml_name;

  if ( system_data.hasValue( INT_STR, code_name ) ) {
    return INT_T;
  }

  if ( system_data.hasValue( BOOL_STR, code_name ) ) {
    return BOOL_T;
  }

  if ( system_data.hasValue( CHAN_STR, code_name ) ) {
    return CHAN_T;
  }

  return NO_T;
}

} // namespace graphsat
