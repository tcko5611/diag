#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "QAfData.h"
using namespace std;
using namespace rapidjson;

QAfData::QAfData()
{
}

QAfData::~QAfData()
{
}

const Document& QAfData::configJson()
{
  return jsonDoc_;
}

const Document& QAfData::flistJson()
{
  return faultList_;
}

Document::AllocatorType& QAfData::getAllocator()
{
  return jsonDoc_.GetAllocator();
}

void QAfData::parseConfigJson(const QString &fileName)
{
  ifstream in(fileName.toStdString());
  IStreamWrapper isw(in);
  if (jsonDoc_.ParseStream<kParseCommentsFlag|kParseNanAndInfFlag>(isw).HasParseError()) {
    exit(0);
  }
  emit configJsonChanged();
}


void QAfData::parseFaultList(const QString &fileName)
{
  ifstream in(fileName.toStdString());
  IStreamWrapper isw(in);
  if (faultList_.ParseStream<kParseCommentsFlag|kParseNanAndInfFlag>(isw).HasParseError()) {
    exit(0);
  }
  emit faultListChanged();
}
