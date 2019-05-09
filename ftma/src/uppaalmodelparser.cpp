#include "io/uppaalmodelparser.h"
#include "util/dbmutil.hpp"
#include <cassert>

namespace graphsat {

UppaalParser::UppaalParser( const string &xmlfile ) {
  XmlConfig  xmldoc( xmlfile );
  child_type declarations = xmldoc.getChild( DECLARATION_STR );
  child_type templates    = xmldoc.getChild( TEMPLATE_STR );
  child_type system       = xmldoc.getChild( SYSTEM_STR );
  child_type queries      = xmldoc.getChild( QUERIES_STR );
  parserDeclaration( declarations );
  parserTemplate( templates );
  parserSystem( system );
  parserQuery( queries );
}

int UppaalParser::parserDeclaration( child_type declarations ) {
  if ( NULL == declarations ) {
    return 0;
  }
  for ( child_iterator it = declarations->begin(); it != declarations->end();
        it++ ) {
    child_type name    = ( *it )->getChild( DECLARATION_STR );
    string     content = ( *it )->getValue();
    cout << content << endl;
  }

  return 0;
}

int UppaalParser::parserTemplate( child_type templates ) {

  if ( NULL == templates ) {
    return 0;
  }

  for ( child_iterator it = templates->begin(); it != templates->end(); it++ ) {

    const XmlConfig *nameConf = ( *it )->getOneChild( NAME_STR );
    if ( NULL != nameConf ) {
      cout << nameConf->getValue() << endl;
    }
    const XmlConfig *parameter = ( *it )->getOneChild( PARAMETER_STR );

    if ( NULL != parameter ) {
      cout << "parameter: " << parameter->getValue() << endl;
    }

    const XmlConfig *declaration = ( *it )->getOneChild( DECLARATION_STR );
    if ( NULL != declaration ) {
      cout << "declaration: " << declaration->getValue() << endl;
    }

    child_type locations = ( *it )->getChild( LOCATION_STR );

    vector<L> ls = parserLocation( locations );

    int locationID = 0;

    child_type transitions = ( *it )->getChild( TRANSITION_STR );

    vector<T> es = parserTransition( transitions );
  }

  return 0;
}

int UppaalParser::parserSystem( child_type system ) { return 0; }

int UppaalParser::parserQuery( child_type queries ) { return 0; }

vector<L> UppaalParser::parserLocation( child_type locations ) {
  vector<L> ls;

  for ( child_iterator lit = locations->begin(); lit != locations->end();
        lit++ ) {

    cout << "location" << endl;
    string idstr = ( *lit )->getAttrValue( ID_STR );
    assert( idstr != "" );
    int locationID       = locationMAP.size();
    locationMAP[ idstr ] = locationID;
    L temp( locationID );

    child_type labels = ( *lit )->getChild( LABEL_STR );
    if ( NULL != labels ) {
      for ( child_iterator llit = labels->begin(); llit != labels->end();
            llit++ ) {
        string kind = "";
        kind        = ( *llit )->getAttrDefault( KIND_STR, kind );

        if ( kind == INVARIANT_STR ) {
          string guard = ( *llit )->getValue();
          parserConstraints( guard );
        }
        if ( kind != "" ) {
          cout << "kind: " << kind << endl;
          cout << "value: " << ( *llit )->getValue() << endl;
        }
      }
    }
    ls.push_back( temp );
  }
  return ls;
}

vector<T> UppaalParser::parserTransition( child_type transitions ) {
  vector<T> es;
  for ( child_iterator tit = transitions->begin(); tit != transitions->end();
        tit++ ) {
    cout << "transition" << endl;
    const XmlConfig *source    = ( *tit )->getOneChild( SOURCE_STR );
    string           sourceRef = source->getAttrValue( REF_STR );

    const XmlConfig *target    = ( *tit )->getOneChild( TARGET_STR );
    string           targetRef = target->getAttrValue( REF_STR );

    int sourceId = locationMAP[ sourceRef ];
    int targetId = locationMAP[ targetRef ];

    T temp( sourceId, targetId );

    child_type labels = ( *tit )->getChild( LABEL_STR );
    if ( NULL != labels ) {

      for ( child_iterator llit = labels->begin(); llit != labels->end();
            llit++ ) {
        string kind = "";
        kind        = ( *llit )->getAttrValue( KIND_STR );
        if ( GUARD_STR == kind ) {
          string guard = ( *llit )->getValue();
          cout << kind << ": " << guard << endl;
          parserConstraints( guard );
        } else if ( ASSIGNMENT_STR == kind ) {
          cout << kind << ": " << ( *llit )->getValue() << endl;
        } else if ( SYNCHRONISATION_STR == kind ) {
          cout << kind << ": " << ( *llit )->getValue() << endl;
        }
      }
    }
    es.push_back( temp );
  }
  return es;
}


vector<CS> UppaalParser::parserConstraints( string guards ) {
  string         delimeter = "&&";
  vector<string> guardvec  = splitStr( guards, delimeter );
  vector<CS> re;

  
  
  return re;
}

} // namespace graphsat
