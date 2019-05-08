/**
 * @file   xmlutil.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Apr 29 19:55:56 2019
 *
 * @brief  xml util function
 *
 *
 */
#ifndef XML_UTIL_HPP
#define XML_UTIL_HPP

#include <map>
#include <string>
#include <vector>

namespace graphsat {

using namespace std;

typedef map<string, string> FXML_ATTRLIST;
class XmlConfig;
typedef map<string, vector<const XmlConfig *> >  XML_NODE;


using namespace std;

int convert( const string, bool & );
int convert( const string, int & );
int convert( const string, double & );
int convert( const string, string & );

/**
 * dek=delete start spaces and  last spaces
 * 
 */

string trim(std::string s);


class XmlConfig {

public:
  XmlConfig()
      : tag( "" ) {}

  ~XmlConfig() {
    for( XML_NODE::iterator it= children.begin( ); it!= children.end( ); it++){
      
      for ( vector<const XmlConfig *>::iterator lit = it->second.begin();
            lit != it->second.end(); lit++ ) {
        delete ( *lit );
      }
    }
  }

  XmlConfig( string, bool b_fileinput = true );
  XmlConfig( const XmlConfig &other ) {
    tag = other.tag;
    for( XML_NODE::const_iterator it= other.children.begin( ); it!= other.children.end( ); it++){
      for ( vector<const XmlConfig *>::const_iterator lit = it->second.begin();
            lit != it->second.end(); lit++ ) {
        children[it->first].push_back( new XmlConfig( **lit ) );
      }
    }
        

    attr = other.attr;
  }

  const XML_NODE *getChildren() const;

  const vector<const XmlConfig *>  *getChild(string id) const;


  template <typename T> int getAttr( const string &a, T &out ) const {
    map<string, string>::const_iterator it = attr.find( a );
    if ( it != attr.end() ) {
      T val;
      convert( it->second, val );
      out = val;
      return 0;
    } else {
      cout << "Can't find attrbute '" << a << "' in Element tag '" << tag
           << "'!" << endl;
      exit( 1 );
    }
  }
  template <typename T> int getAttr( const char *a, T &out ) const {
    string str_a = string( a );
    return getAttr( str_a, out );
  }

  template <typename T> T getAttr( const string &a ) const {
    map<string, string>::const_iterator it = attr.find( a );
    if ( it != attr.end() ) {
      T val;
      convert( it->second, val );
      return val;
    } else {
      cout << "Can't find attrbute '" << a << "' in Element tag '" << tag
           << "'!" << endl;
      exit( 1 );
    }
  }

  template <typename T> T getAttr( const char *a ) const {
    string str_a = string( a );
    return getAttr<T>( str_a );
  }

  template <typename T> T getAttrDefault( const string &a, const T df ) const {
    map<string, string>::const_iterator it = attr.find( a );
    if ( it != attr.end() ) {
      T val;
      convert( it->second, val );
      return val;
    } else {
      return df;
    }
  }

  template <typename T> T getAttrDefault( const char *a, const T df ) const {
    string str_a = string( a );
    return getAttrDefault( str_a, df );
  }

  inline const FXML_ATTRLIST *getAttrList() const { return &attr; }
  inline string               getTag() const { return tag; }
  inline string getValue( ) const{return value;}

  int setAttr( const string, const string );
  int addChild(string key, const XmlConfig * );
  int setTag( const string );
  int setValue(  const string);

private:
  string                    tag;
  string value;
  XML_NODE children;
  FXML_ATTRLIST             attr;
};

} // namespace graphsat

#endif
