
#include "model/transition.h"

namespace graphsat {
Transition::Transition( const Transition &other, const Parameter &param ) {
  source = other.source;
  target = other.target;
  guards = other.guards;

  for ( auto e : other.counter_cons ) {
    CounterConstraint *dummy = copy( e );
    dummy->globalUpdate( param.getCounterMap(), param.getParameterMap() );
    counter_cons.push_back( dummy );
  }

  has_channel = other.has_channel;
  if ( has_channel ) {
    channel = other.channel;
    channel.globalIpUpdate( param.getChanMap() );
  }

  for ( auto a : other.actions ) {
    CounterAction *dummy = a->copy();
    dummy->globalUpdate( param.getCounterMap(), param.getParameterMap() );
    actions.push_back( dummy );
  }
  resets = other.resets;
}

bool Transition::ready( const int component, const TMStateManager &manager,
                        const int *const state ) const {
  if ( !guards.empty() ) {

    const DBMFactory &dbm_manager = manager.getClockManager();
    const int *       source_DBM  = manager.getDBM( state );
    assert( dbm_manager.isConsistent( source_DBM ) );
    int *copy_DBM = dbm_manager.createDBM( source_DBM );

    for ( auto cs : guards ) {
      dbm_manager.andImpl( copy_DBM, cs );
    }

    if ( !dbm_manager.isConsistent( copy_DBM ) ) {
      dbm_manager.destroyDBM( copy_DBM );
      return false;
    }
    dbm_manager.destroyDBM( copy_DBM );
  }

  if ( !counter_cons.empty() ) {
    const int *counter_value = manager.getCounterValue( state );

    for ( auto cs : counter_cons ) {
      if ( !( *cs )( counter_value ) ) {
        return false;
      }
    }
  }
  return true;
}

void Transition::operator()( const int component, const TMStateManager &manager,
                             int *re_state ) const {
  assert( ready( component, manager, re_state ) );

  const DBMFactory &dbm_manager = manager.getClockManager();

  int *source_DBM = manager.getDBM( re_state );

  /**
   * the state which statisfied the guards can jump this transition
   *
   */
  for ( auto cs : guards ) {
    dbm_manager.andImpl( source_DBM, cs );
  }

  for ( auto reset : resets ) {
    assert( reset.first > 0 );   // clock id start from 1
    assert( reset.second >= 0 ); // clock value must positive
    dbm_manager.resetImpl( source_DBM, reset.first, reset.second );
  }

  if ( !actions.empty() ) {

    int *counterValue = manager.getCounterValue( re_state );

    for ( auto act : actions ) {
      ( *act )( counterValue );
    }
  }

  //    return re_state;
}

void Transition::clockShift( int shift ) {
  for ( size_t i = 0; i < guards.size(); i++ ) {
    guards[ i ].clockShift( shift );
  }
  for ( size_t i = 0; i < resets.size(); i++ ) {
    resets[ i ].first += shift;
  }
}

} // namespace graphsat
