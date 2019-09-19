#ifndef MAINWINDOW_H /* -*-c++-*- */
#define MAINWINDOW_H

#include <QMainWindow>

#include "rapidjson/document.h"

class QAfData;
class QTreeWidgetItem;

namespace Ui {
  class MainWindow;
}

using namespace rapidjson;

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void buildTree();
  QTreeWidgetItem *createChildItem(QTreeWidgetItem *parent, const Value& name,
                                   const Value &value);
public slots:
  void onConfigJsonChanged();
  void onFaultListChanged();
                         
private slots:
  void on_actionLoadConfig_triggered();

  void on_actionLoadFdb_triggered();

private:
  Ui::MainWindow *ui;
  QAfData *qAf;
  QTreeWidgetItem *root_;
};

#endif // MAINWINDOW_H
