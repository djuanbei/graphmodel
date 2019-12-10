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

int UppaalParser::parseTemplate( child_type templates ) {

  if ( NULL == templates ) {
    return 0;
  }
  int allVarNum = system_data.getVarNum();
  for ( child_iterator it = templates->begin(); it != templates->end(); it++ ) {

    UppaalData template_data;
    template_data.tat = sys.createTemplate();

    template_data.parent = &system_data;

    XML_P nameConf = ( *it )->getOneChild( NAME_STR );
    template_data.setName( nameConf->getValue() );

    XML_P declaration = ( *it )->getOneChild( DECLARATION_STR );

    if ( NULL != declaration ) {
      string dec_content = declaration->getValue();
      parseProblem( dec_content, &template_data );
    }

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
    vector<pair<string, vector<int>>> clocks =
        template_data.getValues( CLOCK_T );
    for ( auto ee : clocks ) {
      template_data.tat->addClock( ee.first );
    }

    template_data.tat->initial( locations, transitions,
                                template_data.getInitialLoc() );

    template_map[ template_data.name ] = template_data;

    system_data.addValue( TEMPLATE_T, template_data.name );
    allVarNum += template_data.getVarNum();
  }

  return 0;
}

int UppaalParser::parseSystem( XML_P system ) {

  assert( NULL != system );
  UppaalData::IS_SYSTEM_PROCEDURE = true;
  string content                  = system->getValue();
  parseProblem( content, &system_data );
  setCounter();
  setChannel();

  SystemDec *sys_dec = (SystemDec *) system_data.getPointer( SYSTEM_T );
  /**
   Exactly one system declaration
   */
  for ( auto component : sys_dec->timed_automata_list ) {

    auto formal_parameter_list =
        template_map[ component->tmt_name ].getPoints( FORMAL_PARAMETER_T );

    Parameter parameter( formal_parameter_list.size() );
    /**
     * System declaration the corresponding  automation without parameter.
     */
    if ( formal_parameter_list.empty() ) {
      typename INT_TAS_t::Agent_t tma( template_map[ component->tmt_name ].tat,
                                       parameter );
      sys += tma;
    } else if ( component->has_parameter ) {
      int para_id = 0;
      for ( auto formal_p : formal_parameter_list ) {
        const FormalParameterItem *para_item =
            (FormalParameterItem *) formal_p.second[ 0 ];
        if ( ( para_item->type == REF_INT_T ) ||
             ( para_item->type == CONST_REF_INT_T ) ) {
          parameter.setCounterMap( para_id,
                                   component->real_param_list[ para_id ].id );
        } else if ( isRefChan( para_item->type ) ) {
          parameter.setChanMap( para_id,
                                component->real_param_list[ para_id ].id );
        } else if ( para_item->type == CLOCK_T ) {
          // TODO : add clock parameter support
          assert( false );
        } else if ( para_item->type == INT_T ) {
          parameter.setParameterMap( para_id,
                                     component->real_param_list[ para_id ].id );
        } else {
          assert( false );
        }
        para_id++;
      }
      typename INT_TAS_t::Agent_t tma( template_map[ component->tmt_name ].tat,
                                       parameter );

      sys += tma;

    } else {
      // If template has paramters, then the number of parameters is exactly
      // one.
      // Only has one paramter to give the number of instances of this template
      assert( 1 == formal_parameter_list.size() );
      const vector<int> &iarray = system_data.getIntArray(
          ( (FormalParameterItem *) formal_parameter_list[ 0 ].second[ 0 ] )
              ->type_name );
      for ( auto e : iarray ) {
        parameter.setParameterMap( 0, e );
        typename INT_TAS_t::Agent_t tma(
            template_map[ component->tmt_name ].tat, parameter );
        sys += tma;
      }
    }
  }
  UppaalData::IS_SYSTEM_PROCEDURE = false;
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

  //  cout << template_data.getTypeNum( PARAMETER_T ) << endl;
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

          string sync_statement = ( *llit )->getValue();
          parseLabel( template_data, sync_statement );

          vector<int> normol_chan_actions1 =
              template_data.getValue( CHAN_ACTION_T );

          for ( auto e : normol_chan_actions1 ) {

            transition.setChannel( new Channel( e ) );
          }
          vector<int> ref_chan_actions =
              template_data.getValue( REF_CHAN_ACTION_T );

          for ( auto e : ref_chan_actions ) {
            transition.setChannel( new Channel( e, true ) );
          }
          assert( normol_chan_actions1.size() + ref_chan_actions.size() < 2 );

          template_data.clear( CHAN_ACTION_T );
          template_data.clear( REF_CHAN_ACTION_T );
          //  assert( false );
          // cout << kind << ": " << ( *llit )->getValue() << endl;
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

int UppaalParser::setCounter() {
  int counter_num = system_data.getTotalCounterNum();
  
  sys.setCounterNum( counter_num );
  for ( auto e : system_data.counter_id_map ) {
    Counter counter( 0, MAX_COUNTER_VALUE );
    int     v = system_data.getValue( INT_T, e.first );
    counter.setValue( v );
    sys.setCounter( e.second, counter );

  }

  for ( auto temp : template_map ) {
    for ( auto e : temp.second.counter_id_map ) {
      Counter counter( 0, MAX_COUNTER_VALUE );
      int     v = temp.second.getValue( INT_T, e.first );
      counter.setValue( v );
      sys.setCounter( e.second, counter );
    }
  }
  return 0;
}
int UppaalParser::setChannel() {
  int channel_num = system_data.getTotalChannelNum();
  sys.setChannelNum( channel_num );
  for ( auto e : system_data.channel_id_map ) {
    int     v = system_data.getValue( CHAN_T, e.first );
    Channel ch;
    ch.type = (CHANNEL_TYPE) v;
    sys.setChannel( e.second, ch );
  }

  for ( auto temp : template_map ) {
    for ( auto e : temp.second.channel_id_map ) {
      int     v = temp.second.getValue( CHAN_T, e.first );
      Channel ch;
      ch.type = (CHANNEL_TYPE) v;
      sys.setChannel( e.second, ch );
    }
  }
  return 0;
}

} // namespace graphsat
