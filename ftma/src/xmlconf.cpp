
/**
 * @file   xmlconf.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Tue Apr 30 08:19:25 2019
 *
 * @brief  xml config file
 *
 *
 */

#include <fstream>
#include <iostream>

#include "io/rapidxml.hpp"

#include "io/xmlconf.h"

namespace graphsat {
using namespace rapidxml;
using namespace std;

static int parse( xml_node<> *root, XmlConfig *cfg ) {
  if ( root == NULL ) return 1;

  for ( rapidxml::xml_node<> *node = root->first_node(); node != NULL;
        node                       = node->next_sibling() ) {
    string name = trim( string( node->name() ) );
    if ( name.size() > 0 ) {
      XmlConfig *subitem = new XmlConfig();
      parse( node, subitem );
      cfg->addChild( name, subitem );
    }
  }
  string str = trim( string( root->name() ) );
  cfg->setTag( str );
  string value = trim( string( root->value() ) );
  cfg->setValue( value );

  for ( rapidxml::xml_attribute<char> *attr = root->first_attribute();
        attr != NULL; attr                  = attr->next_attribute() ) {
    char *attrname = attr->name();
    char *attrval  = attr->value();
    cfg->setAttr( trim( string( attrname ) ), trim( string( attrval ) ) );
  }

  return 0;
}

XmlConfig::XmlConfig( string input, bool b_fileinput ) {

  xml_document<> doc;

  if ( b_fileinput ) {
    ifstream fin( input );
    if ( !fin.good() ) {
      cout << "Can not open file: " << input << endl;
      fin.close();
      exit( 1 );
    }
    vector<char> buffer( ( istreambuf_iterator<char>( fin ) ),
                         istreambuf_iterator<char>() );
    buffer.push_back( '\0' );
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>( &buffer[ 0 ] );
    fin.close();
  } else {
    vector<char> buffer( input.begin(), input.end() );
    buffer.push_back( '\0' );
    doc.parse<0>( &buffer[ 0 ] );
  }

  xml_node<> *pRoot = doc.first_node();

  string name = trim( string( pRoot->name() ) );
  if ( name.size() < 1 ) {
    return;
  }
  if ( parse( pRoot, this ) == 1 ) {
    throw 0;
  }
}

int XmlConfig::setAttr( string a, string v ) {
  attr[ a ] = v;
  return 0;
}

int XmlConfig::setTag( const string t ) {
  tag = t;
  return 0;
}
int XmlConfig::setValue( const string v ) {
  value = v;
  return 0;
}

int XmlConfig::addChild( string key, const XmlConfig *c ) {
  children[ key ].push_back( c );
  return 0;
}

child_type XmlConfig::getChild( const string s ) const {
  child_type               ptr( NULL );
  XML_NODE::const_iterator it = children.find( s );

  if ( it != children.end() ) {
    ptr = &( it->second );
  }

  return ptr;
}

const XmlConfig *XmlConfig::getOneChild( string id ) const {
  const XmlConfig *        ptr( NULL );
  XML_NODE::const_iterator it = children.find( id );
  if ( it != children.end() ) {
    if ( !it->second.empty() ) {
      ptr = it->second[ 0 ];
    }
  }
  return ptr;
}

const XML_NODE *XmlConfig::getChildren() const { return &children; }

/**
 *
 *
 * @param str
 * @param val
 *
 * @return
 */
int convert( const string str, bool &val ) {

  if ( str[ 0 ] == 'T' || str[ 0 ] == 't' )
    val = true;
  else
    val = false;
  return 0;
}

int convert( const string str, int &val ) {
  val = atoi( str.c_str() );
  return 0;
}

int convert( const string str, double &val ) {
  val = atof( str.c_str() );
  return 0;
}

int convert( const string str, string &val ) {
  val = str;
  return 0;
}

// string trim( std::string s ) {
//   if ( s.empty() ) {
//     return s;
//   }
//   s.erase( 0, s.find_first_not_of( " " ) );
//   s.erase( s.find_last_not_of( " " ) + 1 );
//   return s;
// }

} // namespace graphsat
