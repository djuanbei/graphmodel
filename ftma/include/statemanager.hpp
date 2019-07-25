/**
 * @file   state.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat May 18 09:28:22 2019
 *
 * @brief  state
 *
 *
 */

#ifndef __STATE_MANAGER_H
#define __STATE_MANAGER_H
#include <vector>

#include "discretestate.hpp"
#include "model/ta.hpp"
#include "parameter.h"
#include "util/datacompression.h"

namespace graphsat {

using std::vector;

template <typename C> class StateConvert {

public:
  StateConvert() {
    headLen         = 0;
    headComp        = 0;
    compressionSize = 0;
  }
  StateConvert( int hLen, int bLen, Compression<C> hCom, Compression<C> bCom ) {
    headLen = hLen;

    comHeadLen = hCom.getCompressionSize();

    compressionSize = hCom.getCompressionSize() + bCom.getCompressionSize();
    headComp        = hCom;
    bodyComp        = bCom;
  }

  void encode( const C *data, UINT *out ) const {
    headComp.encode( data, out );
    bodyComp.encode( data + headLen, out + comHeadLen );
  }

  void decode( const UINT *data, C *out ) const {
    headComp.decode( data, out );
    bodyComp.decode( data + comHeadLen, out + headLen );
  }
  int getCompressionSize() const { return compressionSize; }

private:
  int headLen;
  int comHeadLen;

  int            compressionSize;
  Compression<C> headComp;
  Compression<C> bodyComp;
};

template <typename C> class StateManager {

  /**
   * state is [loc, channel_state, counter_state, clock_state]
   * is the corresponding loc is negative integer then this location a commit
   * location the corresponding channel_state is the block channel. The channel
   * state is positive then it is a send channel, and when the value is
   * nonegative integer it is a receive channel.
   *
   */
public:
  StateManager() {

    component_num = stateLen = counter_start_loc = 0;
    channel_num                                  = 0;
  }

  StateManager( int comp_num, int counter_num, int clock_num,
                const vector<C> &                 clockUpperBounds,
                const vector<ClockConstraint<C>> &edifferenceCons,
                const vector<Parameter> &ps, const vector<int> &nodes,
                int channel_n ) {

    component_num = comp_num;

    differenceConstraints = edifferenceCons;
    nodeNums              = nodes;

    channel_num = channel_n;

    if ( channel_num > 0 ) {
      counter_start_loc = 2 * component_num;
      stateLen          = 2 * component_num + counter_num;
    } else {
      counter_start_loc = component_num;
      stateLen          = component_num + counter_num;
    }
    clock_start_loc = stateLen;

    stateLen += ( clock_num + 1 ) * ( clock_num + 1 );

          
    dbmManager =
        DBMFactory<C>( clock_num, clockUpperBounds, differenceConstraints );

    parameters = ps;
  }
  int getStateLen() const { return stateLen; }

  int getClockStart() const { return clock_start_loc; }

  Compression<C> getHeadCompression() const {
    Compression<C> re_comp( clock_start_loc );
    for ( int i = 0; i < component_num; i++ ) {
      re_comp.setBound( i, -nodeNums[ i ] - 1, nodeNums[ i ] + 1 );
    }
    if ( channel_num > 0 ) {
      for ( int i = 0; i < component_num; i++ ) {
        re_comp.setBound( i + component_num, -channel_num, channel_num + 1 );
      }
    }
    return re_comp;
  }

  Compression<C> getBodyCompression() const {

    Compression<C> re_comp( stateLen - clock_start_loc );

    return re_comp;
  }

  bool hasChannel() const { return channel_num > 0; }

  int getLoc( int component, const C *const state ) const {
    if ( isCommitComp( component, state ) ) { // commit location
      return getCommitLoc( component, state );
    }
    return state[ component ];
  }

  C *newState() const {

    C *re_state = new C[ stateLen ];

    fill( re_state, re_state + clock_start_loc, 0 );

    dbmManager.init( re_state + clock_start_loc );

    return re_state;
  }

  
  void copy( C *des_state, const C *const source_state ) const {
    memcpy( des_state, source_state, stateLen * sizeof( C ) );
  }

  C *newState( const C *s ) const {
    C *re = new C[ stateLen ];
    memcpy( re, s, stateLen * sizeof( C ) );
    return re;
  }

  void destroyState( C *s ) const { delete[] s; }

  inline int                  getComponentNum() const { return component_num; }
  inline const DBMFactory<C> &getClockManager() const { return dbmManager; }
  inline const int *          getParameterValue( const int i ) const {
    return parameters[ i ].getValue();
  }

  void norm( const C *const dbm, vector<C *> &re_vec ) const {
    C *newDBM = dbmManager.createDBM( dbm );
    dbmManager.norm( newDBM, re_vec );
  }

  void norm( C *dbm ) { dbmManager.norm( dbm ); }

  inline C *getDBM( C *state ) const { return state + clock_start_loc; }

  inline const C *getDBM( const C *const state ) const {
    return state + clock_start_loc;
  }

  inline C *getCounterValue( C *state ) const {
    return state + counter_start_loc;
  }

  inline const C *getCounterValue( const C *const state ) const {
    return state + counter_start_loc;
  }

  inline void andImpl( const ClockConstraint<C> &cs, C *state ) const {
    return dbmManager.andImpl( getDBM( state ), cs );
  }
  inline bool isConsistent( C *state ) const {
    return dbmManager.isConsistent( getDBM( state ) );
  }

  inline vector<int> blockComponents( const int      chid,
                                      const C *const state ) const {
    vector<int> reBlockComponents;
    for ( int i = 0; i < component_num; i++ ) {

      /**
       * return entire componments which waits for this signal chid
       *
       */

      if ( state[ i + component_num ] == chid ) {
        reBlockComponents.push_back( i );
      }
    }
    return reBlockComponents;
  }

  inline void constructState( const int component_id, const int target,
                              const C *const state, C *dbm, bool isCommit,
                              C *re_state ) const {

    memcpy( re_state, state, clock_start_loc * sizeof( C ) );

    re_state[ component_id ] = target;
    memcpy( re_state + clock_start_loc, dbm,
            ( stateLen - clock_start_loc ) * sizeof( C ) );
    if ( isCommit ) {
      setCommitState( component_id, target, re_state );
    }
  }

  inline void constructState( const int component_id, const int target,
                              bool isCommit, C *state ) {
    state[ component_id ] = target;
    if ( isCommit ) {
      setCommitState( component_id, target, state );
    }
  }

  inline bool isCommitComp( const int      component_id,
                            const C *const state ) const {
    return state[ component_id ] < 0;
  }

  inline void setCommitState( const int component_id, const int target,
                              C *state ) const {
    state[ component_id ] = -1 - state[ component_id ];
  }

  inline int getCommitLoc( const int      component_id,
                           const C *const state ) const {
    return -( state[ component_id ] ) - 1;
  }
  bool hasDiffCons() const { return !differenceConstraints.empty(); }

private:
  bool channel_num;
  int  component_num;
  int  stateLen;

  int counter_start_loc;
  int clock_start_loc;

  vector<ClockConstraint<C>> differenceConstraints;

  DBMFactory<C> dbmManager;

  vector<Parameter> parameters;
  vector<int>       nodeNums;
};
} // namespace graphsat
#endif
