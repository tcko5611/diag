#include "rapidjson/document.h"
using namespace rapidjson;

#include "DiagResultDialog.h"
#include "ui_DiagResultDialog.h"
#include "DiagMeas.h"
#include "DiagReportItem.h"

DiagResultDialog::DiagResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiagResultDialog)
{
  ui->setupUi(this);
  ui->settingsTableWidget->setColumnCount(6);
  QStringList labels;
  labels << "id" << "check" << "meas name" << "min" << "max" << "category";
  ui->settingsTableWidget->setHorizontalHeaderLabels(labels);
  labels.clear();
  ui->resultsTableWidget->setColumnCount(4);
  labels << "gid" << "functional ids" << "detection ids" << "category";
  ui->resultsTableWidget->setHorizontalHeaderLabels(labels);
}

DiagResultDialog::~DiagResultDialog()
{
  delete ui;
}

void DiagResultDialog::settingTables(const QString &fN, Document &config,
                                     Document &flist)
{
  map<string, DiagMeas> diagMeasMap;
  set<string> functionalSet;
  set<string> detectionSet;
  buildDiagMeasMap(fN.toStdString(), config, diagMeasMap, functionalSet, detectionSet);
  ui->settingsTableWidget->setRowCount(0);
  for (map<string, DiagMeas>::const_iterator it = diagMeasMap.begin();
       it != diagMeasMap.end(); ++it) {
    int row = ui->settingsTableWidget->rowCount();
    ui->settingsTableWidget->insertRow( ui->settingsTableWidget->rowCount() );
    QTableWidgetItem *i0 = new QTableWidgetItem(it->second.id().c_str());
    QTableWidgetItem *i1 = new QTableWidgetItem(it->second.check().c_str());
    QTableWidgetItem *i2 = new QTableWidgetItem(it->second.measName().c_str());
    QTableWidgetItem *i3 = new QTableWidgetItem(it->second.min().c_str());
    QTableWidgetItem *i4 = new QTableWidgetItem(it->second.max().c_str());
    QTableWidgetItem *i5 = new QTableWidgetItem(it->second.category().c_str());
    ui->settingsTableWidget->setItem(row, 0, i0);
    ui->settingsTableWidget->setItem(row, 1, i1);
    ui->settingsTableWidget->setItem(row, 2, i2);
    ui->settingsTableWidget->setItem(row, 3, i3);
    ui->settingsTableWidget->setItem(row, 4, i4);
    ui->settingsTableWidget->setItem(row, 5, i5);
  }
  map<int, DiagReportItem> diagReportMap;
  buildDiagReport(flist, functionalSet, detectionSet, diagReportMap);
  ui->resultsTableWidget->setRowCount(0);
  for (map<int, DiagReportItem>::const_iterator it = diagReportMap.begin();
       it != diagReportMap.end(); ++it) {
    int row = ui->resultsTableWidget->rowCount();
    ui->resultsTableWidget->insertRow( ui->resultsTableWidget->rowCount() );
    QTableWidgetItem *i0 = new QTableWidgetItem(QString::number(it->second.gid()));
    QTableWidgetItem *i1 = new QTableWidgetItem(it->second.functionalIds().c_str());
    QTableWidgetItem *i2 = new QTableWidgetItem(it->second.detectionIds().c_str());
    QTableWidgetItem *i3 = new QTableWidgetItem(it->second.category().c_str());
    ui->resultsTableWidget->setItem(row, 0, i0);
    ui->resultsTableWidget->setItem(row, 1, i1);
    ui->resultsTableWidget->setItem(row, 2, i2);
    ui->resultsTableWidget->setItem(row, 3, i3);
  }
}
