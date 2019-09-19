#include <algorithm>
#include <sstream>
#include <iterator>

#include "DiagSetting.h"

using namespace std;
using namespace boost::logic;
using namespace rapidjson;

VcFsmExport::VcFsmExport()
  : isSetted(false), reportMeasuredSafe_(indeterminate),
    reportDcKrf_(indeterminate), reportDcKmpf_(indeterminate)
{
}
Value VcFsmExport::getValue(rapidjson::AllocatorType &allocator)
{
  Value v(kObjectType);
  if (!vcFsmExportFile_.empty()) {
    Value vv;
    vv.SetString(vcFsmExportFile_.c_str(), vcFsmExportFile_.size(), allocator);
    v.AddMember("vc_fsm_export_file", vv, allocator);
  }
  if (reportMeasuredSafe_ != indetermine) {
    if (reportMeasuredSafe_) {
      v.AddMember("report_measured_safe", true, allocator);
    }
    else {
      v.AddMember("report_measured_safe", false, allocator);
    }
  }
  if (reportDcKrf_ != indetermine) {
    if (reportDcKrf_) {
      v.AddMember("report_dc_krf", true, allocator);
    }
    else {
      v.AddMember("report_dc_krf", false, allocator);
    }
  }
  if (reportDcKmpf_ != indetermine) {
    if (reportDcKmpf_) {
      v.AddMember("report_dc_kmpf", true, allocator);
    }
    else {
      v.AddMember("report_dc_kmpf", false, allocator);
    }
  }
  return v;
}

rapidjson::Value FailureMode::getValue(rapidjson::AllocatorType &allocator)
{
  Value v(kObjectType);
  if (!id_.empty()) {
    Value vv;
    vv.SetString(id_.c_str(), id_.size(), allocator);
    v.AddMember("id", vv, allocator);
  }
  if (!idLabel_.empty()) {
    Value vv;
    vv.SetString(idLabel_.c_str(), idLabel_.size(), allocator);
    v.AddMember("id_label", vv, allocator);
  }
  if (!funnctional_.empty()) {
    Value vv(kArrayType);
    for (auto &e : funnctional_) {
      Value vvv(kObjectType);
      Value vvv1, vvv2;
      vvv1.SetString(e.second.first.c_str(), e.second.first.size(), allocator);
      vvv2.SetString(e.second.second.c_str(), e.second.second.size(), allocator);
      vvv.AddMember("id", vvv1, allocator);
      vvv.AddMember("type", vvv2, allocator);
      vv.PushBack(vvv, allocator);
    }
    v.AddMember("functional_output", vv, allocator);
  }
  if (!detection_.empty()) {
    Value vv(kArrayType);
    for (auto &e : detection_) {
      Value vvv(kObjectType);
      Value vvv1, vvv2;
      vvv1.SetString(e.second.first.c_str(), e.second.first.size(), allocator);
      vvv2.SetString(e.second.second.c_str(), e.second.second.size(), allocator);
      vvv.AddMember("id", vvv1, allocator);
      vvv.AddMember("type", vvv2, allocator);
      vv.PushBack(vvv, allocator);
    }
    v.AddMember("detection_output", vv, allocator);
  }
  return v;
}

/*
 * build sim stop from sim_stop command line
 */
static SimStop buildSimStop(const string& line)
{
  SimStop d;
  istringstream iss(line);
  vector<string> tokens;
  copy(istream_iterator<string>(iss),
       istream_iterator<string>(),
       back_inserter(tokens));
  for (int i = 1; i < tokens.size(); ++i) {
    if (tokens[i] == "-check") {
      d.check_ = tokens[++i];
    }
    else if (tokens[i] == "-meas_name") {
      d.measName_ = tokens[++i];
    }
    else if (tokens[i] == "-min") {
      d.min _ = tokens[++i];
    }
    else if (tokens[i] == "-max") {
      d.max_ = tokens[++i];
    }
    else if (tokens[i] == "-id") {
      d.id_ = tokens[++i];
    }
    else if (tokens[i] == "-error") {
      d.error_ = tokens[++i];
    }
  }
  return d;
}

DiagSetting::DiagSetting()
  :enabled(false)
{
}

void DiagSetting::setEnabled(bool b)
{
  enabled = b;
}

void DiagSetting::buildSimStops(const std::vector<std::string> &fNs)
{
  for (auto & fN : fNs) {
    ifstream in(fN);
    string line;
    while(std::getline(in, line)) {
      if (line.find("set_sim_stop") != string::npos) {
        SimStop d = buildSimStop(line);
        simStops_[d.id()] = d; 
      }
    }
  }
}

