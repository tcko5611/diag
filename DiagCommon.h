#ifndef DIAGCOMMON_H /* -*-c++-*- */
#define DIAGCOMMON_H
#include <string>

namespace Diag {
  struct SimStop {
    std::string id_;
    std::string check_;
    std::string measName_;
    std::string min_;
    std::string max_;
    std::string error_;
  };
  std::string removeLastIncorrectCharater(const std::string &s);
  SimStop buildSimStop(const std::string& line) ;
}

#endif /* DIAGCOMMON_H */
