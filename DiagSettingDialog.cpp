#include <QRegExp>
#include <QRegExpValidator>
#include "QAfData.h"
#include "DiagSettingDialog.h"
#include "ui_DiagSettingDialog.h"

using namespace boost::logic;
using namespace std;
using namespace Diag;
/**
 * Delegates implemetation
 */

FailureModeIdDelegate::FailureModeIdDelegate(QObject *parent)
  :QItemDelegate(parent)
{
}

QWidget* FailureModeIdDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
  QLineEdit *l = new QLineEdit(parent);
  return l;
}

void FailureModeIdDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QTableWidget *tb = qobject_cast<QTableWidget*>(this->parent());
  int row = index.row();
  int rowCount = tb->rowCount();
  if (row == rowCount -1) {
    tb->insertRow(row);
    tb->clearSelection();
    oldId_ = "";
  } else {
    QItemDelegate::setEditorData(editor, index);
    oldId_ = index.model()->data(index, Qt::EditRole).toString();
  }
}

void FailureModeIdDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
  QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
  QTableWidget *tb = static_cast<QTableWidget*>(this->parent());
  QString oldId = oldId_;
  if (lineEdit->text().isEmpty()) {
    int row = index.row();
    tb->removeRow(row);
  } else {
    QItemDelegate::setModelData(editor, model, index);
  }
  emit updateFailureModeId(oldId, lineEdit->text());
}

FailureModeIdLabelDelegate::FailureModeIdLabelDelegate(QWidget *parent)
    :QItemDelegate(parent)
{
}

QWidget* FailureModeIdLabelDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
  QLineEdit *l = new QLineEdit(parent);
  return l;
}

void FailureModeIdLabelDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QItemDelegate::setEditorData(editor, index);
}

void FailureModeIdLabelDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
  QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
  QTableWidget *tb = static_cast<QTableWidget*>(this->parent());
  int row = index.row();
  QString id = tb->item(row, 0)->data(Qt::EditRole).toString();
  QItemDelegate::setModelData(editor, model, index);
  emit updateFailureModeIdLabel(id, lineEdit->text());
}

OutputTypeDelegate::OutputTypeDelegate(QWidget *parent)
  :QItemDelegate(parent)
{
}


QWidget* OutputTypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QComboBox *editor = new QComboBox(parent);
  QTableWidget * tb = qobject_cast<QTableWidget*>(this->parent());
  DiagSettingDialog *dlg = qobject_cast<DiagSettingDialog*>(tb->parent());
  // editor->addItems(dlg->getOutputTypes());
  return editor;
}

void OutputTypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
  QTableWidget *tb = qobject_cast<QTableWidget*>(this->parent());
  DiagSettingDialog *dlg = qobject_cast<DiagSettingDialog*>(tb->parent());
  oldType_ = index.model()->data(index, Qt::EditRole).toString();
  int row = index.row();
  int rowCount = tb->rowCount();
  if (row == rowCount - 1) {
    tb->insertRow(row);
    tb->clearSelection();
    QTableWidgetItem *item = new QTableWidgetItem("functional");
    tb->setItem(row, DiagSettingDialog::O_Category, item);
    comboBox->addItems(dlg->getOutputTypes());
  }
  else {
    QStringList l;
    l << "" << oldType_;
    comboBox->addItems(l);
  }
  comboBox->setCurrentText(oldType_);
}

void OutputTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  QTableWidget *tb = static_cast<QTableWidget*>(this->parent());
  QString value = comboBox->currentText();
  if (value.isEmpty()) {
    int row = index.row();
    if (oldType_ == "meas" || oldType_ == "vec" || oldType_ == "dwc") {
      QString typeId;
      if (oldType_ == "meas") {
	typeId = oldType_ + "@=" +
	  tb->item(row, DiagSettingDialog::O_Id)->data(Qt::DisplayRole).toString();
      }
      else {
	typeId = oldType_ + "@=" +
	  tb->item(row, DiagSettingDialog::O_Signal)->data(Qt::DisplayRole).toString();
      }
      QString category = tb->item(row, DiagSettingDialog::O_Category)->data(Qt::DisplayRole).toString();
      emit updateOutputTypeId(typeId, "", category);
    }
    tb->removeRow(row);
  }
  else {
    model->setData(index, value, Qt::EditRole);
    for (int i = DiagSettingDialog::O_Id;
	 i < DiagSettingDialog::OutputColumnEnd; ++i) {
      QTableWidgetItem *item = tb->item(index.row(), i);
      if (item == 0) {
	item = new QTableWidgetItem();
	item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	tb->setItem(index.row(), i, item);
      }
    }
    QTableWidgetItem *item = tb->item(index.row(), DiagSettingDialog::O_Category);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    if (value == "meas") {
      QTableWidgetItem *item = tb->item(index.row(), DiagSettingDialog::O_Id);
      if (item == 0) {
	item = new QTableWidgetItem();
	tb->setItem(index.row(), DiagSettingDialog::O_Id, item);
      }
      item->setFlags(item->flags() | Qt::ItemIsEditable);
      tb->setCurrentItem(item);
    }
    else {
      QTableWidgetItem *item = tb->item(index.row(), DiagSettingDialog::O_Signal);
      if (item == 0) {
	item = new QTableWidgetItem();
	tb->setItem(index.row(), DiagSettingDialog::O_Signal, item);
      }
      item->setFlags(item->flags() | Qt::ItemIsEditable);
      tb->setCurrentItem(item);
    }
  }
}

OutputIdDelegate::OutputIdDelegate(QWidget *parent)
  : QItemDelegate(parent)
{
}

QWidget* OutputIdDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
  QComboBox *editor = new QComboBox(parent);
  QTableWidget * tb = qobject_cast<QTableWidget*>(this->parent());
  DiagSettingDialog *dlg = qobject_cast<DiagSettingDialog*>(tb->parent());
  editor->addItems(dlg->getSimStopIds());
  return editor;
}

void OutputIdDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
  QTableWidget *tb = qobject_cast<QTableWidget*>(this->parent());
  oldId_ = index.model()->data(index, Qt::EditRole).toString();
  comboBox->setCurrentText(oldId_);
}

void OutputIdDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  QTableWidget *tb = static_cast<QTableWidget*>(this->parent());
  QString value = comboBox->currentText();
  QTableWidgetItem *item = tb->item(index.row(), DiagSettingDialog::O_Type);
  QString type = item->data(Qt::DisplayRole).toString();
  item = tb->item(index.row(), DiagSettingDialog::O_Category);
  QString category = item->data(Qt::DisplayRole).toString();
  QString oldTypeId;
  if (!oldId_.isEmpty()) {
    oldTypeId = type + "@=" + oldId_;
  }
  QString newTypeId = type + "@=" + value;
  model->setData(index, value, Qt::EditRole);
  emit updateOutputTypeId(oldTypeId, newTypeId, category);
}

OutputSignalDelegate::OutputSignalDelegate(QWidget *parent)
    :QItemDelegate(parent)
{
}

QWidget* OutputSignalDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
  QLineEdit *l = new QLineEdit(parent);
  QRegExp r(".+");
  QRegExpValidator *re = new QRegExpValidator(r, l);
  l->setValidator(re);
  return l;
}

void OutputSignalDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QItemDelegate::setEditorData(editor, index);
  oldSignal_ = index.model()->data(index, Qt::EditRole).toString();
}

void OutputSignalDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
  QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
  QTableWidget *tb = static_cast<QTableWidget*>(this->parent());
  QString value = lineEdit->text();
  QTableWidgetItem *item = tb->item(index.row(), DiagSettingDialog::O_Type);
  QString type = item->data(Qt::DisplayRole).toString();
  QString oldTypeSignal;
  if (!oldSignal_.isEmpty()) {
    oldTypeSignal = type + "@=" + oldSignal_;
  }
  model->setData(index, value, Qt::EditRole);
  item = tb->item(index.row(), DiagSettingDialog::O_Category);
  QString category = item->data(Qt::DisplayRole).toString();
  QString newTypeSignal = type + "@=" + value;
  emit updateOutputTypeId(oldTypeSignal, newTypeSignal, category);
}

OutputCategoryDelegate::OutputCategoryDelegate(QWidget *parent)
  :QItemDelegate(parent)
{
}

QWidget *OutputCategoryDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
  QComboBox *editor = new QComboBox(parent);

  QStringList strs;
  strs << "functional" << "detection" << "latent_detection";
  editor->addItems(strs);
  return editor;
}

void OutputCategoryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
  QItemDelegate::setEditorData(editor, index);
  oldCategory_ = index.model()->data(index, Qt::EditRole).toString();
  comboBox->setCurrentText(oldCategory_);
}

void OutputCategoryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  QTableWidget *tb = static_cast<QTableWidget*>(this->parent());
  QString category = comboBox->currentText();
  int row = index.row();
  QString oldCategory = oldCategory_;
  QString type =  tb->item(row, DiagSettingDialog::O_Type)->data(Qt::DisplayRole).toString();
  QString typeId;
  if (type == "meas") {
    typeId = type + "@=" + tb->item(row, DiagSettingDialog::O_Id)->data(Qt::DisplayRole).toString();
  }
  else  {
    typeId = type + "@=" + tb->item(row, DiagSettingDialog::O_Signal)->data(Qt::DisplayRole).toString();
  }
  model->setData(index, category, Qt::DisplayRole);
  emit updateOutputCategory(typeId, oldCategory, category);
}
/**
 *  end of delegates implementation
 */

DiagSettingDialog::DiagSettingDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DiagSettingDialog), qAf_(0)
{
  ui->setupUi(this);
  failureModeIdDelegate_ = new FailureModeIdDelegate(this->ui->tableWidgetFailureMode);
  failureModeIdLabelDelegate_ = new FailureModeIdLabelDelegate(this->ui->tableWidgetFailureMode);
  ui->tableWidgetFailureMode->setItemDelegateForColumn(F_Id, failureModeIdDelegate_);
  ui->tableWidgetFailureMode->setItemDelegateForColumn(F_IdLabel, failureModeIdLabelDelegate_);

  QTableWidgetItem *item = ui->tableWidgetFailureMode->item(0, F_IdLabel);
  if (item == 0) {
    item = new QTableWidgetItem();
    ui->tableWidgetFailureMode->setItem(0, F_IdLabel, item);
  }
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);

  outputTypeDelegate_ = new OutputTypeDelegate(this->ui->tableWidgetOutput);
  outputIdDelegate_ = new OutputIdDelegate(this->ui->tableWidgetOutput);
  outputSignalDelegate_ = new OutputSignalDelegate(this->ui->tableWidgetOutput);
  outputCategoryDelegate_ = new OutputCategoryDelegate(this->ui->tableWidgetOutput);
  ui->tableWidgetOutput->setItemDelegateForColumn(O_Type, outputTypeDelegate_);
  ui->tableWidgetOutput->setItemDelegateForColumn(O_Id, outputIdDelegate_);
  ui->tableWidgetOutput->setItemDelegateForColumn(O_Signal, outputSignalDelegate_);
  ui->tableWidgetOutput->setItemDelegateForColumn(O_Category, outputCategoryDelegate_);

  for (int i = O_Id; i < OutputColumnEnd ; ++i) {
    item = ui->tableWidgetOutput->item(0, i);
    if (item == 0) {
      item = new QTableWidgetItem();
      ui->tableWidgetOutput->setItem(0, i, item);
    }
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  }
  
  connect(failureModeIdDelegate_, SIGNAL(updateFailureModeId(const QString &, const QString&)),
          this, SLOT(onUpdateFailureModeId(const QString &, const QString&)));
  connect(failureModeIdLabelDelegate_, SIGNAL(updateFailureModeIdLabel(const QString &, const QString&)),
          this, SLOT(onUpdateFailureModeIdLabel(const QString &, const QString&)));
  connect(outputTypeDelegate_, SIGNAL(updateOutputTypeId(const QString &, const QString&, const QString&)),
          this, SLOT(onUpdateOutputTypeId(const QString &, const QString&, const QString&)));
  connect(outputIdDelegate_, SIGNAL(updateOutputTypeId(const QString &, const QString&, const QString&)),
          this, SLOT(onUpdateOutputTypeId(const QString &, const QString&, const QString&)));
    connect(outputSignalDelegate_, SIGNAL(updateOutputTypeId(const QString &, const QString&, const QString&)),
          this, SLOT(onUpdateOutputTypeId(const QString &, const QString&, const QString&)));
  connect(outputCategoryDelegate_, SIGNAL(updateOutputCategory(const QString &, const QString&, const QString &)),
          this, SLOT(onUpdateOutputCategory(const QString &, const QString&, const QString &)));
}

