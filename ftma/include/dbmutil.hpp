/**
 * @file   dbmutil.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Mar 29 10:54:12 2019
 *
 * @brief   dbm util class
 *
 *
 */

#ifndef DBM_UTIL_HPP
#define DBM_UTIL_HPP
#include<limits>

namespace ftmd{
    
    /**
     *  Both have compare < and <=
     *
     */
    
    template<typename V>
    struct dbmUTIL{
        const V  LTEQ_ZERO=1;
        const V  MAX_INT=std::numeric_limits<V>::max( )/2-2;
        V operator (  ) (  const V x, const V y ) const {
            if(  MAX_INT== x || MAX_INT==y){
                return MAX_INT;
            }
            return x+y-( ( x&1 )|( y&1 ));
        }
        V getRight( V realRight, bool isStrct=true ) const{
            V right=realRight<<1;
            if( !isStrct ){
                right=right |1;
            }
            return right;
        }
        
        bool isSat( V x, V y ) const{
            return x+y> LTEQ_ZERO;
        }
        std::string getComp( V x ) const{
            if( x&1 ){
                return "<=";
            }else{
                return "< ";
            }
            
        }
    };
    
    
    /**
     *  Only have compare <
     *
     */
    
    template<typename V>
    struct dbmWeakStrickUTIL{
        
        const V  LTEQ_ZERO=0;
        const V  MAX_INT=std::numeric_limits<V>::max( )/2;
        V operator (  ) (  const V x, const V y ) const {
            if(  (x>=MAX_INT) || (y>= MAX_INT)){
                return MAX_INT;
            }
            return  x+y;
            
        }
        
        V getRight( V realRight, bool isStrct=true ){
            return realRight;
        }
        
        bool isSat( V x, V y ){
            return x+y> LTEQ_ZERO;
        }
        std::string getComp( V x ) const{
            return "< ";
        }
    };
    
    
    /**
     *  Only have compare <=
     *
     */
    
    template<typename V>
    struct dbmWeakUTIL{
        
        const V  LTEQ_ZERO=0;
        const V  MAX_INT=std::numeric_limits<V>::max( )/2;
        V operator (  ) (  const V x, const V y ) const {
            if(  (x>=MAX_INT) || (y>= MAX_INT)){
                return MAX_INT;
            }
            return  x+y;
            
        }
        
        V getRight( V realRight, bool isStrct=true ){
            return realRight;
        }
        
        bool isSat( V x, V y ){
            return x+y>= LTEQ_ZERO;
        }
        
        std::string getComp( V x ) const{
            return "<=";
        }
    };
    
}

#endif
