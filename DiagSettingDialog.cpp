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
  QTableWidget *tb = qobject_cast<QTableWidget*>(this->parent());
  QItemDelegate::setEditorData(editor,index);
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


MeasIdDelegate::MeasIdDelegate(QWidget *parent)
  :QItemDelegate(parent)
{
}


QWidget* MeasIdDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
  QComboBox *editor = new QComboBox(parent);
  QTableWidget * tb = qobject_cast<QTableWidget*>(this->parent());
  DiagSettingDialog *dlg = qobject_cast<DiagSettingDialog*>(tb->parent());
  editor->addItems(dlg->getSimStopIds());
  return editor;
}

void MeasIdDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
  QTableWidget *tb = qobject_cast<QTableWidget*>(this->parent());
  int row = index.row();
  int rowCount = tb->rowCount();
  if (row == rowCount -1) {
    tb->insertRow(row);
    tb->clearSelection();
    oldId_ = "";
    QTableWidgetItem *item = new QTableWidgetItem("functional");
    tb->setItem(row, DiagSettingDialog::M_Category, item);
  } else {
    QItemDelegate::setEditorData(editor, index);
    oldId_ = index.model()->data(index, Qt::EditRole).toString();
  }
  comboBox->setCurrentText(oldId_);
}

void MeasIdDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  QTableWidget *tb = static_cast<QTableWidget*>(this->parent());
  QString value = comboBox->currentText();
  QString oldId = oldId_;
  QTableWidgetItem *item= tb->item(index.row(), DiagSettingDialog::M_Category);
  QString category = item->data(Qt::DisplayRole).toString();
  if (value.isEmpty()) {
    int row = index.row();
    tb->removeRow(row);
  }
  else {
    model->setData(index, value, Qt::EditRole);
  }

  emit updateMeasId(oldId, value, category);
}

MeasCategoryDelegate::MeasCategoryDelegate(QWidget *parent)
  :QItemDelegate(parent)
{
}

QWidget *MeasCategoryDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
  QComboBox *editor = new QComboBox(parent);

  QStringList strs;
  strs << "functional" << "detection" << "latent_detection";
  editor->addItems(strs);
  return editor;
}

void MeasCategoryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
  QItemDelegate::setEditorData(editor, index);
  oldCategory_ = index.model()->data(index, Qt::EditRole).toString();
  comboBox->setCurrentText(oldCategory_);
}

void MeasCategoryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  QTableWidget *tb = static_cast<QTableWidget*>(this->parent());
  QString category = comboBox->currentText();
  int row = index.row();
  QString oldCategory = oldCategory_;
  QString id = tb->item(row, DiagSettingDialog::M_Id)->data(Qt::DisplayRole).toString();
  model->setData(index, category, Qt::DisplayRole);
  emit updateMeasCategory(id, oldCategory, category);
}

