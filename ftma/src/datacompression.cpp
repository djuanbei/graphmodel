#include "util/datacompression.h"

namespace graphsat {

uint *Compression::encode( int *data ) {
  fill( uintValue.begin(), uintValue.end(), 0 );
  int j=0;
  uint base=1;
  for ( int i = 0; i < row_len; i++ ) {
    assert( data[ i ] >= bounds[ i ].first );
    assert( data[ i ] < bounds[ i ].second );
    if(shift[ i] ){
      j++;
      base=1;
    }
    uintValue[ j]+=(data[ i]-bounds[ i].first)*base;
    base*=domain[i];
  }
  return &( uintValue[ 0 ] );
}

int *Compression::decode( uint *data ) {
  int j=0;
  uint dummy=data[ 0];
  for ( int i = 0; i < row_len; i++ ) {
    if( shift[ i]){
      j++;
      dummy=data[ j];
    }
    
    intValue[ i ] = (dummy% domain[ i])+bounds[ i].first;
    
    dummy/= domain[ i];

  }
  return &( intValue[ 0 ] );
}

} // namespace graphsat
