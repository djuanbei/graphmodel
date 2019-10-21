/**
 * @file   Action.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:39:22 2019
 *
 * @brief  Action class which  do x=x+c where x is a counter
 *        variable and c is a constant integer
 *
 *
 */
#ifndef ACTION_HPP
#define ACTION_HPP

#include <vector>

#include "util/data.hpp"

namespace graphsat {
using std::pair;
using std::vector;

enum Action_e {
  ASSIGNMENT_ACTION, //=
  SELF_INC_ACTION,   //+=
  SELF_DEC_ACTION    //-=
};

enum RHS_TYPE { RHS_CONSTANT_T, RHS_COUNTER_T, RHS_PARAMETER_T };

class CounterAction {
public:
  CounterAction( Action_e ee, RHS_TYPE etype, int elocal_lhs_id,
                 int elocal_rhs_id ) {
    
    action       = ee;
    type         = etype;
    local_lhs_id = elocal_lhs_id;
    local_rhs_id = elocal_rhs_id;
  }
  void operator()( int *counter_value ) const {
   
    switch ( action ) {
    case ASSIGNMENT_ACTION: {
      switch ( type ) {
      case RHS_CONSTANT_T:
        counter_value[ global_lhs_id ] = global_rhs_id;
        return;
      case RHS_COUNTER_T:
        counter_value[ global_lhs_id ] = counter_value[ global_rhs_id ];
        return;
      case RHS_PARAMETER_T:
        counter_value[ global_lhs_id ] = global_rhs_id;
        return;
      }
    }
    case SELF_INC_ACTION: {
      switch ( type ) {
      case RHS_CONSTANT_T:
        counter_value[ global_lhs_id ] += global_rhs_id;
        return;
      case RHS_COUNTER_T:
        counter_value[ global_lhs_id ] += counter_value[ global_rhs_id ];
        return;
      case RHS_PARAMETER_T:
        counter_value[ global_lhs_id ] += global_rhs_id;
        return;
      }
    }
    case SELF_DEC_ACTION: {
      switch ( type ) {
      case RHS_CONSTANT_T:
        counter_value[ global_lhs_id ] -= global_rhs_id;
        return;
      case RHS_COUNTER_T:
        counter_value[ global_lhs_id ] -= counter_value[ global_rhs_id ];
        return;
      case RHS_PARAMETER_T:
        counter_value[ global_lhs_id ] -= global_rhs_id;
        return;
      }
    }
    }
  }
  CounterAction *copy() const {
    return new CounterAction( action, type, local_lhs_id, local_rhs_id );
  }
  void globalUpdate( const map<int, int> &id_map,
                     const vector<int> &  parameter_value ) {
    global_lhs_id = id_map.at( local_lhs_id );
    if ( type == RHS_CONSTANT_T ) {
       global_rhs_id=local_rhs_id;
      return;
    }
    if ( type == RHS_PARAMETER_T ) {
      global_rhs_id = parameter_value[ local_rhs_id ];
      return;
    }
    global_rhs_id = id_map.at( local_rhs_id );
  }

private:
  Action_e action;
  RHS_TYPE type;
  int      local_lhs_id;
  int      local_rhs_id;

  int global_lhs_id;
  int global_rhs_id;
};


} // namespace graphsat
#endif