DiagSettingDialog::~DiagSettingDialog()
{
  delete ui;
}

void DiagSettingDialog::cleanUiContain()
{
  // clear ui
  ui->checkBox->setChecked(false);
  ui->comboBoxUndetectedFaults->setCurrentIndex(0);

  ui->groupBox->setChecked(false);
  ui->lineEditFile->setText("");
  ui->comboBoxRMS->setCurrentIndex(0);
  ui->comboBoxKrf->setCurrentIndex(0);
  ui->comboBoxKmpf->setCurrentIndex(0);
  int rowCount = ui->tableWidgetFailureMode->rowCount();
  while (rowCount > 1) {
    ui->tableWidgetFailureMode->removeRow(0);
    rowCount = ui->tableWidgetFailureMode->rowCount();
  }
  rowCount = ui->tableWidgetOutput->rowCount();
  while (rowCount > 1) {
    ui->tableWidgetOutput->removeRow(0);
    rowCount = ui->tableWidgetOutput->rowCount();
  }
  // setting clear
  setting_.clear();
  currentFailureModeId_ = "";
}

void DiagSettingDialog::setAfData(QAfData *qAf)
{
  qAf_ = qAf;
  cleanUiContain();
  // const Document& configJson = qAf->configJson();
  setting_.buildDiagSetting(qAf->configJson());
  if (!setting_.enabled_) return;
  ui->checkBox->setChecked(true);
  QString s = setting_.undetectedFaults_.c_str();
  int choise = (s == "") ? 0 : (s == "assumed_safe" ? 1 : 2);
  ui->comboBoxUndetectedFaults->setCurrentIndex(choise);

  if (!setting_.vcFsmExport_.enabled_) {
    ui->groupBox->setChecked(false);
  }
  else {
    ui->groupBox->setChecked(true);
    QString s = setting_.vcFsmExport_.vcFsmExportFile_.c_str();
    ui->lineEditFile->setText(s);
    boost::logic::tribool b = setting_.vcFsmExport_.reportMeasuredSafe_;
    int choise = (b ? 1 : ((!b) ? 2 : 0));
    ui->comboBoxRMS->setCurrentIndex(choise);
    b = setting_.vcFsmExport_.reportDcKrf_;
    choise = (b ? 1 : ((!b) ? 2 : 0));
    ui->comboBoxKrf->setCurrentIndex(choise);
    b = setting_.vcFsmExport_.reportDcKmpf_;
    choise = (b ? 1 : ((!b) ? 2 : 0));
    ui->comboBoxKmpf->setCurrentIndex(choise);
  }
  for (auto &e : setting_.failureModes_) {
    string id = e.second.id_, idLabel = e.second.idLabel_;
    int rowCount = ui->tableWidgetFailureMode->rowCount();
    ui->tableWidgetFailureMode->insertRow(rowCount-1);
    QTableWidgetItem * i0 = new QTableWidgetItem(id.c_str()),
      *i1 = new QTableWidgetItem(idLabel.c_str());
    ui->tableWidgetFailureMode->setItem(rowCount - 1, 0, i0);
    ui->tableWidgetFailureMode->setItem(rowCount - 1, 1, i1);
  }
  if (!setting_.failureModes_.empty()) {
    QString id = setting_.failureModes_.begin()->first.c_str();
    updateTableWidgetOutput(id);
  }
}