/**
 * end of delegates implementation
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
  item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable);
  measIdDelegate_ = new MeasIdDelegate(this->ui->tableWidgetMeas);
  measCategoryDelegate_ = new MeasCategoryDelegate(this->ui->tableWidgetMeas);
  ui->tableWidgetMeas->setItemDelegateForColumn(M_Id, measIdDelegate_);
  ui->tableWidgetMeas->setItemDelegateForColumn(M_Category, measCategoryDelegate_);

  for (int i = M_Check; i < MeasColumnEnd ; ++i) {
    item = ui->tableWidgetMeas->item(0, i);
    if (item == 0) {
      item = new QTableWidgetItem();
      ui->tableWidgetMeas->setItem(0, i, item);
    }
      item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable);
  }
  
  connect(failureModeIdDelegate_, SIGNAL(updateFailureModeId(const QString &, const QString&)),
          this, SLOT(onUpdateFailureModeId(const QString &, const QString&)));
  connect(failureModeIdLabelDelegate_, SIGNAL(updateFailureModeIdLabel(const QString &, const QString&)),
          this, SLOT(onUpdateFailureModeIdLabel(const QString &, const QString&)));

  connect(measIdDelegate_, SIGNAL(updateMeasId(const QString &, const QString&, const QString&)),
          this, SLOT(onUpdateMeasId(const QString &, const QString&, const QString&)));
  connect(measCategoryDelegate_, SIGNAL(updateMeasCategory(const QString &, const QString&, const QString &)),
          this, SLOT(onUpdateMeasCategory(const QString &, const QString&, const QString &)));
}

DiagSettingDialog::~DiagSettingDialog()
{
  delete ui;
}

void DiagSettingDialog::clear()
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
  rowCount = ui->tableWidgetMeas->rowCount();
  while (rowCount > 1) {
    ui->tableWidgetFailureMode->removeRow(0);
    rowCount = ui->tableWidgetMeas->rowCount();
  }
  // setting clear
  setting_.clear();
  currentFailureModeId_ = "";
}

void DiagSettingDialog::setAfData(QAfData *qAf)
{
  qAf_ = qAf;
  clear();
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
    updateTableWidgetMeas(id);
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
    emit qAf_->configJsonChanged();
    return;
  }
  Value v = setting_.getValue(allocator);
  qAf_->setValueField("diagnostic_coverage", v);
  emit qAf_->configJsonChanged();
}

QStringList DiagSettingDialog::getSimStopIds()
{
  vector<string> strs = setting_.getSimStopIds();
  QStringList l;
  l << "";
  for (auto &e : strs) {
    l << e.c_str();
  }
  return l;
}

void DiagSettingDialog::updateTableWidgetMeas(const QString &id)
{
  if (id == currentFailureModeId_) return;
  currentFailureModeId_ = id;
  int rowCount = ui->tableWidgetMeas->rowCount();
  while (rowCount > 1) {
    ui->tableWidgetMeas->removeRow(0);
    rowCount = ui->tableWidgetMeas->rowCount();
  }
  FailureMode &f = setting_.failureModes_[id.toStdString()];
  for (auto &e : f.functional_) {
    int rowCount = ui->tableWidgetMeas->rowCount();
    ui->tableWidgetMeas->insertRow(rowCount-1);
    string id = e.first;
    SimStop simStop = setting_.simStops_[id];
    QStringList l;
    l << id.c_str() << simStop.check_.c_str() << simStop.measName_.c_str()
      << simStop.min_.c_str() << simStop.max_.c_str() << "functional";
    for (int i = 0; i < 6; ++i) {
      QTableWidgetItem *item = new QTableWidgetItem(l[i]);
      ui->tableWidgetMeas->setItem(rowCount - 1, i, item);
    }
  }
  for (auto &e : f.detection_) {
    int rowCount = ui->tableWidgetMeas->rowCount();
    ui->tableWidgetMeas->insertRow(rowCount-1);
    string id = e.first;
    SimStop simStop = setting_.simStops_[id];
    QStringList l;
    l << id.c_str() << simStop.check_.c_str() << simStop.measName_.c_str()
      << simStop.min_.c_str() << simStop.max_.c_str() << "detection";
    for (int i = 0; i < 6; ++i) {
      QTableWidgetItem *item = new QTableWidgetItem(l[i]);
      ui->tableWidgetMeas->setItem(rowCount - 1, i, item);
    }
  }
  for (auto &e : f.latentDetection_) {
    int rowCount = ui->tableWidgetMeas->rowCount();
    ui->tableWidgetMeas->insertRow(rowCount-1);
    string id = e.first;
    SimStop simStop = setting_.simStops_[id];
    QStringList l;
    l << id.c_str() << simStop.check_.c_str() << simStop.measName_.c_str()
      << simStop.min_.c_str() << simStop.max_.c_str() << "latent_detection";
    for (int i = 0; i < 6; ++i) {
      QTableWidgetItem *item = new QTableWidgetItem(l[i]);
      ui->tableWidgetMeas->setItem(rowCount - 1, i, item);
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

void DiagSettingDialog::removeFuctional(const QString &id)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.functional_.erase(id.toStdString());
}

void DiagSettingDialog::removeDetection(const QString &id)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.detection_.erase(id.toStdString());
}

void DiagSettingDialog::removeLatentDetection(const QString &id)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.latentDetection_.erase(id.toStdString());
}

void DiagSettingDialog::insertFuctional(const QString &id, const QString &type)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.functional_[id.toStdString()] = make_pair(id.toStdString(), type.toStdString());
}

void DiagSettingDialog::insertDetection(const QString &id, const QString &type)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.detection_[id.toStdString()] = make_pair(id.toStdString(), type.toStdString());
}

void DiagSettingDialog::insertLatentDetection(const QString &id, const QString &type)
{
  FailureMode &f = setting_.failureModes_[currentFailureModeId_.toStdString()];
  f.latentDetection_[id.toStdString()] = make_pair(id.toStdString(), type.toStdString());
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

void DiagSettingDialog::onUpdateMeasId(const QString &oldId, const QString &newId, const QString& category)
{
  if (oldId.isEmpty() && newId.isEmpty()) return;
  if (!newId.isEmpty()) {
    insertMeas(newId, category);
  }
  if (!oldId.isEmpty()) {
    removeMeas(oldId, category);
  }
}

void DiagSettingDialog::onUpdateMeasCategory(const QString &id, const QString &oldCategory, const QString &newCategory)
{
  removeMeas(id, oldCategory);
  insertMeas(id, newCategory);
}

void DiagSettingDialog::insertMeas(const QString &id, const QString &category)
{
  if (category == "functional") {
    insertFuctional(id, "meas");
  }
  else if (category == "detection") {
    insertDetection(id, "meas");
  }
  else if (category == "latent_detection") {
    insertLatentDetection(id, "meas");
  }

}

void DiagSettingDialog::removeMeas(const QString &id, const QString &category)
{
  if (category == "functional") {
    removeFuctional(id);
  }
  else if (category == "detection") {
    removeDetection(id);
  }
  else if (category == "latent_detection") {
    removeLatentDetection(id);
  }
}

void DiagSettingDialog::on_tableWidgetFailureMode_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
  int row = ui->tableWidgetFailureMode->rowCount();
  if (currentRow == row) return;
  QTableWidgetItem *item = ui->tableWidgetFailureMode->item(currentRow, F_Id);
  if (item) {
    QString id = item->data(Qt::DisplayRole).toString();
    updateTableWidgetMeas(id);
  }
}

void DiagSettingDialog::accept()
{
  restoreAfData();
  QDialog::accept();
}
