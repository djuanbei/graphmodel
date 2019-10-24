#include "io/uppaaldata.h"

namespace graphsat {

UppaalData::UppaalData() {
  init_loc = 0;
  name     = "";

  parent  = NULL;
  startId = 0;

  base_types.push_back( INT_T );

  base_types.push_back( CLOCK_T );

  base_types.push_back( BOOL_T );

  base_types.push_back( CHAN_T );

  base_types.push_back( SYSTEM_T );

  base_types.push_back( PARAMETER_T );

  base_types.push_back( REF_PARAMETER_T );

  base_types.push_back( TEMPLATE_T );

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

int UppaalData::getGlobalId( const string &name ) const {
  int re = startId;
  for ( vector<TYPE_T>::const_iterator it = base_types.begin();
        it != base_types.end(); it++ ) {
    int typeId = getId( *it, name );
    if ( typeId != NOT_FOUND ) {
      return re + typeId;
    }
    re += getTypeNum( *it );
  }
  if ( NULL != parent ) {
    return parent->getGlobalId( name );
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
