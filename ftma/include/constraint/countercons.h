/**
 * @file   countercons.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu May  9 10:02:20 2019
 *
 * @brief  counter constraint like  pid==id
 *
 *
 */

#ifndef COUNTER_CONS_H
#define COUNTER_CONS_H

#include <iomanip>
#include <iostream>
#include <vector>

#include "util/dbmutil.hpp"

#include "util/data.hpp"

namespace graphsat {
using std::ostream;
using std::setw;
using std::to_string;

class InstanceFactory;

class CounterConstraint {

public:
  virtual bool operator()( const int *counter_value ) const = 0;
  /**
   *Deley set the counter id
   */
  virtual void         globalUpdate( const vector<int> &id_map,
                                     const vector<int> &  parameter_value ) = 0;
  friend std::ostream &operator<<( std::ostream &           out,
                                   const CounterConstraint &cons ) {
    return cons.dump( out );
  }

protected:
  virtual ~CounterConstraint() {}

private:
  virtual CounterConstraint *copy() const               = 0;
  virtual std::ostream &     dump( ostream &out ) const = 0;
  //   return out;
  // }

  friend class InstanceFactory;
};

/**
 * A constant bool value  class.
 */
class OneParameterConstraint : public CounterConstraint {
public:
  /**
   * @return  a constant bool value which does not depend on value of
   * counter_value
   */
  bool operator()( const int *counter_value ) const { return value; }
  void globalUpdate( const vector<int> &id_map,
                     const vector<int> &  parameter_value ) {
    int lhs = parameter_value[ parameter_id ];
    value   = executeOp( lhs, op, rhs );
  }

protected:
  ~OneParameterConstraint() {}

private:
  OneParameterConstraint( int eparameter_id, COMP_OPERATOR opp, int erhs ) {
    parameter_id = eparameter_id;
    op           = opp;
    rhs          = erhs;
  }

  CounterConstraint *copy() const {
    return new OneParameterConstraint( parameter_id, op, rhs );
  }

  std::ostream &dump( ostream &out ) const {
    out << std::setw( VALUE_OUT_WIDTH );
    if ( value ) {
      out << "true";
    } else {
      out << "false";
    }
    return out;
  }
  bool          value;
  int           parameter_id;
  COMP_OPERATOR op;
  int           rhs;
  friend class InstanceFactory;
};

/**
 *  Form of counter op constant value
 *
 */
class OneCounterConstraint : public CounterConstraint {

public:
  /**
   * @return true if  the vlaue corresponding counter op rhs, false otherwise.
   */
  bool operator()( const int *counter_value ) const {
    return executeOp( counter_value[ global_counter_id ], op, rhs );
  }
  void globalUpdate( const vector<int> &id_map,
                     const vector<int> &  parameter_value ) {

    global_counter_id = vector[ref_parameter_id];
  }

protected:
  ~OneCounterConstraint() {}

private:
  OneCounterConstraint( int out_ref_parameter_id, COMP_OPERATOR opp, int right_side ) {

    ref_parameter_id  = out_ref_parameter_id;
    global_counter_id = 0;
    op                = opp;
    rhs               = right_side;
  }

  CounterConstraint *copy() const {

    return new OneCounterConstraint( ref_parameter_id, op, rhs );
  }
  ostream &dump( ostream &out ) const {
    out << "counter_" << global_counter_id << setw( OP_OUT_WIDTH )
        << getOpStr( op ) << setw( VALUE_OUT_WIDTH ) << rhs;
    return out;
  }

  int           ref_parameter_id;
  int           global_counter_id;
  COMP_OPERATOR op;
  int           rhs;
  friend class InstanceFactory;
};

/**
 *  Form of counter - parameter op rhs
 */

class CounterParameterConstraint : public CounterConstraint {

public:
  /**
   * @return  true if the counter op parameter, false otherwise.
   */
  bool operator()( const int *counter_value ) const {
    return executeOp( counter_value[ global_counter_id ], op, rhs );
  }

  void globalUpdate( const vector<int> &id_map,
                     const vector<int> &  parameter_value ) {

    global_counter_id = id_map[ref_parameter_id];
    rhs               = parameter_value[ parameter_id ] + erhs;
  }

protected:
  ~CounterParameterConstraint() {}

private:
  int           ref_parameter_id;
  int           global_counter_id;
  COMP_OPERATOR op;
  int           parameter_id;
  int           erhs;
  int           rhs;

  CounterParameterConstraint( int out_ref_parameter_id, int eparameter_id,
                              COMP_OPERATOR opp, int out_rhs ) {

    ref_parameter_id = out_ref_parameter_id;
    op               = opp;
    parameter_id     = eparameter_id;
    erhs             = out_rhs;
    rhs              = 0;
  }

  CounterConstraint *copy() const {

    return new CounterParameterConstraint( ref_parameter_id, parameter_id, op,
                                           erhs );
  }
  ostream &dump( ostream &out ) const {
    out << "counter_" << global_counter_id << setw( OP_OUT_WIDTH )
        << getOpStr( op ) << setw( VALUE_OUT_WIDTH ) << rhs;
    return out;
  }

