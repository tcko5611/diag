#ifndef DIAGRESULTDIALOG_H /* -*-c++-*- */
#define DIAGRESULTDIALOG_H

#include <QDialog>
#include "rapidjson/document.h"
using namespace rapidjson;

namespace Ui {
class DiagResultDialog;
}

class DiagResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DiagResultDialog(QWidget *parent = 0);
    ~DiagResultDialog();
    void settingTables(const QString &fN, Document &config, Document &flist);
private:
    Ui::DiagResultDialog *ui;
};

#endif // DIAGRESULTDIALOG_H
