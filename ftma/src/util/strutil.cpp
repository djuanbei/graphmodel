#include <random>

#include "util/strutil.h"

namespace graphsat {

std::string getFunArg(const std::string& fun) {
  std::string::size_type start = fun.find("(");
  if (start == std::string::npos) {
    return "";
  }
  std::string::size_type end = fun.find(")");
  if (end == std::string::npos) {
    return "";
  }
  if (end > start + 1) {
    return trim(fun.substr(start + 1, end - start - 1));
  }
  return "";
}

std::string getFunName(const std::string& fun) {
  std::string::size_type start = fun.find("(");
  if (start == std::string::npos) {
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

  std::string re(len, '\0');
  for (auto& dis : re) dis = distribution(generator);
  return re;
}

std::vector<std::string> splitStr(const std::string& string_to_splitted,
                                  const std::string& delimeter) {
  std::vector<std::string> split_string;
  size_t start_index = 0;
  size_t found = string_to_splitted.find(delimeter, start_index);
  while (found != std::string::npos) {
    std::string val =
        trim(string_to_splitted.substr(start_index, found - start_index));
    if (val != "") {
      split_string.push_back(val);
    }
    start_index = found + delimeter.size();
    found = string_to_splitted.find(delimeter, start_index);
  }
  if (start_index < string_to_splitted.size()) {
    std::string val = trim(string_to_splitted.substr(start_index));
    if (val != "") {
      split_string.push_back(val);
    }
  }
  return split_string;
}
std::string deleteChar(const std::string& value, const size_t start,
                       const char ch) {
  char* buf = new char[value.length() + 1];
  size_t i = 0;
  size_t k = 0;
  for (; i < start; i++) {
    buf[i] = value[i];
  }
  k = i;
  for (; i < value.length(); i++) {
    if (value[i] != ch) {
      buf[k++] = value[i];
    }
  }
  buf[k] = '\0';
  return std::string(buf);
}

}  // namespace graphsat
