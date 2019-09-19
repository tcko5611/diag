#include "DiagSettingDialog.h"
#include "ui_DiagSettingDialog.h"

FaultModeDelegate::FaultModeDelegate(QWidget *parent)
  :QItemDelegate(parent)
{

}

QWidget* FaultModeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
  QLineEdit *lineEdit = new QLineEdit(parent);
  return lineEdit;
}

void FaultModeItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QTableWidget *tb = static_cast<QTableWidget*>(this->parent());
  int row = index.row();
  int rowCount = tb->rowCount();
  if(row == rowCount -1) {
    tb->insertRow(row);
    tb->setRowCount(rowCount + 1);
    tb->clearSelection();
  } else {
    QItemDelegate::setEditorData(editor,index);
  }
}

void FaultModeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
  QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
  if(lineEdit->text().isEmpty()) {
    QTableWidget *tb = static_cast<QTableWidget*>(this->parent());
    int row = index.row();
    int rowCount = tb->rowCount();
    tb->removeRow(row);
    tb->setRowCount(rowCount-1);
  } else {
    QItemDelegate::setModelData(editor, model, index);
  }
}

SimStop buildSimStop(const string & line)
{
  SimStop d;
  istringstream iss(line);
  vector<string> tokens;
  copy(istream_iterator<string>(iss),
       istream_iterator<string>(),
       back_inserter(tokens));
  for (int i = 1; i < tokens.size(); ++i) {
    if (tokens[i] == "-check") {
      d.check_ = tokens[++i];
    }
    else if (tokens[i] == "-meas_name") {
      d.measName_ = tokens[++i];
    }
    else if (tokens[i] == "-min") {
      d.min_ = tokens[++i];
    }
    else if (tokens[i] == "-max") {
      d.max_ = tokens[++i];
    }
    else if (tokens[i] == "-id") {
      d.id_ = tokens[++i];
    }
    else if (tokens[i] == "-error") {
      d.error_ = tokens[++i];
    }
  }
  return d;
}

DiagSettingDialog::DiagSettingDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DiagSettingDialog)
{
  ui->setupUi(this);
  faultModeDelegate = new FaultModeDelegate(ui->tableWidget_faultModel);
  ui->tableWidgetFaultMode->setItemDelegateForColumn(0, this->itemDelegate);
  QTableWidgetItem *item = ui->tableWidgetFaultMode->item(0, 1);
  if (item == 0) {
	item = new QTableWidgetItem();
	ui->tableWidget_faultModel->setItem(0, 1, item);
  }
  item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable);
}

DiagSettingDialog::~DiagSettingDialog()
{
  delete ui;
}

void DiagSettingDialog::setAfData(QAfData *qAf)
{
  // const Document& configJson = qAf->configJson();
  if (!qAf->hasMember("diagnostic_coverage")) return;
  ui->checkBox->setEnabled(true);
  QString s = qAf->getStringField("diagnostic_coverage", "undetected_faults");
  if (s == "assumed_safe") {
    ui->comboBoxUndetectedFaults->setCurrentIndex(0);
  }
  else {
    ui->comboBoxUndetectedFaults->setCurrentIndex(1);
  }
  if (!qAf->hasMember("diagnostic_coverage", "vc_fsm_export")) {
    ui->groupBox->setChecked(false);
  }
  else {
    QString s = qAf->getStringField("diagnostic_coverage", "vc_fsm_export",
                                    "vc_fsm_export_file");
    ui->lineEditFile->setText(s);
    bool s = qAf->getBoolField("diagnostic_coverage", "vc_fsm_export",
                               "report_measured_safe");
    
  }
  
}

void DiagSettingDialog::restoreAfData(QAfData* qAf)
{

}

void DiagSettingDialog::buildSimStops(const std::vector<std::string> &fNs)
{
  for (auto & fN : fNs) {
    ifstream in(fN);
    string line;
    while(std::getline(in, line)) {
      if (line.find("set_sim_stop") != string::npos) {
        SimStop d = buildSimStop(line);
        simStops_[d.id_] = d; 
      }
    }
  }
}
