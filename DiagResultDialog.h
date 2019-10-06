#ifndef DIAGRESULTDIALOG_H /* -*-c++-*- */
#define DIAGRESULTDIALOG_H

#include <QDialog>
#include "rapidjson/document.h"

#include "DiagResult.h"
class QAfData;

using namespace rapidjson;

namespace Ui {
class DiagResultDialog;
}

class DiagResultDialog : public QDialog
{
    Q_OBJECT
  enum SimStopsColumnIndex {
    S_Id, S_Check, S_MeasName, S_Max, S_Min, S_Category,  SimStopsColumnEnd
  };
  enum ResultColumnIndex {
    R_Gid, R_Functional, R_Detection, R_LatentDetection,  ResultColumnEnd
  };


public:
  explicit DiagResultDialog(QWidget *parent = 0);
  ~DiagResultDialog();
  void setAfData(QAfData *qAf);
  
private slots:
  void on_comboBoxFailureMode_currentIndexChanged(const QString &arg1);

  void on_comboBoxTag_currentIndexChanged(const QString &arg1);

private:
  void updateTableSimStops(const QString &);
  void updateTableResult(const QString &, const QString &);
  
  Ui::DiagResultDialog *ui;
  Diag::DiagResult result_;
  QString currentFailureMode_, currentTag_;
};

#endif // DIAGRESULTDIALOG_H
