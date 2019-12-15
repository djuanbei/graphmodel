#include "util/parallel.h"

// openmp
#if defined(OPENMP)
#include <omp.h>

int getWorkers() { return omp_get_max_threads(); }
void setWorkers(int n) { omp_set_num_threads(n); }

// c++
#else

int getWorkers() { return 1; }
void setWorkers(int n) {}

#endif