void DiagSettingDialog::restoreAfData()
{
  // for undetected_faults
  setting_.undetectedFaults_ = ui->comboBoxUndetectedFaults->currentText().toStdString();
  // for vc fsm export
  if (ui->groupBox->isChecked()) {
    setting_.vcFsmExport_.enabled_ = true;
    setting_.vcFsmExport_.vcFsmExportFile_ = ui->lineEditFile->text().toStdString();
    int choise = ui->comboBoxRMS->currentIndex();
    setting_.vcFsmExport_.reportMeasuredSafe_ = (choise == 0) ? tribool(indeterminate) : ((choise == 1)? tribool(true) : tribool(false));

    choise = ui->comboBoxKrf->currentIndex();
    setting_.vcFsmExport_.reportDcKrf_ = ((choise == 0) ? tribool(indeterminate) : ((choise == 1)? tribool(true) : tribool(false)));
    choise = ui->comboBoxKmpf->currentIndex();
    setting_.vcFsmExport_.reportDcKmpf_ = (choise == 0) ? tribool(indeterminate) : ((choise == 1)? tribool(true) : tribool(false));   
  }
  else {
    setting_.vcFsmExport_.enabled_ = false;
  }
  Document::AllocatorType& allocator = qAf_->getAllocator();
  if (!ui->checkBox->checkState()) {
    qAf_->removeField("diagnostic_coverage");
    // need to disable in CF
    emit qAf_->configJsonChanged();
    return;
  }
  Value v = setting_.getValue(allocator);
  qAf_->setValueField("diagnostic_coverage", v);
  // need to disable in CF
  emit qAf_->configJsonChanged();
}

QStringList DiagSettingDialog::getOutputTypes()
{
  QStringList l;
  l << "" << "meas" << "vec" << "dwc";
  return l;
}

QStringList DiagSettingDialog::getSimStopIds()
{
  vector<string> strs = setting_.getSimStopIds();
  QStringList l;
  for (auto &e : strs) {
    l << e.c_str();
  }
  return l;
}

