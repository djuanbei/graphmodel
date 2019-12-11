
/**
 * @file   linsimpcons.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Mar 29 10:47:56 2019
 *
 * @brief  linear simple constraint such clock_x-clock_y < ( <= ) right
 *
 *
 */
#ifndef LIN_SIMP_CONS_HPP
#define LIN_SIMP_CONS_HPP


#include <iomanip>
#include <iostream>
#include <random>


#include "util/dbmutil.hpp"
#include "model/clock.h"

namespace graphsat {

using namespace std;

const int GLOBAL_CLOCK_ID = 0;

/**
 *  clock_x -clock_y < ( <= ) realRight
 *
 */
class ClockConstraint {

public:

  ClockConstraint( const Clock& clock_id1,COMP_OPERATOR eop, const int rhs);
  
  ClockConstraint( const Clock& clock_id1, const Clock& clock_id2, COMP_OPERATOR eop,
                   const int rhs, const int eparameter_id = -100 );
  


  void globalUpdate( const vector<int> &parameter_value );

  void clockShift( int shift );

  ClockConstraint neg( void ) const;

  bool isSat( const ClockConstraint &cons ) const;

  friend std::ostream &operator<<( std::ostream &         out,
                                   const ClockConstraint &cons );

private:
  /**
   is_strict_ref  is true : <
   is_strict_ref  is true : <=
   */
  ClockConstraint( const int clock_id1, const int clock_id2, const int rhs,
                   bool is_strict_ref );

  void neg_impl( void );

  void init( const int clock_id1, const int clock_id2, COMP_OPERATOR eop,
             const int rhs );

public:
  int           clock_x;
  int           clock_y;
  COMP_OPERATOR op;
  int           matrix_value;

private:
  int                    parameter_id;
  friend ClockConstraint randConst(const int num, const int low, const int up );
  friend class DBMFactory;
};
ClockConstraint randConst(const int num, const int low, const int up );

} // namespace graphsat
#endif
