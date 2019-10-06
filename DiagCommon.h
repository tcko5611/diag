#ifndef DIAGCOMMON_H /* -*-c++-*- */
#define DIAGCOMMON_H
namespace Diag {
  struct SimStop {
    std::string id_;
    std::string check_;
    std::string measName_;
    std::string min_;
    std::string max_;
    std::string error_;
  };
}
#endif /* DIAGCOMMON_H */
