/**
 * @file   DBMset.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:58:00 2019
 *
 * @brief  A dbm matrix set class.
 *
 *
 */
#ifndef DBM_SET_H
#define DBM_SET_H
#include <algorithm>
#include <map>
#include <vector>

#include "util/dbmutil.hpp"

namespace graphsat {
using namespace std;
template <typename C, typename DBM> class DBMset {
private:
  map<uint32_t, int> passedD;
  vector<C *>        mapD;
  vector<DF_T>       mapDFeature;

  vector<C *>  recoveryD;
  vector<DF_T> recoveryDFeature;

  C *getD( uint32_t hashValue ) { return mapD[ passedD[ hashValue ] ]; }

  void mapDAdd( C *D, DF_T &value ) {
    mapD.push_back( D );
    mapDFeature.push_back( value );
  }

  void recoveryDAdd( C *D, DF_T &value ) {
    recoveryD.push_back( D );
    recoveryDFeature.push_back( value );
  }

public:
  class const_iterator {
  protected:
    const DBMset<C, DBM> *data;
    size_t                index;

  public:
    const_iterator( const DBMset<C, DBM> *odata )
        : data( odata ) {
      index = 0;
    }

    const_iterator( const DBMset<C, DBM> *odata, size_t oindex )
        : data( odata ) {
      index = oindex;
    }

    const_iterator( const const_iterator &other )
        : data( other.data ) {
      index = other.index;
    }

    const_iterator &operator++() {
      index++;
      return *this;
    }
    bool operator==( const const_iterator &other ) const {
      return index == other.index;
    }
    bool operator!=( const const_iterator &other ) const {
      return index != other.index;
    }

    const C *operator*() const {
      size_t dSize = data->mapD.size();
      if ( index < dSize ) {

        return data->mapD[ index ];
      }
      if ( index >= dSize + data->recoveryD.size() ) {
        return NULL;
      }

      return data->recoveryD[ index - dSize ];
    }
  };

  class iterator {

  protected:
    const DBMset<C, DBM> *data;
    size_t                index;

  public:
    iterator( DBMset<C, DBM> *odata )
        : data( odata ) {
      index = 0;
    }

    iterator( DBMset<C, DBM> *odata, size_t oindex )
        : data( odata ) {
      index = oindex;
    }

    iterator( const iterator &other )
        : data( other.data ) {
      index = other.index;
    }
    iterator &operator++() {
      index++;
      return *this;
    }

    bool operator==( const iterator &other ) const {
      return index == other.index;
    }
    bool operator!=( const iterator &other ) const {
      return index != other.index;
    }

    C *operator*() {
      size_t dSize = data->mapD.size();
      if ( index < dSize ) {

        return data->mapD[ index ];
      }
      if ( index >= dSize + data->recoveryD.size() ) {
        return NULL;
      }

      return data->recoveryD[ index - dSize ];
    }
  };

  iterator begin() { return iterator( this ); }

  const_iterator begin() const { return const_iterator( this ); }

  iterator end() {

    return iterator( this, mapD.size() + recoveryD.size() );
    /**/
  }

  const_iterator end() const {
    return const_iterator( this, mapD.size() + recoveryD.size() );
    /**/
  }

  /**

   * @param DBM  A dbm matrix
   *
   * @return true if real insert DBM into set
   * false otherwise.
   */
  bool add( const DBM &dbmManager, C *DM ) {
    uint32_t hashValue = dbmManager.getHashValue( DM );
    typename std::pair<typename std::map<uint32_t, int>::iterator, bool> ret;
    ret = passedD.insert( std::pair<uint32_t, int>( hashValue, mapD.size() ) );
    DF_T featrue = dbmManager.getIncludeFeature( DM );
    if ( false == ret.second ) {
      // hashValue has in passedD
      C *D1 = getD( hashValue );

      if ( !dbmManager.MEqual( DM, D1 ) ) {
        bool have = false;
        for ( typename vector<C *>::iterator it = recoveryD.begin();
              it != recoveryD.end(); it++ ) {
          if ( dbmManager.MEqual( DM, *it ) ) {
            have = true;
            break;
          }
        }
        if ( have ) {
          delete[] DM;
          return false;
        }
        if ( isInclude( dbmManager, DM, featrue ) ) {
          delete[] DM;
          return false;
        } else {
          recoveryDAdd( DM, featrue );
        }

      } else {
        delete[] DM;
        return false;
      }
    } else {
      mapDAdd( DM, featrue );
    }

    return true;
  }

  bool isInclude( const DBM &dbmManager, C *DM, DF_T &featrue ) const {

    for ( size_t i = 0; i < mapD.size(); i++ ) {
      if ( ( mapDFeature[ i ] >= featrue ) &&
           dbmManager.isInclude( mapD[ i ], DM ) ) {
        return true;
      }
    }

    for ( size_t i = 0; i < recoveryD.size(); i++ ) {
      if ( ( recoveryDFeature[ i ] >= featrue ) &&
           dbmManager.isInclude( recoveryD[ i ], DM ) ) {
        return true;
      }
    }
    return false;
  }

  size_t size() const { return passedD.size() + recoveryD.size(); }

  void clear( void ) {
    passedD.clear();

    mapD.clear();
    mapDFeature.clear();

    recoveryD.clear();
    recoveryDFeature.clear();
  }

  void deleteAll( void ) {

    for ( typename vector<C *>::const_iterator it = mapD.begin();
          it != mapD.end(); it++ ) {
      delete[] * it;
    }

    for ( typename vector<C *>::iterator it = recoveryD.begin();
          it != recoveryD.end(); it++ ) {
      delete[] * it;
    }
    clear();
  }

  void And( const DBM &dbmManager, DBMset<C, DBM> &other ) {

    for ( typename vector<C *>::iterator it = other.mapD.begin();
          it != other.mapD.end(); it++ ) {
      add( dbmManager, *it );
    }

    for ( typename vector<C *>::iterator it = other.recoveryD.begin();
          it != other.recoveryD.end(); it++ ) {
      add( dbmManager, *it );
    }
    other.clear();
  }
};
} // namespace graphsat
#endif
