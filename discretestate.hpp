/**
 * @file   discretestate.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Wed May 15 08:18:14 2019
 *
 * @brief  state class
 *
 *
 */
#ifndef __DISCRETE_STATE_H
#define __DISCRETE_STATE_H

#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>
#include <vector>

#include "util/fastHash.h"

namespace graphsat {

using std::cin;
using std::cout;
using std::endl;
using std::fill;
using std::map;
using std::pair;
using std::unordered_map;

using std::vector;

class StateElem {
public:
  virtual int  hash_value() const { return 0; }
  virtual bool contain( const StateElem *other ) const { return false; }
  virtual bool equal( const StateElem *other ) const { return false; }
};

class ComposeState : public StateElem {

public:
  int hash_value() const {
    return left->hash_value() * 100 + right->hash_value();
  }
  virtual bool contain( const StateElem *other ) const {
    const ComposeState *rhs = (const ComposeState *) other;
    return left->contain( rhs->left ) && right->contain( rhs->right );
  }
  virtual bool equal( const StateElem *other ) const {
    const ComposeState *rhs = (const ComposeState *) other;
    return left->equal( rhs->left ) && right->equal( rhs->right );
  }

private:
  StateElem *left;
  StateElem *right;
};

template <typename T> class SingleElem {

public:
  SingleElem() {
    stateId = -1;
    value   = NULL;
  }
  ~SingleElem() {
    delete value;
    value = NULL;
  }

  SingleElem( int id, T *v ) {
    stateId = id;
    value   = v;
  }
  const T *getValue() const { return value; }

private:
  int stateId;
  T * value;
};

template <typename T> class StateSet {
  typedef StateSet<T> StateSet_t;

public:
  class const_iterator;
  class iterator;
  StateSet() {
    addELementNum=0;
    stateId     = -1;
    element_len = element_start =second_part_len= 0;
  }
  StateSet( int id, int n, int s ) {
    addELementNum=0;
    stateId       = id;
    element_len   = n;
    element_start = s;
    second_part_len=n-s;
  }
  void setParam( const int n, int s ) {
    addELementNum=0;
    element_len   = n;
    element_start = s;
    second_part_len=n-s;
  }
  ~StateSet() { deleteAll(); }
  void deleteAll() { clear(); }
  void clear() {
    passedElement.clear();

  }

  size_t size() const {
    return addELementNum;
  }

  bool empty() const { return 0==size( );
  }

  int getID( void ) const { return stateId; }

  void setId( int id ) { stateId = id; }

  bool add( T *one ) {
    int id=addHead( one);
    T *oneSecondPart=one+element_start;
    
    /**
     * check whether has element is set has same hash_value
     *
     */

    for( size_t i=0; i< passedElement[ id].size( ); i+=second_part_len  ){
      if(contain(&(passedElement[ id][ i]), oneSecondPart ) ){
        return false;
      }
    }

    addValue( id, oneSecondPart );
    return true;
  }

  bool contain( const T *const one ) const {

    int id = containHead( one );
    if( id>-1){

      const T *oneSecondPart=one+element_start;

      for( size_t i=0; i< passedElement[ id].size( ); i+=second_part_len  ){
        if(contain(&(passedElement[ id][ i]), oneSecondPart ) ){
          return true;
        }
      }
    }

    return false;
  }
  

  iterator begin() { return iterator( this ); }

  const_iterator begin() const { return const_iterator( this ); }

  iterator end() {
    return iterator( this, passedElement.end( ), 0 );
  }

  const_iterator end() const {
    return const_iterator( this,passedElement.end( ), 0);
  }

  class const_iterator {
  protected:
    const StateSet_t *data;
    typename unordered_map<int, vector<T> >::const_iterator it;
    size_t            index;
    vector<T> temp_vec;

  public:
    const_iterator( const StateSet_t *odata )
        : data( odata ) {
      it=odata->passedElement.begin( );
      index = 0;
    }

    const_iterator( const StateSet_t *odata,     unordered_map<int, vector<int> >::const_iterator oit, size_t oindex )
        : data( odata ) {
      it=oit;
      index = oindex;
    }

    const_iterator( const const_iterator &other )
        : data( other.data ) {
      it=other.it;
      index = other.index;
    }

    const_iterator &operator++() {
      index += data->second_part_len;
      if(index== it->second( ).size( )){
        it++;
        index=0;
      }
      return *this;
    }
    bool operator==( const const_iterator &other ) const {
      return (it==other.it) &&(index == other.index);
    }
    
    bool operator!=( const const_iterator &other ) const {
      return (it!=other.it)|| (index != other.index);
    }

    const T *operator*() const {
      
      temp_vec.clear( );
      temp_vec.insert(temp_vec.begin( ), it->first.begin( ), it->first.end( ) );
      temp_vec.insert( temp_vec.end( ), &(it->second[ index]),&(it->second[ index+data->second_part_len])  );
      return &(temp_vec[ 0]);
      
    }
  };

  class iterator {

  protected:
    StateSet_t *data;
    typename unordered_map<int, vector<T> >::iterator it;
    size_t      index;
    vector<T> temp_vec;
  public:
    iterator( StateSet_t *odata )
        : data( odata ) {
      it=odata->passedElement.begin( );
      index = 0;
    }

    iterator( StateSet_t *odata,unordered_map<int, vector<int> >::iterator oit,  size_t oindex )
        : data( odata ) {
      it=oit;
      index = oindex;
    }

    iterator( const iterator &other )
        : data( other.data ) {
      it=other.it;
      index = other.index;
    }
    iterator &operator++() {
      index += data->second_part_len;
      if(index== it->second( ).size( )){
        it++;
        index=0;
      }
      return *this;
    }

    bool operator==( const iterator &other ) const {
      return (it==other.it) &&(index == other.index);
      
    }
    bool operator!=( const iterator &other ) const {
      return (it!=other.it)|| (index != other.index);
    }

    T *operator*() {
      temp_vec.clear( );
      temp_vec.insert(temp_vec.begin( ), it->first.begin( ), it->first.end( ) );
      temp_vec.insert( temp_vec.end( ), &(it->second[ index]),&(it->second[ index+data->second_part_len])  );
      return &(temp_vec[ 0]);
    }
  };

private:
  int                     stateId;
  
  vector< vector<T > > passedElement;
  
  unordered_map<int, pair<vector<T >, vector<int> >  > passedHead;
  
  
  int                     addELementNum;
  int                     element_len;
  
  int                     element_start;
  int       second_part_len;


  int addHead(T * head ){
    
    int hashV = hash_value( head );
    
    std::unordered_map<int, pair<vector<T >, vector<int> >  >::iterator ret = passedHead.find( hashV );
    if( ret!=passedHead){
      for(size_t i=0; i< ret->second.second.size( ); i++  ){
        if(0== memcmp( head,&(ret->second.first[ i*element_start]),element_start*sizeof(T))){
          return ret->second.second[ i];
        }
      }
    }
    
    passedHead[hashV].first.insert( passedHead[hashV].first.end( ),head, head+element_start );
    passedHead[hashV].second.push_back(passedElement.size( ) );
    
    int re=passedElement.size( );
    vector<T> temp;
    passedElement.push_back( temp);
    
    return re;
  }


  int containHead(T * head ) const{
    
    int hashV = hash_value( head );
    
    std::unordered_map<int, pair<vector<T >, vector<int> >  >::iterator ret = passedHead.find( hashV );
    if( ret!=passedHead){
      for(size_t i=0; i< ret->second.second.size( ); i++  ){
        if(0== memcmp( head,&(ret->second.first[ i*element_start]),element_start*sizeof(T))){
          return ret->second.second[ i];
        }
      }
    }
    return -1;
  }

  bool addValue( int id, T *D ) {
    passedElement[id].insert( passedElement[id].end(), D, D + second_part_len );
    addELementNum++;
    return true;
  }


  int hash_value( const T *const one ) const {
    return FastHash( (char *) one, element_start * sizeof( T ) );
  }
  bool equal( const T *const lhs, const T *const rhs ) const {
    return memcmp( lhs, rhs, element_len * sizeof( T ) ) == 0;
  }
  bool contain( const T *const lhs, const T *const rhs ) const {

    for ( int i = 0; i <second_part_len; i++ ) {
      if ( lhs[ i ] < rhs[ i ] ) {
        return false;
      }
    }
    return true;
  }
};

template <typename T> class SingleStateSet {

public:
  SingleStateSet() { stateId = -1; }
  SingleStateSet( int id ) { stateId = id; }
  int getStateId() const { return stateId; }

  virtual bool add( const pair<int, T *> &one ) { return false; }

  virtual bool contain( const pair<int, T *> &one ) const { return false; }

protected:
  int stateId;
};

template <typename T> class ComposeStateSet {

public:
  ComposeStateSet() { num = 0; }
  bool addCompent( SingleStateSet<T> &comp ) {
    int id = comp.getStateId();
    if ( stateIndexMap.find( id ) != stateIndexMap.end() ) {
      return false;
    }
    int re              = stateIndexMap.size();
    stateIndexMap[ id ] = re;
    addCompentImpl( comp );
    num = re + 1;
    return true;
  }

  /*
    pair<int,T> stateID -> State
   */
  virtual bool add( const vector<pair<int, T *>> &one ) { return false; }

  virtual bool contain( const vector<pair<int, T *>> &one ) const {
    return false;
  }

  virtual ComposeStateSet<T> &operator+( const ComposeStateSet<T> &other ) {
    return *this;
  }

protected:
  int           num;
  map<int, int> stateIndexMap; // id -> loc_index

  virtual void addCompentImpl( SingleStateSet<T> &comp ) {}
};

template <typename T> class ExpandComposeStateSet : public ComposeStateSet<T> {

public:
  ExpandComposeStateSet()
      : ComposeStateSet<T>() {
    len = 0;
  }

  ~ExpandComposeStateSet() { clear(); }
  void clear() {
    for ( typename vector<T *>::iterator it = values.begin();
          it != values.end(); it++ ) {
      delete ( *it );
    }
    values.clear();
  }

  bool add( const vector<pair<int, T *>> &one ) {
    int num = ComposeStateSet<T>::num;
    assert( one.size() == num );
    vector<T *> dummy( num );
    for ( int i = 0; i < num; i++ ) {
      int id      = ComposeStateSet<T>::stateIndexMap.at( one[ i ].first );
      dummy[ id ] = one[ i ].second;
    }

    if ( !contain( dummy ) ) {
      for ( int i = 0; i < ComposeStateSet<T>::num; i++ ) {
        values.push_back( dummy[ i ] );
      }
      len++;
      return true;
    }

    return false;
  }

  bool contain( const vector<pair<int, T *>> &one ) const {
    vector<T *> dummy;
    for ( int i = 0; i < ComposeStateSet<T>::num; i++ ) {
      for ( int j = 0; j < ComposeStateSet<T>::num; j++ ) {
        if ( i == one[ j ].first ) {
          dummy.push_back( one[ j ].second );
          break;
        }
      }
    }
    return contain( dummy );
  }

private:
  vector<T *> values;
  int         len;

  ExpandComposeStateSet( const ExpandComposeStateSet<T> &other ) {}
  ExpandComposeStateSet<T> &operator=( const ExpandComposeStateSet<T> &other ) {
    return *this;
  }

  bool contain( const vector<T *> &dummy ) const {
    int num = ComposeStateSet<T>::num;
    for ( size_t i = 0; i < len; i++ ) {
      int j = 0;
      for ( ; j < num; j++ ) {
        if ( !values[ i * num + j ]->contain( dummy[ j ] ) ) {
          break;
        }
      }
      if ( j == num ) {
        return true;
      }
    }
    return false;
  }

protected:
  void addStateIdImpl() { len = 0; }

  //  CompactComposeStateSet<T> toCompactCompStateSet( ) const;
};

template <typename T>
class SingleCompactComposeStateSet : public SingleStateSet<T> {

public:
  SingleCompactComposeStateSet( int id )
      : SingleStateSet<T>( id ) {}
  bool add( const pair<int, T *> &one ) {
    if ( contain( one ) ) {
      return false;
    }
    singleValue = SingleElem<T>( one.first, one.second );
    return true;
  }
  bool contain( const pair<int, T *> &one ) const {

    if ( SingleStateSet<T>::stateId == -1 ) {
      return false;
    }
    assert( SingleStateSet<T>::stateId == one.first );
    if ( singleValue.getValue()->contain( one.second ) ) {
      return true;
    }
    return false;
  }

protected:
  void addStateIdImpl() {}

private:
  SingleElem<T> singleValue;
  bool          isInitial;
};

template <typename T>
class SingleSetCompactComposeStateSet : public SingleStateSet<T> {

public:
  SingleSetCompactComposeStateSet( int id )
      : SingleStateSet<T>( id )
      , setValue( id ) {}
  bool contain( const pair<int, T *> &one ) const {

    assert( one.first == SingleStateSet<T>::stateId );
    return setValue.contain( one.second );
  }

