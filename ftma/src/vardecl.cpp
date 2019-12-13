#include "model/vardecl.h"

namespace graphsat {

Argument VarDecl::addClock( const string &n ) {
  assert( !contain( n ) );
  int re = data.getTypeNum( CLOCK_T ) + 1;
  data.addValue( CLOCK_T, n, new BaseDecl( n ) );
  Argument  temp( CONST_ARG, re);
  temp.name=n;
  return temp;
}

int VarDecl::addInt( const string &n, int num ) {
  BaseDecl b( n );
  b.num=num;
  return addInt( b );
}

int VarDecl::addInt( const string &name, int num, int  low, int high ){
  BaseDecl b( name, num, low, high);
  return addInt( b );
}

int VarDecl::addInt( const BaseDecl &ch ) {
  assert( !contain( ch.name ) );
  int re = data.getTypeNum( INT_T );
  data.addValue( INT_T, ch.name, new BaseDecl( ch ) );
  return re;
}

int VarDecl::addConstant( const string &n, const int v ) {
  assert( !contain( n ) );
  int re            = const_values.size();
  const_values[ n ] = v;
  return re;
}

int VarDecl::addChan( const ChanDecl &ch ) {
  assert( !contain( ch.name ) );
  int re = data.getTypeNum( CHAN_T );
  data.addValue( CHAN_T, ch.name, new ChanDecl( ch ) );
  return re;
}
int VarDecl::addChan( const string &name, int num, CHANNEL_TYPE type ) {
  ChanDecl dummy( name);
  dummy.type=type;
  dummy.num=num;
  return addChan( dummy);
}

int VarDecl::addFun( const string & name, shared_ptr<Function> fun ){
  int re=functions.size( );
  functions[ name]=fun;
  return re;
}

shared_ptr<Function> VarDecl::getFun( const string &name) const{
  return functions.at( name);
}

const map<string, shared_ptr<Function> > & VarDecl::getFuns() const{
  return functions;
}

int VarDecl::addType( const string &n, const TypeDefArray &type ) {
  assert( !contain( n ) );
  int re          = self_types.size();
  self_types.push_back(type );
  return re;
}
int VarDecl::addType( const string &n, const int low, const int high){
  assert( !contain( n ) );
  int re          = self_types.size();
  TypeDefArray t( n, 0, high ); // typedef int[ 0,N-1] id_t;
  self_types.push_back( t);
  return re;
}

vector<BaseDecl> VarDecl::getInts( ) const{
  vector<pair<string, vector<void*>>> temp=data.getValue( INT_T);
  vector<BaseDecl> re;
  for(auto &e: temp ){
    BaseDecl dummy= *((BaseDecl*)e.second[ 0]);
    if(1==dummy.num  ){
      re.push_back( dummy);
    }else{
      for( int i=0; i<dummy.num; i++ ){
        BaseDecl t=dummy;
        t.name+=to_string( i);
        re.push_back(t );
      }
    }
  }
  return re;
}

int VarDecl::operator[]( const string &k ) const {
  assert(contain( k ) );
  return const_values.at( k );
}

bool VarDecl::contain( const string &n ) const {
  if ( data.hasValue( CLOCK_T, n ) ) {
    return true;
  }
  if ( data.hasValue( INT_T, n ) ) {

    return true;
  }

  if ( data.hasValue( CHAN_T, n ) ) {
    return true;
  }

  for ( auto &e : const_values ) {
    if ( e.first == n ) {
      return true;
    }
  }
  for ( auto &e : self_types ) {
    if ( e.getName( ) == n ) {
      return true;
    }
  }

  return false;
  
}
int VarDecl::getChanNumber( ) const{
  return getTypeNumber( CHAN_T);
}
int VarDecl::getCounterNumber() const { return getTypeNumber( INT_T ); }
int VarDecl::getClockNumber() const { return getTypeNumber( CLOCK_T ); }

int VarDecl::getTypeNumber( const int type ) const {
  int                                  re     = 0;
  vector<pair<string, vector<void *>>> clocks = data.getValue( type );
  for ( auto &e : clocks ) {
    re += ( (BaseDecl *) e.second[ 0 ] )->num;
  }
  return re;
}

vector<string> VarDecl::getKeys(const TYPE_T type ) const{
  vector<string> re;
  vector<pair<string, vector<void *>>> counters = data.getValue( type );
  for ( vector<pair<string, vector<void *>>>::const_iterator it =
            counters.begin();
        it != counters.end(); it++ ) {
    re.push_back(( (BaseDecl *) it->second[ 0 ] )->name);
  }
  return re;
}

int VarDecl::getKeyStart( const TYPE_T type, const string &key ) const {
  int                                  re       = 0;
  vector<pair<string, vector<void *>>> counters = data.getValue( type );
  for ( vector<pair<string, vector<void *>>>::const_iterator it =
            counters.begin();
        it != counters.end(); it++ ) {
    if ( ( (BaseDecl *) it->second[ 0 ] )->name == key ) {
      return re;
    }
    re += ( (BaseDecl *) it->second[ 0 ] )->num;
  }
  //assert( false );
  return -1;
}
} // namespace graphsat
