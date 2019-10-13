#include <algorithm>
#include <sstream>
#include <iterator>
#include <fstream>

#include "DiagResult.h"

using namespace std;
using namespace boost::logic;
using namespace rapidjson;
using namespace Diag;

DiagResult::DiagResult()
{
}

void DiagResult::buildSimStops(const std::vector<std::string> &fNs)
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

void DiagResult::build(const rapidjson::Document &config,
			const rapidjson::Document &flist)
{
  // build simstops
  vector<string> fileNames;
  if (config.HasMember("top_netlist") &&
      config["top_netlist"].HasMember("schematic")) {
    fileNames.push_back(config["top_netlist"]["schematic"].GetString());
  }
  if (config.HasMember("sim_cmdline") &&
      config["sim_cmdline"].HasMember("suffix")) {
    istringstream iss(config["sim_cmdline"]["suffix"].GetString());
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));
    for (unsigned i = 0; i < tokens.size(); ++i) {
      if (tokens[i] == "-c") {
        fileNames.push_back(tokens[++i]);
        break;
      }
    }
  }
  buildSimStops(fileNames);
  // end of build simstops
  
  if (!config.HasMember("diagnostic_coverage")) return;
  const Value &diag = config["diagnostic_coverage"];
  // set failure mode
  failureModeResults_.clear();
  if (diag.HasMember("failure_mode")) {
    const Value &failureModes = diag["failure_mode"];
    for (auto &v : failureModes.GetArray()) {
      FailureModeResult f;
      f.id_ = v["id"].GetString();
      if (v.HasMember("functional_output")) {
        for (auto &vv : v["functional_output"].GetArray()) {
          string type = vv["type"].GetString();
	  if (type == "meas") {
	    string id = vv["id"].GetString();
	    f.functional_.insert(id);
	  }
        }
      }
      if (v.HasMember("detection_output")) {
        for (auto &vv : v["detection_output"].GetArray()) {
          string type = vv["type"].GetString();
	  if (type == "meas") {
	    string id = vv["id"].GetString();
	    f.detection_.insert(id);
	  }
        }
      }
      if (v.HasMember("latent_detection_output")) {
        for (auto &vv : v["latent_detection_output"].GetArray()) {
          string type = vv["type"].GetString();
	  if (type == "meas") {
	    string id = vv["id"].GetString();
	    f.latentDetection_.insert(id);
	  }
        }
      }

      failureModeResults_[f.id_] = f;
    }
  }
  // end of create failure mode results
  // parse result
  if (flist.HasMember("fault_list") && flist["fault_list"].IsArray()) {
    for (auto &v : flist["fault_list"].GetArray()) {
      if (!v.HasMember("result") || !v["result"].IsArray()) continue;
      int gid = v["gid"].GetInt();
      string failureModeId;
      string tag;
      string category;
      string functionalIds;
      string detectionIds;
      string latentDetectionIds;
      vector<std::string> simStopIds;
      for (auto &vv : v["result"].GetArray()) {
        tag = vv["tag"].GetString();
        simStopIds.clear();
        for (auto &vvv : vv["details"].GetArray()) {
          simStopIds.push_back(vvv["id"].GetString());
        }
        for (auto &vvv : vv["failure_mode"].GetArray()) {
          failureModeId = vvv["id"].GetString();
          category = vvv["category"].GetString();
          FailureModeResult &f = failureModeResults_[failureModeId];
          FaultResult fr;
          fr.gid_ = gid;
          fr.failureModeId_ = failureModeId;
          fr.tag_ = tag;
          fr.category_ = category;
          for (auto id : simStopIds) {
            if (f.functional_.find(id) != f.functional_.end()) {
              fr.functionalIds_ += id + " ";
            }
            else if (f.detection_.find(id) != f.detection_.end()) {
              fr.detectionIds_ += id + " ";
            }
            else if (f.latentDetection_.find(id) != f.latentDetection_.end()) {
              fr.latentDetectionIds_ += id + " ";
            }
          }
          
          if (f.tagResults_.find(tag) == f.tagResults_.end()) {
            TagResult r;
            r.tag_ = tag;
            f.tagResults_[tag] = r;
          }
          TagResult &r = f.tagResults_[tag];
          r.faultResult_.push_back(fr);
        }
      }
    }
  }  
}

void DiagResult::clear()
{
  simStops_.clear();
  failureModeResults_.clear();
}
