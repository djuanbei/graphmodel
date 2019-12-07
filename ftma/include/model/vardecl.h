/**
 * @file   vardecl.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Dec  6 20:13:19 2019
 * 
 * @brief  variable dec function
 * 
 * 
 */

#ifndef VAR_DECL_H
#define VAR_DECL_H
#include<string>
#include <map>


#include "arraytype.h"
#include "channel.h"

namespace graphsat{

using namespace std;
struct BaseDecl{
  string name;
  int num;
  BaseDecl( ){
    num=0;
  }
  BaseDecl(const string& n):name( n), num( 1){
  }  
};


struct ChanDecl: public BaseDecl{
  CHANNEL_TYPE type;
  ChanDecl(const string &n ):BaseDecl( n){
    type=ONE2ONE_CH;
  }
  
};


class VarDecl{
 public:
  virtual int addClock( const std::string &n);
  virtual int addInt( const string &n);

  virtual int addInt(const BaseDecl& ch);

  virtual int addChan(  const ChanDecl& ch);
  
      
  virtual int addConstant( const string &n, const int v);

  virtual int addType( const string & n, const TypeDefArray & type);


  

  bool contain( const string& n)const;

  int operator[ ] ( const string &k) const;
  
 protected:
  
  vector< BaseDecl> clock_id_map;
  vector< BaseDecl> int_id_map;
  vector<ChanDecl> chan_id_map;

  
  map<string, int>  const_values;
  map<string,TypeDefArray> self_types;
};
}

#endif 
