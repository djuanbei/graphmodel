/**
 * @file dbm.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date Thu Mar 28 19:47:51 2019
 *
 * @brief difference bound D
 * ref paper Timed Automata: Semantics, Algorithms and Tools
 *
 * ftmd: fast timed automata model checker
 *
 */



#ifndef __DBM__HPP
#define __DBM__HPP
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <limits>
#include <vector>
#include <sstream>
#include <map>

#include<random>


#include "fastHash.h"

#include "dbmutil.hpp"

namespace ftma{
using namespace std;
  
/**
 *@param C the type of value of clock
 *@param add the operator of x+y
 *
 */
  
template < typename C, typename Cons >
class dbm{
 private:
  /**
   * number of clocks
   */
  int n;
  int size; // n*n
  C MAX_INT;
  C LTEQ_ZERO;
  std::default_random_engine generator;
  std::uniform_int_distribution<int>      distribution;
  
  inline int loc(const int row, const int col) const{
    return row* n +col;
  }

  void andImpl(C * newD, const Cons & cons) const{
    
    Cons  negCons=cons.neg( );
    
    if (negCons.matrix_value>=newD[loc( cons.y, cons.x)]){
      newD[0]= getMatrixValue(-1, false);
    }else if ( cons.matrix_value < newD[loc(cons.x, cons.y)]){
      newD[loc(cons.x, cons.y)]=cons.matrix_value;
      for(int i=0; i< n; i++){
        for(int j=0; j< n ; j++){
          C temp=add(newD[loc(i, cons.x)], newD[loc(cons.x, j)]);
            
          if (temp < newD[loc(i, j)]){
            newD[loc(i, j)] = temp;
          }
            
          C DandC=add(newD[loc(i, cons.y)], newD[loc(cons.y, j)]);
          if(DandC < newD[loc(i,j)]){
            newD[loc(i,j)]=DandC;
          }
        }
      }
    }
      
  }
 public:
    
  dbm(int nn):n(nn+1){
    size=n*n;
    MAX_INT=dbmUTIL<C>::MAX_INT;
    LTEQ_ZERO=dbmUTIL<C>::LTEQ_ZERO;
    
    distribution=std::uniform_int_distribution<int>(-MAX_INT+1, MAX_INT);
  }
  ~dbm(){
    n=0;
  }
    
  C* newMatrix() const{
    C* D=new C[size]();
    fill(D, D+size, LTEQ_ZERO); // x-x<=0
    return D;
  }
  /**
   * Create a new matrix and initial the values with D
   *
   * @param D
   *
   * @return
   */
  C * newMatrix(const C * D) const{
    C* newD=new C[size]();
    memcpy(newD, D, sizeof(C)*size);
    return newD;
  }

  C* randomMatirx()   {
    
    C* newD=new C[size]();
    for(int i=0;i< size; i++){
      newD[i]=distribution(generator);
    }
    return newD;
  }
  //TODO
  C* randomFeasiableMatrix(){
    C* D=newMatrix();
    C* D1=up(D);
    delete[  ] D;

    C* D2=free(D1, 0 );
    return D2;
        
  }
  
  std::string dump(const C* D) const{

    std::stringstream ss;

    for(int i=0; i< n; i++){
      ss<<"[ ";
      for(int j=0; j< n; j++){
        ss<<" (";
        C v=D[loc(i,j)];
        

        if(isStrict<C>(v)){
          ss<<"< ,";          
        }else{
          ss<<"<=,";          
        }
        ss.width(4);
        ss<<std::left<<(int)getRight<C>(v);
        ss<<") ";
      }
      ss<<"]\n";
    }
    return ss.str();
  }
    
  uint32_t getHashValue(const C* D) const{
    return FastHash((char*)D, sizeof(C)*size);
  }
    
  bool MEqual(const C* lhs, const C* rhs) const{
    return 0==memcmp(lhs, rhs, size*sizeof(C));
  }
    
  /**
   * recovery memory
   *
   * @param Cvec
   */
  void deleteVectorM(vector<C*> & Cvec)const{
    for(typename vector<C*>::iterator it=Cvec.begin(); it!= Cvec.end(); it++){
      delete[] (*it);
    }
    Cvec.clear();
  }
    
    
  /**
   * Floyds algorithm for computing all shortest path
   * TODO improve efficient
   */
  void canonicalForm(C * D) const{
    for (int k=0; k< n; k++){
      for(int i=0; i< n; i++){
        for (int j=0; j< n; j++){
          C temp=add(D[loc(i,k)], D[loc(k,j)]);
          D[loc(i,j)]= D[loc(i,j)]< temp? D[loc(i,j)]: temp;
        }
      }
    }
  }
    
