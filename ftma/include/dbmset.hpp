/**
 * @file   dbmset.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:58:00 2019
 * 
 * @brief  A dbm matrix set class.
 * 
 * 
 */
#ifndef DBM_SET_H
#define DBM_SET_H
#include<map>
#include<vector>
#include<algorithm>

namespace ftma{
using namespace std;
template<typename C, typename D>
class dbmset{
 private:
  map<uint32_t, C*>  passedD;
  vector<C*>  recoveryD;
  D dbmManager;
  
 public:
  dbmset( D & d ){
    dbmManager=d;
  }

  /** 

   * @param D  A dbm matrix
   * 
   * @return true if real insert D into set
   * false otherwise. 
   */
  bool add( C * DM ){
    uint32_t hashValue=dbmManager.getHashValue( DM );
    typename std::pair<typename std::map<uint32_t,C*>::iterator,bool> ret;
    ret= passedD,insert(std::pair<uint32_t, C*>( hashValue, DM )  );

    if(false== ret.second ){
      // hashValue has in passedD
      C* D1=passedD[ hashValue ];
      if(!dbmManager.MEqual(DM, D1  )){
        bool have=false;
        for( typename vector<C*>::iterator it=recoveryD.begin( ); it!=recoveryD.end(); it++){
          if( dbmManager.MEqual(DM, *it  ) ){
            have=true;
            break;
          }
        }
        if( have ){
          delete [ ] DM;
          return false;
        }
        recoveryD.push_back(DM);
      }else{
        delete[ ] DM;
        return false;
      }
    }
    return true;
  }
  
  void toVector( vector<C*>& re )const{
    re.clear(  );
    for( typename map<uint32_t, C*>::iterator it= passedD.begin(); it!= passedD.end(); it++ ){
      re.push_back( it->second );
    }
    re.insert( re.end( ), recoveryD.begin( ), recoveryD.end( ) );
  }

  void clear( void ){
    passedD.clear(  );
    recoveryD.clear(  );
  }

  void deleteAll ( void ){
    for( typename map<uint32_t, C*>::iterator it= passedD.begin(); it!= passedD.end(); it++ ){
      delete [ ] it->second;
    }
    for( typename vector<C*>::iterator it=recoveryD.begin(  ); it!= recoveryD.end(  ); it++ ){
      delete [ ] *it;
    }
    clear( );
  }
  
  void And(  dbmset<C,D > & other  ){
    for( typename map<uint32_t, C*>::iterator it= other.passedD.begin(); it!= other.passedD.end(); it++ ){
      add(it->second);
    }
    
    for( typename vector<C*>::iterator it=other.recoveryD.begin(  ); it!= other.recoveryD.end(  ); it++ ){
      add(*it);
    }
    other.clear(  );
  }

};
}
#endif
