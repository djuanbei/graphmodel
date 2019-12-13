#include "gtest/gtest.h"


#include "model/vardecl.h"


using namespace graphsat;


TEST(VAR_DECL_H, VarDecl){

  VarDecl vdel;
  EXPECT_EQ(vdel.getClockNumber( ), 0 );
  vdel.addClock( "x");
  vdel.addClock( "y");
  EXPECT_EQ(vdel.getClockNumber( ), 2 );
  vdel.addInt( "z" );
  EXPECT_EQ(vdel.getClockNumber( ), 2 );

  EXPECT_EQ( vdel.getCounterNumber( ), 1);
  
  vdel.addInt( "za",10, 0,100 );
  
  EXPECT_EQ( vdel.getCounterNumber( ), 11);

  vdel.addChan( "go", 10, ONE2ONE_CH);
  EXPECT_EQ( vdel.getCounterNumber( ), 11);

  EXPECT_EQ( vdel.getKeyStart( INT_T, "za"), 1);
  vdel.addInt( "aa",10, 0,100 );
  EXPECT_EQ( vdel.getKeyStart( INT_T, "aa"), 11);
  EXPECT_EQ( vdel.getInts( ).size( ), 21);
  EXPECT_EQ( vdel.getCounterNumber( ), 21);
  
  
}