  friend class InstanceFactory;
};

/**
 * Form of parameterA - parameterB op rhs (constant value)
 */
class TwoParameterConstraint : public CounterConstraint {
public:
  bool operator()( const int *counter_value ) const { return value; }
  void globalUpdate( const vector<int> &id_map,
                     const vector<int> &  parameter_value ) {
    int lhs = parameter_value[ first_paramter_id ] -
              parameter_value[ second_parameter_id ];
    value = executeOp( lhs, op, rhs );
  }

protected:
  ~TwoParameterConstraint() {}

private:
  TwoParameterConstraint( int efirst_parameter_id, int esecond_parameter_id,
                          COMP_OPERATOR eop, int erhs ) {

    value               = false;
    first_paramter_id   = efirst_parameter_id;
    second_parameter_id = esecond_parameter_id;
    op                  = eop;
    rhs                 = erhs;
  }
  CounterConstraint *copy() const {
    return new TwoParameterConstraint( first_paramter_id, second_parameter_id,
                                       op, rhs );
  }
  ostream &dump( ostream &out ) const {
    out << std::setw( VALUE_OUT_WIDTH );
    if ( value ) {
      out << "true";
    } else {
      out << "false";
    }
    return out;
  }

  bool          value;
  int           first_paramter_id, second_parameter_id, rhs;
  COMP_OPERATOR op;
  friend class InstanceFactory;
};

/**
 * Form of counterA - counterB op constant value
 */

class TwoCounterConstraint : public CounterConstraint {

public:
  /**
   * @return  true if counterA - counterB op constant, false otherwise.
   */
  bool operator()( const int *counter_value ) const {
    int diff =
        counter_value[ global_counter_x ] - counter_value[ global_counter_y ];
    return executeOp( diff, op, rhs );
  }

  void globalUpdate( const vector<int> &id_map,
                     const vector<int> &  parameter_value ) {
    if ( parameter_id > -1 ) {
      rhs = parameter_value[ parameter_id ];
    }

    global_counter_x = id_map[ ref_parameter_id_x ];
    global_counter_y = id_map[ref_parameter_id_y];
  }

protected:
  ~TwoCounterConstraint() {}

private:
  TwoCounterConstraint( int x, int y, COMP_OPERATOR p, int r,
                        int out_parameter_id = -10 ) {
    parameter_id = out_parameter_id;

    ref_parameter_id_x = x;
    ref_parameter_id_y = y;
    op              = p;
    rhs             = r;
  }
  CounterConstraint *copy() const {

    return new TwoCounterConstraint( ref_parameter_id_x, ref_parameter_id_y, op, rhs,
                                     parameter_id );
  }
  ostream &dump( ostream &out ) const {
    out << "counter_" << global_counter_x << setw( OP_OUT_WIDTH ) << "-"
        << "counter_" << global_counter_x << setw( OP_OUT_WIDTH )
        << getOpStr( op ) << setw( VALUE_OUT_WIDTH ) << rhs;
    return out;
  }

  int ref_parameter_id_x, ref_parameter_id_y, global_counter_x, global_counter_y;
  COMP_OPERATOR op;
  int           rhs;
  int           parameter_id;
  friend class InstanceFactory;
};

/**
 * Form  \sum c_i counter_i op \sum f_i parameter_i
 *
 */

class DefaultCounterConstraint : public CounterConstraint {

public:
  /**
   * @return  true if \sum c_i counter_i op \sum f_i parameter_i
   */
  bool operator()( const int *counter_value ) const {
    int dummy = parameter_part;

    for ( size_t i = 0; i < local_constraint.size(); i += 2 ) {
      dummy +=
          local_constraint[ i ] * counter_value[ local_constraint[ i + 1 ] ];
    }
    return executeOp( dummy, op, rhs );
  }

  void globalUpdate( const vector<int> & id_map,
                     const vector<int> &  parameter_value ) {

    for ( size_t i = 1; i < local_constraint.size(); i += 2 ) {
      global_constraint[ i ] = id_map[local_constraint[ i ]];
    }

    parameter_part = 0;
    for ( size_t i = 0; i < pconstraint.size(); i += 2 ) {
      parameter_part +=
          pconstraint[ i ] * parameter_value[ pconstraint[ i + 1 ] ];
    }
  }

protected:
  ~DefaultCounterConstraint() {}

private:
  DefaultCounterConstraint( const vector<int> &pcons, const vector<int> &cons,
                            int erhs, COMP_OPERATOR eop )
      : pconstraint( pcons )
      , local_constraint( cons )
      , global_constraint( cons )
      , rhs( erhs )
      , op( eop ) {}

  CounterConstraint *copy() const {

    return new DefaultCounterConstraint( pconstraint, local_constraint, rhs,
                                         op );
  }
  ostream &dump( ostream &out ) const { return out; }

private:
  vector<int>   pconstraint;
  vector<int>   local_constraint;
  vector<int>   global_constraint;
  int           rhs;
  COMP_OPERATOR op;
  int           parameter_part;
  friend class InstanceFactory;
};

} // namespace graphsat

#endif
