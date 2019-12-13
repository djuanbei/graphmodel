#include <random>
#include <vector>

#include "gtest/gtest.h"

#include "state/discretestate.hpp"
#include "constraint/clockdiffcons.h"

#include "domain/dbm.h"
#include "domain/dbmset.hpp"

using namespace graphsat;
using namespace std;

TEST( DBMFactory, createDBM ) {
  int             n   = 10;
  int             len = ( n + 1 ) * ( n + 1 );
  DBMFactory df( n );

  for ( int i = 0; i < 5; i++ ) {
    int *d = df.createDBM();
    for ( int j = 0; j < len; j++ ) {
      EXPECT_EQ( d[ j ], 1 );
    }
    df.destroyDBM( d );
  }
}
TEST( DBM, copy ) {
  int             n   = 12;
 
  DBMFactory df( n );
  for ( int i = 0; i < 10; i++ ) {
    int *d  = df.randomFeasiableDBM();
    int *d1 = df.createDBM( d );
    EXPECT_TRUE( df.equal( d, d1 ) );
    df.destroyDBM( d );
    df.destroyDBM( d1 );
  }
}

TEST( DBM, canonicalForm ) {
  int             n   = 12;

  DBMFactory df( n );
  for ( int i = 0; i < 10; i++ ) {
    int *d  = df.randomFeasiableDBM();
    int *d1 = df.createDBM( d );
    df.canonicalForm( d );
    df.canonicalForm( d1 );
    df.canonicalForm( d1 );
    EXPECT_TRUE( df.equal( d, d1 ) );
    df.destroyDBM( d );
    df.destroyDBM( d1 );
  }
}
TEST( DBM, isConsistent ) {
  int             n   = 3;
 
  DBMFactory df( n );
  for ( int i = 0; i < 10; i++ ) {
    int *                d    = df.randomFeasiableDBM();
    
    //df.dump( d);
    ClockConstraint cons = df.getCons( d, 1, 2 );
    
    //cout<<cons<<endl;
    cons    = cons.neg();
    //cout<<cons<<endl;
    int *d1 = df.And( d, cons );
    //df.dump( d1);
    EXPECT_FALSE( df.isConsistent( d1 ) );
    cons.matrix_value++;
    df.andImpl( d, cons );
    EXPECT_TRUE( df.isConsistent( d ) );
    df.destroyDBM( d );
    df.destroyDBM( d1 );
  }
}
TEST( DBM, include ) {
  int             n   = 12;
  //int             len = ( n + 1 ) * ( n + 1 );
  DBMFactory df( n );
  for ( int i = 0; i < 10; i++ ) {
    int *                d    = df.randomFeasiableDBM();
    ClockConstraint cons = randConst( n, -100, 100 );
    int *                d1   = df.And( d, cons );
    EXPECT_TRUE( df.include( d, d1 ) );
    cons=df.getCons( d, 4, 5 );
    df.destroyDBM( d1 );
    cons.matrix_value--;
    d1   = df.And( d, cons );
    EXPECT_FALSE(df.include( d1, d) );
    df.destroyDBM( d );
    df.destroyDBM( d1 );
  }
}

TEST(DBM, getIncludeFeature ){
  int             n   = 13;
  //int             len = ( n + 1 ) * ( n + 1 );
  DBMFactory df( n );
  for ( int i = 0; i < 10; i++ ) {
    int *                d    = df.randomFeasiableDBM();
    int *                d1   = df.createDBM( d);
    for( int j=0; j <100; j++){
      ClockConstraint cons = randConst( n, -100, 100 );
      df.andImpl(d1, cons);
      EXPECT_TRUE( df.include( d, d1 ) );
      DF_T df1=df.getIncludeFeature(d);
      DF_T df2=df.getIncludeFeature(d1);
      EXPECT_LE( df2, df1);
    }


    df.destroyDBM( d );
    df.destroyDBM( d1 );
  }
}

//int main( int argc, char *argv[] ) {
//  testing::InitGoogleTest( &argc, argv );
//  return RUN_ALL_TESTS();
//}