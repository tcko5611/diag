#ifndef QAFDATA_H /* -*-c++-*- */
#define QAFDATA_H

#include <QString>
#include <QObject>

#include "rapidjson/document.h"
using namespace rapidjson;

class QAfData : public QObject
{
  Q_OBJECT
public:
  QAfData();
  ~QAfData();
  bool hasMember(const char* field1, const char* field2 = 0,
                 const char* field3 = 0, const char* field4 = 0);
  QString getStringField(const char* field1,const char* field2,const char* field3);
  QString getStringField(const char* field1,const char* field2);
  QString getStringField(const char* field);

  int getIntField(const char* field1,const char* field2,const char* field3,int def = 0);
  int getIntField(const char* field1,const char* field2,int def = 0);
  int getIntField(const char* field,int def = 0);

  double getDoubleField(const char* field1,const char* field2,const char* field3,const char* field4,double def = 0.0);
  double getDoubleField(const char* field1,const char* field2,const char* field3,double def = 0.0);
  double getDoubleField(const char* field1,const char* field2,double def = 0.0);
  double getDoubleField(const char* field,double def = 0.0);
  
  bool getBoolField(const char* field1,const char* field2,const char* field3,bool def = false);
  bool getBoolField(const char* field1,const char* field2,bool def = false);
  bool getBoolField(const char* field,bool def = false);
  
  void setStringField(const char* field1,QString value);
  void setStringField(const char* field1,const char* field2,QString value);
  void setStringField(const char* field1,const char* field2,const char* field3,QString value);

  void setIntField(const char* field1,int value);
  void setIntField(const char* field1,const char* field2,int value);
  void setIntField(const char* field1,const char* field2,const char* field3,int value);
  
  void setDoubleField(const char* field1,double value);
  void setDoubleField(const char* field1,const char* field2,double value);
  void setDoubleField(const char* field1,const char* field2,const char* field3,double value);
  void setDoubleField(const char* field1,const char* field2,const char* field3,const char* field4,double value);
  
  void setBoolField(const char* field1,bool value);
  void setBoolField(const char* field1,const char* field2,bool value);
  void setBoolField(const char* field1,const char* field2,const char* field3,bool value);
  
  void setValueField(const char* field1, Value &value);
  
  void setValueField(const char* field1, const char* field2, Value &value);
  
  
  void parseConfigJson(const QString &fileName);
  void parseFaultList(const QString &fileName);
  const Document& configJson();
  const Document& flistJson();
  Document::AllocatorType& getAllocator();

signals:
  void configJsonChanged();
  void faultListChanged();
private:
  Document jsonDoc_;
  Document faultList_;
};
#endif /* QAFDATA_H */
