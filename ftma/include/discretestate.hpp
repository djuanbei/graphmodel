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


#include<iostream>
#include<map>
#include<vector>
#include <random>

namespace graphsat {

using namespace std;

class StateElem{
 public:
  virtual int  digitalFeature( ) const{
    return 0;
  }
  virtual bool isContained( const StateElem * other) const{
    return false;
  }
  virtual bool equal( const  StateElem * other) const{
    return false;
  }
};


class ComposeState:public StateElem{
 private:
  StateElem * left;
  StateElem* right;
 public:
  int  digitalFeature( ) const{
    return left->digitalFeature( )*100+right->digitalFeature( );
  }
  virtual bool isContained( const StateElem * other) const{
    const ComposeState* rhs=( const ComposeState*) other;
    return  left->isContained( rhs->left) && right->isContained( rhs->right);

  }
  virtual bool equal( const  StateElem * other) const{
    const ComposeState* rhs=( const ComposeState*) other;
    return  left->equal( rhs->left) && right->equal( rhs->right);
  }
  
};


template<typename T>
class SingleElem{
 private:
  int stateId;
  T* value;
 public:
  SingleElem( ){
    stateId=-1;
    value=NULL;
  }
  ~SingleElem( ){
    delete value;
    value=NULL;
  }
  
  SingleElem(int id, T* v ){
    stateId=id;
    value=v;
  }
  const  T * getValue( ) const{
    return value;
  }
};



template <typename T>
class StateSet{
 private:
  int stateId;
  map<int, T*> mapValues;
  vector<T*>  recoveryValues;
 public:
  StateSet( ){
    stateId=-1;
  }
  StateSet( int id){
    stateId=id;
  }
  ~StateSet( ){
    deleteAll( );
  }
  void deleteAll( ){
    for( typename vector<T*>::const_iterator it= recoveryValues.begin( ); it!= recoveryValues.end( );
         it++){
      delete (*it);
    }
    
    recoveryValues.clear( );
    
    for( typename map<int, T*>::const_iterator it= mapValues.begin( ); it!= mapValues.end( ); it++){
      delete (it->second);
    }
    
    mapValues.clear( );
    
  }
  void clear( ){
    recoveryValues.clear( );
    mapValues.clear( );
  }

  bool empty( ) const{
    return mapValues.empty( ) &&  recoveryValues.empty( );
  }
  int getID( void) const{
    return stateId;
  }
  
  void setId( int id){
    stateId=id;
  }
  
  bool add( T *one ) {
    int f=one->digitalFeature( );
    if( contain( one)){
      return false;
    }
    typename map<int,T*>::iterator it= mapValues.find(f );
    bool state=it==mapValues.end( );
    
    if( state){
      mapValues[ f]=one;
    }else{
      recoveryValues.push_back( one);      
    }
    return true;
  }

  
  bool contain( const T *one) const{
    int f=one->digitalFeature( );
    typename map<int,T*>::const_iterator it= mapValues.find(f );
    bool state=it==mapValues.end( );
    if( !state){
      if(it->second->equal(one) ){
        return true;
      }
    }
    for( typename vector<T*>::const_iterator it= recoveryValues.begin( ); it!= recoveryValues.end( );
         it++){
      if(one->isContained((*it ))){
        return true;
      }
    }
    for( typename map<int, T*>::const_iterator it= mapValues.begin( ); it!= mapValues.end( ); it++){
      if( one->isContained((it->second))){
        return true;
      }
    }
    return false;
  }


  class const_iterator {
   protected:
      const StateSet<T> *data;

    size_t                index;

  public:
    const_iterator( const StateSet<T> *odata )
        : data( odata ) {
      index = 0;
    }

    const_iterator( const StateSet<T> *odata, size_t oindex )
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

    const T *operator*() const {
      size_t dSize = data->mapValues.size();
      if ( index < dSize ) {

        return data->mapValues[ index ];
      }
      if ( index >= dSize + data->recoveryValues.size() ) {
        return NULL;
      }

      return data->recoveryValues[ index - dSize ];
    }
  };



  class iterator {

  protected:
    StateSet<T> *data;
    size_t                index;

  public:
    iterator( StateSet<T> *odata )
        : data( odata ) {
      index = 0;
    }

    iterator(StateSet<T> * odata, size_t oindex )
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

    T *operator*() {
      size_t dSize = data->mapValues.size();
      if ( index < dSize ) {

        return data->mapValues[ index ];
      }
      if ( index >= dSize + data->recoveryValues.size() ) {
        return NULL;
      }

      return data->recoveryValues[ index - dSize ];
    }
  };

