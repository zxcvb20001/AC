#include "reportwindow.h"
#include "ui_reportwindow.h"
#include <QDebug>

ReportWindow::ReportWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportWindow)
{
    ui->setupUi(this);
    setWindowTitle("报表");
    reportFromatGroup=new QButtonGroup(this);

    currentreportmode=0;
    currentroomnum=-1;

    connect(ui->reporttable,&QTableWidget::cellClicked,this,&ReportWindow::processClick);
    connect(ui->UpdateButton,&QPushButton::clicked,this,&ReportWindow::updateAll);
    connect(ui->ClearButton,&QPushButton::clicked,this,&ReportWindow::clearTableAndData);
    connect(ui->RoomNumberCombox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeRoomNum()));

    reportFromatGroup->addButton(ui->DayFormatButton,0);
    reportFromatGroup->addButton(ui->WeekFormatButton,1);
    reportFromatGroup->addButton(ui->MonthFormatButton,2);
    connect(ui->DayFormatButton,&QRadioButton::clicked,this,&ReportWindow::onReportFormatclick);
    connect(ui->WeekFormatButton,&QRadioButton::clicked,this,&ReportWindow::onReportFormatclick);
    connect(ui->MonthFormatButton,&QRadioButton::clicked,this,&ReportWindow::onReportFormatclick);
    connect(ui->DayChoose,&QDateEdit::dateChanged,this,&ReportWindow::fillTable);
    connect(ui->WeekChoose,SIGNAL(currentIndexChanged(int)),this,SLOT(fillTable()));
    connect(ui->MonthChoose,SIGNAL(currentIndexChanged(int)),this,SLOT(fillTable()));
}

ReportWindow::~ReportWindow(){
    delete ui;
}

