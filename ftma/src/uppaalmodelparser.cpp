#include "io/uppaalmodelparser.h"
#include "util/dbmutil.hpp"
#include <cassert>

namespace graphsat {

UppaalParser::UppaalParser( const string &xmlfile ) {
  XmlConfig  xmldoc( xmlfile );
  child_type declarations = xmldoc.getChild( DECLARATION_STR );
  child_type templates    = xmldoc.getChild( TEMPLATE_STR );
  XML_P system       = xmldoc.getOneChild( SYSTEM_STR );
  child_type queries      = xmldoc.getChild( QUERIES_STR );

  parserDeclaration( declarations );
  parserTemplate( templates );
  parserSystem( system );
  parserQuery( queries );
  
  int counter_num = system_data.getTypeNum( COUNTER_STR );
  for ( int i = 0; i < counter_num; i++ ) {
    Counter                          counter( 0, MAX_COUNTER_VALUE );
    const pair<string, vector<int>> &hh =
        system_data.getValue( COUNTER_STR, i );
    counter.setValue( hh.second[ 0 ] );
    sys += counter;
  }
  
}

int UppaalParser::parserDeclaration( child_type declarations ) {
  if ( NULL == declarations ) {
    return 0;
  }
  for ( child_iterator it = declarations->begin(); it != declarations->end();
        it++ ) {
    string content = ( *it )->getValue();
    if ( content.length() > 0 ) {
      parseProblem( content, &system_data, &system_data );
    }
  }

  return 0;
}

int UppaalParser::parserTemplate( child_type templates ) {

  if ( NULL == templates ) {
    return 0;
  }

  for ( child_iterator it = templates->begin(); it != templates->end(); it++ ) {
    UppaalTemplateData template_data;

    XML_P nameConf = ( *it )->getOneChild( NAME_STR );

    template_data.name = nameConf->getValue();

    system_data.addValue( TEMPLATE_STR, template_data.name );

    XML_P parameter = ( *it )->getOneChild( PARAMETER_STR );

    if ( NULL != parameter ) {
      parserTemplateParamter( template_data, parameter );
    }
    
    XML_P declaration = ( *it )->getOneChild( DECLARATION_STR );

    if ( NULL != declaration ) {
      string dec_content = declaration->getValue();
      parseProblem( dec_content, &system_data, &template_data );
    }
    
    
    child_type location_comps = ( *it )->getChild( LOCATION_STR );

    vector<typename INT_TAS_t::L_t> locations =
        parserLocation( template_data, location_comps );
 
    
    XML_P init_conf = ( *it )->getOneChild( INIT_STR );

    if ( NULL != init_conf ) {
      string loc = init_conf->getAttrValue( REF_STR );
      template_data.setInitialLoc( template_data.getId( LOCATION_STR, loc ) );
    }

    child_type transition_comps = ( *it )->getChild( TRANSITION_STR );

    
    vector<typename INT_TAS_t::T_t> transitions =
        parserTransition( template_data, transition_comps );
    template_data.tat =
        INT_TAS_t::TAT_t( locations, transitions, template_data.getInitialLoc(),
                          template_data.getTypeNum( CLOCK_STR ) );
  
    
    
    template_map[ template_data.name ] = template_data;
  }

  return 0;
} // namespace graphsat

int UppaalParser::parserSystem( XML_P system ) {

  if ( NULL != system ) {
    
    string content        = system->getValue();
    parseProblem( content, &system_data, &system_data );

    vector<void *> sys_dec_vec =
        system_data.getPoints( SYSTEM_STR, SYSTEM_STR );
    SystemDec *sys_dec = (SystemDec *) sys_dec_vec[ 0 ];
    for ( size_t i = 0; i < sys_dec->ta_list.size(); i++ ) {
      if ( sys_dec->ta_list[ i ]->no_param ) {
        string template_name=sys_dec->ta_list[ i ]->name;
        
        vector<pair<string, vector<void*>>> temp= template_map[template_name].getPoints(PARAMETER_STR);
        if(temp.empty()){
          Parameter param;
          
          
          typename INT_TAS_t::TA_t tma( &template_map[template_name].tat, param );
          sys+=tma;
        }else{
          assert(1==temp.size());
          
          const vector<int> & iarray=system_data.getIntArray(((ParaElement*) temp[0].second[0])->type_name);
          for (auto e: iarray){
            Parameter param;
            
            vector<pair<string, vector<int>>> temp= template_map[template_name].getValue(USING_COUNTER);
            for(auto ee: temp){
              param.setCounterMap( ee.second[0], ee.second[0]);
            }
            
            param.addParameterValue( e );
            typename INT_TAS_t::TA_t tma( &template_map[template_name].tat, param );
            sys+=tma;
          }
        }
      } else {
        
        
      }
    }
  }

  return 0;
}

int UppaalParser::parserQuery( child_type queries ) { return 0; }

vector<INT_TAS_t::L_t>
    UppaalParser::parserLocation( UppaalTemplateData &template_data,
                                  child_type          locations ) {
  vector<INT_TAS_t::L_t> return_locations;

  for ( child_iterator lit = locations->begin(); lit != locations->end();
        lit++ ) {

    string id_str = ( *lit )->getAttrValue( ID_STR );
    assert( id_str != "" );
    template_data.addValue( LOCATION_STR, id_str );

    int location_id = template_data.getId( LOCATION_STR, id_str );

    INT_TAS_t::L_t location( location_id );

    child_type labels = ( *lit )->getChild( LABEL_STR );
    if ( NULL != labels ) {
      for ( child_iterator llit = labels->begin(); llit != labels->end();
            llit++ ) {
        string kind = "";
        kind        = ( *llit )->getAttrDefault( KIND_STR, kind );

        if ( kind == INVARIANT_STR ) {
          string invariants = ( *llit )->getValue();
          parser_label( template_data, invariants );
          vector<void *> cons = template_data.getPoints( CLOCK_CS, CLOCK_CS );
          for ( auto cs : cons ) {
            location += *( (INT_TAS_t::CS_t *) ( cs ) );
          }
        }
      }
    }
    return_locations.push_back( location );
  }
  return return_locations;
}

int UppaalParser::parserTemplateParamter( UppaalTemplateData &template_data,
                                          XML_P               parameter ) {

  string         para_content = parameter->getValue();
  vector<string> terms        = splitStr( para_content, "," );
  for ( auto str : terms ) {
    bool   is_ref = false;
    size_t start  = str.find( PARAMETER_REF_STR );
    if ( start != std::string::npos ) {
      str    = deleteChar( str, start, '&' );
      is_ref = true;
    }

    ParaElement *temp = new ParaElement();
   
    vector<string> parts = splitStr( str, " " );

    string name = parts.back();
    if ( is_ref ) {
      name = name.substr( 1 );
    }
    temp->name = name;
    temp->type_name=parts[parts.size()-2];
    
    if ( find( parts.begin(), parts.end(), BOOL_STR ) != parts.end() ) {
      if ( is_ref ) {
        temp->type = PARAM_BOOL_REF_T;
      } else {
        temp->type = PARAM_BOOL_T;
      }
    } else if ( find( parts.begin(), parts.end(), CHAN_STR ) != parts.end() ) {
      if ( is_ref ) {
        temp->type = PARAM_CHANNEL_REF_T;
      } else {
        temp->type = PARAM_CHANNEL_T;
      }

    } else if ( find( parts.begin(), parts.end(), INT_STR ) != parts.end() ) {
      if ( is_ref ) {
        temp->type = PARAM_INT_REF_T;
      } else {
        temp->type = PARAM_INT_T;
      }
    } else {
      temp->type = PARAM_SCALAR_T;
    }

    template_data.addPointer( PARAMETER_STR, name, temp );
  }
  return 0;
}

vector<INT_TAS_t::T_t>
    UppaalParser::parserTransition( UppaalTemplateData &template_data,
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
          parser_label( template_data, guard );
          vector<void *> cons = template_data.getPoints( CLOCK_CS, CLOCK_CS );
          for ( auto cs : cons ) {
            transition += *( (INT_TAS_t::CS_t *) ( cs ) );
          }
          vector<void *> counterCs =
              template_data.getPoints( COUNTER_CS, COUNTER_CS );
          for ( auto cs : counterCs ) {
            transition.addCounterCons( (CounterConstraint *) cs );
          }

        } else if ( ASSIGNMENT_STR == kind ) {
          string assign_statement = ( *llit )->getValue();
          parser_label( template_data, assign_statement );
          vector<void *> updates =
              template_data.getPoints( COUNTER_UPDATE, COUNTER_UPDATE );
          for ( auto update : updates ) {
            transition.addCounterAction( (CounterAction *) update );
          }
          vector<void *> resets =
              template_data.getPoints( RESET_STR, RESET_STR );
          for ( auto reset : resets ) {
            transition.addReset( *( (pair<int, int> *) reset ) );
            delete (pair<int, int> *) reset;
          }
          template_data.clearPoints(RESET_STR);

        } else if ( SYNCHRONISATION_STR == kind ) {
          cout << kind << ": " << ( *llit )->getValue() << endl;
        }
      }
    }
    return_transitions.push_back( transition );
  }
  return return_transitions;
}

void UppaalParser::parser_label( UppaalTemplateData &template_data,
                                 string              guards ) {
 // template_data.clearPoints();
  parseProblem( guards, &system_data, &template_data );
}

} // namespace graphsat
