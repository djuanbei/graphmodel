/**
 * @file   property.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon May 20 20:53:42 2019
 *
 * @brief   LTL ( Linear temper logical)
 *
 *
 */
#ifndef _PROPERTY_H
#define _PROPERTY_H
#include <vector>

namespace graphsat {
using std::vector;

class Property {
public:
  virtual bool operator()( const StateManager &manager,
                           const int *const         state ) const {
    return false;
  }
};

class LocReachProperty : public Property {
public:
  LocReachProperty( vector<int> &ls ) { loc = ls; }
  bool operator()( const StateManager &manager,
                   const int *const         state ) const {

    return ( 0 == memcmp( state, &loc[ 0 ],
                          manager.getComponentNum() * sizeof( int ) ) );
  }

private:
  vector<int> loc;
};

class FischerMutual : public Property {

public:
  bool operator()( const StateManager &manager,
                   const int *const         state ) const {
    int component_num = manager.getComponentNum();
    int cs_num        = 0;
    for ( int i = 0; i < component_num; i++ ) {
      if ( state[ i ] == 3 ) {
        cs_num++;
      }
    }
    return cs_num >= 2;
  }
};

} // namespace graphsat

#endif