void DiagSettingDialog::updateTableWidgetOutput(const QString &id)
{
  if (id == currentFailureModeId_) return;
  currentFailureModeId_ = id;
  int rowCount = ui->tableWidgetOutput->rowCount();
  while (rowCount > 1) {
    ui->tableWidgetOutput->removeRow(0);
    rowCount = ui->tableWidgetOutput->rowCount();
  }
  FailureMode &f = setting_.failureModes_[id.toStdString()];
  for (auto &e : f.functional_) {
    int rowCount = ui->tableWidgetOutput->rowCount();
    ui->tableWidgetOutput->insertRow(rowCount-1);
    string delimiter = "@=";
    size_t pos = e.find(delimiter);
    string type = e.substr(0, pos);
    string id = e.substr(pos + 2);
    QStringList l;
    if (type == "meas") {
      SimStop simStop = setting_.simStops_[id];
      l << type.c_str() << id.c_str() << "" << simStop.measName_.c_str() << simStop.min_.c_str()
	<< simStop.max_.c_str() << "functional";
    }
    else {
      l << type.c_str() << "" << id.c_str() << "" << "" << "" << "functional";
    }
    for (int i = O_Type; i < OutputColumnEnd; ++i) {
      QTableWidgetItem *item = new QTableWidgetItem(l[i]);
      ui->tableWidgetOutput->setItem(rowCount - 1, i, item);
    }
    for (int i = O_Id; i < O_Category; ++i) {
      QTableWidgetItem *item = ui->tableWidgetOutput->item(rowCount - 1, i);
      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    }
    if (type == "meas") {
      QTableWidgetItem *item = ui->tableWidgetOutput->item(rowCount - 1, O_Id);
      item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    else {
      QTableWidgetItem *item = ui->tableWidgetOutput->item(rowCount - 1, O_Signal);
      item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
  }
  for (auto &e : f.detection_) {
    int rowCount = ui->tableWidgetOutput->rowCount();
    ui->tableWidgetOutput->insertRow(rowCount-1);
    string delimiter = "@=";
    size_t pos = e.find(delimiter);
    string type = e.substr(0, pos);
    string id = e.substr(pos + 2);
    QStringList l;
    if (type == "meas") {
      SimStop simStop = setting_.simStops_[id];
      l << type.c_str() << id.c_str() << "" << simStop.measName_.c_str() << simStop.min_.c_str()
	<< simStop.max_.c_str() << "detection";
    }
    else {
      l << type.c_str() << "" << id.c_str() << "" << "" << "" << "detection";
    }
    for (int i = O_Type; i < OutputColumnEnd; ++i) {
      QTableWidgetItem *item = new QTableWidgetItem(l[i]);
      ui->tableWidgetOutput->setItem(rowCount - 1, i, item);
    }
    for (int i = O_Id; i < O_Category; ++i) {
      QTableWidgetItem *item = ui->tableWidgetOutput->item(rowCount - 1, i);
      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    }
    if (type == "meas") {
      QTableWidgetItem *item = ui->tableWidgetOutput->item(rowCount - 1, O_Id);
      item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    else {
      QTableWidgetItem *item = ui->tableWidgetOutput->item(rowCount - 1, O_Signal);
      item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
  }
  for (auto &e : f.latentDetection_) {
    int rowCount = ui->tableWidgetOutput->rowCount();
    ui->tableWidgetOutput->insertRow(rowCount-1);
    string delimiter = "@=";
    size_t pos = e.find(delimiter);
    string type = e.substr(0, pos);
    string id = e.substr(pos + 2);
    QStringList l;
    if (type == "meas") {
      SimStop simStop = setting_.simStops_[id];
      l << type.c_str() << id.c_str() << "" << simStop.measName_.c_str() << simStop.min_.c_str()
	<< simStop.max_.c_str() << "latent_detection";
    }
    else {
      l << type.c_str() << "" << id.c_str() << "" << "" << "" << "latent_detection";
    }
    for (int i = O_Type; i < OutputColumnEnd; ++i) {
      QTableWidgetItem *item = new QTableWidgetItem(l[i]);
      ui->tableWidgetOutput->setItem(rowCount - 1, i, item);
    }
    for (int i = O_Type; i < OutputColumnEnd; ++i) {
      QTableWidgetItem *item = new QTableWidgetItem(l[i]);
      ui->tableWidgetOutput->setItem(rowCount - 1, i, item);
    }
    for (int i = O_Id; i < O_Category; ++i) {
      QTableWidgetItem *item = ui->tableWidgetOutput->item(rowCount - 1, i);
      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    }
    if (type == "meas") {
      QTableWidgetItem *item = ui->tableWidgetOutput->item(rowCount - 1, O_Id);
      item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    else {
      QTableWidgetItem *item = ui->tableWidgetOutput->item(rowCount - 1, O_Signal);
      item->setFlags(item->flags() | Qt::ItemIsEditable);
    }

  }
}

void DiagSettingDialog::removeFailureMode(const QString &id)
{
  setting_.failureModes_.erase(id.toStdString());
}

void DiagSettingDialog::insertFailureMode(const QString &id)
{
  setting_.failureModes_[id.toStdString()] = FailureMode();
  setting_.failureModes_[id.toStdString()].id_ = id.toStdString();
}

void DiagSettingDialog::changeFailureModeId(const QString &oldId, const QString &newId)
{
  FailureMode f = setting_.failureModes_[oldId.toStdString()];
  f.id_ = newId.toStdString();
  setting_.failureModes_.erase(oldId.toStdString());
  setting_.failureModes_[newId.toStdString()] = f;
}

void DiagSettingDialog::changeFailureModeIdLabel(const QString &id, const QString &idLabel)
{
  FailureMode &f = setting_.failureModes_[id.toStdString()];
  f.idLabel_ = idLabel.toStdString();
}

void DiagSettingDialog::removeFuctional(const QString &typeId)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.functional_.erase(typeId.toStdString());
}

void DiagSettingDialog::removeDetection(const QString &typeId)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.detection_.erase(typeId.toStdString());
}

void DiagSettingDialog::removeLatentDetection(const QString &typeId)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.latentDetection_.erase(typeId.toStdString());
}

void DiagSettingDialog::insertFuctional(const QString &typeId)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.functional_.insert(typeId.toStdString());
}

void DiagSettingDialog::insertDetection(const QString &typeId)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.detection_.insert(typeId.toStdString());
}