  bool add( const pair<int, T *> &one ) {
    if ( contain( one ) ) {
      return false;
    }

    setValue.add( one.second );

    return true;
  }

private:
  StateSet<T> setValue;
};

/**
 * one complete times two
 *
 * @param int
 * @param one
 *
 * @return
 */
template <typename T>
class CompleteCompactComposeStateSet : public ComposeStateSet<T> {

public:
  /**
   * there is only one compoment which does not contain argument one
   *
   * @param int
   * @param one
   *
   * @return
   */
  int leftOneContain( const vector<pair<int, T *>> &one ) const {

    assert( one.size() == ComposeStateSet<T>::num );
    int re = -1;
    for ( int i = 0; i < ComposeStateSet<T>::num; i++ ) {
      assert( ComposeStateSet<T>::stateIndexMap.find( one[ i ].first ) !=
              ComposeStateSet<T>::stateIndexMap.end() );
      int id = ComposeStateSet<T>::stateIndexMap.at( one[ i ].first );
      if ( !composeValue[ id ].contain( one[ i ] ) ) {
        if ( re != -1 ) {
          return -1;
        }
        re = i;
      }
    }
    return re;
  }

  bool contain( const vector<pair<int, T *>> &one ) const {
    assert( one.size() == ComposeStateSet<T>::num );
    int i = 0;
    for ( ; i < ComposeStateSet<T>::num; i++ ) {
      assert( ComposeStateSet<T>::stateIndexMap.find( one[ i ].first ) !=
              ComposeStateSet<T>::stateIndexMap.end() );
      int id = ComposeStateSet<T>::stateIndexMap.at( one[ i ].first );
      if ( !composeValue[ id ].contain( one[ i ] ) ) {
        break;
      }
    }
    if ( i == ComposeStateSet<T>::num ) {
      return true;
    }
    return false;
  }

