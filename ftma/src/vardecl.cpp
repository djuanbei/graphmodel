#include"model/vardecl.h"

namespace graphsat{


int VarDecl::addClock( const string &n){
  assert(!contain( n) );
  int re=clock_id_map.size( )+1;
  clock_id_map.push_back( BaseDecl( n));
  return re;
}

int VarDecl::addInt( const string &n){
  assert(!contain( n) );
  
  int re=int_id_map.size( );
  int_id_map.push_back( BaseDecl( n));
  return re;
}

int VarDecl::addInt(const BaseDecl& ch){
  assert(!contain( ch.name) );
  int re=int_id_map.size( );
  int_id_map.push_back( ch);
  return re;
}

int VarDecl::addConstant( const string &n, const int v){
  assert(!contain( n) );
  int re=const_values.size( );
  const_values[ n]=v;
  return re;
  
}

int VarDecl::addChan( const ChanDecl& ch){
    assert(!contain( ch.name) );
    int re=chan_id_map.size( );
    chan_id_map.push_back( ch);
    return re;
}

int VarDecl::addType( const string & n, const TypeDefArray & type){
  assert(!contain( n) );
  int re=self_types.size( );
  self_types[ n]=type;
  return re;
}


int VarDecl::operator[ ] ( const string &k) const{
  assert(!contain( k) );
  return const_values.at( k);
}

bool VarDecl::contain( const string &n) const{
  for(auto &e:clock_id_map ){
    if( e.name == n){
      return true;
    }
  }
  for( auto &e: int_id_map){
    if( e.name == n){
      return true;
    }
  }
  for( auto &e:chan_id_map ){
    if( e.name== n){
      return true;
    }
  }
  
  for(auto &e: const_values ){
    if( e.first== n){
      return true;
    }
  }
  for( auto &e: self_types){
    if( e.first==n){
      return true;
    }
  }
  
  return false;
}
}
