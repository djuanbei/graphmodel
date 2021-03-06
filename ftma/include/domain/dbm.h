
/**
 * @file DBMFactory.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date Thu Mar 28 19:47:51 2019
 *
 * @brief difference bound D
 * ref paper Timed Automata: Semantics, Algorithms and Tools
 *
 * ftmd: fast timed automata model checker
 *
 */

#ifndef __DBM__HPP
#define __DBM__HPP
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <map>
#include <random>
#include <sstream>
#include <utility>
#include <vector>

#include "constraint/clockdiffcons.h"
#include "util/dbmutil.hpp"
#include "util/fastHash.h"

namespace graphmodel {
// using namespace std;

#define ADD(x, y)                               \
  (((x) >= MAX_INT || (y) >= MAX_INT) ? MAX_INT \
                                      : ((x) + (y) - (((x)&1) | ((y)&1))))

#define LOC(row, col) (row) * (clock_num) + (col)

#define LOC_N(row, col, N) (row) * (N) + (col)

#define IMPL(FUN, dbm)            \
  int* new##dbm = createDBM(dbm); \
  FUN##Impl(new##dbm);            \
  return new##dbm;

static const std::string DBM_DOT_TABLE_HEADER =
    "<table border=\"1\"  bgcolor=\"#00FF00\" >";

struct MatrixValue {
  MatrixValue() : strict(false), value(0) {}
  MatrixValue(int matrix_value);
  MatrixValue(int value, bool s);
  bool operator==(const MatrixValue& other) const;
  int matrixValue() const;
  bool strict;
  int value;
};

const static int GLOBAL_CLOCK_ID = 0;

class DBMManager {
 public:
  DBMManager(void) {
    clock_num = 0;
    matrix_size = 0;
    MAX_INT = getMAX_INT<int>();

    distribution = std::uniform_int_distribution<int>(-100, 100);
  }

  explicit DBMManager(const int n);

  DBMManager(const int n, const std::vector<int>& oclockUppuerBound,
             const std::vector<ClockConstraint>& odifferenceCons);

  ~DBMManager() { clock_num = 0; }

  void setClockNum(int num) {
    clock_num = num + 1;
    matrix_size = clock_num * clock_num;
  }

  int* createDBM() const {
    int* dbm = new int[matrix_size];
    std::fill(dbm, dbm + matrix_size, LTEQ_ZERO);  // x-y<=0
    return dbm;
  }

  /**
   * Create a new matrix and initial the values with D
   *
   * @param dbm dbm matrix
   *
   * @return a copy dbm matrix
   */
  int* createDBM(const int* const dbm) const {
    int* newD = new int[matrix_size];
    memcpy(newD, dbm, sizeof(int) * matrix_size);
    return newD;
  }

  void destroyDBM(int* dbm) const { delete[] dbm; }

  void init(int* dbm) const {
    std::fill(dbm, dbm + matrix_size, LTEQ_ZERO);  // x-y<=0
  }

  int* randomDBM() {
    int* newD = new int[matrix_size]();
    for (int i = 0; i < matrix_size; i++) {
      newD[i] = distribution(generator);
    }
    return newD;
  }

  int* randomFeasiableDBM() const;

  int getSize() const { return matrix_size; }

  static std::ostream& dumpDot(std::ostream& out, const int* const dbm,
                               const int clock_num);

  static std::ostream& dump(std::ostream& out, const int* const dbm,
                            const int clock_num);

  std::ostream& dumpDot(std::ostream& out, const int* const dbm) const {
    return DBMManager::dumpDot(out, dbm, clock_num);
  }

  std::ostream& dump(std::ostream& out, const int* const dbm) const {
    return DBMManager::dump(out, dbm, clock_num);
  }
  // dump clock_ids clocks constrainted relations
  std::ostream& dump(std::ostream& out, const int* const dbm,
                     const std::vector<int>& clock_ids) const;

  void dump(const int* const dbm) const { dump(std::cout, dbm); }

  uint32_t getHashValue(const int* const dbm) const {
    return FastHash((char*)dbm, sizeof(int) * matrix_size);
  }

