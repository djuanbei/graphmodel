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


template<typename C>
class StateConvert{

 public:
  StateConvert( ){
    headLen=0;
    bodyLen=0;
    headComp=0;
    bodyComp=0;
  }
  StateConvert(int hLen, int bLen, Compression<C> hCom, Compression<C> bCom ){
    headLen=hLen;
    bodyLen=bLen;
    comHeadLen=hCom.getCompressionSize( );
    comBodyLen=bCom.getCompressionSize( );
    
    headComp=hCom;
    bodyComp=bCom;
    
    uintValue.resize( comHeadLen+comBodyLen, 0);
    
    value.resize( hLen+bLen, 0);
  }
  
  UINT* encode( const  C *  data ){
    UINT * temp=headComp.encode( data);
    copy( temp,  temp+ comHeadLen,  uintValue.begin( ));
    UINT * temp1=bodyComp.encode( data+headLen);
    copy( temp1, temp1+comBodyLen,  uintValue.begin( )+comHeadLen );
    return &( uintValue[ 0]);
  }

  C* decode(  const UINT  *  data){
    C* temp=headComp.decode( data);
    copy( temp, temp+headLen,  value.begin( ));
    C* temp1=bodyComp.decode( data+comHeadLen);
    copy(temp1, temp1+ bodyLen,  value.begin( )+ headLen);
    return &( value[ 0]);
  }

 private:
  int headLen;
  int bodyLen;
  int comHeadLen;
  int comBodyLen;
  Compression<C> headComp;
  Compression<C> bodyComp;
  vector<UINT>  uintValue;
  vector<C> value;
  
};

template <typename C> class StateManager {

  /**
   * state is [loc, channel_state, counter_state, clock_state]
   * is the corresponding loc is negative integer then this location a commit location
   * the corresponding channel_state is the block channel. The channel state is positive then it is a send channel, and when the value is nonegative integer it is a receive channel.
   *
   */
public:
  StateManager() {

    component_num = stateLen = counter_start_loc = 0;
    haveChannel   = false;
  }

  StateManager( int comp_num, int counter_num, int clock_num,
                const vector<C> &                 clockUpperBounds,
                const vector<ClockConstraint<C>> &edifferenceCons,
                const vector<Parameter> &ps, bool haveCh ) {
 
    component_num         = comp_num;

    differenceConstraints = edifferenceCons;
    haveChannel           = haveCh;
   
    if ( haveChannel ) {
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

  // StateConvert<C> getStateConvert( ) const{
  //   return stateConvert;
  // }
  
  Compression<C>  getHeadCompression( ) const{
    Compression<C> re(clock_start_loc );
    return re;
  }
  
  Compression<C> getBodyCompression( ) const{
    
    Compression<C> re(stateLen-clock_start_loc );
    
    return re;
  }

  bool hasChannel() const { return haveChannel; }

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

  bool add( const int component_id, const int target, StateSet<UINT> &stateSet, StateConvert<C> & stateConvert,
            const C *const state, C *dbm, bool isCommit, C *re_state ) const {

    memcpy( re_state, state, clock_start_loc * sizeof( C ) );

    re_state[ component_id ] = target;
    memcpy( re_state + clock_start_loc, dbm,
            ( stateLen - clock_start_loc ) * sizeof( C ) );
    if ( isCommit ) {
      setCommitState( component_id, target, re_state );
    }

#ifndef CHECK_MEMORY
    UINT* temp=stateConvert.encode( re_state);
    if ( !stateSet.add( temp ) ) {
      return false;
    }
#endif
    return true;
  }

  bool add( const int component_id, const int target, StateSet<UINT> &stateSet, StateConvert<C> & stateConvert,
            C *state, bool isCommit ) const {

    state[ component_id ] = target;
    if ( isCommit ) {
      setCommitState( component_id, target, state );
    }
#ifndef CHECK_MEMORY
    UINT* temp=stateConvert.encode( state);
    if ( !stateSet.add( temp ) ) {
      return false;
    }
#endif
    return true;
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
  bool haveChannel;
  int  component_num;
  int  stateLen;
  
  int counter_start_loc;
  int clock_start_loc;



  vector<ClockConstraint<C>> differenceConstraints;

  DBMFactory<C> dbmManager;

  vector<Parameter> parameters;
  
  //  StateConvert<C> stateConvert;

};
} // namespace graphsat
#endif