  iterator begin() { return iterator( this ); }

  const_iterator begin() const { return const_iterator( this ); }

  iterator end() {

    return iterator( this, mapValues.size() + recoveryValues.size() );
   
  }

  const_iterator end() const {
    return const_iterator( this, mapValues.size() + recoveryValues.size() );
   
  }
};



template<typename T>
class SingleStateSet{
 protected:
  int stateId;
 public:
  SingleStateSet( ){
    stateId=-1;
  }
  SingleStateSet(int id ){
    stateId=id;
  }
  int getStateId( ) const{
    return stateId;
  }

  virtual bool add( const pair<int, T*> &one){
    return false;
  }

  virtual bool contain( const pair<int, T*> & one) const{
    return false;
  }

};
  

template<typename T>
class ComposeStateSet{
 protected:
  int num;
  map<int, int> stateIndexMap; // id -> loc_index
  
  virtual void     addCompentImpl( SingleStateSet<T> &comp ){
  
  }
 public:
  ComposeStateSet( ){
    num=0;
  }
  bool addCompent( SingleStateSet<T> &comp ){
    int id=comp.getStateId( );
    if(stateIndexMap.find(id)!=stateIndexMap.end( ) ){
      return false;
    }
    int re=stateIndexMap.size( );
    stateIndexMap[ id]=re;
    addCompentImpl(comp );
    num=re+1;
    return  true;
  }
  
  /*
    pair<int,T> stateID -> State
   */
  virtual bool add(const vector<pair<int,T*> > & one ){
    return false;
  }
  
  virtual bool contain( const vector<pair<int, T*> > &one) const{
    return false;
  }

  virtual ComposeStateSet<T>&  operator + ( const ComposeStateSet<T> & other  ){
    return *this;
  }

  // virtual ComposeStateSet<T> & operator += (const ComposeStateSet<T> & other  ){
  //   *this;
  // }
  
};



template<typename T>
class ExpandComposeStateSet: public ComposeStateSet<T>{
 private:
  vector<T* > values;
  int len;

  ExpandComposeStateSet(const ExpandComposeStateSet<T> & other ){
    
  }
  ExpandComposeStateSet<T>& operator = (const ExpandComposeStateSet<T> &other )  {
    return *this;
  }
  
  bool contain( const vector< T* > &dummy) const{
    int num=ComposeStateSet<T>::num;
    for(size_t i=0; i< len; i++ ){
      int j=0;
      for( ; j< num; j++ ){
        if( !dummy[ j]->isContained( values[ i*num+j])){
          break;
        }
      }
      if( j==num){
        return true;
      }
    }
    return false;
  }
 protected:
   void addStateIdImpl( ){
     len=0;
   }
 public:
  
  ExpandComposeStateSet( ):ComposeStateSet<T>( ){
    len=0;
  }
  
  ~ExpandComposeStateSet( ){
    clear( );
  }
  void clear( ){
    for( typename vector<T*>::iterator it=values.begin( ); it!= values.end( ); it++){
      delete (*it);
    }
    values.clear( );
  }
  
  bool add(const vector<pair<int, T*> > & one ){
    int num=ComposeStateSet<T>::num;
    assert( one.size( )==num);
    vector<T*> dummy( num);
    for( int i=0; i<num; i++){
      int id=ComposeStateSet<T>::stateIndexMap.at(one[i].first );
      dummy[ id]=one[ i].second;
    }
    
    if( !contain( dummy)){
      for( int i=0; i< ComposeStateSet<T>::num; i++){
        values.push_back( dummy[ i]);
      }
      len++;
      return true;
    }

    return false;
  }

  bool contain( const vector<pair<int, T*> > &one) const{
    vector<T*> dummy;
    for( int i=0; i< ComposeStateSet<T>::num; i++){
      for( int j=0; j< ComposeStateSet<T>::num; j++){
        if(i== one[ j].first){
          dummy.push_back( one[ j].second);
          break;
        }
      }
    }
    return contain( dummy);

  }
  
  
  //  CompactComposeStateSet<T> toCompactCompStateSet( ) const;

};




template<typename T>
class SingleCompactComposeStateSet: public SingleStateSet<T>{
 private:
  SingleElem<T> singleValue;
  bool isInitial;
 protected:
   void addStateIdImpl( ){
     
   }

