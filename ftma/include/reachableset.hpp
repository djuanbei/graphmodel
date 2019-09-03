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

    convert_C_t = manager.newState();

    sys.initState( manager, cache_state );

    int body_length = manager.getStateLen() - manager.getClockStart();

    compress_state = StateConvert<C_t>( manager.getClockStart(), body_length,
                                        manager.getHeadCompression(),
                                        manager.getBodyCompression() );
    convert_UINT   = new UINT[ compress_state.getCompressionSize() ]();
    reach_set.setParam( compress_state.getCompressionSize(),
                        compress_state.getCompressionHeadSize() );

    if ( manager.getClockManager().isConsistent(
             manager.getDBM( cache_state ) ) ) {

      addToReachableSet( cache_state );
      addToWait( cache_state );
    }
  }

  ~ReachableSet() {
    reach_set.clear();
    manager.destroyState( cache_state );
    manager.destroyState( next_state );
    manager.destroyState( convert_C_t );

    delete[] convert_UINT;
    while ( !wait_set.empty() ) {
      C_t *temp_state = wait_set.front();
      wait_set.pop_front();
      manager.destroyState( temp_state );
    }
  }

  const SYS &getSYS( void ) const { return sys; }
  /**
   * BFS
   * @return
   */
  C_t *next() {
    C_t *state = wait_set.front();
    wait_set.pop_front();
    return state;
  }
  bool   waitEmpty() const { return wait_set.empty(); }
  size_t waitSize() const { return wait_set.size(); }

  Check_State search( const Property *prop ) {

    for ( auto state : reach_set ) {
      compress_state.decode( state, convert_C_t );
      if ( isReach( prop, convert_C_t ) ) {
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
    if ( state[ manager.getFreezeLocation() ] > 0 ) {
      for ( int component = 0; component < component_num; component++ ) {
        if ( manager.isCommitComp( component, state ) ) {
          commit_component = component;
          break;
        }
      }
    }
    if ( commit_component > -1 ) {
      return oneComponent( commit_component, prop, state );
    }
    for ( int component = 0; component < component_num; component++ ) {

      if ( manager.hasChannel() &&
           state[ component + component_num ] != NO_CHANNEL ) {
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

  size_t size() const { return reach_set.size(); }

  void project( int m, vector<vector<C_t>> &re ) {
    re.clear();
    int clock_start_loc = manager.getClockStart();

    for ( auto state : reach_set ) {

      compress_state.decode( state, convert_C_t );

      vector<C_t> dummy;
      for ( int i = 0; i < m; i++ ) {
        dummy.push_back( convert_C_t[ i ] );
      }
      for ( int i = 0; i <= m; i++ ) {
        for ( int j = 0; j <= m; j++ ) {
          dummy.push_back(
              convert_C_t[ i * ( component_num + 1 ) + j + clock_start_loc ] );
        }
      }

      re.push_back( dummy );
    }
  }

  void generatorDot( const string &filename ) {
#ifdef DRAW_GRAPH
    ofstream fout( filename );
    fout << "digraph G {" << endl;
    int len = compress_state.getCompressionSize();
    for ( size_t i = 1; i < state_parent.size(); i++ ) {
      int parent = state_parent[ i ];

      compress_state.decode( &( process_states[ parent * len ] ), cache_state );
      compress_state.decode( &( process_states[ i * len ] ), convert_C_t );
      fout << "\t" << state_parent[ i ] << " -> " << i << "  [label=\"";
      for ( int j = 0; j < component_num; j++ ) {
        if ( cache_state[ j ] != convert_C_t[ j ] ) {
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
    wait_set.push_back( newState );
#ifdef DRAW_GRAPH
    compress_state.encode( state, convert_UINT );
    process_states.insert( process_states.end(), convert_UINT,
                           convert_UINT + compress_state.getCompressionSize() );
    state_parent.push_back( current_parent );
#endif
  }

  inline bool addToReachableSet( const C_t *const state ) {

#ifndef CHECK_MEMORY
    compress_state.encode( state, convert_UINT );

    return reach_set.add( convert_UINT ) > -1;
#endif
    return true;
  }

private:
  StateSet<UINT> reach_set;
  deque<C_t *>   wait_set;

  C_t *                        cache_state;
  C_t *                        next_state;
  const SYS &                  sys;
  typename SYS::StateManager_t manager;
  template <typename R1> friend class Reachability;
  int                        component_num;
  std::default_random_engine generator;
  StateConvert<C_t>          compress_state;
  UINT *                     convert_UINT;
  C_t *                      convert_C_t;

#ifdef DRAW_GRAPH
  vector<UINT> process_states;
  vector<int>  state_parent;
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

    int out_degree = sys.tas[ component ].graph.getOutDegree( source );
    for ( int j = 0; j < out_degree; j++ ) {

      int link = sys.tas[ component ].graph.getAdj( source, j );
      /**
       * Whether the jump conditions satisfies except synchronize signal
       *
       */

      if ( !sys.tas[ component ].transitions[ link ].ready( component, manager,
                                                            state ) ) {
        continue;
      }

      if ( sys.tas[ component ].transitions[ link ].hasChannel() ) {
        const Channel &channel =
            sys.tas[ component ].transitions[ link ].getChannel();
        // channel.id start from 1

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
   * @param is_send ture if block_component_id is send part, false otherwise.
   *
   * @return  true the next state make prop true, false otherwise.
   */
  bool unBlockOne( const int current_component, const int block_component_id,
                   const int link, C_t *state, const Property *prop,
                   bool is_send ) {

    manager.copy( next_state, state );
    next_state[ block_component_id + component_num ] = NO_CHANNEL;

    const int block_link   = next_state[ block_component_id ];
    int       block_source = 0;
    sys.tas[ block_component_id ].graph.findSrc( block_link, block_source );

    next_state[ block_component_id ] = block_source;
    int send_component_id            = current_component;
    int send_link                    = link;
    int receive_component_id         = block_component_id;
    int receive_link                 = block_link;

    if ( !is_send ) {
      send_component_id    = block_component_id;
      send_link            = block_link;
      receive_component_id = current_component;
      receive_link         = link;
    }

    // send do firstly
    /**
     *  has some problems
     */
    sys.tas[ send_component_id ].transitions[ send_link ](
        send_component_id, manager,
        next_state ); // send part firstly update state

    sys.tas[ receive_component_id ].transitions[ receive_link ](
        receive_component_id, manager, next_state );

    int send_target = 0;
    sys.tas[ send_component_id ].graph.findSnk( send_link, send_target );
    next_state[ send_component_id ] = send_target;

    // bool is_send_commit =
    //     sys.tas[ send_component_id ].locations[ send_target ].is_commit();

    int receive_target = 0;

    sys.tas[ receive_component_id ].graph.findSnk( receive_link,
                                                   receive_target );
    next_state[ receive_component_id ] = receive_target;

    bool is_receive_commit =
        sys.tas[ receive_component_id ].locations[ receive_target ].isCommit();

    // if ( is_send_commit ) {
    //   manager.setCommitState( send_component_id, next_state );
    // }
    if ( is_receive_commit ) {
      manager.setCommitState( receive_component_id, next_state );
    }

    int source, target;
    source = target = 0;
    sys.tas[ send_component_id ].graph.findSrcSnk( send_link, source, target );

    if ( sys.tas[ send_component_id ].locations[ source ].isFreezeLocation() ) {
      next_state[ manager.getFreezeLocation() ]--;
      assert( next_state[ manager.getFreezeLocation() ] >= 0 );
    }
    if ( sys.tas[ send_component_id ].locations[ target ].isFreezeLocation() ) {
      next_state[ manager.getFreezeLocation() ]++;
      assert( next_state[ manager.getFreezeLocation() ] <= component_num );
    }

    sys.tas[ receive_component_id ].graph.findSrcSnk( receive_link, source,
                                                      target );

    if ( sys.tas[ receive_component_id ]
             .locations[ source ]
             .isFreezeLocation() ) {
      next_state[ manager.getFreezeLocation() ]--;
      assert( next_state[ manager.getFreezeLocation() ] >= 0 );
    }

    if ( sys.tas[ receive_component_id ]
             .locations[ target ]
             .isFreezeLocation() ) {
      next_state[ manager.getFreezeLocation() ]++;
      assert( next_state[ manager.getFreezeLocation() ] <= component_num );
    }

    // can not stay on current_target and block_target locations when
    // send_target or receive_target is a commit or urgent location.

    return delay( send_component_id, send_target, prop, next_state );
  }

  /**
   * TODO: urgent chan
   *
   */
  bool doSynchronize( int component, const Property *prop, C_t *state, int link,
                      const Channel &channel ) {

    vector<int> wait_components;
    bool        is_send = true;
    if ( CHANNEL_SEND == channel.action ) {
      wait_components = manager.blockComponents( -channel.id, state );
    } else if ( CHANNEL_RECEIVE == channel.action ) {
      is_send         = false;
      wait_components = manager.blockComponents( channel.id, state );
    }
    if ( !wait_components.empty() ) {
      if ( channel.type == ONE2ONE_CH ) {
        std::uniform_int_distribution<int> distribution(
            0, wait_components.size() - 1 );
        int id                 = distribution( generator );
        int block_component_id = wait_components[ id ];
        return unBlockOne( component, block_component_id, link, state, prop,
                           is_send );
      } else if ( channel.type == BROADCAST_CH ) {
        for ( auto id : wait_components ) {
          int block_component_id = wait_components[ id ];
          if ( unBlockOne( component, block_component_id, link, state, prop,
                           is_send ) ) {
            return true;
          }
        }
      }

    } else {
      manager.copy( cache_state, state );
      assert( channel.id > 0 );
      if ( CHANNEL_SEND == channel.action ) {
        cache_state[ component + component_num ] = channel.id; // send part
      } else if ( CHANNEL_RECEIVE == channel.action ) {
        cache_state[ component + component_num ] = -channel.id; // receive part
      }

      cache_state[ component ] = link; // block link
      if ( addToReachableSet( cache_state ) ) {
        addToWait( cache_state );
      }
    }
    return false;
  }

  /**
   *@brief The current location of TA is on src location of link, then
   * it wants to jump through link to snk location of link.
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
    int source = 0;
    int target = 0;
    sys.tas[ component ].graph.findSrcSnk( link, source, target );
    if ( sys.tas[ component ].locations[ source ].isFreezeLocation() ) {
      next_state[ manager.getFreezeLocation() ]--;
      assert( next_state[ manager.getFreezeLocation() ] >= 0 );
    }
    if ( sys.tas[ component ].locations[ target ].isFreezeLocation() ) {
      next_state[ manager.getFreezeLocation() ]++;
      assert( next_state[ manager.getFreezeLocation() ] <= component_num );
    }

    sys.tas[ component ].transitions[ link ](
        component, manager,
        next_state ); // update counter state and reset clock state

    return delay( component, target, prop, next_state );
  }

  bool delay( const int component, const int target, const Property *prop,
              C_t *state ) {

    state[ component ] = target;
    if ( !sys.tas[ component ].locations[ target ].isReachable(
             manager.getClockManager(), manager.getDBM( state ) ) ) {
      return false;
    }
    /**
     Whether there is some component in freeze location
     */
    if ( 0 == state[ manager.getFreezeLocation() ] ) {
      sys.tas[ component ].locations[ target ]( manager.getClockManager(),
                                                manager.getDBM( state ) );
    }

    return postDelay( component, target, prop, state );
  }

  bool postDelay( const int component, const int target, const Property *prop,
                  C_t *state ) {

    bool is_commit = sys.tas[ component ].locations[ target ].isCommit();

    bool re_bool = false;
    if ( 0 == state[ manager.getFreezeLocation() ] ) {
      for ( int comp_id = 0; comp_id < component_num; comp_id++ ) {
        sys.tas[ comp_id ]
            .locations[ manager.getLoc( comp_id, state ) ]
            .employInvariants( manager.getClockManager(),
                               manager.getDBM( state ) );
      }
    }
    if ( manager.hasDiffCons() ) {

      vector<typename SYS::C_t *> next_dbms;
      manager.norm( manager.getDBM( state ), next_dbms );

      for ( auto dbm : next_dbms ) {
        manager.constructState( component, target, state, dbm, is_commit,
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
      manager.norm( manager.getDBM( state ) );
      manager.constructState( component, target, is_commit, state );

      if ( addToReachableSet( state ) ) { // add to reachableSet
        addToWait( state );
        if ( isReach( prop, state ) ) {
          return true;
        }
      }
    }
    return re_bool;
  }
}; // namespace graphsat
// typedef ReachableSet< TAS_t> R_t;
} // namespace graphsat

#endif
