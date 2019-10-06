#ifndef DIAGRESULT_H /* -*-c++-*- */
#define DIAGRESULT_H
#include <string>
#include <vector>
#include <map>
#include <set>
#include <boost/logic/tribool.hpp>
#include "rapidjson/document.h"
#include "DiagCommon.h"

namespace Diag {
  struct FaultResult {
    int gid_;
    std::string failureModeId_;
    std::string tag_;
    std::string category_;
    std::string functionalIds_;
    std::string detectionIds_;
    std::string latentDetectionIds_;
  };

  struct TagResult {
    void clear();
    std::string tag_;
    std::vector<FaultResult> faultResult_;
  };
  
  struct FailureModeResult {
    void clear();
    std::string id_;
    std::set<std::string> functional_;
    std::set<std::string> detection_;
    std::set<std::string> latentDetection_;
    std::map<std::string, TagResult> tagResults_;
  };
  
  struct DiagResult {
  public:
    enum FailureModeCategory { SafeDetected = 0,SafeUndetected,
			       DangerousDetected, DangerousUndetected};
    
    DiagResult();
    void build(const rapidjson::Document &config, const rapidjson::Document &flist);
    void clear();
    
  private:
    void buildSimStops(const std::vector<std::string> &fileNames);
  public:
    std::map<std::string, SimStop> simStops_;
    std::map<std::string, FailureModeResult> failureModeResults_;
    
  };
}
#endif /* DIAGRESULT_H */