 public:
  SingleCompactComposeStateSet( int id):SingleStateSet<T>( id){
    
  }
  bool add(const pair<int,T*>  & one ){
    if( contain( one)){
      return false;
    }
    singleValue=SingleElem<T>( one.first, one.second);
    return true;
  }
  bool contain( const pair<int, T*>  &one) const{

    if( SingleStateSet<T>::stateId==-1){
      return false;
    }
    assert(SingleStateSet<T>::stateId== one.first );
    if(one.second->isContained ( singleValue.getValue() ) ){
      return true;
    }
    return false;
  }
};



template<typename T>
class SingleSetCompactComposeStateSet: public SingleStateSet<T> {

 private:
  StateSet<T> setValue;
 public:
  SingleSetCompactComposeStateSet( int id):SingleStateSet<T> ( id), setValue( id){
    
  }
  bool contain( const pair<int, T*>  &one) const{

    assert( one.first==SingleStateSet<T>::stateId);
    return setValue.contain( one.second) ;
    
  }
  
  bool add(const pair<int,T*>  & one ){
    if( contain( one)){
      return false;
    }

    setValue.add( one.second);
    
    return true;
  }

  
};

/** 
 * one complete times two
 * 
 * @param int 
 * @param one 
 * 
 * @return 
 */
template<typename T>
class CompleteCompactComposeStateSet: public ComposeStateSet<T>{

 private:
  vector<SingleStateSet< T> > composeValue;

  virtual void   addCompentImpl( SingleStateSet<T> &comp ){
    composeValue.push_back(comp );
  }
  
 public:

  /** 
   * there is only one compoment which does not contain argument one
   * 
   * @param int 
   * @param one 
   * 
   * @return 
   */
  int leftOneContain(const vector<pair<int,T*> > & one ) const{

    assert(one.size( )==ComposeStateSet<T>::num );
    int re=-1;
    for(int i=0 ; i< ComposeStateSet<T>::num; i++){
      assert(ComposeStateSet<T>::stateIndexMap.find(one[ i].first )!= ComposeStateSet<T>::stateIndexMap.end( ) );
      int id=ComposeStateSet<T>::stateIndexMap.at( one[ i].first);
      if( !composeValue[ id].contain( one[ i])){
        if( re!=-1){
          return -1;
        }
        re= i;
      }
    }
    return re;
  }
  
  bool contain(const vector<pair<int,T*> > & one ) const{
    assert(one.size( )==ComposeStateSet<T>::num );
    int i=0;
    for( ; i< ComposeStateSet<T>::num; i++){
      assert(ComposeStateSet<T>::stateIndexMap.find(one[ i].first )!= ComposeStateSet<T>::stateIndexMap.end( ) );
      int id=ComposeStateSet<T>::stateIndexMap.at( one[ i].first);
      if( !composeValue[ id].contain( one[ i])){
        break;
      }
    }
    if( i==ComposeStateSet<T>::num){
      return true;
    }
    return false;
  }

  bool add(const vector<pair<int,T*> > & one ){
    assert(one.size( )==ComposeStateSet<T>::num );
    if( contain( one)){
      return false;
    }
    int id=leftOneContain( one);
    if( id<0){
      return false;
    }
    int vid=ComposeStateSet<T>::stateIndexMap[ one[ id].first];
    composeValue[ vid].add( one[ id]);
    for( int i=0; i< ComposeStateSet<T>::num; i++){
      if( i!=id){
        delete one[ i].second;
      }
    }
    return true;
  }
};

  
  
template<typename T>
class CompactComposeStateSet: public ComposeStateSet<T>{
 private:
  vector<CompleteCompactComposeStateSet<T> > composeValue;
  ExpandComposeStateSet<T> other;

 protected:
  void     addCompentImpl( SingleStateSet<T> &comp ){
    other.addCompent( comp);
  }
 public:
  bool contain(const vector<pair<int,T*> > & one ) const{
    for( typename vector<CompleteCompactComposeStateSet<T> >::const_iterator it=composeValue.begin( );
         it!=composeValue.end( ); it++ ){
      if( it->contain( one)){
        return true;
      }
    }
    if( other.contain(one )){
      return true;
    }
    return false;
  }
  bool add(const vector<pair<int,T*> > & one ){
    assert(one.size( )==ComposeStateSet<T>::num );
    if( contain( one)){
      return false;
    }
    for( typename vector<CompleteCompactComposeStateSet<T> >::iterator it=composeValue.begin( );  it!=composeValue.end( ); it++ ){
      if( it->add( one)){
        return true;
      }
    }
    return other.add( one);
  }
  

 };

  

class IntState: public StateElem{
 private:
  int value[ 2];
 public:
  IntState( ){
    value[ 0]=value[ 1]=0;
  }
  IntState(int f, int s ){
    value[ 0]=f;
    value[ 1]=s;
  }
  IntState( int* v){
    value[ 0]=v[ 0];
    value[ 1]=v[ 1];
  }

