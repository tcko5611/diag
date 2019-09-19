#ifndef JSONUTILS_H /* -*-c++-*- */
#define JSONUTILS_H

#include <string>
using namespace std;

#include "rapidjson/document.h"
using namespace rapidjson;

Document parseJsonFileToDocument(const string &filename);
void dumpJsonDocumentToFile(const string &file, Document &d);
#endif /* JSONUTILS_H */
