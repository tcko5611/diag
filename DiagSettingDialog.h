#ifndef DIAGSETTINGDIALOG_H /* -*-c++-*- */
#define DIAGSETTINGDIALOG_H

#include <QDialog>
#include <QItemDelegate>

#include "DiagSetting.h"

/**
 * Delegates declaration
 */
/**
 * for fault mode id
 */
class FailureModeIdDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  FailureModeIdDelegate(QObject *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
signals:
  void updateFailureModeId(const QString &oldId, const QString&newId) const;

private:
  mutable QString oldId_;
};

/**
 * for fault mode id_label
 */
class FailureModeIdLabelDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  FailureModeIdLabelDelegate(QWidget *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
signals:
  void updateFailureModeIdLabel(const QString &id, const QString&idLabel) const;
};


/**
 * for meas id
 */
class MeasIdDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  MeasIdDelegate(QWidget *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
signals:
  /**
   * update or add meas id
   * @param oldId old id, if empty then a new one
   * @param newId new id 
   * @param category, if new can be empty
   */
  void updateMeasId(const QString &oldId, const QString &newId, const QString &category) const;
  
private:
  mutable QString oldId_;
};

/**
 * for meas category
 */ 

class MeasCategoryDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  MeasCategoryDelegate(QWidget *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
signals:
  void updateMeasCategory(const QString &id,const QString &oldCategory,
                          const QString &newCategory) const;
private:
  mutable QString oldCategory_; 
};

/**
 * end of delegates
 */

namespace Ui {
  class DiagSettingDialog;
}

class QAfData;
class DiagSetting;

class DiagSettingDialog : public QDialog
{
  Q_OBJECT
public:
  enum FailureModeColumnIndex {
    F_Id = 0, F_IdLabel, FailureModeColumnEnd
  };
  enum MeasColumnIndex {
    M_Id = 0, M_Check, M_MeasName, M_Min, M_Max, M_Category, MeasColumnEnd 
  };
  
public:
  explicit DiagSettingDialog(QWidget *parent = 0);
  ~DiagSettingDialog();
  void setAfData(QAfData *qAf);
  void restoreAfData();
  QStringList getSimStopIds();
  void clear();
  void updateTableWidgetMeas(const QString &failureModeId);

public slots:
  void onUpdateFailureModeId(const QString &, const QString&);
  void onUpdateFailureModeIdLabel(const QString &id, const QString &idLabel);
  void onUpdateMeasId(const QString &oldId, const QString &newId, const QString &category);
  void onUpdateMeasCategory(const QString &id, const QString &oldCategory, const QString &newCategory);
  void accept() override;
private slots:
  void on_tableWidgetFailureMode_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
  void removeFailureMode(const QString &id);
  void insertFailureMode(const QString &id);
  void changeFailureModeId(const QString &oldId, const QString &newId);
  void changeFailureModeIdLabel(const QString &id, const QString &idLable);
  void removeFuctional(const QString &id);
  void removeDetection(const QString &id);
  void removeLatentDetection(const QString &id);
  void insertFuctional(const QString &id, const QString &type);
  void insertDetection(const QString &id, const QString &type);
  void insertLatentDetection(const QString &id, const QString &type);
  void insertMeas(const QString &id, const QString &category);
  void removeMeas(const QString &id, const QString &category);
private:
  Ui::DiagSettingDialog *ui;
  DiagSetting setting_;
  FailureModeIdDelegate *failureModeIdDelegate_;
  FailureModeIdLabelDelegate *failureModeIdLabelDelegate_;
  MeasIdDelegate *measIdDelegate_;
  MeasCategoryDelegate *measCategoryDelegate_;
  
  QString currentFailureModeId_;
  QAfData *qAf_;
};

#endif /* DIAGSETTINGDIALOG_H */
