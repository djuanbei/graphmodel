#include "io/uppaaldata.h"

namespace graphsat {

UppaalData::UppaalData() {
  init_loc       = 0;
  name           = "";

  parent         = NULL;
  startId=0;
  
  TYPE_MAP( type_to_name, INT_T );
  base_types.push_back( INT_T );
  TYPE_MAP( type_to_name, CLOCK_T );
  base_types.push_back( CLOCK_T );
  TYPE_MAP( type_to_name, BOOL_T );
  base_types.push_back( BOOL_T );

  TYPE_MAP( type_to_name, CHAN_T );
  TYPE_MAP( type_to_name, URGENT_CHAN_T );
  TYPE_MAP( type_to_name, BROADCAST_CHAN_T );
  base_types.push_back( CHAN_T );

  type_to_name[ SYSTEM_T ] = STRING( SYSTEM_T );
  base_types.push_back( SYSTEM_T );

  type_to_name[ PARAMETER_T ] = STRING( PARAMETER_T );
  base_types.push_back( PARAMETER_T );
  

  type_to_name[ REF_PARAMETER_T ] = STRING( REF_PARAMETER_T );
  base_types.push_back( REF_PARAMETER_T );
  
  type_to_name[ TEMPLATE_T ] = STRING( TEMPLATE_T );
  base_types.push_back( TEMPLATE_T );

  type_to_name[ LOCATION_T ] = STRING( LOCATION_T );

  type_to_name[ CLOCK_CS_T ] = STRING( CLOCK_CS_T );
  base_types.push_back( CLOCK_CS_T );
  type_to_name[ INT_CS_T ] = STRING( INT_CS_T );
  base_types.push_back( INT_CS_T );

  type_to_name[ INT_UPDATE_T ] = type_to_name[ INT_UPDATE_T ];
  type_to_name[ RESET_T ]      = type_to_name[ RESET_T ];

  type_to_name[ NO_T ] = STRING( NO_T );
}



TYPE_T UppaalData::getType( const string &name ) const {
  for ( vector<TYPE_T>::const_iterator it = base_types.begin();
        it != base_types.end(); it++ ) {
    if ( hasPointer( *it, name ) ) {
      return *it;
    }
    if ( hasValue( *it, name ) ) {
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
int UppaalData::getVarNum( void ) const{
  int re=0;
  for( auto e: base_types){
    re+=getTypeNum( e);
  }
  return re;
}

} // namespace graphsat
