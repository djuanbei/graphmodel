/**
 * @file   strutil.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Tue Dec 10 10:33:34 2019
 *
 * @brief  string util functions
 *
 *
 */

#ifndef STR_UTIL_H
#define STR_UTIL_H
#include <string>
#include <vector>
namespace graphmodel {

std::vector<std::string> splitStr(const std::string& stringToBeSplitted,
                                  const std::string& delimeter);

std::string deleteChar(const std::string& value, const size_t start,
                       const char ch);

std::string getFunArg(const std::string& fun);

std::string getFunName(const std::string& fun);

/**
 * delete start spaces and  last spaces
 *
 */
std::string trim(std::string s);

std::string rand_str(int len = 6);

}  // namespace graphmodel

#endif
