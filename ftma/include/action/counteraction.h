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

#include "util/data.hpp"
#include "util/dbmutil.hpp"
#include <vector>

namespace graphsat {
using std::ostream;
using std::pair;
using std::setw;
using std::vector;

enum Action_e {
  ASSIGNMENT_ACTION, //=
  SELF_INC_ACTION,   //+=
  SELF_DEC_ACTION    //-=
};

enum RHS_TYPE { RHS_CONSTANT_T, RHS_COUNTER_T, RHS_PARAMETER_T };

class CounterAction {
public:
  CounterAction( Action_e ee, RHS_TYPE etype, int eglobal_lhs_id,
                 int eglobal_rhs_id ) {

    action        = ee;
    type          = etype;
    global_lhs_id = eglobal_lhs_id;
    global_rhs_id = eglobal_rhs_id;
    parameter_id  = eglobal_rhs_id;
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
    return new CounterAction( action, type, global_lhs_id, parameter_id );
  }
  void globalUpdate( const vector<int> &id_map,
                     const vector<int> &parameter_value ) {

    if ( type == RHS_CONSTANT_T ) {
      return;
    }
    if ( type == RHS_PARAMETER_T ) {
      global_rhs_id = parameter_value[ parameter_id ];
      return;
    }
  }
  friend ostream &operator<<( ostream &out, const CounterAction &act ) {
    switch ( act.action ) {
    case ASSIGNMENT_ACTION: {
      string op_str = "=";
      switch ( act.type ) {

      case RHS_CONSTANT_T:
        out << "counter_" << act.global_lhs_id << setw( OP_OUT_WIDTH ) << op_str
            << setw( VALUE_OUT_WIDTH ) << act.global_rhs_id;
        return out;
      case RHS_COUNTER_T:
        out << "counter_" << act.global_lhs_id << setw( OP_OUT_WIDTH ) << op_str
            << "counter_" << act.global_rhs_id;

        return out;
      case RHS_PARAMETER_T:
        out << "counter_" << act.global_lhs_id << setw( OP_OUT_WIDTH ) << op_str
            << setw( VALUE_OUT_WIDTH ) << act.global_rhs_id;

        return out;
      }
    }
    case SELF_INC_ACTION: {
      string op_str = "+=";
      switch ( act.type ) {

      case RHS_CONSTANT_T:
        out << "counter_" << act.global_lhs_id << setw( OP_OUT_WIDTH ) << op_str
            << setw( VALUE_OUT_WIDTH ) << act.global_rhs_id;
        return out;
      case RHS_COUNTER_T:
        out << "counter_" << act.global_lhs_id << setw( OP_OUT_WIDTH ) << op_str
            << "counter_" << act.global_rhs_id;
        return out;
      case RHS_PARAMETER_T:
        out << "counter_" << act.global_lhs_id << setw( OP_OUT_WIDTH ) << op_str
            << setw( VALUE_OUT_WIDTH ) << act.global_rhs_id;
        return out;
      }
    }
    case SELF_DEC_ACTION: {
      string op_str = "-=";
      switch ( act.type ) {

      case RHS_CONSTANT_T:
        out << "counter_" << act.global_lhs_id << setw( OP_OUT_WIDTH ) << op_str
            << setw( VALUE_OUT_WIDTH ) << act.global_rhs_id;
        return out;
      case RHS_COUNTER_T:
        out << "counter_" << act.global_lhs_id << setw( OP_OUT_WIDTH ) << op_str
            << "counter_" << act.global_rhs_id;
        return out;
      case RHS_PARAMETER_T:
        out << "counter_" << act.global_lhs_id << setw( OP_OUT_WIDTH ) << op_str
            << setw( VALUE_OUT_WIDTH ) << act.global_rhs_id;
        return out;
      }
    }
    }
  }

private:
  Action_e action;
  RHS_TYPE type;
  int      global_lhs_id;
  int      global_rhs_id;
  int      parameter_id;
};

} // namespace graphsat
#endif
