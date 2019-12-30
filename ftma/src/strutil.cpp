#include <random>

#include "util/strutil.h"

namespace graphsat {
string getFunArg(const string &fun) {
  string::size_type start = fun.find("(");
  if (start == string::npos) {
    return "";
  }
  string::size_type end = fun.find(")");
  if (end == string::npos) {
    return "";
  }
  if (end > start + 1) {
    return trim(fun.substr(start + 1, end - start - 1));
  }
  return "";
}

string getFunName(const string &fun) {
  string::size_type start = fun.find("(");
  if (start == string::npos) {
    return trim(fun);
  }
  return trim(fun.substr(0, start));
}

std::string trim(std::string s) {
  if (s.empty()) {
    return s;
  }

  s.erase(0, s.find_first_not_of(" "));
  s.erase(s.find_last_not_of(" ") + 1);
  return s;
}

std::string rand_str(int len) {
  std::mt19937 generator{std::random_device{}()};
  // modify range according to your need "A-Z","a-z" or "0-9" or whatever you
  // need.
  std::uniform_int_distribution<int> distribution{'a', 'z'};

  string re(len, '\0');
  for (auto &dis : re)
    dis = distribution(generator);
  return re;
}

} // namespace graphsat
