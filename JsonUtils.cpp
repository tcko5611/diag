#include <fstream>
#include <cassert>
#include <iostream>
using namespace std;

#include "JsonUtils.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

Document parseJsonFileToDocument(const string &fileName)
{
  Document d;

  ifstream ifile(fileName);
  assert(ifile);
  IStreamWrapper isw(ifile);

  if (d.ParseStream<kParseCommentsFlag|kParseNanAndInfFlag>(isw).HasParseError()) {
    uint o = (unsigned) d.GetErrorOffset();
    if (o < 40) o = 40;
    string content;
    ifstream nfile(fileName);
    content.assign((istreambuf_iterator<char>(nfile)),
                   (istreambuf_iterator<char>()));
    cerr << string("JSON parse error\n\nError:")
         << GetParseError_En(d.GetParseError()) << "\nMost likely here: \n" 
         << content.substr(o-40, 80) << "...";
  }

  // copy to output document
  Document out;
  out.CopyFrom(d, out.GetAllocator());
  return out;
}

static void filePutContents(const string& name,
                     const string& content,
                     bool append=false)
{
  ofstream outfile;
  if (append)
    outfile.open(name, std::ios_base::app);
  else
    outfile.open(name);
  outfile << content;
}

void dumpJsonDocumentToFile(const string &filename, Document& d)
{
  StringBuffer buffer;
  PrettyWriter<StringBuffer> writer(buffer);
  d.Accept(writer);

  filePutContents(filename, buffer.GetString());
}

