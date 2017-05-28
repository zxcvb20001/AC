#include "adjustdialog.h"
#include "ui_adjustdialog.h"
#include <QDebug>

AdjustDialog::AdjustDialog(const QVector<SingleAdjustData> *reportdata, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdjustDialog)
{
    ui->setupUi(this);
    setWindowTitle("详细温控信息");
    int col=0;
    ui->tableWidget->setRowCount(reportdata->size());
    for(QVector<SingleAdjustData>::const_iterator i=reportdata->constBegin();i!=reportdata->constEnd();++i){
        if(i->shown==false){
            continue;
        }
        if(i->mode==true)
            ui->tableWidget->setItem(col,0,new QTableWidgetItem("制冷"));
        else
            ui->tableWidget->setItem(col,0,new QTableWidgetItem("制热"));

        if(i->speed==LOW_WIND_SPEED)
            ui->tableWidget->setItem(col,1,new QTableWidgetItem("低风"));
        else if(i->speed==MID_WIND_SPEED)
            ui->tableWidget->setItem(col,1,new QTableWidgetItem("中风"));
        else
            ui->tableWidget->setItem(col,1,new QTableWidgetItem("高风"));

        ui->tableWidget->setItem(col,2,new QTableWidgetItem(tr("%1").arg(i->starttemp)));
        ui->tableWidget->setItem(col,3,new QTableWidgetItem(tr("%1").arg(i->stoptemp)));
        ui->tableWidget->setItem(col,4,new QTableWidgetItem(i->starttime.toString("yyyy-MM-dd hh:mm:ss")));
        ui->tableWidget->setItem(col,5,new QTableWidgetItem(i->stoptime.toString("yyyy-MM-dd hh:mm:ss")));
        ui->tableWidget->setItem(col,6,new QTableWidgetItem(tr("%1").arg(i->fee)));
        ++col;//move to next row
    }
}

AdjustDialog::~AdjustDialog()
{
    delete ui;
}
