#include "io/uppaalmodelparser.h"
#include "util/dbmutil.hpp"
#include <cassert>

namespace graphsat {

UppaalParser::UppaalParser( const string &xmlfile ) {
  XmlConfig  xmldoc( xmlfile );
  child_type declarations = xmldoc.getChild( DECLARATION_STR );
  child_type templates    = xmldoc.getChild( TEMPLATE_STR );
  // child_type system       = xmldoc.getChild( SYSTEM_STR );
  // child_type queries      = xmldoc.getChild( QUERIES_STR );
  parserDeclaration( declarations );
  parserTemplate( templates );
  // parserSystem( system );
  // parserQuery( queries );
  int counter_num = data.getTypeNum( COUNTER_STR );

  for ( int i = 0; i < counter_num; i++ ) {

    Counter                          counter( 0, 1000000 );
    const pair<string, vector<int>> &hh = data.getValue( COUNTER_STR, i );
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
    // child_type name    = ( *it )->getChild( DECLARATION_STR );
    string content = ( *it )->getValue();
    parseProblem( content, &data, &data );
  }

  return 0;
}

int UppaalParser::parserTemplate( child_type templates ) {

  if ( NULL == templates ) {
    return 0;
  }

  for ( child_iterator it = templates->begin(); it != templates->end(); it++ ) {
    UppaalData templateData;

    // XML_P nameConf  = ( *it )->getOneChild( NAME_STR );
    XML_P parameter = ( *it )->getOneChild( PARAMETER_STR );
    if ( NULL != parameter ) {
      string         para_content = parameter->getValue();
      vector<string> terms        = splitStr( para_content, " " );
      templateData.addValue( PARAMETER_STR, terms.back() );
    }

    XML_P declaration = ( *it )->getOneChild( DECLARATION_STR );

    if ( NULL != declaration ) {
      string dec_content = declaration->getValue();
      parseProblem( dec_content, &data, &templateData );
    }

    child_type location_comps = ( *it )->getChild( LOCATION_STR );

    vector<typename INT_TAS_t::L_t> locations =
        parserLocation( templateData, location_comps );

    XML_P initConf = ( *it )->getOneChild( INIT_STR );

    if ( NULL != initConf ) {
      string loc = initConf->getAttrValue( REF_STR );
      templateData.setInitialLoc( templateData.getId( LOCATION_STR, loc ) );
    }

    child_type transition_comps = ( *it )->getChild( TRANSITION_STR );

    vector<typename INT_TAS_t::T_t> transitions =
        parserTransition( templateData, transition_comps );

    typename INT_TAS_t::TA_t ta( locations, transitions,
                                 templateData.getInitialLoc(),
                                 templateData.getTypeNum( CLOCK_STR ) );
    if ( NULL != parameter ) {
      string         para_content = parameter->getValue();
      vector<string> terms        = splitStr( para_content, " " );
      int            size         = (int) terms.size();
      size -= 2;
      string      global_array = terms[ size ];
      vector<int> intArray     = data.getIntArray( global_array );
      for ( auto e : intArray ) {
        INT_TAS_t::TA_t temp = ta;
        temp.addOnePara( e );
        sys += temp;
      }
    } else {
      sys += ta;
    }
  }

  return 0;
}

int UppaalParser::parserSystem( child_type system ) { return 0; }

int UppaalParser::parserQuery( child_type queries ) { return 0; }

vector<INT_TAS_t::L_t> UppaalParser::parserLocation( UppaalData &templateData,
                                                     child_type  locations ) {
  vector<INT_TAS_t::L_t> return_locations;

  for ( child_iterator lit = locations->begin(); lit != locations->end();
        lit++ ) {

    string idStr = ( *lit )->getAttrValue( ID_STR );
    assert( idStr != "" );
    templateData.addValue( LOCATION_STR, idStr );

    int locationID = templateData.getId( LOCATION_STR, idStr );

    INT_TAS_t::L_t location( locationID );

    child_type labels = ( *lit )->getChild( LABEL_STR );
    if ( NULL != labels ) {
      for ( child_iterator llit = labels->begin(); llit != labels->end();
            llit++ ) {
        string kind = "";
        kind        = ( *llit )->getAttrDefault( KIND_STR, kind );

        if ( kind == INVARIANT_STR ) {
          string invariants = ( *llit )->getValue();
          parserLabel( templateData, invariants );
          vector<void *> cons = templateData.getPoints( CLOCK_CS );
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

vector<INT_TAS_t::T_t>
    UppaalParser::parserTransition( UppaalData &templateData,
                                    child_type  transitions ) {
  vector<INT_TAS_t::T_t> return_transitions;
  for ( child_iterator tit = transitions->begin(); tit != transitions->end();
        tit++ ) {

    XML_P  source    = ( *tit )->getOneChild( SOURCE_STR );
    string sourceRef = source->getAttrValue( REF_STR );

    XML_P  target    = ( *tit )->getOneChild( TARGET_STR );
    string targetRef = target->getAttrValue( REF_STR );

    int sourceId = templateData.getId( LOCATION_STR, sourceRef );
    int targetId = templateData.getId( LOCATION_STR, targetRef );

    INT_TAS_t::T_t transition( sourceId, targetId );

    child_type labels = ( *tit )->getChild( LABEL_STR );
    if ( NULL != labels ) {

      for ( child_iterator llit = labels->begin(); llit != labels->end();
            llit++ ) {
        string kind = "";
        kind        = ( *llit )->getAttrValue( KIND_STR );
        if ( GUARD_STR == kind ) {
          string guard = ( *llit )->getValue();
          parserLabel( templateData, guard );
          vector<void *> cons = templateData.getPoints( CLOCK_CS );
          for ( auto cs : cons ) {
            transition += *( (INT_TAS_t::CS_t *) ( cs ) );
          }
          vector<void *> counterCs = templateData.getPoints( COUNTER_CS );
          for ( auto cs : counterCs ) {
            transition.addCounterCons( (CounterConstraint *) cs );
          }

        } else if ( ASSIGNMENT_STR == kind ) {
          string assign_statement = ( *llit )->getValue();
          parserLabel( templateData, assign_statement );
          vector<void *> updates = templateData.getPoints( COUNTER_UPDATE );
          for ( auto update : updates ) {
            transition.addCounterAction( (CounterAction *) update );
          }
          vector<void *> resets = templateData.getPoints( RESET_STR );
          for ( auto reset : resets ) {
            transition.addReset( *( (pair<int, int> *) reset ) );
            delete (pair<int, int> *) reset;
          }

        } else if ( SYNCHRONISATION_STR == kind ) {
          cout << kind << ": " << ( *llit )->getValue() << endl;
        }
      }
    }
    return_transitions.push_back( transition );
  }
  return return_transitions;
}

void UppaalParser::parserLabel( UppaalData &templateData, string guards ) {
  templateData.clearPoints();
  parseProblem( guards, &data, &templateData );
}

} // namespace graphsat