  /**
   *
   * precondition D is canonicalForm
   * @param D
   *
   * @return true if dbm D is not empty,
   * false otherwise.
   */
  bool isConsistent(const C* D) const{
    return D[0] >= LTEQ_ZERO;
  }
    
    
  /**
   *
   *
   * @param C
   * @param other
   *
   * @return true if this is included by other lhs <= rhs
   */
  bool isInclude (const C * lhs, const C *rhs) const{
      
    for(int i=0; i< size; i++){
      if(lhs[i]> rhs[i]){
        return false;
      }
    }
    return true;
  }
    
  /**
   * D_{y,x} + cons.matrix_value ">=" 0
   } x-y < ( <= ) rhs and
   * y-x < ( <= ) d is feasiable if and only if -rhs< d
   
   * This is to check D and x-y < (<=) m is non-empty
   * @param cons
   *
   * @return true if there is a value in this domain which satisfies cons
   * false, otherwise.
   */
  bool isSatisfied(const C* D, const Cons &cons) const{
    Cons negCons=cons.neg( );
    return negCons.matrix_value<D[loc(cons.y, cons.x)];
  }
    
  /**
   * Can not modify value of D
   * compute strongest post condition
   * up(D)={u+d | u \in D, d\in R+ }
   *
   */
  C* up(const C* D) const{
    C* newD=newMatrix(D);
    for(int i; i< n; i++) {
      newD[loc(i, 0)]=MAX_INT;
    }
    return newD;
  }
    
    
  /**
   * Can not modify value of D
   * compute weakest precondition
   *
   */
  C* down(const C * D) const{
    C *newD=newMatrix(D);
    for (int i=1; i< n; i++){
      newD[i] =LTEQ_ZERO;
      for(int j=1; j< n; j++){
        int k=loc(j,i);
        if(newD[i]>newD[k]){
          newD[i]=newD[k];
        }
      }
    }
    return newD;
  }
    

  /**
   * Can not modify value of D
   * The most used operation in state-space exploration in conjunction
   * @param cons
   */
  C * And(const C *D, const Cons & cons) const{
      
    C *newD=newMatrix(D);
    andImpl(newD, cons);
      
    return newD;
  }
    
    
    
  /**
   * Can not modify value of D
   * The free operation removes all constraints on a given clock x.
   *
   * @param x
   */
  C* free(const C * D, const int x) const{
    C *newD=newMatrix(D);
    for(int i=0; i<n; i++){
      newD[loc(x, i)]=MAX_INT;
      newD[loc(i, x)]=newD[loc(i,0)];
    }
    newD[loc(x, x)]=LTEQ_ZERO;
    return newD;
  }
    
    
    
  /**
   * In forwards exploration this operation is used to set clocks to specific values x:=m
   * Can not modify value of D
   * @param x
   * @param m
   */
  C* reset (const C* D, const int x,const C m) const {
    C *newD=newMatrix(D);
    C postM=getMatrixValue(m, false);
    C negM=getMatrixValue(-m, false);
      
    for(int i=0; i< n ;i++){
      newD[loc(x, i)] =add(postM, newD[i]);
        
      newD[loc(i,x)]=add(newD[loc(i, 0)], negM);
        
    }
    return newD;
  }
    
    
    
  /**
   * This is another operation used in forwards state space exporation.
   * Can not modify value of D
   * @param x
   * @param y
   */
  C* copy(const C * D, const int x, const int y) const{
    C *newD=newMatrix(D);
    for(int i=0; i< n; i++){
      newD[loc(x,i)]=newD[loc(y,i)];
      newD[loc(i,x)]=newD[loc(i,y)];
    }
      
    newD[loc(x,x)]=newD[loc(x,y)]=newD[loc(y,x)]=LTEQ_ZERO;
    return newD;
  }
    
    
    
  /**
   * The last reset operation is shifting a clock, i.e. adding or substracting a clock with an integer value.
   *
   * @param x
   * @param m
   */
  C* shift(const C * D, const int x, const C m) const{
    C *newD=newMatrix(D);
    C postM=getMatrixValue(m, false);
    C negM= getMatrixValue(-m, false);
      
      
    for (int i=0; i< n; i++){
      newD[loc(x,i)]=add(newD[loc(x,i), postM]);
      newD[loc(i, x)]=add(newD[loc(i, x)], negM);
    }
      
    newD[loc(x,x)]=LTEQ_ZERO;
    int temp= loc(x,0);
    newD[temp]=newD[temp]> LTEQ_ZERO? newD[temp]: LTEQ_ZERO;
      
    newD[x]=newD[x]< LTEQ_ZERO? newD[x] : LTEQ_ZERO;
      
    return newD;
      
  }
    
    
  /**
   * For a timed automaton and safty prperty to be checked, that contain no difference constraints.
   * assert(k.size()==2*n)
   * k[ i ]:= <= k_i
   * k[i+n]:= < -k_i
   * @param k k[i] is the maximum upper for x_i
   */
  void norm(C * D, const C* k) const {
    
    for (int i=0; i< n; i++){
      for(int j=0; j< n; j++){
        if(D[loc(i,j)]< MAX_INT){
          if(D[loc(i,j)]> k[i]){
            D[loc(i,j)]=MAX_INT;
          }
          else if(D[loc(i, j)]< k[j+n]){
            D[loc(i, j)]=k[j+n];
          }
        }
      }
    }
    canonicalForm(D);
  }
    