  bool equal(const int* lhs, const int* rhs) const {
    return 0 == memcmp(lhs, rhs, matrix_size * sizeof(int));
  }

  /**
   * recovery memory
   *
   * @param Cvec the vector need to delete
   */
  void deleteVectorM(std::vector<int*>& Cvec) const {
    for (auto d : Cvec) {
      delete[] d;
    }
    Cvec.clear();
  }

  /**
   * Floyds algorithm for computing all shortest path
   * TODO improve efficient
   */
  void canonicalForm(int* dbm) const;

  /**
   *
   * precondition D is canonicalForm
   * @param dbm a dbm matrix
   *
   * @return true if DBMFactory D is not empty,
   * false otherwise.
   */
  bool isConsistent(const int* const dbm) const { return dbm[0] >= LTEQ_ZERO; }

  /**
   *
   *
   * @brief  check whether lhs >= rhs, in other words whether lhs contains rhs
   *

   * @return true if this is included by other lhs>= rhs
   */
  bool include(const int* const lhs, const int* const rhs) const;

  /**
   * If lhs contains  rhs, then getIncludeFeature(lhs ) >= getIncludeFeature(
   * rhs)
   *
   */

  DF_T getIncludeFeature(const int* const dbm) const;

  /**
   * D_{y,x} + cons.matrix_value ">=" 0
   } x-y < ( <= ) rhs and
   * y-x < ( <= ) d is feasiable if and only if -rhs< d

   * This is to check D and x-y < (<=) m is non-empty
   * @param cons the constraints vector
   *
   * @return true if there is a value in this domain which satisfies cons
   * false, otherwise.
   */
  bool isSatisfied(const int* const dbm, const ClockConstraint& cons) const;

  void upImpl(int* dbm) const;

  /**
   * Can not modify value of D
   * compute strongest post condition
   * up(D)={u+d | u \in D, d\in R+ }
   *
   */
  int* up(const int* const dbm) const { IMPL(up, dbm); }

  void downImpl(int* dbm) const;

  /**
   * Can not modify value of D
   * compute weakest precondition
   *
   */
  int* down(const int* const dbm) const { IMPL(down, dbm); }

  void andImpl(int* newD, const ClockConstraint& cons) const;

  /**
   * Can not modify value of D
   * The most used operation in state-space exploration in conjunction
   */
  int* And(const int* const dbm, const ClockConstraint& cons) const {
    int* newD = createDBM(dbm);
    andImpl(newD, cons);
    return newD;
  }

  void freeImpl(int* dbm, const int x) const;

  /**
   * Can not modify value of D
   * The free operation removes all constraints on a given clock x.
   *
   */
  int* free(const int* const dbm, const int x) const {
    int* newdbm = createDBM(dbm);
    freeImpl(newdbm, x);
    return newdbm;
  }

  void resetImpl(int* dbm, const int x, const int m) const;

  /**
   * In forwards exploration this operation is used to set clocks to specific
   * values x:=m Can not modify value of D
   * @param x the reset clock id
   * @param m rhe reset clock value
   */
  int* reset(const int* const dbm, const int x, const int m) const {
    int* newD = createDBM(dbm);
    resetImpl(newD, x, m);
    return newD;
  }

  void copyImpl(int* dbm, const int x, const int y) const;

  /**
   * This is another operation used in forwards state space exporation.
   * Can not modify value of D
   * @param x source clock id
   * @param y target clock id
   */
  int* copy(const int* const dbm, const int x, const int y) const {
    int* newD = createDBM(dbm);
    copyImpl(newD, x, y);
    return newD;
  }

  int* shiftImpl(int* dbm, const int x, const int m) const;

  /**
   *@brief x-> x+m
   * The last reset operation is shifting a clock, i.e. adding or substracting
   * a clock with an integer value.
   *
   * @param x the shift clock id
   * @param m the reset clock value
   */
  int* shift(const int* const dbm, const int x, const int m) const {
    int* newD = createDBM(dbm);
    shiftImpl(newD, x, m);
    return newD;
  }

