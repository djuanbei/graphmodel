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
namespace graphsat {
using std::string;

string getFunArg(const string &fun);

/**
 * delete start spaces and  last spaces
 *
 */
std::string trim(std::string s);

} // namespace graphsat

#endif
