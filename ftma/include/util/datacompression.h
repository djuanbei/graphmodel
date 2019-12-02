/**
 * @file   datacompression.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Jun 10 11:25:22 2019
 *
 * @brief  compression to unsign int when known the lower bound and upper bound
 * of data [ low, up]
 *
 *
 */

#ifndef __DATA_COMPRESSION_H
#define __DATA_COMPRESSION_H
#include <cmath>
#include <limits>
#include <vector>
namespace graphsat {
  typedef unsigned int UINT;
  using std::make_pair;
  using std::numeric_limits;
  using std::pair;
  using std::vector;
  template <typename T> class Compression {
  public:
    Compression() { row_len = 0; }
    Compression( int len )
    : row_len( len )
    , bounds( len )
    , domain( len, numeric_limits<UINT>::max() )
    , shift( len, true ) {
      for ( int i = 0; i < len; i++ ) {
        bounds[ i ].first  = numeric_limits<int>::min();
        bounds[ i ].second = numeric_limits<int>::max();
      }
      update();
    }
    
    void setBound( int id, int low, int up ) {
      assert( up >= low );
      bounds[ id ].first  = low;
      bounds[ id ].second = up;
      
      UINT bound   = up - low + 1;
      domain[ id ] = bound;
      update();
    }
    int getCompressionSize() const { return compressionSize; }
    
    void encode( const T *const data, UINT *out ) const {
      fill( out, out + row_len, 0 );
      int  j    = 0;
      UINT base = 1;
      for ( int i = 0; i < row_len; i++ ) {
        T temp = data[ i ];
        // if(temp<bounds[ i ].first  ){
        //   temp=bounds[ i ].first  ;
        // }
        // if(temp>bounds[ i ].second  ){
        //   temp=bounds[ i ].second;
        // }
        assert( data[ i ] >= bounds[ i ].first );
        assert( data[ i ] <= bounds[ i ].second );
        
        if ( shift[ i ] ) {
          j++;
          base = 1;
        }
        out[ j ] += ( temp - bounds[ i ].first ) * base;
        base *= domain[ i ];
      }
    }
    
    void decode( const UINT *const data, T *out ) const {
      int  j     = 0;
      UINT dummy = data[ 0 ];
      for ( int i = 0; i < row_len; i++ ) {
        if ( shift[ i ] ) {
          j++;
          dummy = data[ j ];
        }
        out[ i ] = ( dummy % domain[ i ] ) + bounds[ i ].first;
        dummy /= domain[ i ];
      }
    }
    
  private:
    int row_len;
    /**
     * bounds[ i].frist  <= value[ i] <= bounds[ i].second
     *
     */
    
    vector<pair<T, T>> bounds;
    vector<UINT>       domain;
    
    vector<bool> shift;
    int          compressionSize;
    
    void update() {
      fill( shift.begin(), shift.end(), false );
      compressionSize = 1;
      UINT dummy      = numeric_limits<UINT>::max();
      for ( int i = 0; i < row_len; i++ ) {
        if ( dummy < domain[ i ] ) {
          shift[ i ] = true;
          dummy      = numeric_limits<UINT>::max();
          compressionSize++;
        }
        dummy /= domain[ i ];
      }
    }
  };
  
  template <typename C> class StateConvert {
    
  public:
    StateConvert() {
      head_length      = 0;
      head_comp        = 0;
      compression_size = 0;
    }
    StateConvert( int hLen, int bLen, Compression<C> hCom, Compression<C> bCom ) {
      head_length = hLen;
      
      com_head_length = hCom.getCompressionSize();
      
      compression_size = hCom.getCompressionSize() + bCom.getCompressionSize();
      head_comp        = hCom;
      body_comp        = bCom;
    }
    
    void encode( const C *data, UINT *out ) const {
      head_comp.encode( data, out );
      body_comp.encode( data + head_length, out + com_head_length );
    }
    
    void decode( const UINT *data, C *out ) const {
      head_comp.decode( data, out );
      body_comp.decode( data + com_head_length, out + head_length );
    }
    int getCompressionSize() const { return compression_size; }
    
    int getCompressionHeadSize() const { return head_comp.getCompressionSize(); }
    
  private:
    int head_length;
    int com_head_length;
    
    int            compression_size;
    Compression<C> head_comp;
    Compression<C> body_comp;
  };
  
  
} // namespace graphsat

#endif
