#include <QTableWidgetItem>
#include <QDebug>
#include "rapidjson/document.h"
using namespace rapidjson;

#include "DiagResultDialog.h"
#include "ui_DiagResultDialog.h"
#include "DiagResult.h"
#include "QAfData.h"

using namespace Diag;

DiagResultDialog::DiagResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiagResultDialog)
{
  ui->setupUi(this);
}

DiagResultDialog::~DiagResultDialog()
{
  delete ui;
}

void DiagResultDialog::setAfData(QAfData *qAf)
{
  // setting result data
  result_.clear();
  result_.build(qAf->configJson(), qAf->flistJson());

  // set comboBox;
  ui->comboBoxFailureMode->setCurrentIndex(-1);
  ui->comboBoxFailureMode->clear();
  ui->comboBoxTag->setCurrentIndex(-1);
  ui->comboBoxTag->clear();
  QStringList strs;
  for (auto &e: result_.failureModeResults_) {
    strs << e.first.c_str();
  }
  ui->comboBoxFailureMode->addItems(strs);

  if (ui->comboBoxFailureMode->count() != 0) {
    qDebug() << ui->comboBoxFailureMode->count();
    ui->comboBoxFailureMode->setCurrentIndex(0);
  }
  std::set<std::string> tags;
  for (auto &e: result_.failureModeResults_) {
    for (auto &ee : e.second.tagResults_) {
      tags.insert(ee.first);
    }
  }
  strs.clear();
  for (auto e : tags) {
    strs << e.c_str();
  }
  ui->comboBoxTag->addItems(strs);
  if (ui->comboBoxTag->count() != 0) {
    qDebug() << ui->comboBoxTag->count();
    ui->comboBoxTag->setCurrentIndex(0);
  }
  updateTableSimStops(ui->comboBoxFailureMode->currentText());
  updateTableResult(ui->comboBoxFailureMode->currentText(),
		    ui->comboBoxTag->currentText());
}

void DiagResultDialog::updateTableSimStops(const QString &failureMode)
{
  if (currentFailureMode_ == failureMode) return;
  ui->tableWidgetSimStops->setRowCount(0);
  currentFailureMode_ = failureMode;
  FailureModeResult &f = result_.failureModeResults_.at(currentFailureMode_.toStdString());
  for (auto &id : f.functional_) {
    SimStop &s = result_.simStops_.at(id);
    QStringList strs;
    strs << id.c_str() << s.check_.c_str() << s.measName_.c_str()
	 << s.max_.c_str() << s.min_.c_str() << "functional";
    int row = ui->tableWidgetSimStops->rowCount();
    ui->tableWidgetSimStops->insertRow(row);
    for (int i = S_Id; i < SimStopsColumnEnd; ++i) {
      QTableWidgetItem *item = new  QTableWidgetItem(strs[i]);
      ui->tableWidgetSimStops->setItem(row, i, item);
    }
  }
  for (auto &id : f.detection_) {
    SimStop &s = result_.simStops_.at(id);
    QStringList strs;
    strs << id.c_str() << s.check_.c_str() << s.measName_.c_str()
	 << s.max_.c_str() << s.min_.c_str() << "detection";
    int row = ui->tableWidgetSimStops->rowCount();
    ui->tableWidgetSimStops->insertRow(row);
    for (int i = S_Id; i < SimStopsColumnEnd; ++i) {
      QTableWidgetItem *item = new  QTableWidgetItem(strs[i]);
      ui->tableWidgetSimStops->setItem(row, i, item);
    }
  }
  for (auto &id : f.latentDetection_) {
    SimStop &s = result_.simStops_.at(id);
    QStringList strs;
    strs << id.c_str() << s.check_.c_str() << s.measName_.c_str()
	 << s.max_.c_str() << s.min_.c_str() << "latent_detection";
    int row = ui->tableWidgetSimStops->rowCount();
    ui->tableWidgetSimStops->insertRow(row);
    for (int i = S_Id; i < SimStopsColumnEnd; ++i) {
      QTableWidgetItem *item = new  QTableWidgetItem(strs[i]);
      ui->tableWidgetSimStops->setItem(row, i, item);
    }
  }

}

void DiagResultDialog::updateTableResult(const QString &failureMode, const QString &tag)
{
  if ((currentFailureMode_ == failureMode) &&
      (currentTag_ == tag)) return;
  if (currentFailureMode_ != failureMode) {
    updateTableSimStops(failureMode);
  }
  ui->tableWidgetResult->setRowCount(0);
  FailureModeResult &f = result_.failureModeResults_.at(currentFailureMode_.toStdString());
  currentTag_ = tag;
  TagResult &t = f.tagResults_.at(tag.toStdString());
  for (auto &fr : t.faultResult_) {
    QStringList strs;
    strs << QString::number(fr.gid_) << fr.functionalIds_.c_str()
	 << fr.detectionIds_.c_str() << fr.latentDetectionIds_.c_str()
	 << fr.category_.c_str();
    int row = ui->tableWidgetResult->rowCount();
    ui->tableWidgetResult->insertRow(row);
    for (int i = R_Gid; i < ResultColumnEnd; ++i) {
      QTableWidgetItem *item = new  QTableWidgetItem(strs[i]);
      if (fr.category_ == "dangerous_undetected") {
        item->setBackground(Qt::red);
      }
      ui->tableWidgetResult->setItem(row, i, item);
    }
  }

}

void DiagResultDialog::on_comboBoxFailureMode_currentIndexChanged(const QString &failureMode)
{
  if (failureMode.isEmpty()) return;
  updateTableSimStops(failureMode);
  updateTableResult(failureMode, currentTag_);
}

void DiagResultDialog::on_comboBoxTag_currentIndexChanged(const QString &tag)
{
  if (tag.isEmpty()) return;
  updateTableResult(currentFailureMode_, tag);
}
