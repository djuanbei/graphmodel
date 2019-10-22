
#include <random>
#include <vector>

#include <gtest/gtest.h>

#include "discretestate.hpp"

#include "domain/dbm.hpp"
#include "domain/dbmset.hpp"

using namespace graphsat;
using namespace std;

std::default_random_engine         generator;
std::uniform_int_distribution<int> distribution( 0, 1000 );

int *randV( int n, int s ) {

  int *v = new int[ n ];

  for ( int i = 0; i < n; i++ ) {
    v[ i ] = distribution( generator );
  }

  return v;
}

TEST( StateSet, add_contain ) {
  StateSet<int> sets;

  vector<int *> vecs;
  int           n   = 10;
  int           s   = 4;
  int           num = 1000;
  sets.setParam( n, s );
  for ( int i = 0; i < num; i++ ) {
    int *temp = randV( n, s );
    if ( sets.add( temp ) ) {
      vecs.push_back( temp );
    } else {
      delete temp;
    }
  }
  for ( auto e : vecs ) {
    EXPECT_TRUE( sets.contain( e ) );
  }
}

TEST( DBMSET, add ) {

  vector<int *>   vecs;
  int             n = 10;
  DBMFactory<int> manager( n );
  DBMset<int>     sets( manager );
  int             num = 10000;

  for ( int i = 0; i < num; i++ ) {
    int *dbm = manager.randomFeasiableDBM();
    if ( sets.add( dbm ) )
      vecs.push_back( dbm );
    else
      manager.destroyDBM( dbm );
  }
  for ( auto e : vecs ) {
    EXPECT_TRUE( sets.contain( e ) );
  }
}
TEST( DBM, include ) {
  int                        MAX_INT = 100000;
  std::default_random_engine generator;
  distribution =
      std::uniform_int_distribution<int>( -MAX_INT / 4 + 1, MAX_INT / 4 );
  int             clock_num = 50;
  DBMFactory<int> manager( clock_num );
  int             num = 1000;
  for ( int i = 0; i < num; i++ ) {
    int *dbm = manager.randomFeasiableDBM();
    int  x   = abs( distribution( generator ) ) % ( clock_num + 1 );
    int  y   = abs( distribution( generator ) ) % ( clock_num + 1 );
    if ( x != y ) {
      int                  rhs = distribution( generator );
      ClockConstraint<int> cs( x, y, LE, rhs );
      int *                dbm1 = manager.And( dbm, cs );
      EXPECT_TRUE( manager.include( dbm, dbm1 ) );
      manager.destroyDBM( dbm1 );
    }
    manager.destroyDBM( dbm );
  }
}

int main( int argc, char *argv[] ) {
  testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