  /**
   * For a timed automaton and safty prperty to be checked, that contain no
   * difference constraints. assert(maximum.matrix_size()==2*clock_num) maximum[
   * i ]:=
   * <= k_i maximum[i+clock_num]:= < -k_i
   * @param maximums maximums[i] is the maximum upper for x_i
   */
  void norm(int* dbm, const std::vector<int>& maximums) const;

  /**
   * TODO: The difference bounds will adjust depend on source
   *
   * @param dbm the dbm matrix which want to norm
   * @param re_vec the return norm dbm vector
   */
  void norm(int* dbm, std::vector<int*>& re_vec) const;

  void norm(int* dbm) const { norm(dbm, clock_upper_bounds); }

  /**
   * @brief For compress the state data, we reduce the data size. The result dbm
   * does not protect canonicalForm. The result matrix element guarantee that
   * for x-y< c, c <= clock_upper_bounds[ x]+1, and c >= clock_upper_bounds[ y]
   * @param dbm
   */

  void encode(int* dbm) const;

  void decode(int* dbm) const;
  /**
   get the clock_i -clock_y constraint in this dbm
   */

  ClockConstraint getCons(const int* const dbm, const int i, const int j) const;

  void swap(int* dbm, int clock_x, int clock_y) const;

  int at(const int* const dbm, const int i, const int j) const {
    return dbm[LOC(i, j)];
  }
  static int getIndex(int i, int j, const int clock_num) {
    return LOC_N(i, j, clock_num);
  }
  // // x-y >(>= )
  // MatrixValue getLowerBound( const int * const dbm ,const int clock_x, const
  // int clock_y) const;

  // void setLowerBound( int * dbm, const int clock_x, const int clock_y, const
  // MatrixValue & value ) const;

  // x-y < ( <=)  re
  MatrixValue getUpperBound(const int* const dbm, const int clock_x,
                            const int clock_y) const;

  // x-y < ( <=)  value
  void setUppperBound(int* dbm, const int clock_x, const int clock_y,
                      const MatrixValue& value) const;

  // // x> ( >=) re
  // MatrixValue getLowerBound( const int * const dbm, const int clock_id)
  // const;
  // //  x > ( x>=)  value
  // void setLowerBound( int * dbm, const int clock_id, const MatrixValue &
  // value) const;

  // // x< ( <=) re
  // MatrixValue getUpperBound( const int * const dbm, const int clock_id)
  // const;

  // // x < ( <= ) value
  // void setUppperBound( int * dbm, const int clock_id,  const MatrixValue &
  // value) const;

  // every clock id >=1
  int* project(const int* const dbm, const std::vector<int>& clock_ids) const;

 private:
  /**
   * number of clocks
   */
  int clock_num;    // contain global clock which id is 0
  int matrix_size;  // clock_num*clock_num
  int MAX_INT;
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution;
  std::vector<int> clock_upper_bounds;

  std::vector<ClockConstraint> difference_cons;

  bool contain(const std::vector<int*>& values, const int* const dbm) const {
    for (auto v : values) {
      if (equal(v, dbm)) {
        return true;
      }
    }
    return false;
  }

  /**
   * For automaton containing difference constraints in the guards, it is more
   * complicated and expensive to compute the normalized zones.
   * assert(maximums.matrix_size()==maximums*clock_num)
   * maximums[ i ]:= <= maximums_i
   * maximums[clock_num +n]:= < -maximums_i
   * @param dbm D will deen destroied before return
   * @param maximums maximums[i] is the maximum upper for x_i
   * @param diff_cons All different constraint in model like x-y < c
   *@param re_vec the return dbm vector
   */
  void norm(int* dbm, const std::vector<int>& maximums,
            const std::vector<ClockConstraint>& diff_cons,
            std::vector<int*>& re_vec) const;

  /**
   split dbm to some smaller box in which the entire domain is
   satisfy or not in diffCons.
   */
  void split(int* dbm, const std::vector<ClockConstraint>& diffCons,
             std::vector<int*>& re_vec) const;

  int* corn_norm(int* dbm, const std::vector<int>& maximums,
                 const std::vector<ClockConstraint>& difference_cons) const;

  static std::ostream& dumpElement(std::ostream& out, const int* const dbm,
                                   int i, int j, int clock_num);
};

}  // namespace graphmodel

#endif
