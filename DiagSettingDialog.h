#ifndef DIAGSETTINGDIALOG_H /* -*-c++-*- */
#define DIAGSETTINGDIALOG_H

#include <QDialog>
#include <QItemDelegate>

class FModeDelegate : public QItemDelegate
{
public:
  FModeDelegate(QWidget *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const;
};

namespace Ui {
  class DiagSettingDialog;
}

struct SimStop {
  string id_;
  string check_;
  string measName_;
  string min_;
  string max_;
  string error_;
};

class DiagSettingDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit DiagSettingDialog(QWidget *parent = 0);
  ~DiagSettingDialog();
  void buildSimStops(const std::vector<std::string> &fileNames);
  void setAfData(QAfData *qAf);
  void restoreAfData(QAfData* qAf);
  
private:
  Ui::DiagSettingDialog *ui;
  map<string, SimStop> simStops_;
};

#endif /* DIAGSETTINGDIALOG_H */
