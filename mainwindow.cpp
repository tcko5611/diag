#include <QStringList>
#include <QTreeWidgetItem>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QAfData.h"
#include "rapidjson/document.h"
#include "DiagSettingDialog.h"

using namespace rapidjson;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), root_(0)
{
  ui->setupUi(this);
  qAf = new QAfData();
  connect(qAf, SIGNAL(configJsonChanged()),
          this, SLOT(onConfigJsonChanged()));
  connect(qAf, SIGNAL(faultListChanged()),
          this, SLOT(onFaultListChanged()));
  diagSettingDialog_ = new DiagSettingDialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QTreeWidgetItem *MainWindow::createChildItem(QTreeWidgetItem *parent, const Value& name, const Value &value)
{
  QTreeWidgetItem *child;  
  int type = value.GetType();
  QStringList l;
  switch(type) {

  case 0:
    l << name.GetString() << "null";
    child = new QTreeWidgetItem(parent, l);
    break;
  case 1:
    l << name.GetString() << "False";
    child = new QTreeWidgetItem(parent, l);
    break;
  case 2:
    l << name.GetString() << "True";
    child = new QTreeWidgetItem(parent, l);
    break;
  case 3:
    l << name.GetString();
    child = new QTreeWidgetItem(parent, l);
    for (Value::ConstMemberIterator itr = value.MemberBegin();
           itr != value.MemberEnd(); ++itr) {
      QTreeWidgetItem * item = createChildItem(child, itr->name, itr->value);
      child->addChild(item);
    }
    break;
  case 4:
    l << name.GetString();
    child = new QTreeWidgetItem(parent, l);
    for (auto &v : value.GetArray()) {
      QTreeWidgetItem * item = createChildItem(child, Value("array"), v);
      child->addChild(item);
    }
    break;
  case 5:
    l << name.GetString() << value.GetString();
    child = new QTreeWidgetItem(parent, l);
    break;
  case 6:
    l << name.GetString() << QString::number(value.GetDouble());
    child = new QTreeWidgetItem(parent, l);
    break;
  }
  return child;
}

void MainWindow::buildTree()
{
  if (root_) delete root_;
  const Document& jsonDoc = qAf->configJson();
  assert(jsonDoc.IsObject());
  root_ = new QTreeWidgetItem(ui->treeWidget, QStringList(QString("Root")));
  QTreeWidgetItem *child;
  for (Value::ConstMemberIterator itr = jsonDoc.MemberBegin();
         itr != jsonDoc.MemberEnd(); ++itr) {
    child = createChildItem(root_, itr->name, itr->value);
    root_->addChild(child);
  }
}


void MainWindow::on_actionLoadConfig_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Config File"), ".",
                               tr("Configs (*.json)"));
  qAf->parseConfigJson(fileName);
  diagSettingDialog_->setAfData(qAf);
}


void MainWindow::on_actionLoadFdb_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Config File"), ".",
                               tr("Configs (*.json)"));
  qAf->parseFaultList(fileName);
}


void MainWindow::onConfigJsonChanged()
{
  buildTree();
}

void MainWindow::onFaultListChanged()
{
}

void MainWindow::on_actionDiagSetting_triggered()
{
  diagSettingDialog_->open();
}