  C* corn_norm(C* D, const C* k, vector<Cons> & Gd) const{
      
    vector<Cons> Gunsat;
      
    for(size_t i=0; i< Gd.size(); i++){
      /**
       * If D and C does not satisiable then norm(D,k) and C does not satisable
       * 
       */

      if(!isSatisfied(D, Gd[i])){
        Gunsat.push_back(Gd[i]);
      }
      /**
       * If D and neg(C) does not satisiable then norm(D,k) and C does not satisable
       * 
       */

      if(!isSatisfied(D, Gd[i].neg())) {
        Gunsat.push_back(Gd[i].neg());;
      }
    }
      
    norm(D, k);
    for(typename vector<Cons>::iterator it= Gunsat.begin(); it!= Gunsat.end(); it++){
      andImpl(D, it->neg());
    }
    return D;
  }
    
  void split(const C* D, const vector<Cons> &Gd, vector<C*> &re){
    deleteVectorM(re);
    map<uint32_t, C*>passed;
      
    vector<C*> waitS;
    re.push_back(newMatrix(D));
      
    for(typename vector<Cons>::const_iterator cit=Gd.begin(); cit!=Gd.end(); cit++){
        
      vector<bool> addToWaitS(re.size(), false);
      int i=0;
      for (typename vector<C*>::iterator dit =re.begin(); dit!= re.end(); dit++){
        i++;
        /**
         * split
         * (D and C) && (D and -C) satisfies then using C to split D into two parts
         */
          
        if(isSatisfied(*dit, *cit) && isSatisfied(*dit, (*cit).neg())){
            
          C* DandC=Add(*dit, *cit) ;
          C* DandNegC=Add(*dit, (*cit).neg());
            
          uint32_t DandCHash=getHashValue(DandC);
          uint32_t DandNegCHash=getHashValue(DandNegC);
            
          typename map<uint32_t, C*>::iterator DandCfid;
          DandCfid=passed.find(DandCHash);
          typename map<uint32_t, C*>::iterator DandNegCfid;
          DandNegCfid =passed.find(DandNegCHash);
            
          if(DandCfid==passed.end()){
            passed[DandCHash]=DandC;
            waitS.push_back(DandC);
              
          }else{
            if(MEqual(DandC, DandCfid->second)){
              deleteVectorM(DandC);
            }else{
              waitS.push_back(DandC);
            }
          }
            
          if(DandNegCfid==passed.end()){
            passed[DandNegCHash]=DandNegC;
            waitS.push_back(DandNegC);
          }else{
            if(MEqual(DandNegC, DandNegCfid->second)){
              deleteVectorM(DandNegC);
            }else{
              waitS.push_back(DandNegC);
            }
          }
        }else{
          uint32_t Dhash=getHashValue(*dit);
          typename map<uint32_t, C*>::iterator DhashFid= passed.find(Dhash);
            
          if(DhashFid== passed.end()){
            passed[Dhash]= *dit;
            waitS.push_back(*dit);
            addToWaitS[i]=true;
          }else{
            if(!MEqual(*dit, DhashFid->second)){
              waitS.push_back(*dit);
              addToWaitS[i]=true;
            }
          }
        }
      }
      re.swap(waitS);
      for(size_t i=0; i< waitS.size(); i++){
        if(!addToWaitS[i]){
          delete[] waitS[i];
        }
      }
      waitS.clear();
    }
  }
    
    
  /**
   * For automaton containing difference constraints in the guards, it is more
   * complicated and expensive to compute the normalized zones.
   * assert(k.size()==2*n)
   * k[ i ]:= <= k_i
   * k[i+n]:= < -k_i
   * @param k k[i] is the maximum upper for x_i
   * @param D
   * @param G
   * @param re
   */
  void norm(C * D, const C* k, const vector<Cons> &Gd, vector<C*> &re)const{
    deleteVectorM(re);
      
    vector<C*> splitDomain;
    split(D, Gd, splitDomain);
      
    for(typename vector<C*> ::iterator it=splitDomain.begin(); it!=splitDomain.end(); it++){
      re.push_back(corn_norm(*it, k, Gd));
    }
  }
    
};
  
  
}



#endif 
