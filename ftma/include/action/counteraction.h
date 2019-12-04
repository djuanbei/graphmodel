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
#include <cassert>

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

#define TYPE_CASE( op )                                                        \
  switch ( rhs.type ) {                                                        \
  case CONST_ARG:                                                              \
    counter_value[ lhs_value ] op rhs_value;                                   \
    return;                                                                    \
  case COUNTER_ARG:                                                            \
    counter_value[ lhs_value ] op counter_value[ rhs_value ];                  \
    return;                                                                    \
  case PARAMETER_ARG:                                                          \
    counter_value[ lhs_value ] op rhs_value;                                   \
    return;                                                                    \
  case REF_PARAMETER_ARG:                                                      \
    counter_value[ lhs_value ] op counter_value[ rhs_value ];                  \
    return;                                                                    \
  case EMPTY_ARG:                                                              \
    assert( false );                                                           \
  }

#define TYPE_CASE_OUT( op_str )                                                \
  switch ( act.rhs.type ) {                                                    \
  case CONST_ARG:                                                              \
    out << "counter_" << act.lhs_value << setw( OP_OUT_WIDTH ) << op_str       \
        << setw( VALUE_OUT_WIDTH ) << act.rhs_value;                           \
    return out;                                                                \
  case COUNTER_ARG:                                                            \
    out << "counter_" << act.lhs_value << setw( OP_OUT_WIDTH ) << op_str       \
        << "counter_" << act.rhs_value;                                        \
    return out;                                                                \
  case PARAMETER_ARG:                                                          \
    out << "counter_" << act.lhs_value << setw( OP_OUT_WIDTH ) << op_str       \
        << setw( VALUE_OUT_WIDTH ) << act.rhs_value;                           \
    return out;                                                                \
  case REF_PARAMETER_ARG:                                                      \
    out << "counter_" << act.lhs_value << setw( OP_OUT_WIDTH ) << op_str       \
        << setw( VALUE_OUT_WIDTH ) << "counter_" << act.rhs_value;             \
    return out;                                                                \
  case EMPTY_ARG:                                                              \
    assert( false );                                                           \
  }

class CounterAction {
public:
  CounterAction( Argument out_lhs, Action_e ee, Argument out_rhs ) {

    action = ee;
    lhs    = out_lhs;
    rhs    = out_rhs;
  }
  void operator()( int *counter_value ) const {

    switch ( action ) {
    case ASSIGNMENT_ACTION: {
      TYPE_CASE( = );
    }
    case SELF_INC_ACTION: {
      TYPE_CASE( += );
    }
    case SELF_DEC_ACTION: {
      TYPE_CASE( -= );
    }
    }
  }
  CounterAction *copy() const { return new CounterAction( lhs, action, rhs ); }
  void           globalUpdate( const vector<int> &counter_map,
                               const vector<int> &parameter_value ) {
    lhs_value = lhs.value;
    rhs_value = rhs.value;

    switch ( lhs.type ) {
    case CONST_ARG:
      assert( false );
      break;
    case COUNTER_ARG:
      break;
    case PARAMETER_ARG:
      assert( false );
      break;
    case REF_PARAMETER_ARG:
      lhs_value = counter_map[ lhs.value ];
      break;
    case EMPTY_ARG:
      assert( false );
    }
    switch ( rhs.type ) {
    case CONST_ARG:
      break;
    case COUNTER_ARG:
      break;
    case PARAMETER_ARG:
      rhs_value = parameter_value[ rhs.value ];
      break;
    case REF_PARAMETER_ARG:
      rhs_value = counter_map[ rhs.value ];
      break;
    case EMPTY_ARG:
      assert( false );
    }
  }
  friend ostream &operator<<( ostream &out, const CounterAction &act ) {
    switch ( act.action ) {
    case ASSIGNMENT_ACTION: {
      TYPE_CASE_OUT( "=" );
    }
    case SELF_INC_ACTION: {
      TYPE_CASE_OUT( "+=" );
    }
    case SELF_DEC_ACTION: {
      TYPE_CASE_OUT( "-=" );
    }
    }
  }

private:
  Action_e action;
  Argument lhs;
  Argument rhs;
  int      lhs_value;
  int      rhs_value;
};

} // namespace graphsat
#endif
