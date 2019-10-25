#include "io/uppaaldata.h"

namespace graphsat {

UppaalData::UppaalData() {
  init_loc = 0;
  name     = "";

  parent = NULL;

  next_counter_id = next_channel_id = 0;

  base_types.push_back( INT_T );
  base_types.push_back( PARAMETER_INT_T );
  base_types.push_back( REF_PARAMETER_INT_T );

  base_types.push_back( CLOCK_T );
  base_types.push_back( PARAMETER_CLOCK_T );
  base_types.push_back( REF_PARAMETER_CLOCK_T );

  //  base_types.push_back( BOOL_T );
  //  base_types.push_back( PARAMETER_BOOL_T );
  //  base_types.push_back( REF_PARAMETER_BOOL_T );

  base_types.push_back( CHAN_T );
  base_types.push_back( REF_PARAMETER_CHAN_T );

  base_types.push_back( SYSTEM_T );

  //  base_types.push_back( REF_PARAMETER_T );

  base_types.push_back( TEMPLATE_T );

  //  base_types.push_back( FORMAL_PARAMETER_T );

  base_types.push_back( CLOCK_CS_T );

  base_types.push_back( INT_CS_T );

  base_types.push_back( SELF_DEF_T );
}

TYPE_T UppaalData::getType( const string &name ) const {
  for ( vector<TYPE_T>::const_iterator it = base_types.begin();
        it != base_types.end(); it++ ) {
    if ( hasValue( *it, name ) ) {
      return *it;
    }
    if ( hasPointer( *it, name ) ) {
      return *it;
    }
  }

  if ( NULL != parent ) {
    return parent->getType( name );
  }

  return NO_T;
}
bool UppaalData::isConstant( const string &name ) const {
  if ( NO_T == getType( name ) ) {
    return false;
  }
  for ( vector<TYPE_T>::const_iterator it = base_types.begin();
        it != base_types.end(); it++ ) {
    if ( hasValue( *it, name ) ) {

      if ( getValue( *it, name ) == UN_DEFINE ) {
        return false;
      }
      return true;
    }
  }
  if ( NULL != parent ) {
    return parent->isConstant( name );
  }
  return false;
}

int UppaalData::getConstant( const string &name ) const {
  assert( isConstant( name ) );

  for ( vector<TYPE_T>::const_iterator it = base_types.begin();
        it != base_types.end(); it++ ) {

    if ( hasValue( *it, name ) ) {
      int value = getValue( *it, name );
      if ( value == UN_DEFINE ) {
        return UN_DEFINE;
      }
      return value;
    }
  }
  if ( NULL != parent ) {
    return parent->isConstant( name );
  }
  return UN_DEFINE;
}
int UppaalData::getVarNum( void ) const {
  int re = 0;
  for ( auto e : base_types ) {
    re += getTypeNum( e );
  }
  return re;
}

int UppaalData::getGlobalCounterId( const string &name ) {
  if ( counter_id_map.find( name ) != counter_id_map.end() ) {
    return counter_id_map.at( name );
  }

  if ( hasValue( INT_T, name ) ) {
    int re                 = getNextCounterId();
    counter_id_map[ name ] = re;
    return re;
  }

  if ( NULL != parent ) {
    return parent->getGlobalCounterId( name );
  }
  return NOT_FOUND;
}

int UppaalData::getGlobalChannelId( const string &name ) {
  if ( channel_id_map.find( name ) != channel_id_map.end() ) {
    return channel_id_map.at( name );
  }

  if ( hasValue( CHAN_T, name ) ) {
    int re                 = getNextChannelId();
    channel_id_map[ name ] = re;
    return re;
  }

  if ( NULL != parent ) {
    return parent->getGlobalChannelId( name );
  }
  return NOT_FOUND;
}

void UppaalData::addClockConstraint( int clock1_id, int clock2_id,
                                     COMP_OPERATOR op, int rhs,
                                     int parameter_id ) {
  if ( EQ == op ) {
    void *cs = new INT_TAS_t::CS_t( clock1_id, clock2_id, GE, rhs,
                                    parameter_id ); // x-y<= c
    addValue( CLOCK_CS_T, STRING( CLOCK_CS_T ), cs );

    cs = new INT_TAS_t::CS_t( clock1_id, clock2_id, LE, rhs,
                              parameter_id ); // x-y>= c
    addValue( CLOCK_CS_T, STRING( CLOCK_CS_T ), cs );

  } else {
    void *cs = new INT_TAS_t::CS_t( clock1_id, clock2_id, op, rhs,
                                    parameter_id ); // x op c
    addValue( CLOCK_CS_T, STRING( CLOCK_CS_T ), cs );
  }
}
} // namespace graphsat
