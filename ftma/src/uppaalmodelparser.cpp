#include "io/uppaalmodelparser.h"
#include "util/dbmutil.hpp"
#include <cassert>
#include <iostream>

namespace graphsat {
using namespace std;
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
    parseProblem( content, &system_data );
  }

  return 0;
}

int UppaalParser::parseTemplateDeclaration( child_type templates ) {
  assert( NULL != templates );

  int allVarNum = system_data.getVarNum();
  for ( child_iterator it = templates->begin(); it != templates->end(); it++ ) {
    UppaalData template_data;

    template_data.parent = &system_data;
    template_data.setStartId( allVarNum );
    XML_P nameConf = ( *it )->getOneChild( NAME_STR );
    template_data.setName( nameConf->getValue() );

    XML_P declaration = ( *it )->getOneChild( DECLARATION_STR );

    if ( NULL != declaration ) {
      string dec_content = declaration->getValue();
      parseProblem( dec_content, &template_data );
    }
    template_map[ template_data.name ] = template_data;
    allVarNum += template_data.getVarNum();
  }

  return 0;
}

int UppaalParser::parseTemplate( child_type templates ) {

  if ( NULL == templates ) {
    return 0;
  }

  for ( child_iterator it = templates->begin(); it != templates->end(); it++ ) {

    XML_P nameConf = ( *it )->getOneChild( NAME_STR );

    UppaalData &template_data = template_map[ nameConf->getValue() ];

    system_data.addValue( TEMPLATE_T, template_data.name );

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
      template_data.setInitialLoc( template_data.getId( LOCATION_T, loc ) );
    }

    child_type transition_comps = ( *it )->getChild( TRANSITION_STR );

    vector<typename INT_TAS_t::T_t> transitions =
        parseTransition( template_data, transition_comps );
    template_data.tat =
        INT_TAS_t::TAT_t( locations, transitions, template_data.getInitialLoc(),
                          template_data.getTypeNum( CLOCK_T ) );
  }

  return 0;
}

int UppaalParser::parseSystem( XML_P system ) {

  assert( NULL != system );

  string content = system->getValue();

  parseProblem( content, &system_data );

  int counter_num = system_data.getTypeNum( INT_T );
  for ( int i = 0; i < counter_num; i++ ) {
    Counter                          counter( 0, MAX_COUNTER_VALUE );
    const pair<string, vector<int>> &hh = system_data.getValue( INT_T, i );
    // TODO: update counter initial value
    counter.setValue( hh.second[ 0 ] );
    sys += counter;
  }

  for ( auto e : template_map ) {
    counter_num = e.second.getTypeNum( INT_T );
    for ( int i = 0; i < counter_num; i++ ) {
      Counter                          counter( 0, MAX_COUNTER_VALUE );
      const pair<string, vector<int>> &hh = system_data.getValue( INT_T, i );
      // TODO: update counter initial value
      counter.setValue( hh.second[ 0 ] );
      sys += counter;
    }
  }

  SystemDec *sys_dec = (SystemDec *) system_data.getPointer( SYSTEM_T );
  /**
   Exactly one system declaration
   */

  int startId = system_data.getVarNum();

  for ( size_t i = 0; i < sys_dec->timed_automata_list.size(); i++ ) {

    string template_name = sys_dec->timed_automata_list[ i ]->tmt_name;

    vector<RealParameterItem> param_list =
        sys_dec->timed_automata_list[ i ]->param_list;

    vector<pair<string, vector<void *>>> template_parameter_vec =
        template_map[ template_name ].getPoints( PARAMETER_T );

    int parameter_num = (int) template_parameter_vec.size();
    // int allVarNum =
    //     system_data.getVarNum() + template_map[ template_name ].getVarNum();
    Parameter parameter_template( parameter_num );

    if ( template_parameter_vec.empty() ) {
      Parameter parameter = parameter_template;

      typename INT_TAS_t::TA_t tma( &template_map[ template_name ].tat,
                                    parameter );
      sys += tma;
    }
    /**
     * System declaration the corresponding  automation without parameter.
     */
    if ( sys_dec->timed_automata_list[ i ]->has_parameter ) {
      Parameter parameter = parameter_template;
      for ( int j = 0; j < parameter_num; j++ ) {
        FormalParameterItem *p =
            (FormalParameterItem *) template_parameter_vec[ j ].second[ 0 ];
        if ( p->is_ref ) {
          if ( p->type == INT_T || p->type == BOOL_T ) {
            parameter.setCounterMap( j, param_list[ j ].id );
          } else if ( p->type == CHAN_T ) {
            parameter.setChanMap( j, param_list[ j ].id );
          } else if ( p->type == CLOCK_T ) {
            // TODO : add clock parameter support
            assert( false );
          }

        } else {
          if ( p->type == INT_T || p->type == BOOL_T ) {
            parameter.setParameterMap( j, param_list[ j ].id );
          } else {
            assert( false );
          }
        }
      }
      typename INT_TAS_t::TA_t tma( &template_map[ template_name ].tat,
                                    parameter );
      sys += tma;

    } else {

      // If template has paramters, then the number of parameters is exactly
      // one.
      // Only has one paramter to give the number of instances of this template
      assert( 1 == template_parameter_vec.size() );

      const vector<int> &iarray = system_data.getIntArray(
          ( (FormalParameterItem *) template_parameter_vec[ 0 ].second[ 0 ] )
              ->type_name );
      for ( auto e : iarray ) {
        Parameter parameter = parameter_template;

        parameter.setParameterMap( 0, e );
        typename INT_TAS_t::TA_t tma( &template_map[ template_name ].tat,
                                      parameter );
        sys += tma;
      }
    }
  }

  return 0;
}

