#include <algorithm>
#include <sstream>
#include <iterator>
#include <fstream>

#include "DiagSetting.h"

using namespace std;
using namespace boost::logic;
using namespace rapidjson;

VcFsmExport::VcFsmExport()
  : enabled_(false), reportMeasuredSafe_(indeterminate),
    reportDcKrf_(indeterminate), reportDcKmpf_(indeterminate)
{
}

void VcFsmExport::clear()
{
  enabled_ = false;
  vcFsmExportFile_ = "";
  reportMeasuredSafe_ = indeterminate;
  reportDcKrf_ = indeterminate;
  reportDcKmpf_ = indeterminate;
}

Value VcFsmExport::getValue(Document::AllocatorType &allocator)
{
  Value v(kObjectType);
  if (!vcFsmExportFile_.empty()) {
    Value vv;
    vv.SetString(vcFsmExportFile_.c_str(), vcFsmExportFile_.size(), allocator);
    v.AddMember("vc_fsm_export_file", vv, allocator);
  }
  if (reportMeasuredSafe_) {
    v.AddMember("report_measured_safe", true, allocator);
  }
  else if (!reportMeasuredSafe_) {
      v.AddMember("report_measured_safe", false, allocator);
  }
  if (reportDcKrf_) {
    v.AddMember("report_dc_krf", true, allocator);
  }
  else if (!reportDcKrf_){
    v.AddMember("report_dc_krf", false, allocator);
  }
  if (reportDcKmpf_) {
    v.AddMember("report_dc_kmpf", true, allocator);
  }
  else if (!reportDcKmpf_) {
    v.AddMember("report_dc_kmpf", false, allocator);
  }
  return v;
}

Value FailureMode::getValue(Document::AllocatorType &allocator)
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
  if (!functional_.empty()) {
    Value vv(kArrayType);
    for (auto &e : functional_) {
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
  if (!latentDetection_.empty()) {
    Value vv(kArrayType);
    for (auto &e : latentDetection_) {
      Value vvv(kObjectType);
      Value vvv1, vvv2;
      vvv1.SetString(e.second.first.c_str(), e.second.first.size(), allocator);
      vvv2.SetString(e.second.second.c_str(), e.second.second.size(), allocator);
      vvv.AddMember("id", vvv1, allocator);
      vvv.AddMember("type", vvv2, allocator);
      vv.PushBack(vvv, allocator);
    }
    v.AddMember("latent_detection_output", vv, allocator);
  }
  return v;
}

static string removeLastIncorrectCharater(const string &s)
{
  string n = s;
  if (n[n.size() - 1] == '"') {
    n = n.substr(0, n.size()-1);
  }
  return n;
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
      d.check_ = removeLastIncorrectCharater(tokens[++i]);
    }
    else if (tokens[i] == "-meas_name") {
      d.measName_ = removeLastIncorrectCharater(tokens[++i]);
    }
    else if (tokens[i] == "-min") {
      d.min_ = removeLastIncorrectCharater(tokens[++i]);
    }
    else if (tokens[i] == "-max") {
      d.max_ = removeLastIncorrectCharater(tokens[++i]);
    }
    else if (tokens[i] == "-id") {
      d.id_ = removeLastIncorrectCharater(tokens[++i]);
    }
    else if (tokens[i] == "-error") {
      d.error_ = removeLastIncorrectCharater(tokens[++i]);
    }
  }
  return d;
}

DiagSetting::DiagSetting()
  :enabled_(false)
{
}

void DiagSetting::buildSimStops(const std::vector<std::string> &fNs)
{
  for (auto & fN : fNs) {
    ifstream in(fN);
    string line;
    while(std::getline(in, line)) {
      if (line.find("set_sim_stop") != string::npos) {
        SimStop d = buildSimStop(line);
        simStops_[d.id_] = d; 
      }
    }
  }
}

void DiagSetting::buildDiagSetting(const rapidjson::Document &json)
{
  vector<string> fileNames;
  if (json.HasMember("top_netlist") &&
      json["top_netlist"].HasMember("schematic")) {
    fileNames.push_back(json["top_netlist"]["schematic"].GetString());
  }
  if (json.HasMember("sim_cmdline") &&
      json["sim_cmdline"].HasMember("suffix")) {
    istringstream iss(json["sim_cmdline"]["suffix"].GetString());
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));
    for (int i = 0; i < tokens.size(); ++i) {
      if (tokens[i] == "-c") {
        fileNames.push_back(tokens[++i]);
        break;
      }
    }
  }
  buildSimStops(fileNames);
  if (!json.HasMember("diagnostic_coverage")) return;
  enabled_ = true;
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
    vcFsmExport_.enabled_ = true;
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
    vcFsmExport_.enabled_ = false;
  }
  // set faulure mode
  failureModes_.clear();
  if (diag.HasMember("failure_mode")) {
    const Value &failureModes = diag["failure_mode"];
    for (auto &v : failureModes.GetArray()) {
      FailureMode f;
      f.id_ = v["id"].GetString();
      f.idLabel_ = v["id_label"].GetString();
      if (v.HasMember("functional_output")) {
        for (auto &vv : v["functional_output"].GetArray()) {
          string id = vv["id"].GetString(), type = vv["type"].GetString();
          f.functional_[id] = make_pair(id, type);
        }
      }
      if (v.HasMember("detection_output")) {
        for (auto &vv : v["detection_output"].GetArray()) {
          string id = vv["id"].GetString(), type = vv["type"].GetString();
          f.detection_[id] = make_pair(id, type);
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
    v.push_back(e.first);
  }
  return v;
}

Value DiagSetting::getValue(Document::AllocatorType &allocator)
{
  Value o;
  if (!enabled_) return o;
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
  if (vcFsmExport_.enabled_) {
    Value vc = vcFsmExport_.getValue(allocator);
    o.AddMember("vc_fsm_export", vc, allocator);
  }
    // for failure mode
  Value failures(kArrayType);
  for (auto & a: failureModes_) {
    failures.PushBack(a.second.getValue(allocator), allocator);
  }
  o.AddMember("failure_mode", failures, allocator);
  
  return o;
}

void DiagSetting::clear()
{
  simStops_.clear();
  enabled_ = false;
  undetectedFaults_ = "";
  vcFsmExport_.clear();
  failureModes_.clear();
}
