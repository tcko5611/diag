#ifndef DIAGSETTING_H /* -*-c++-*- */
#define DIAGSETTING_H
#include <string>
#include <vector>
#include <map>
#include <set>
#include <boost/logic/tribool.hpp>
#include "rapidjson/document.h"
#include "DiagCommon.h"

namespace Diag {
  
  struct VcFsmExport {
    VcFsmExport();
    void clear();
    bool enabled_;
    std::string vcFsmExportFile_;
    boost::logic::tribool reportMeasuredSafe_;
    boost::logic::tribool reportDcKrf_;
    boost::logic::tribool reportDcKmpf_;
    rapidjson::Value getValue(rapidjson::Document::AllocatorType &allocator);
  };
  
  struct FailureMode {
    std::string id_;
    std::string idLabel_;
    std::set<std::string> functional_;
    std::set<std::string> detection_;
    std::set<std::string> latentDetection_;
    rapidjson::Value getValue(rapidjson::Document::AllocatorType &allocator);
  };
  
  struct DiagSetting {
  public:
    enum UndetectedFaultsType { None = 0, AssumedSafe, AssumedDangerous};
    enum MeasType { Meas = 0, Vec, Dwc};
    enum OutputType { Functional = 0, Detection, LatentDetection};
    DiagSetting();
    void buildDiagSetting(const rapidjson::Document &json);
    void buildSimStops(const std::vector<std::string> &fileNames);
    rapidjson::Value getValue(rapidjson::Document::AllocatorType &allocator);
    std::vector<std::string> getSimStopIds();
    void clear();
  public:
    bool enabled_;
    std::map<std::string, SimStop> simStops_;
    
    std::string undetectedFaults_;
    VcFsmExport vcFsmExport_;
    std::map<std::string, FailureMode> failureModes_;
    boost::logic::tribool reportTestOrder_;
    boost::logic::tribool faultDatabaseViewer_;
    boost::logic::tribool faultDbViewer_;

  };
}
#endif /* DIAGSETTING_H */
