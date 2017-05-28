#include "monitorwindow.h"
#include "ui_monitorwindow.h"
#include <QDebug>
#pragma execution_character_set("utf-8")

MonitorWindow::MonitorWindow(const QMap<int,SingleSubMacInfo> *dataptr,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonitorWindow)
{
    ui->setupUi(this);
    monitorinfo=dataptr;
    setWindowTitle("从控机运行状况");
    updatetimer=new QTimer(this);
    connect(updatetimer,&QTimer::timeout,this,&MonitorWindow::updatetable);
    connect(ui->updatefrequency,SIGNAL(valueChanged(double)),this,SLOT(changeFreq(double)));
    changeFreq(1);//default value is 1 sec
}

MonitorWindow::~MonitorWindow()
{
    delete ui;
}

void MonitorWindow::changeFreq(double newval){
    updatetimer->stop();
    updatetimer->setInterval(int(newval*1000));
    updatetimer->start();
}

void MonitorWindow::updatetable(){
    ui->infotable->clearContents();
    ui->infotable->setRowCount(monitorinfo->size());
    int currentrow=0;
    for(QMap<int,SingleSubMacInfo>::const_iterator i=monitorinfo->constBegin();i!=monitorinfo->constEnd();++i){
        for(int col=0;col<6;++col){
            QString temstring;
            switch (col) {
            case 0:
                ui->infotable->setItem(currentrow,col,new QTableWidgetItem(QString("%1").arg(i.key())));
                break;
            case 1:
                if(i.value().currentT>0)
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem(QString("%1").arg(i.value().currentT)));
                else
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem("--"));
                break;
            case 2:
                if(i.value().targetT>0)
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem(QString("%1").arg(i.value().targetT)));
                else
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem("--"));
                break;
            case 3:
                if(i.value().speed==LOW_WIND_SPEED){
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem("低风"));
                }
                else if(i.value().speed==MID_WIND_SPEED){
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem("中风"));
                }
                else if(i.value().speed==HIGH_WIND_SPEED){
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem("高风"));
                }
                else{
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem("----"));
                }
                break;
            case 4:
                if(i.value().useE>0)
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem(temstring.setNum(i.value().useE,'g',1)));
                else
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem("--"));
                break;
            case 5:
                if(i.value().useF>0)
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem(temstring.setNum(i.value().useF,'g',1)));
                else
                    ui->infotable->setItem(currentrow,col,new QTableWidgetItem("--"));
                break;
            default:
                break;
            }
        }
        currentrow++;
    }
}
