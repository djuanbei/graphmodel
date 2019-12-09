/**
 * @file   macrodef.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Thu Dec  5 10:07:31 2019
 *
 * @brief  def some macro
 *
 *
 */
#ifndef MACRO_DEF_H
#define MACRO_DEF_H

#include <dlfcn.h>
#include <iomanip>
#include <iostream>

using namespace std;
#define PRINT_STATE 1

#define STRING( s ) #s

#ifdef PRINT_STATE

#define PRINT_STATE_MACRO                                                      \
  for ( int i = 0; i < component_num; i++ ) {                                  \
    if ( state[ i + component_num ] == NO_CHANNEL )                            \
      std::cout << setw( LOC_OUT_WIDTH )                                       \
                << sys.agents[ i ].getLocName( state[ i ] );                   \
    else {                                                                     \
      int block_source;                                                        \
      sys.agents[ i ].ta_tempate->graph.findSrc( state[ i ], block_source );   \
      cout << setw( LOC_OUT_WIDTH )                                            \
           << sys.agents[ i ].getLocName( block_source );                      \
    }                                                                          \
  }                                                                            \
  cout << endl;                                                                \
  manager->getClockManager().dump( cout, manager->getDBM( state ) ) << endl;

#else

#define PRINT_STATE_MACRO

#endif

#define LTEQ_ZERO ( 1 )

#define newA( __E, __n ) (__E *) malloc( ( __n ) * sizeof( __E ) )

#define TYPE_TYPE( T )                                                         \
  T, REF_##T, CONST_##T, CONST_REF_##T, PARAMETER_##T, REF_PARAMETER_##T,      \
      ARRAY_##T

#define TYPE_STR( T )                                                          \
  case T:                                                                      \
    return STRING( T );                                                        \
  case REF_##T:                                                                \
    return STRING( REF_##T );                                                  \
  case CONST_##T:                                                              \
    return STRING( CONST_##T );                                                \
  case CONST_REF_##T:                                                          \
    return STRING( CONST_REF_##T );                                            \
  case ARRAY_##T:                                                              \
    return STRING( ARRAY_##T );                                                \
  case PARAMETER_##T:                                                          \
    return STRING( PARAMETER_##T );                                            \
  case REF_PARAMETER_##T:                                                      \
    return STRING( REF_PARAMETER_##T );

const static int TYPE_FAMILY_LEN = 7;

#define ENUM_ITEM_STR( T )                                                     \
  case T:                                                                      \
    return STRING( T );

#define ADD_CLOCK( T, x ) int x = T.addClock( STRING( x ) );

#define ADD_INT( T, x ) int x = T.addInt( STRING( x ) );

#define SINGLETON( T )                                                         \
private:                                                                       \
  T() {}                                                                       \
  T( const T &other ) { assert( false ); }                                     \
  T &operator=( const T &other ) {                                             \
    assert( false );                                                           \
    return *this;                                                              \
  }                                                                            \
                                                                               \
public:                                                                        \
  static T &getInstance() {                                                    \
    static T instance;                                                         \
    return instance;                                                           \
  }

#define deleteType( Var, TYPE )                                                \
  {                                                                            \
    vector<void *> temp = Var.getValue( CLASS_TYPE, #TYPE );                   \
    for ( auto ee : temp ) {                                                   \
      delete (TYPE *) ee;                                                      \
    }                                                                          \
    Var.clear( CLASS_TYPE, #TYPE );                                            \
  }

#define EXPORT __attribute__( ( visibility( "default" ) ) )

#define PASSFAIL "Passed" : "Failed"
#define UNTST "Untested"

#endif
