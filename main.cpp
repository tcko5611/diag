#include <QApplication>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <set>
using namespace std;

#include "JsonUtils.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  //  DiagResultDialog w;
  
  // Document config = parseJsonFileToDocument("config.json");
  // Document flist = parseJsonFileToDocument("test2.fdb");
  // w.settingTables("cfg.xa", config, flist);
  MainWindow w;
  w.show();
  return a.exec();
}
