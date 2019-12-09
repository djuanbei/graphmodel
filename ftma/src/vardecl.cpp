#include "model/vardecl.h"

namespace graphsat {

int VarDecl::addClock( const string &n ) {
  assert( !contain( n ) );
  int re = data.getTypeNum( CLOCK_T ) + 1;
  data.addValue( CLOCK_T, n, new BaseDecl( n ) );
  return re;
}

int VarDecl::addInt( const string &n ) {
  assert( !contain( n ) );

  int re = data.getTypeNum( INT_T );
  data.addValue( INT_T, n, new BaseDecl( n ) );
  return re;
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

int VarDecl::addType( const string &n, const TypeDefArray &type ) {
  assert( !contain( n ) );
  int re          = self_types.size();
  self_types[ n ] = type;
  return re;
}

int VarDecl::operator[]( const string &k ) const {
  assert( !contain( k ) );
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
    if ( e.first == n ) {
      return true;
    }
  }

  return false;
}
int VarDecl::getCounterNumber() const { return getTypeNum( INT_T ); }
int VarDecl::getClockNumber() const { return getTypeNum( CLOCK_T ); }

int VarDecl::getTypeNum( const int type ) const {
  int                                  re     = 0;
  vector<pair<string, vector<void *>>> clocks = data.getValue( type );
  for ( auto &e : clocks ) {
    re += ( (BaseDecl *) e.second[ 0 ] )->num;
  }
  return re;
}

int VarDecl::getStart( const string &key ) const {
  int                                  re       = 0;
  vector<pair<string, vector<void *>>> counters = data.getValue( INT_T );
  for ( vector<pair<string, vector<void *>>>::const_iterator it =
            counters.begin();
        it != counters.end(); it++ ) {
    if ( ( (BaseDecl *) it->second[ 0 ] )->name == key ) {
      return re;
    }

    re += ( (BaseDecl *) it->second[ 0 ] )->num;
  }
  assert( false );
  return -1;
}
} // namespace graphsat
