/**
 * @file   datacompression.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Jun 10 11:25:22 2019
 *
 * @brief  compression to unsign int when known the lower bound and upper bound
 * of data
 *
 *
 */

#ifndef __DATA_COMPRESSION_H
#define __DATA_COMPRESSION_H
#include <cmath>
#include <limits>
#include <vector>
namespace graphsat {
typedef unsigned int uint;
using std::make_pair;
using std::numeric_limits;
using std::pair;
using std::vector;

class Compression {
public:
  Compression( int len )
      : row_len( len )
      , bounds( len )
      , domain( len, numeric_limits<uint>::max( )  )
      , uintValue( len )
      , intValue( len )
      , shift( len, true ) {
    for ( int i = 0; i < len; i++ ) {
      bounds[ i ].first  = numeric_limits<int>::min();
      bounds[ i ].second = numeric_limits<int>::max();
      //      shift[ i ]         = make_pair( i, 0 );
    }
  }

  void setBound( int id, int low, int up ) {
    assert( up > low );
    bounds[ id ].first  = low;
    bounds[ id ].second = up;
    uint bound          = up - low;
    domain[ id ] = bound;

  }
  void update() {
    fill(shift.begin( ),shift.end( ), false );
    
    uint dummy=numeric_limits<uint>::max( );
    for ( int i = 0; i < row_len; i++ ) {
      if(dummy<domain[ i ] ){
        shift[ i]=true;
        dummy=numeric_limits<uint>::max( );
      }
      dummy/=domain[ i ];
    }
  }

  uint *encode( int *data );

  int *decode( uint *data );

private:
  int row_len;
  /**
   * bounds[ i].frist  <= value[ i] < bounds[ i].second
   *
   */

  vector<pair<int, int>> bounds;
  vector<uint>            domain;
  vector<uint>           uintValue;
  vector<int>            intValue;
  vector<bool> shift;
};
} // namespace graphsat

#endif
