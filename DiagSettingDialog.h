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
 * for output type
 */
class OutputTypeDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  OutputTypeDelegate(QWidget *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
signals:
  /**
   * update or add output id
   * @param oldTypeId old type@=id, if empty then a new one
   * @param newTypeId new type@=id 
   * @param category, if new can be empty
   */
  void updateOutputTypeId(const QString &oldId, const QString &newId, const QString &category) const;

private:
  mutable QString oldType_;
};

/**
 * for output id
 */
class OutputIdDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  OutputIdDelegate(QWidget *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
signals:
  /**
   * update or add output id
   * @param oldTypeId old type@=id, if empty then a new one
   * @param newTypeId new type@=id 
   * @param category, if new can be empty
   */
  void updateOutputTypeId(const QString &oldId, const QString &newId, const QString &category) const;
  
private:
  mutable QString oldId_;
};

/**
 * for output signal
 */
class OutputSignalDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  OutputSignalDelegate(QWidget *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
signals:
  /**
   * update or add output id
   * @param oldTypeId old type@=signal, if empty then a new one
   * @param newTypeId new type@=signal 
   * @param category, if new can be empty
   */
  void updateOutputTypeId(const QString &oldTypeId, const QString &newTypeId, const QString &category) const;
  
private:
  mutable QString oldSignal_;
};

/**
 * for output category
 */ 

class OutputCategoryDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  OutputCategoryDelegate(QWidget *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
signals:
  void updateOutputCategory(const QString &typeId,const QString &oldCategory,
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

class DiagSettingDialog : public QDialog
{
  Q_OBJECT
public:
  enum FailureModeColumnIndex {
    F_Id = 0, F_IdLabel, FailureModeColumnEnd
  };
  enum OutputColumnIndex {
    O_Type = 0, O_Id, O_Signal, O_MeasName, O_Min, O_Max, O_Category, OutputColumnEnd 
  };
  
public:
  explicit DiagSettingDialog(QWidget *parent = 0);
  ~DiagSettingDialog();
  void setAfData(QAfData *qAf);
  void restoreAfData();
  QStringList getOutputTypes();
  QStringList getSimStopIds();
  void updateTableWidgetOutput(const QString &failureModeId);
  void cleanUiContain();
                       
public slots:
  void onUpdateFailureModeId(const QString &, const QString&);
  void onUpdateFailureModeIdLabel(const QString &id, const QString &idLabel);
  void onUpdateOutputTypeId(const QString &oldTypeId, const QString &newTypeId, const QString &category);
  void onUpdateOutputCategory(const QString &TypeId, const QString &oldCategory, const QString &newCategory);
  void accept() override;
private slots:
  void on_tableWidgetFailureMode_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
  void removeFailureMode(const QString &id);
  void insertFailureMode(const QString &id);
  void changeFailureModeId(const QString &oldId, const QString &newId);
  void changeFailureModeIdLabel(const QString &id, const QString &idLable);
  void removeFuctional(const QString &typeId);
  void removeDetection(const QString &typeId);
  void removeLatentDetection(const QString &typeId);
  void insertFuctional(const QString &typeId);
  void insertDetection(const QString &typeId);
  void insertLatentDetection(const QString &typeId);
  void insertOutput(const QString &typeId, const QString &category);
  void removeOutput(const QString &typeId, const QString &category);
private:
  Ui::DiagSettingDialog *ui;
  Diag::DiagSetting setting_;
  FailureModeIdDelegate *failureModeIdDelegate_;
  FailureModeIdLabelDelegate *failureModeIdLabelDelegate_;
  OutputTypeDelegate *outputTypeDelegate_;
  OutputIdDelegate *outputIdDelegate_;
  OutputSignalDelegate *outputSignalDelegate_;
  OutputCategoryDelegate *outputCategoryDelegate_;
  
  QString currentFailureModeId_;
  QAfData *qAf_;
};

#endif /* DIAGSETTINGDIALOG_H */
