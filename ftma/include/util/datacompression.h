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
using std::make_pair;
using std::vector;


class Compression {
public:
  Compression( int len )
      :row_len( len), bounds( len )
      , nbit( len, sizeof( uint ) ), uintValue( len),intValue( len), shift( len) {
    for ( int i = 0; i < len; i++ ) {
      bounds[ i ].first  = numeric_limits<int>::min();
      bounds[ i ].second = numeric_limits<int>::max();
      shift[ i]=make_pair( i, 0);
    }
  }
  
  void setBound( int id, int low, int up ) {
    assert( up > low );
    bounds[ id ].first  = low;
    bounds[ id ].second = up;
    uint bound          = up - low;
    int  len            = log2( bound );
    uint temp=1<<len;
    if( bound>temp){
      len++;
    }
    nbit[ id]=len;
  }
  void update( ){
    int uintId=0;
    int  temp=0; 
    int uint_size=sizeof( uint);
    
    for( int i=0; i<row_len; i++ ){
      if( temp+nbit[ i]>=uint_size){
        uintId++;
        temp=0; 
      }
      shift[ i]=make_pair( uintId, temp);
      temp+=nbit[i];
    }
  }

  uint *encode( int *data ) ;

  int *decode( uint *data ) ;

private:
  int row_len;
  vector<pair<int, int>> bounds;
  vector<int>            nbit;
  vector<uint> uintValue;
  vector<int> intValue;
  vector<pair<int, int> > shift;


};
} // namespace graphsat

#endif
