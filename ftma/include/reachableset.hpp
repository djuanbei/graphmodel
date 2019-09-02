/**
 * @file   reachableset.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sat Apr 27 08:49:05 2019
 *
 * @brief  the data of reachableset
 *
 *
 */
#ifndef __REACH_SET_HPP
#define __REACH_SET_HPP
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

namespace graphsat {
using std::copy;
using std::deque;
using std::fill;
using std::ofstream;
using std::vector;

template <typename SYS> class ReachableSet {
public:
  typedef SYS                 SYS_t;
  typedef typename SYS_t::C_t C_t;
  ReachableSet( const SYS &outta )
      : sys( outta ) {

#ifdef DRAW_GRAPH
    current_parent = -1;
#endif

    manager       = sys.getStateManager();
    component_num = sys.getComponentNum();

    cache_state = manager.newState();
    next_state  = manager.newState();

    convertC_t = manager.newState();

    sys.initState( manager, cache_state );

    reachSet.setParam( manager.getStateLen(), manager.getClockStart() );
    int bodyLen = manager.getStateLen() - manager.getClockStart();

    compressState = StateConvert<C_t>( manager.getClockStart(), bodyLen,
                                       manager.getHeadCompression(),
                                       manager.getBodyCompression() );
    convertUINT   = new UINT[ compressState.getCompressionSize() ]();

    if ( manager.getClockManager().isConsistent(
             manager.getDBM( cache_state ) ) ) {

      addToReachableSet( cache_state );
      addToWait( cache_state );
    }
  }

  ~ReachableSet() {
    reachSet.clear();
    manager.destroyState( cache_state );
    manager.destroyState( next_state );
    manager.destroyState( convertC_t );

    delete[] convertUINT;
    while ( !waitSet.empty() ) {
      C_t *temp_state = waitSet.front();
      waitSet.pop_front();
      manager.destroyState( temp_state );
    }
  }

  const SYS &getSYS( void ) const { return sys; }
  /**
   * BFS
   * @return
   */
  C_t *next() {
    C_t *state = waitSet.front();
    waitSet.pop_front();
    return state;
  }
  bool   waitEmpty() const { return waitSet.empty(); }
  size_t waitSize() const { return waitSet.size(); }

  Check_State search( const Property *prop ) {

    for ( auto state : reachSet ) {
      compressState.decode( state, convertC_t );
      if ( isReach( prop, convertC_t ) ) {
        return TRUE;
      }
    }
    return UNKOWN;
  }

  bool oneStep( const Property *prop, C_t *state ) {
#ifdef DRAW_GRAPH
    current_parent++;
#endif

#ifdef PRINT_STATE

    for ( int i = 0; i < component_num; i++ ) {
      cout << state[ i ] << " ";
    }
    cout << endl
         << manager.getClockManager().dump( manager.getDBM( state ) ) << endl;
#endif
    int commit_component = -1;
    for ( int component = 0; component < component_num; component++ ) {
      if ( manager.isCommitComp( component, state ) ) {
        commit_component = component;
        break;
      }
    }
    if ( commit_component > -1 ) {
      return oneComponent( commit_component, prop, state );
    }
    for ( int component = 0; component < component_num; component++ ) {

      if ( manager.hasChannel() && state[ component + component_num ] != NO_CHANNEL ) {
        /**
         * Waiting for synchronize signal
         *
         */
        continue;
      }
      if ( oneComponent( component, prop, state ) ) {

        return true;
      }
    }
    return false;
  }

  size_t size() const { return reachSet.size(); }

  void project( int m, vector<vector<C_t>> &re ) {
    re.clear();
    int clock_start_loc = manager.getClockStart();

    for ( auto state : reachSet ) {

      compressState.decode( state, convertC_t );

      vector<C_t> dummy;
      for ( int i = 0; i < m; i++ ) {
        dummy.push_back( convertC_t[ i ] );
      }
      for ( int i = 0; i <= m; i++ ) {
        for ( int j = 0; j <= m; j++ ) {
          dummy.push_back(
              convertC_t[ i * ( component_num + 1 ) + j + clock_start_loc ] );
        }
      }

      re.push_back( dummy );
    }
  }

  void generatorDot( const string &filename ) {
#ifdef DRAW_GRAPH
    ofstream fout( filename );
    fout << "digraph G {" << endl;
    int len = compressState.getCompressionSize();
    for ( size_t i = 1; i < stateParent.size(); i++ ) {
      int parent = stateParent[ i ];

      compressState.decode( &( processStates[ parent * len ] ), cache_state );
      compressState.decode( &( processStates[ i * len ] ), convertC_t );
      fout << "\t" << stateParent[ i ] << " -> " << i << "  [label=\"";
      for ( int j = 0; j < component_num; j++ ) {
        if ( cache_state[ j ] != convertC_t[ j ] ) {
          fout << j << " ";
        }
      }

      fout << "\"];" << endl;
    }
    fout << "}";
    fout.close();
#endif
  }

  void addToWait( const C_t *const state ) {
    C_t *newState = manager.newState( state );
    waitSet.push_back( newState );
#ifdef DRAW_GRAPH
    compressState.encode( state, convertUINT );
    processStates.insert( processStates.end(), convertUINT,
                          convertUINT + compressState.getCompressionSize() );
    stateParent.push_back( current_parent );
#endif
  }

  inline bool addToReachableSet( const C_t *const state ) {

#ifndef CHECK_MEMORY
    compressState.encode( state, convertUINT );

    return reachSet.add( convertUINT ) > -1;
#endif
    return true;
  }

private:
  StateSet<UINT> reachSet;
  deque<C_t *>   waitSet;

  C_t *                        cache_state;
  C_t *                        next_state;
  const SYS &                  sys;
  typename SYS::StateManager_t manager;
  template <typename R1> friend class Reachability;
  int                        component_num;
  std::default_random_engine generator;
  StateConvert<C_t>          compressState;
  UINT *                     convertUINT;
  C_t *                      convertC_t;

#ifdef DRAW_GRAPH
  vector<UINT> processStates;
  vector<int>  stateParent;
  int          current_parent;
#endif

  /**
   *
   *
   * @param prop  verifies property
   * @param state one reachable state
   *
   * @return ture if prop is true under state, false otherwise.
   */
  bool isReach( const Property *prop, const C_t *const state ) const {
    return ( *prop )( manager, state );
  }

  /**
   *
   * @param component one step component
   * @param prop  the verifies property
   * @param state  the current state
   *
   * @return ture if prop satisfied by child state of state
   */

  bool oneComponent( int component, const Property *prop, C_t *state ) {

    int source = manager.getLoc( component, state );

    int outDegree = sys.tas[ component ].graph.getOutDegree( source );
    for ( int j = 0; j < outDegree; j++ ) {

      int link = sys.tas[ component ].graph.getAdj( source, j );
      /**
       * Whether the jump conditions satisfies except synchronize signal
       *
       */

      if ( !sys.tas[ component ].transitions[ link ].ready( component, manager,
                                                            state ) ) {
        continue;
      }

      const Channel &channel =
          sys.tas[ component ].transitions[ link ].getChannel();

      if ( channel.id > -1 ) {
        if ( doSynchronize( component, prop, state, link, channel ) ) {
          return true;
        }
      } else {
        if ( oneTranision( component, link, prop, state ) ) {
          return true;
        }
      }
    }
    return false;
  }

  /**
   * TODO: first do send counter action then do receive action
   * @param component current run component
   * @param block_component_id blocked  component id
   * @param link  the block transition
   * @param state the current state
   * @param prop
   * @param isSend ture if block_component_id is send part, false otherwise.
   *
   * @return  true the next state make prop true, false otherwise.
   */
  bool unBlockOne( const int current_component, const int block_component_id,
                   const int link, C_t *state, const Property *prop,
                   bool isSend ) {

    manager.copy( next_state, state );
    next_state[ block_component_id + component_num ] = NO_CHANNEL;

    const int block_link  = next_state[ block_component_id ];
    int       blockSource = 0;
    sys.tas[ block_component_id ].graph.findSrc( block_link, blockSource );

    next_state[ block_component_id ] = blockSource;
    int send_component_id            = current_component;
    int send_link                    = link;
    int receive_component_id         = block_component_id;
    int receive_link                 = block_link;

    if ( !isSend ) {
      send_component_id    = block_component_id;
      send_link            = block_link;
      receive_component_id = current_component;
      receive_link         = link;
    }

    // send do firstly
    /**
     *  has some problems
     * TODO: need rewrite bellow code
     */
    sys.tas[ send_component_id ].transitions[ send_link ](
        send_component_id, manager,
        next_state ); // send part firstly update state

    sys.tas[ receive_component_id ].transitions[ receive_link ](
        receive_component_id, manager, next_state );

    int send_target = 0;
    sys.tas[ send_component_id ].graph.findSnk( send_link, send_target );
    next_state[ send_component_id ] = send_target;

    bool is_send_commit =
        sys.tas[ send_component_id ].locations[ send_target ].isCommit();

    int receive_target = 0;

    sys.tas[ receive_component_id ].graph.findSnk( receive_link,
                                                   receive_target );
    next_state[ receive_component_id ] = receive_target;

    bool is_receive_commit =
        sys.tas[ receive_component_id ].locations[ receive_target ].isCommit();

    bool re_bool = false;

    // can not stay on current_target and block_target locations when
    //send_target or receive_target is a commit or urgent location.
    
    if ( is_send_commit || is_receive_commit ) {
      for ( int comp_id = 0; comp_id < component_num; comp_id++ ) {
        sys.tas[ comp_id ]
            .locations[ manager.getLoc( comp_id, next_state ) ]
            .employInvariants( manager.getClockManager(),
                               manager.getDBM( next_state ) );
      }
      if ( manager.hasDiffCons() ) {
        vector<typename SYS::C_t *> next_dbms;
        manager.norm( manager.getDBM( next_state ), next_dbms );

        for ( auto dbm : next_dbms ) {
          manager.constructState( send_component_id, send_target, next_state,
                                  dbm, false, cache_state );
          if ( is_send_commit ) {
            manager.setCommitState( send_component_id, cache_state );
          }
          if ( is_receive_commit ) {
            manager.setCommitState( receive_component_id, cache_state );
          }

          if ( addToReachableSet( cache_state ) ) { // add to reachableSet
            addToWait( cache_state );
            if ( isReach( prop, cache_state ) ) {
              re_bool = true;
              break;
            }
          }
        }
        for ( auto dbm : next_dbms ) {
          manager.getClockManager().destroyDBM( dbm );
        }
      } else {

        manager.norm( manager.getDBM( next_state ) );

        manager.constructState(  send_component_id, send_target, false,
                                next_state );

        if ( is_send_commit ) {
          manager.setCommitState( send_component_id, cache_state );
        }
        if ( is_receive_commit ) {
          manager.setCommitState( receive_component_id, cache_state );
        }
          
        if ( addToReachableSet( next_state ) ) { // add to reachableSet
          addToWait( next_state );
          if ( isReach( prop, next_state ) ) {
            return true;
          }
        }
      }
    }else{
       
      if ( sys.tas[ send_component_id ].locations[ send_target ](
              manager.getClockManager(), manager.getDBM( next_state ) ) ) {

        for ( int comp_id = 0; comp_id < component_num; comp_id++ ) {

          sys.tas[ comp_id ]
              .locations[ manager.getLoc( comp_id, next_state ) ]
              .employInvariants( manager.getClockManager(),
                                 manager.getDBM( next_state ) );
        }
        if ( manager.hasDiffCons() ) {

          vector<typename SYS::C_t *> next_dbms;
          manager.norm( manager.getDBM( next_state ), next_dbms );

          for ( auto dbm : next_dbms ) {
            manager.constructState( send_component_id, send_target, next_state, dbm, false,
                                    cache_state );
            if ( addToReachableSet( cache_state ) ) { // add to reachableSet
              addToWait( cache_state );
              if ( isReach( prop, cache_state ) ) {
                re_bool = true;
                break;
              }
            }
          }
          for ( auto dbm : next_dbms ) {
            manager.getClockManager().destroyDBM( dbm );
          }
        } else {
          manager.norm( manager.getDBM( next_state ) );
          manager.constructState( send_component_id, send_target, false, next_state );

          if ( addToReachableSet( next_state ) ) { // add to reachableSet
            addToWait( next_state );
            if ( isReach( prop, next_state ) ) {
              return true;
            }
          }
        }
      }
      
    }


    return re_bool;
  }

  /**
   * TODO: urgent chan
   *
   */
  bool doSynchronize( int component, const Property *prop, C_t *state, int link,
                      const Channel &channel ) {

    vector<int> waitComponents;
    bool        isSend = true;
    if ( CHANNEL_SEND == channel.action ) {
      waitComponents = manager.blockComponents( -channel.id, state );
    } else if ( CHANNEL_RECEIVE == channel.action ) {
      isSend         = false;
      waitComponents = manager.blockComponents( channel.id, state );
    }
    if ( !waitComponents.empty() ) {
      if ( channel.type == ONE2ONE_CH ) {
        std::uniform_int_distribution<int> distribution(
            0, waitComponents.size() - 1 );
        int id                 = distribution( generator );
        int block_component_id = waitComponents[ id ];

        if ( unBlockOne( component, block_component_id, link, state, prop,
                         isSend ) ) {
          return true;
        }
      } else if ( channel.type == BROADCAST_CH ) {
        for ( auto id : waitComponents ) {
          int block_component_id = waitComponents[ id ];
          if ( unBlockOne( component, block_component_id, link, state, prop,
                           isSend ) ) {
            return true;
          }
        }
      }

    } else {
      manager.copy( cache_state, state );

      if ( CHANNEL_SEND == channel.action ) {
        cache_state[ component + component_num ] = channel.id;
      } else if ( CHANNEL_RECEIVE == channel.action ) {
        cache_state[ component + component_num ] = -channel.id;
      }

      cache_state[ component ] = link; // block link
      if ( addToReachableSet( cache_state ) ) {
        addToWait( cache_state );
      }
    }
    return false;
  }

  /**
   *
   *
   * @param component Current compoent id
   * @param link  Current transition id
   * @param prop  verifying property
   * @param state  Current state
   *
   * @return  true if find a state makes prop true, false otherwise.
   */

  bool oneTranision( const int component, const int link, const Property *prop,
                     const C_t *const state ) {

    manager.copy( next_state, state );
    int target = 0;
    sys.tas[ component ].graph.findSnk( link, target );

    sys.tas[ component ].transitions[ link ]( component, manager,
                                              next_state ); // update state

    next_state[ component ] = target;
    bool isCommit = sys.tas[ component ].locations[ target ].isCommit();

    bool re_bool = false;
    if ( sys.tas[ component ].locations[ target ](
             manager.getClockManager(), manager.getDBM( next_state ) ) ) {

      for ( int comp_id = 0; comp_id < component_num; comp_id++ ) {

        sys.tas[ comp_id ]
            .locations[ manager.getLoc( comp_id, next_state ) ]
            .employInvariants( manager.getClockManager(),
                               manager.getDBM( next_state ) );
      }
      if ( manager.hasDiffCons() ) {

        vector<typename SYS::C_t *> next_dbms;
        manager.norm( manager.getDBM( next_state ), next_dbms );

        for ( auto dbm : next_dbms ) {
          manager.constructState( component, target, next_state, dbm, isCommit,
                                  cache_state );
          if ( addToReachableSet( cache_state ) ) { // add to reachableSet
            addToWait( cache_state );
            if ( isReach( prop, cache_state ) ) {
              re_bool = true;
              break;
            }
          }
        }
        for ( auto dbm : next_dbms ) {
          manager.getClockManager().destroyDBM( dbm );
        }
      } else {
        manager.norm( manager.getDBM( next_state ) );
        manager.constructState( component, target, isCommit, next_state );

        if ( addToReachableSet( next_state ) ) { // add to reachableSet
          addToWait( next_state );
          if ( isReach( prop, next_state ) ) {
            return true;
          }
        }
      }
    }
    return re_bool;
  }
};
// typedef ReachableSet< TAS_t> R_t;
} // namespace graphsat

#endif