  bool add( const vector<pair<int, T *>> &one ) {
    assert( one.size() == ComposeStateSet<T>::num );
    if ( contain( one ) ) {
      return false;
    }
    int id = leftOneContain( one );
    if ( id < 0 ) {
      return false;
    }
    int vid = ComposeStateSet<T>::stateIndexMap[ one[ id ].first ];
    composeValue[ vid ].add( one[ id ] );
    for ( int i = 0; i < ComposeStateSet<T>::num; i++ ) {
      if ( i != id ) {
        delete one[ i ].second;
      }
    }
    return true;
  }

private:
  vector<SingleStateSet<T>> composeValue;

  virtual void addCompentImpl( SingleStateSet<T> &comp ) {
    composeValue.push_back( comp );
  }
};

template <typename T> class CompactComposeStateSet : public ComposeStateSet<T> {

public:
  bool contain( const vector<pair<int, T *>> &one ) const {
    for ( typename vector<CompleteCompactComposeStateSet<T>>::const_iterator
              it = composeValue.begin();
          it != composeValue.end(); it++ ) {
      if ( it->contain( one ) ) {
        return true;
      }
    }
    if ( other.contain( one ) ) {
      return true;
    }
    return false;
  }
  bool add( const vector<pair<int, T *>> &one ) {
    assert( one.size() == ComposeStateSet<T>::num );
    if ( contain( one ) ) {
      return false;
    }
    for ( typename vector<CompleteCompactComposeStateSet<T>>::iterator it =
              composeValue.begin();
          it != composeValue.end(); it++ ) {
      if ( it->add( one ) ) {
        return true;
      }
    }
    return other.add( one );
  }

protected:
  void addCompentImpl( SingleStateSet<T> &comp ) { other.addCompent( comp ); }

private:
  vector<CompleteCompactComposeStateSet<T>> composeValue;
  ExpandComposeStateSet<T>                  other;
};

// class NIntState : public StateElem {

// public:
//   NIntState()
//       : n( 0 )
//       , start( 0 )
//       , value( NULL ) {}
//   NIntState( int s, int ss = 0 ) {
//     assert( s > 0 );
//     n     = s;
//     start = ss;
//     value = new int[ n ];
//     fill( value, value + n, 0 );
//   }

//   ~NIntState() {
//     delete[] value;
//     value = NULL;
//   }

//   NIntState *copy() const {
//     NIntState *re = new NIntState( n, start );
//     memcpy( re->value, value, n * sizeof( int ) );
//     return re;
//   }

//   inline int hash_value() const {
//     return FastHash( (char *) value, n * sizeof( int ) );
//   }
//   void setValue( const int *v ) { memcpy( value, v, sizeof( int ) * n ); }

//   inline bool contain( const StateElem *other ) const {

//     const NIntState *rhs = (const NIntState *) other;

//     if ( 0 != memcmp( value, rhs->value, start * sizeof( int ) ) ) {
//       return false;
//     }

//     for ( int i = start; i < n; i++ ) {
//       if ( value[ i ] < rhs->value[ i ] ) {
//         return false;
//       }
//     }
//     return true;
//   }

//   bool equal( const StateElem *other ) const {

//     const NIntState *rhs = (const NIntState *) other;

//     return ( 0 == memcmp( value, rhs->value, n * sizeof( int ) ) );
//   }

// private:
//   int  n;
//   int  start = 0;
//   int *value;
//   NIntState( const NIntState &other ) { assert( false ); }
//   NIntState &operator=( const NIntState &other ) { return *this; }

//   template <typename C> friend class StateManager;

//   template <typename C, typename L, typename T> friend class TAS;
//   template <typename S> friend class ReachableSet;
// };

} // namespace graphsat
#endif