void DiagSetting::buildDiagSetting(const rapidjson::Document &json)
{
  if (!json.HasMember("diagnostic_coverage")) return;
  const Value &diag = json["diagnostic_coverage"];
  // set undetected_faults
  if (diag.HasMember("undetected_faults")) {
    undetectedFaults_ = diag["undetected_faults"].GetString();
  }
  else {
    undetectedFaults_ = "";
  }
  // set vc_fsm_export
  if (diag.HasMember("vc_fsm_export")) {
    const Value &vc = diag["vc_fsm_export"];
    vcFsmExport_.isSetted = true;
    if (vc.HasMember("report_measured_safe")) {
      vcFsmExport_.reportMeasuredSafe_ = vc["report_measured_safe"].GetBool();
    }
    if (vc.HasMember("report_dc_krf")) {
      vcFsmExport_.reportDcKrf_ = vc["report_dc_krf"].GetBool();
    }
    if (vc.HasMember("report_dc_kmpf")) {
      vcFsmExport_.reportDcKmpf_ = vc["report_dc_kmpf"].GetBool();
    }    
    if (vc.HasMember("vc_fsm_export_file")) {
      vcFsmExport_.vcFsmExportFile_ = vc["vc_fsm_export_file"].GetString();
    }    
  }
  else {
    vcFsmExport_.isSetted = false;
  }
  // set faulure mode
  failureModes_.clear();
  if (diag.HasMember("failure_mode")) {
    const Value &failureModes = diag["failure_mode"];
    for (auto &v : failureModes) {
      FailureMode f;
      f.id_ = v["id"];
      f.idLabel_ = v["id_label"];
      if (v.HasMember("functional_output")) {
        for (auto &vv : v["functional_output"]) {
          string id = vv["id"], type = vv["meas"];
          f.funnctional_[id] = make_pair(id, meas);
        }
      }
      if (v.HasMember("detection_output")) {
        for (auto &vv : v["detection_output"]) {
          string id = vv["id"], type = vv["meas"];
          f.detection_[id] = make_pair(id, meas);
        }
      }
      failureModes_[f.id_] = f;
    }
  }  
}


vector<string> DiagSetting::getSimStopIds()
{
  vector<string> v;
  for (auto &e : simStops_) {
    v.psh_back(e.first);
  }
  return v;
}
void DiagSetting::setUndetectedFaults(const std::string &s)
{
  undetectedFaults_ = s; 
}

void  DiagSetting::setVcFsmExport(bool b)
{
  vcFsmExport_.isSetted_ = b;
}

void DiagSetting::setVcMeasSafe(boost::logic::tribool b)
{
  vcFsmExport_.reportMeasuredSafe_ = b;
}

void DiagSetting::setVcDcKrf(boost::logic::tribool b)
{
  vcFsmExport_.reportDcKrf_ = b;
}

void DiagSetting::setVcDcKmpf(boost::logic::tribool b)
{
  vcFsmExport_.reportDcKmpf_ = b;
}
void DiagSetting::setVcFsmExportFile(const std::string &s)
{
  vcFsmExport_.vcFsmExportFile_ = s;
}

void DiagSetting::addFailureMode(const FailureMode & f)
{
  failureModes_[f.id_] = f;
}

void DiagSetting::removeFailureMode(const string &id)
{
  failureModes_.erase(id);
}

void DiagSetting::addFailFunc(const string &fid, const string &id, const string &type)
{
  map<string, pair<string, string> > &f = failureModes_[fid].functional_;
  f[id] = make_pair(id, type);   
}

void DiagSetting::removeFailFunc(const string &fid, const string &id)
{
  map<string, pair<string, string> > &f = failureModes_[fid].functional_;
  f.erase(id);
}

void DiagSetting::addFailDetec(const string &fid, const string &id, const string &type)
{
  map<string, pair<string, string> > &f = failureModes_[fid].detection_;
  f[id] = make_pair(id, type);   
}

void DiagSetting::removeFailDetec(const string &fid, const string &id)
{
  map<string, pair<string, string> > &f = failureModes_[fid].detection_;
  f.erase(id);
}

Value DiagSetting::getValue(AllocatorType &allocator)
{
  Value o;
  if (!enabled) return o;
  o.SetObject();
  // for undetectedFaults_
  if (!undetectedFaults_.empty()) {
    Value undetectedFaults;
    undetectedFaults.SetString(undetectedFaults_.c_str(),
                               undetectedFaults_.size(),
                               allocator);
    o.AddMember("undetected_faults", undetectedFaults, allocator);
  }
  // for vc_fsm_export
  if (vcFsmExport_.isSetted) {
    Value vc = vcFsmExport_.getValue(allocator);
    o.AddMember("vc_fsm_export", vc, allocator);
  }
    // for failure mode
    Value failures(kArrayType);
    for (auto & a: failureModes_) {
    failures.PushBack(a.second.getValue(allocator), allocator)
  }
  o.AddMember("failure_mode", vc, allocator);
  
  return o;
}
