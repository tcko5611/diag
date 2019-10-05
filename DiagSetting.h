#ifndef DIAGSETTING_H /* -*-c++-*- */
#define DIAGSETTING_H
#include <string>
#include <vector>
#include <map>
#include <boost/logic/tribool.hpp>
#include "rapidjson/document.h"

struct SimStop {
  std::string id_;
  std::string check_;
  std::string measName_;
  std::string min_;
  std::string max_;
  std::string error_;
};

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
  std::map<std::string, std::pair<std::string, std::string> > functional_;
  std::map<std::string, std::pair<std::string, std::string> > detection_;
  std::map<std::string, std::pair<std::string, std::string> > latentDetection_;
  rapidjson::Value getValue(rapidjson::Document::AllocatorType &allocator);
};

struct DiagSetting {
public:
  enum UndetectedFaultsType { None = 0, AssumedSafe, AssumedDangerous};
  enum MeasType { Functional = 0, Detection, LatentDetection};
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
#endif /* DIAGSETTING_H */
