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
using std::numeric_limits;
using std::pair;
using std::vector;

class Compression {
public:
  Compression( int len )
      : bounds( len )
      , nbit( len, sizeof( uint ) ) {
    for ( int i = 0; i < len; i++ ) {
      bounds[ i ].first  = numeric_limits<int>::min();
      bounds[ i ].second = numeric_limits<int>::max();
    }
    bitLen = len * sizeof( uint );
  }
  void setBound( int id, int low, int up ) {
    assert( up > low );
    bounds[ id ].first  = low;
    bounds[ id ].second = up;
    uint bound          = up - low;
    int  len            = log2( bound );
  }

  uint *encode( int *data ) const;

  int *decode( uint *data ) const;

private:
  vector<pair<int, int>> bounds;
  vector<int>            nbit;
  int                    bitLen = 0;
};
} // namespace graphsat

#endif