void DiagSettingDialog::insertLatentDetection(const QString &typeId)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.latentDetection_.insert(typeId.toStdString());
}

void DiagSettingDialog::onUpdateFailureModeId(const QString &oldId, const QString &newId)
{
  if (oldId.isEmpty() && newId.isEmpty()) return;
  if (oldId.isEmpty()) {
    insertFailureMode(newId);
  }
  else if (newId.isEmpty()) {
    removeFailureMode(oldId);
  }
  else {
    changeFailureModeId(oldId, newId);
  }
}

void DiagSettingDialog::onUpdateFailureModeIdLabel(const QString &id, const QString &idLabel)
{
  FailureMode &f = setting_.failureModes_[id.toStdString()];
  f.idLabel_ = idLabel.toStdString();
}

void DiagSettingDialog::onUpdateOutputTypeId(const QString &oldTypeId,
					     const QString &newTypeId,
					     const QString &category)
{
  if (oldTypeId.isEmpty() && newTypeId.isEmpty()) return;
  if (!oldTypeId.isEmpty()) {
    removeOutput(oldTypeId, category);
  }
  if (!newTypeId.isEmpty()) {
    insertOutput(newTypeId, category);
  }
}

void DiagSettingDialog::onUpdateOutputCategory(const QString &typeId, const QString &oldCategory, const QString &newCategory)
{
  removeOutput(typeId, oldCategory);
  insertOutput(typeId, newCategory);
}

void DiagSettingDialog::insertOutput(const QString &typeId, const QString &category)
{
  if (category == "functional") {
    insertFuctional(typeId);
  }
  else if (category == "detection") {
    insertDetection(typeId);
  }
  else if (category == "latent_detection") {
    insertLatentDetection(typeId);
  }

}

void DiagSettingDialog::removeOutput(const QString &typeId, const QString &category)
{
  if (category == "functional") {
    removeFuctional(typeId);
  }
  else if (category == "detection") {
    removeDetection(typeId);
  }
  else if (category == "latent_detection") {
    removeLatentDetection(typeId);
  }
}

void DiagSettingDialog::on_tableWidgetFailureMode_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
  int row = ui->tableWidgetFailureMode->rowCount();
  if (currentRow == row) return;
  QTableWidgetItem *item = ui->tableWidgetFailureMode->item(currentRow, F_Id);
  if (item) {
    QString id = item->data(Qt::DisplayRole).toString();
    updateTableWidgetOutput(id);
  }
}

void DiagSettingDialog::accept()
{
  restoreAfData();
  QDialog::accept();
}