int UppaalParser::parseQuery( child_type queries ) { return 0; }

vector<INT_TAS_t::L_t> UppaalParser::parseLocation( UppaalData &template_data,
                                                    child_type  locations ) {
  vector<INT_TAS_t::L_t> return_locations;

  for ( child_iterator lit = locations->begin(); lit != locations->end();
        lit++ ) {

    string id_str = ( *lit )->getAttrValue( ID_STR );
    assert( id_str != "" );
    template_data.addValue( LOCATION_T, id_str );

    int location_id = template_data.getId( LOCATION_T, id_str );

    INT_TAS_t::L_t location( location_id );
    if ( NULL != ( *lit )->getOneChild(
                     NAME_STR ) ) { // Not evvery location require name property
      string location_name = ( *lit )->getOneChild( NAME_STR )->getValue();
      location.setName( location_name );
    }

    child_type labels = ( *lit )->getChild( LABEL_STR );
    if ( NULL != labels ) {
      for ( child_iterator llit = labels->begin(); llit != labels->end();
            llit++ ) {
        string kind = "";
        kind        = ( *llit )->getAttrDefault( KIND_STR, kind );

        if ( kind == INVARIANT_STR ) {
          string invariants = ( *llit )->getValue();
          parseLabel( template_data, invariants );
          vector<void *> cons =
              template_data.getPoints( CLOCK_CS_T, STRING( CLOCK_CS_T ) );
          for ( auto cs : cons ) {
            location += *( (INT_TAS_t::CS_t *) ( cs ) );
          }
          template_data.clear( CLOCK_CS_T );
        }
      }
    }
    return_locations.push_back( location );
  }
  return return_locations;
}

int UppaalParser::parseTemplateParamter( UppaalData &template_data,
                                         XML_P       parameter ) {

  string para_content = "argument " + parameter->getValue();
  parseProblem( para_content, &template_data );

  cout << template_data.getTypeNum( PARAMETER_T ) << endl;
  // TODO:x
  return 0;
}

vector<INT_TAS_t::T_t> UppaalParser::parseTransition( UppaalData &template_data,
                                                      child_type transitions ) {
  vector<INT_TAS_t::T_t> return_transitions;
  for ( child_iterator tit = transitions->begin(); tit != transitions->end();
        tit++ ) {

    XML_P  source     = ( *tit )->getOneChild( SOURCE_STR );
    string source_ref = source->getAttrValue( REF_STR );

    XML_P  target     = ( *tit )->getOneChild( TARGET_STR );
    string target_ref = target->getAttrValue( REF_STR );

    int source_id = template_data.getId( LOCATION_T, source_ref );
    int target_id = template_data.getId( LOCATION_T, target_ref );

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
          vector<void *> cons =
              template_data.getPoints( CLOCK_CS_T, STRING( CLOCK_CS_T ) );
          for ( auto cs : cons ) {
            transition += *( (INT_TAS_t::CS_t *) ( cs ) );
          }

          template_data.clear( CLOCK_CS_T );

          vector<void *> counterCs =
              template_data.getPoints( INT_CS_T, STRING( INT_CS_T ) );
          for ( auto cs : counterCs ) {
            transition.addCounterCons( (CounterConstraint *) cs );
          }

          template_data.clear( INT_CS_T );

        } else if ( ASSIGNMENT_STR == kind ) {
          string assign_statement = ( *llit )->getValue();
          parseLabel( template_data, assign_statement );
          vector<void *> updates =
              template_data.getPoints( INT_UPDATE_T, STRING( INT_UPDATE_T ) );
          for ( auto update : updates ) {
            transition.addCounterAction( (CounterAction *) update );
          }
          template_data.clear( INT_UPDATE_T );

          vector<void *> resets =
              template_data.getPoints( RESET_T, getTypeStr( RESET_T ) );
          for ( auto reset : resets ) {
            transition.addReset( *( (pair<int, int> *) reset ) );
            delete (pair<int, int> *) reset;
          }
          template_data.clear( RESET_T );

        } else if ( SYNCHRONISATION_STR == kind ) {
          cout << kind << ": " << ( *llit )->getValue() << endl;
        }
      }
    }
    return_transitions.push_back( transition );
  }
  return return_transitions;
}

void UppaalParser::parseLabel( UppaalData &template_data, string guards ) {
  // template_data.clear();
  parseProblem( guards, &template_data );
}

} // namespace graphsat
