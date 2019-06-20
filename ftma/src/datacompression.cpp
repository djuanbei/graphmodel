#include "util/datacompression.h"

namespace graphsat {

uint *Compression::encode( int *data ) {
  fill( uintValue.begin(), uintValue.end(), 0 );

  for ( int i = 0; i < row_len; i++ ) {
    assert( data[ i ] >= bounds[ i ].first );
    assert( data[ i ] <= bounds[ i ].second );
    uint temp = data[ i ] - bounds[ i ].first;
    temp <<= shift[ i ].second;
    uintValue[ shift[ i ].first ] += temp;
  }
  return &( uintValue[ 0 ] );
}

int *Compression::decode( uint *data ) {
  for ( int i = 0; i < row_len; i++ ) {
    uint temp = data[ shift[ i ].first ];
    int  high = shift[ i ].second + nbit[ i ];
    temp <<= sizeof( uint ) - high;
    temp >>= ( sizeof( uint ) - nbit[ i ] );
    intValue[ i ] = temp;
  }
  return &( intValue[ 0 ] );
}

} // namespace graphsat
