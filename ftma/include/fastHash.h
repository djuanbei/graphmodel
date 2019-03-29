/**
 * @file   fastHash.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Fri Mar 29 21:08:28 2019
 * 
 * @brief  a fast hash function implement
 * 
 * 
 */
#ifndef Fast_HASH_H
#define Fast_HASH_H
#include<cstdint>


#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

uint32_t FastHash (const char * data, int len);



#endif
