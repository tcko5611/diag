#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include "DiagCommon.h"

using namespace Diag;
using namespace std;

string Diag::removeLastIncorrectCharater(const string &s)
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
SimStop Diag::buildSimStop(const string& line)
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
