/**
 * @file   parallel.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon May  6 14:57:54 2019
 *
 * @brief   parallel util
 *
 *
 */

#ifndef __PARALLE_H
#define __PARALLE_H
// openmp
#if defined(OPENMP)
#include <omp.h>
#define cilk_spawn
#define cilk_sync
#define parallel_main main
#define parallel_for _Pragma("omp parallel for") for
#define parallel_for_1 _Pragma("omp parallel for schedule (static,1)") for
#define parallel_for_256 _Pragma("omp parallel for schedule (static,256)") for
static int getWorkers() { return omp_get_max_threads(); }
static void setWorkers(int n) { omp_set_num_threads(n); }

// c++
#else
#define cilk_spawn
#define cilk_sync
#define parallel_main main
#define parallel_for for
#define parallel_for_1 for
#define parallel_for_256 for
#define cilk_for for
static int getWorkers() { return 1; }
static void setWorkers(int n) {}

#endif

#endif