  int  digitalFeature( ) const{
    return value[ 0]+value[ 1];
  }
    
  bool isContained ( const StateElem *other ) const{
    const IntState* rhs=( const IntState*) other;
    if(value[ 0]>rhs->value[ 0]){
      return false;
    }
    return value[ 1]<=rhs->value[ 1];
  }
  bool equal ( const StateElem *other ) const{
    const IntState* rhs=( const IntState*) other;
    return (value[ 1]==rhs->value[ 1]) &&(value[ 0]==rhs->value[ 0]);
  }
  
};


class NIntState: public StateElem{
 private:

  NIntState( const NIntState & other){
    assert( false);
  }
  NIntState& operator =( const NIntState & other){
    return *this;
  }
 public:
  int n;
  int start=0;
  int * value;
  NIntState( ):n( 0),start( 0), value( NULL){
    
  }
  NIntState( int s, int ss=0){
    assert( s>0);
    n=s;
    start=ss;
    value=new int[ n];
    fill( value, value+n, 0);
  }


  ~NIntState( ){
    delete [ ] value;
    value=NULL;
  }

  NIntState* copy( )const{
    NIntState *re=new NIntState( n, start);
    memcpy(re->value, value, n*sizeof( int) );
    return re;
  }
  
  int  digitalFeature( ) const{
    int re=0;
    for( int i=0; i<n; i++){
      re+=value[ i];
    }
    return re;
  }

  bool isContained ( const StateElem *other ) const{

    const NIntState* rhs=( const NIntState*) other;

    if( 0!=memcpy(value, rhs->value, start*sizeof( int))){
      return false;
    }
    for( int i=start; i< n; i++){
      if(value[i]>rhs->value[ i]){
        return false;
      }
    }
    return true;
  }


  
  bool equal ( const StateElem *other ) const{

    const NIntState* rhs=( const NIntState*) other;

    if( 0!=memcpy(value, rhs->value, start*sizeof( int))){
      return false;
    }
    for( int i=start; i< n; i++){
      if( value[ i]!= rhs->value[ i]){
        return false;
      }
    }
    return true;
  }

  
};


class DoubleState: public StateElem{
 private:
  double value[ 3];
 public:
  DoubleState( ){
    value[ 0]=value[1]=value[2]=0.0;
  }
  DoubleState(double v1, double v2, double v3 ){
    value[ 0]=v1;
    value[1]=v2;
    value[2]=v3;
  }
  DoubleState( double* d){
    value[ 0]=d[ 0];
    value[ 1]=d[ 1];
    value[ 2]=d[ 2];
  }
  
  int  digitalFeature( ) const{
    return (int)(value[ 0]+value[ 1]+value[ 2]);
  }
  
  bool isContained( const StateElem * other) const{
    const DoubleState* rhs=( const DoubleState *) other;
    if(value[ 0]>rhs->value[ 0]){
      return false;
    }
    if(value[ 1]>rhs->value[ 1]){
      return false;
    }
    return value[ 2]<=rhs->value[ 2];
  }
  
  bool equal( const StateElem * other) const{
    const DoubleState* rhs=( const DoubleState *) other;
    if((value[ 0]== rhs->value[ 0])&& (value[ 1]== rhs->value[ 1]) &&( value[ 2]== rhs->value[ 2])  ){
      return true;
    }
    return false;
  }
};

// int example( ){
//   int id=0;
//   int addNum=10000;
//   //  ExpandComposeStateSet<StateElem> temp1;
  
//   CompactComposeStateSet<StateElem> temp1;
//   SingleCompactComposeStateSet<StateElem> one( id++);
//   SingleSetCompactComposeStateSet<StateElem> two( id++);
//   temp1.addCompent(one );
//   temp1.addCompent(two );
  
//   int oneID=0;
//   int twoID=1;
//   std::default_random_engine re;
//   std::uniform_real_distribution<int> unit(0,1000000);
//   std::uniform_real_distribution<double> unif(0,1000000);
//   for( int i=0; i< addNum; i++){
//     vector<pair<int, StateElem*> > dummy;

//     IntState *itemp=new IntState( unit( re), unit( re));
//     dummy.push_back( make_pair( oneID, itemp) );
//     DoubleState* dtemp=new DoubleState( unif( re), unif( re), unif( re));
    
//     dummy.push_back( make_pair( twoID, dtemp) );
    
//     if(!temp1.add( dummy)){
//       delete dummy[ 0].second;
//       delete dummy[ 1].second;
//     }
//   }
  
//   return 0;
  
// }

}
#endif