void ReportWindow::fillTable(){
    double roomfee;
    int adjustcnt;
    int openclosecnt;
    int roomcnt=0;
    //set row count of the table
    ui->reporttable->clearContents();
    if(currentroomnum!=-1){
        ui->reporttable->setRowCount(1);
    }
    else{
        ui->reporttable->setRowCount(reportdata.size());
    }
    switch (currentreportmode) {
    case 0:
        for(QVector<SingleReportData>::iterator i=reportdata.begin();i!=reportdata.end();++i){
            //only show selected rooms info,-1 means no selected rooms
            if(currentroomnum!=-1&&currentroomnum!=i->roomid)
                continue;
            roomfee=0;
            adjustcnt=0;
            openclosecnt=0;
            QDate selectdate=ui->DayChoose->date();
            for(QVector<QDateTime>::iterator j=i->openclosedata.begin();j!=i->openclosedata.end();++j){
                //find openclose info which occured in the selected date
                if(j->date()==selectdate){
                    openclosecnt++;
                }
            }
            for(QVector<SingleAdjustData>::iterator j=i->adjustdata.begin();j!=i->adjustdata.end();++j){
                //find adjust info which occured in the selected date
                if(j->stoptime.date()==selectdate){
                    roomfee+=j->fee;
                    adjustcnt++;
                    j->shown=true;//need to show in the adjust dialog
                }
                else{
                    j->shown=false;
                }
            }
            ui->reporttable->setItem(roomcnt,0,new QTableWidgetItem(QString("%1").arg(i->roomid)));
            ui->reporttable->setItem(roomcnt,1,new QTableWidgetItem(QString("%1").arg(openclosecnt)));
            ui->reporttable->setItem(roomcnt,2,new QTableWidgetItem(QString("%1").arg(adjustcnt)));
            ui->reporttable->setItem(roomcnt,3,new QTableWidgetItem(QString("%1").arg(roomfee)));
            roomcnt++;
        }
        break;
    case 1:
        for(QVector<SingleReportData>::iterator i=reportdata.begin();i!=reportdata.end();++i){
            if(currentroomnum!=-1&&currentroomnum!=i->roomid)
                continue;
            roomfee=0;
            adjustcnt=0;
            openclosecnt=0;
            int selectdate=ui->WeekChoose->currentIndex()+1;
            for(QVector<QDateTime>::iterator j=i->openclosedata.begin();j!=i->openclosedata.end();++j){
                if(j->date().weekNumber()==selectdate){
                    openclosecnt++;
                }
            }
            for(QVector<SingleAdjustData>::iterator j=i->adjustdata.begin();j!=i->adjustdata.end();++j){
                if(j->stoptime.date().weekNumber()==selectdate){
                    roomfee+=j->fee;
                    adjustcnt++;
                    j->shown=true;
                }
                else{
                    j->shown=false;
                }
            }
            ui->reporttable->setItem(roomcnt,0,new QTableWidgetItem(QString("%1").arg(i->roomid)));
            ui->reporttable->setItem(roomcnt,1,new QTableWidgetItem(QString("%1").arg(openclosecnt)));
            ui->reporttable->setItem(roomcnt,2,new QTableWidgetItem(QString("%1").arg(adjustcnt)));
            ui->reporttable->setItem(roomcnt,3,new QTableWidgetItem(QString("%1").arg(roomfee)));
            roomcnt++;
        }
        break;
    case 2:
        for(QVector<SingleReportData>::iterator i=reportdata.begin();i!=reportdata.end();++i){
            if(currentroomnum!=-1&&currentroomnum!=i->roomid)
                continue;

            roomfee=0;
            adjustcnt=0;
            openclosecnt=0;
            int selectdate=ui->MonthChoose->currentIndex()+1;
            for(QVector<QDateTime>::iterator j=i->openclosedata.begin();j!=i->openclosedata.end();++j){
                if(j->date().month()==selectdate){
                    openclosecnt++;
                }
            }
            for(QVector<SingleAdjustData>::iterator j=i->adjustdata.begin();j!=i->adjustdata.end();++j){
                if(j->stoptime.date().month()==selectdate){
                    roomfee+=j->fee;
                    adjustcnt++;
                    j->shown=true;
                }
                else{
                    j->shown=false;
                }
            }
            ui->reporttable->setItem(roomcnt,0,new QTableWidgetItem(QString("%1").arg(i->roomid)));
            ui->reporttable->setItem(roomcnt,1,new QTableWidgetItem(QString("%1").arg(openclosecnt)));
            ui->reporttable->setItem(roomcnt,2,new QTableWidgetItem(QString("%1").arg(adjustcnt)));
            ui->reporttable->setItem(roomcnt,3,new QTableWidgetItem(QString("%1").arg(roomfee)));
            roomcnt++;
        }
        break;
    default:
        break;
    }
}

void ReportWindow::updateData(){
    emit getReportData(reportdata);
}

void ReportWindow::updateAll(){
    updateData();
    fillTable();
}

void ReportWindow::clearTableAndData(){
    emit clearAllData();//send clear signal to data controller
    ui->reporttable->clearContents();
    reportdata.clear();
}

void ReportWindow::changeRoomNum(){
    currentroomnum=ui->RoomNumberCombox->currentIndex()-1;
    fillTable();
}

void ReportWindow::processClick(int row,int col){
    if(col==2){
        AdjustDialog adjustdialog(&reportdata[row].adjustdata,this);
        adjustdialog.exec();
    }
}

void ReportWindow::onReportFormatclick(){
    ui->RoomNumberCombox->setCurrentIndex(0);
    switch (reportFromatGroup->checkedId()) {
    case 0:
        currentreportmode=0;
        ui->SelectStackedWidget->setCurrentIndex(0);//day
        fillTable();
        break;
    case 1:
        currentreportmode=2;
        ui->SelectStackedWidget->setCurrentIndex(1);//week
        fillTable();
        break;
    case 2:
        currentreportmode=1;
        ui->SelectStackedWidget->setCurrentIndex(2);//month
        fillTable();
        break;
    default:
        break;
    }
}
