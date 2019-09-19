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
  bool isSetted_;
  std::string vcFsmExportFile_;
  boost::logic::tribool reportMeasuredSafe_;
  boost::logic::tribool reportDcKrf_;
  boost::logic::tribool reportDcKmpf_;
  rapidjson::Value getValue(rapidjson::AllocatorType &allocator);
};

struct FailureMode {
  std::string id_;
  std::string idLabel_;
  std::map<std::string, std::pair<std::string, std::string> > funnctional_;
  std::map<std::string, std::pair<std::string, std::string> > detection_;
  rapidjson::Value getValue(rapidjson::AllocatorType &allocator);
};

struct DiagSetting {
public:
  DiagSetting();
  void buildSimStops(const std::vector<std::string> &fileNames);
  void buildDiagSetting(const rapidjson::Document &json);
  std::vector<std::string> getSimStopIds();
  void setEnabled(bool b);
  void setUndetectedFaults(const std::string &s);
  void setVcFsmExport(bool b);
  void setVcMeasSafe(boost::logic::tribool b);
  void setVcDcKrf(boost::logic::tribool b);
  void setVcDcKmpf(boost::logic::tribool b);
  void setVcFsmExportFile(const std::string &s);
  void addFailureMode(const FailureMode & f);
  void removeFailureMode(const std::string &id);
  void addFailFunc(const string &fid, const string &id, const string &type);
  void removeFailFunc(const string &fid, const string &id);
  void addFailDetec(const string &fid, const string &id, const string &type);
  void removeFailDetec(const string &fid, const string &id);

  rapidjson::Value getValue(rapidjson::AllocatorType &allocator);
private:
  bool enabled;
  std::map<std::string, std::SimStop> simStops_;
  VcFsmExport vcFsmExport_;
  std::string undetectedFaults_;
  std::map<std::string, FailureMode> failureModes_;
};
#endif /* DIAGSETTING_H */
