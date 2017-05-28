#ifndef ADJUSTDIALOG_H
#define ADJUSTDIALOG_H

#include <QDialog>
#include "DataStruct.h"
#include <QVector>
#include <QTableWidgetItem>
#include "constant.h"

namespace Ui {
class AdjustDialog;
}

class AdjustDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdjustDialog(const QVector<SingleAdjustData> *reportdata,QWidget *parent = 0);
    ~AdjustDialog();

private:
    Ui::AdjustDialog *ui;
};

#endif // ADJUSTDIALOG_H
