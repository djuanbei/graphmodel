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
#include <iostream>
// openmp
#if defined(OPENMP)
#include <omp.h>
#define cilk_spawn
#define cilk_sync
#define parallel_main main
#define parallel_for _Pragma("omp parallel for") for
#define parallel_for_1 _Pragma("omp parallel for schedule (static,1)") for
#define parallel_for_256 _Pragma("omp parallel for schedule (static,256)") for
int getWorkers();
void setWorkers(int n);

// c++
#else
#define cilk_spawn
#define cilk_sync
#define parallel_main main
#define parallel_for for
#define parallel_for_1 for
#define parallel_for_256 for
#define cilk_for for
int getWorkers();
void setWorkers(int n);

#endif

template <class T>
inline bool CAS(T* ptr, T oldv, T newv) {
  if (sizeof(T) == 1) {
    return __sync_bool_compare_and_swap((bool*)ptr, *((bool*)&oldv),
                                        *((bool*)&newv));
  } else if (sizeof(T) == 4) {
    return __sync_bool_compare_and_swap((int*)ptr, *((int*)&oldv),
                                        *((int*)&newv));
  } else if (sizeof(T) == 8) {
    return __sync_bool_compare_and_swap((long*)ptr, *((long*)&oldv),
                                        *((long*)&newv));
  } else {
    std::cout << "CAS bad length : " << sizeof(T) << std::endl;
    abort();
  }
}

template <class T>
inline bool writeMin(T* a, T b) {
  T c;
  bool r = 0;
  do
    c = *a;
  while (c > b && !(r = CAS(a, c, b)));
  return r;
}

template <class T>
inline void writeAdd(T* a, T b) {
  volatile T newV, oldV;
  do {
    oldV = *a;
    newV = oldV + b;
  } while (!CAS(a, oldV, newV));
}

#endif
