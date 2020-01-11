/**
 * @file   example.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Dec  5 09:29:10 2019
 *
 * @brief  some example.h
 *
 *
 */

#ifndef EXAMPLE_H
#define EXAMPLE_H

namespace graphsat {

void example1(void);
void example50();

void example2(void);

void example5(void);

void example6();

void fischer(int n = 2);

void fischerSymmetry( int n=3);

void testIsConsistent();

void incrementalTestFS();

void incrementalTest();

void incrementalTestTG();

void fisher1();

void testOP();

void testcompression();

void train_gate(const int n);

void lift_customer(int n);

}  // namespace graphsat

#endif
