#include "domain/dbm.h"
#include <cassert>

namespace graphsat {
DBMFactory::DBMFactory(const int n) : clock_num(n + 1) {
  matrix_size = clock_num * clock_num;
  MAX_INT = getMAX_INT<int>();

  distribution = std::uniform_int_distribution<int>(-100, 100);

  for (int i = 0; i < clock_num; i++) {
    clock_upper_bounds.push_back(MAX_INT / 4);
  }

  for (int i = 0; i < clock_num; i++) {
    clock_upper_bounds.push_back(-(MAX_INT / 4));
  }
}

DBMFactory::DBMFactory(int n, const vector<int> &oclockUppuerBound,
                       const vector<ClockConstraint> &odifferenceCons)
    : clock_num(n + 1) {
  matrix_size = clock_num * clock_num;

  MAX_INT = getMAX_INT<int>();

  distribution = std::uniform_int_distribution<int>(-100, 100);
  clock_upper_bounds = oclockUppuerBound;
  difference_cons = odifferenceCons;
}

int *DBMFactory::randomFeasiableDBM() {
  int num_cons = sqrt(clock_num) + 2;
  int *dbm = createDBM();
  while (num_cons >= 0) {
    init(dbm); // x-y<=0
    upImpl(dbm);
    for (int i = 1; i < sqrt(clock_num); i++) {
      int rhs = abs(distribution(generator)) + 50;
      ClockConstraint cs(i, 0, LT, rhs);
      andImpl(dbm, cs);
    }
    for (int i = 0; i < sqrt(clock_num); i++) {
      int x = abs(distribution(generator)) % (clock_num);
      if (x > 0)
        freeImpl(dbm, x);
    }
    for (int i = 0; i < num_cons; i++) {
      int x = abs(distribution(generator)) % (clock_num);
      int y = abs(distribution(generator)) % (clock_num);
      if (x != y) {
        int rhs = distribution(generator);
        ClockConstraint cs(x, y, LT, rhs);
        andImpl(dbm, cs);
      }
      x = abs(distribution(generator)) % (clock_num);
      if (x > 0)
        freeImpl(dbm, x);
    }
    for (int i = 0; i < matrix_size; i++) {
      if (dbm[i] > MAX_INT / 10) {
        dbm[i] = MAX_INT / 10;
      } else if (dbm[i] < -(MAX_INT / 10)) {
        dbm[i] = -(MAX_INT / 10);
      }
    }
    canonicalForm(dbm);
    if (isConsistent(dbm)) {
      return dbm;
    }
    num_cons -= 1;
  }
  init(dbm); // x-y<=0
  return dbm;
}

ostream &DBMFactory::dump(ostream &out, const int *const dbm) const {

  for (int i = 0; i < clock_num; i++) {
    out << "[ ";
    for (int j = 0; j < clock_num; j++) {
      out << "(";
      int v = dbm[LOC(i, j)];
      out << setw(3);
      if (isStrict<int>(v)) {
        out << "< ";
      } else {
        out << "<=";
      }
      out << setw(4);
      int right = getRight(v);
      if (right >= MAX_INT / 2) {
        out << std::left << ((char)126);
      } else {
        out << std::left << (int)right;
      }

      out << ") ";
    }
    out << "]\n";
  }
  return out;
}

void DBMFactory::canonicalForm(int *dbm) const {
  for (int k = 0; k < clock_num; k++) {
    for (int i = 0; i < clock_num; i++) {
      int row_index = LOC(i, 0);
      for (int j = 0; j < clock_num; j++) {
        int temp = ADD(dbm[row_index + k], dbm[LOC(k, j)]);
        dbm[row_index + j] =
            dbm[row_index + j] < temp ? dbm[row_index + j] : temp;
      }
    }
  }
}

bool DBMFactory::include(const int *const lhs, const int *const rhs) const {

  for (int i = 0; i < matrix_size; i++) {
    if (lhs[i] < rhs[i]) {
      return false;
    }
  }
  return true;
}

DF_T DBMFactory::getIncludeFeature(const int *const dbm) const {
  DF_T re = 0;

  for (int i = 0; i < clock_num; i++) {
    int k = LOC(i, 0);
    for (int j = 0; j < clock_num; j++) {
      if (i == j)
        continue;
      re = (2 * re * MAX_INT + (dbm[k + j] + MAX_INT));
    }
  }
  return re;
}

bool DBMFactory::isSatisfied(const int *const dbm,
                             const ClockConstraint &cons) const {

  return ADD(cons.matrix_value, dbm[LOC(cons.clock_y, cons.clock_x)]) >=
         LTEQ_ZERO;
}

void DBMFactory::upImpl(int *dbm) const {
  int row_index = 0;
  for (int i = 1; i < clock_num; i++) {
    row_index += clock_num;
    dbm[row_index] = MAX_INT;
  }
}

void DBMFactory::downImpl(int *dbm) const {
  for (int i = 1; i < clock_num; i++) {
    dbm[i] = LTEQ_ZERO;
    for (int j = 1; j < clock_num; j++) {
      int k = LOC(j, i);
      dbm[i] = dbm[i] < dbm[k] ? dbm[i] : dbm[k];
    }
  }
}

void DBMFactory::andImpl(int *newD, const ClockConstraint &cons) const {
  if (newD[0] < LTEQ_ZERO) {
    return;
  }

  if (ADD(cons.matrix_value, newD[LOC(cons.clock_y, cons.clock_x)]) <
      LTEQ_ZERO) {
    newD[0] = getMatrixValue(-1, false);
  } else if (cons.matrix_value < newD[LOC(cons.clock_x, cons.clock_y)]) {
    newD[LOC(cons.clock_x, cons.clock_y)] = cons.matrix_value;
    for (int i = 0; i < clock_num; i++) {
      for (int j = 0; j < clock_num; j++) {
        int temp1 = ADD(newD[LOC(i, cons.clock_x)], newD[LOC(cons.clock_x, j)]);
        int k = LOC(i, j);
        newD[k] = newD[k] < temp1 ? newD[k] : temp1;

        int temp2 = ADD(newD[LOC(i, cons.clock_y)], newD[LOC(cons.clock_y, j)]);
        newD[k] = newD[k] < temp2 ? newD[k] : temp2;
      }
    }
  }
}
void DBMFactory::freeImpl(int *dbm, const int x) const {
  for (int i = 0; i < clock_num; i++) {
    dbm[LOC(x, i)] = MAX_INT;
    dbm[LOC(i, x)] = dbm[LOC(i, 0)];
  }
  dbm[LOC(x, x)] = LTEQ_ZERO;
}

void DBMFactory::resetImpl(int *dbm, const int x, const int m) const {
  // clock id start from 1
  assert(x > 0);
  assert(m >= 0);
  int postM = getMatrixValue(m, false);
  int negM = getMatrixValue(-m, false);
  int xStart = LOC(x, 0);

  for (int i = 0; i < clock_num; i++) {
    dbm[xStart + i] = ADD(postM, dbm[i]);
    dbm[LOC(i, x)] = ADD(dbm[LOC(i, 0)], negM);
  }

  dbm[LOC(x, x)] = LTEQ_ZERO;
}

void DBMFactory::copyImpl(int *dbm, const int x, const int y) const {
  for (int i = 0; i < clock_num; i++) {
    dbm[LOC(x, i)] = dbm[LOC(y, i)];
    dbm[LOC(i, x)] = dbm[LOC(i, y)];
  }
  dbm[LOC(x, x)] = dbm[LOC(x, y)] = dbm[LOC(y, x)] = LTEQ_ZERO;
}

int *DBMFactory::shiftImpl(int *dbm, const int x, const int m) const {

  int postM = getMatrixValue(m, false);
  int negM = getMatrixValue(-m, false);

  for (int i = 0; i < clock_num; i++) {
    dbm[LOC(x, i)] = ADD(dbm[LOC(x, i)], postM);
    dbm[LOC(i, x)] = ADD(dbm[LOC(i, x)], negM);
  }

  dbm[LOC(x, x)] = LTEQ_ZERO;
  int temp = LOC(x, 0);
  dbm[temp] = dbm[temp] > LTEQ_ZERO ? dbm[temp] : LTEQ_ZERO;
  dbm[x] = dbm[x] < LTEQ_ZERO ? dbm[x] : LTEQ_ZERO;

  return dbm;
}

void DBMFactory::norm(int *dbm, const vector<int> &maximums) const {
  bool modify = false;
  for (int i = 0; i < clock_num; i++) {
    int row_index = LOC(i, 0);
    for (int j = 0; j < clock_num; j++) {
      int index = row_index + j;
      if (dbm[index] < MAX_INT) {
        if (dbm[index] > maximums[i]) {
          modify = true;
          dbm[index] = MAX_INT;
        } else if (dbm[index] < maximums[j + clock_num]) {
          modify = true;
          dbm[index] = maximums[j + clock_num];
        }
      }
    }
  }
  if (modify) {
    canonicalForm(dbm);
  }
}

void DBMFactory::norm(int *dbm, vector<int *> &re_vec) const {
  if (difference_cons.empty()) {
    norm(dbm, clock_upper_bounds);
    re_vec.push_back(dbm);
    return;
  }
  norm(dbm, clock_upper_bounds, difference_cons, re_vec);
}

void DBMFactory::encode(int *dbm) const {

  for (int i = 0; i < clock_num; i++) {
    int row_index = LOC(i, 0);
    if (i == 0) {
      for (int j = 0; j < clock_num; j++) {
        if (dbm[row_index + j] > LTEQ_ZERO) {
          dbm[row_index + j] = LTEQ_ZERO;
        }
      }
    } else {
      for (int j = 0; j < clock_num; j++) {
        if (j == i) {
          dbm[row_index + j] = LTEQ_ZERO;
        } else if (dbm[row_index + j] > clock_upper_bounds[i]) {
          dbm[row_index + j] = clock_upper_bounds[i] + 1;
        }
      }
    }
  }
}

void DBMFactory::decode(int *dbm) const {

  for (int i = 0; i < clock_num; i++) {
    int row_index = LOC(i, 0);
    for (int j = 0; j < clock_num; j++) {
      if (dbm[row_index + j] > clock_upper_bounds[i]) {
        dbm[row_index + j] = MAX_INT;
      }
    }
  }
}

ClockConstraint DBMFactory::getCons(const int *const dbm, const int i,
                                    const int j) const {
  assert(i <= clock_num);
  assert(j <= clock_num);
  if (isStrict(dbm[LOC(i, j)])) {

    return ClockConstraint(Argument(NORMAL_VAR_ARG, i),
                           Argument(NORMAL_VAR_ARG, j), LT,
                           Argument(getRight(dbm[LOC(i, j)])));
  } else {
    return ClockConstraint(Argument(NORMAL_VAR_ARG, i),
                           Argument(NORMAL_VAR_ARG, j), LE,
                           Argument(getRight(dbm[LOC(i, j)])));
  }
}

void DBMFactory::norm(int *dbm, const vector<int> &maximums,
                      const vector<ClockConstraint> &diff_cons,
                      vector<int *> &re_vec) const {

  assert(re_vec.empty());

  vector<int *> split_domains;
  split(dbm, diff_cons, split_domains);

  for (auto temp_dbm : split_domains) {
    re_vec.push_back(corn_norm(temp_dbm, maximums, diff_cons));
  }
}

void DBMFactory::split(int *dbm, const vector<ClockConstraint> &diffCons,
                       vector<int *> &re_vec) const {

  assert(re_vec.empty());
  vector<int *> wait_s;
  re_vec.push_back(dbm);

  for (auto cs : diffCons) {

    vector<bool> addToWaitS(re_vec.size(), false);

    for (size_t i = 0; i < re_vec.size(); i++) {
      /**
       * split
       * (D and int) && (D and -int) satisfies then using int to split D
       * into two parts
       */
      if (isSatisfied(re_vec[i], cs) && isSatisfied(re_vec[i], cs.neg())) {

        int *D_and_C = And(re_vec[i], cs);
        int *D_and_neg_C = And(re_vec[i], cs.neg());
        if (!contain(wait_s, D_and_C)) {
          wait_s.push_back(D_and_C);
        } else {
          destroyDBM(D_and_C);
        }
        if (!contain(wait_s, D_and_neg_C)) {
          wait_s.push_back(D_and_neg_C);
        } else {
          destroyDBM(D_and_neg_C);
        }

      } else {
        if (!contain(wait_s, re_vec[i])) {
          wait_s.push_back(re_vec[i]);
          addToWaitS[i] = true;
        }
      }
    }
    re_vec.swap(wait_s);
    for (size_t i = 0; i < wait_s.size(); i++) {
      if (!addToWaitS[i]) {
        destroyDBM(wait_s[i]);
      }
      wait_s.clear();
    }
  }
}

int *DBMFactory::corn_norm(
    int *dbm, const vector<int> &maximums,
    const vector<ClockConstraint> &difference_cons) const {

  vector<ClockConstraint> G_unsat;

  for (size_t i = 0; i < difference_cons.size(); i++) {
    /**
     * If D and int does not satisiable then norm(D,k) and int does not
     * satisable
     *
     */

    if (!isSatisfied(dbm, difference_cons[i])) {
      G_unsat.push_back(difference_cons[i]);
    }
    /**
     * If D and neg(int) does not satisiable then norm(D,k) and int does not
     * satisable
     *
     */

    if (!isSatisfied(dbm, difference_cons[i].neg())) {
      G_unsat.push_back(difference_cons[i].neg());
    }
  }

  norm(dbm, maximums);
  for (auto cs : G_unsat) {
    andImpl(dbm, cs.neg());
  }
  return dbm;
}

} // namespace graphsat
