#include "io/uppaalmodelparser.h"


namespace graphsat{

UppaalParser::UppaalParser(const string& xmlfile ){
  XmlConfig xmldoc(xmlfile );
  const vector<const  XmlConfig*>* declarations=xmldoc.getChild(DECLARATION_STR );
  const vector<const XmlConfig*>* templates=xmldoc.getChild( TEMPLATE_STR);
  const  vector<const XmlConfig*> * system=xmldoc.getChild( SYSTEM_STR);
  const vector<const XmlConfig*> * queries=xmldoc.getChild(QUERIES_STR);
  if( NULL!= declarations){
    for(vector<const  XmlConfig*>::const_iterator it=declarations->begin( ); it!= declarations->end( ); it++  ){
      cout<<(*it)->getValue( )<<endl;
    }
  }

  
}



}
