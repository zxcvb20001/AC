#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "netcontrollor.h"
#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //initial setting
    isopen=false;
    sendwindtimer=new QTimer(this);
    sendwindtimer->setInterval(1000);//send wind every sec
    connect(sendwindtimer,&QTimer::timeout,this,&MainWindow::sendWindToSubMachine);

    //ui
    ui->setupUi(this);
    workModeGroup=new QButtonGroup(this);
    workModeGroup->addButton(ui->ColdModeButton,0);
    workModeGroup->addButton(ui->WarmModeButton,1);

    connect(ui->ColdModeButton,&QPushButton::clicked,this,&MainWindow::onModeClick);
    connect(ui->WarmModeButton,&QPushButton::clicked,this,&MainWindow::onModeClick);
    connect(ui->OpenCloseButton,&QPushButton::clicked,this,&MainWindow::openCloseCentralMachine);
    connect(ui->IncreaseTButton,&QPushButton::clicked,this,&MainWindow::increaseCenralT);
    connect(ui->DecreaseTButton,&QPushButton::clicked,this,&MainWindow::decreaseCenralT);

    //controllor
    datacontrollor=new DataControllor(this);
    netcontrollor=new NetControllor(this);
    monitorwindow=new MonitorWindow(&submacinfo);
    reportwindow=new ReportWindow;
    connect(netcontrollor,&NetControllor::subMachineStart,this,&MainWindow::subMachineStart);
    connect(netcontrollor,&NetControllor::subMachineStop,this,&MainWindow::subMachineStop);
    connect(netcontrollor,&NetControllor::subMachineRequestWind,this,&MainWindow::subMachineRequestWind);
    connect(netcontrollor,&NetControllor::subMachineStopWind,this,&MainWindow::subMachineStopWind);
    connect(netcontrollor,&NetControllor::updateRoomTemp,this,&MainWindow::updateRoomTemp);
    connect(ui->MonitorButton,&QPushButton::clicked,this,&MainWindow::setMonitorVisible);
    connect(ui->ReportButton,&QPushButton::clicked,this,&MainWindow::setReportVisible);
    connect(reportwindow,&ReportWindow::getReportData,datacontrollor,&DataControllor::getAllData);
    connect(reportwindow,&ReportWindow::clearAllData,datacontrollor,&DataControllor::clearAllData);

    //make monitor table and report table not visible
    monitorwindow->setVisible(false);
    reportwindow->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onModeClick(){
    if(workModeGroup->checkedId()==0){
        workmode=COLDMODE;
        setCentralT(currenteentralT);
    }
    else if(workModeGroup->checkedId()==1){
        workmode=WARMMODE;
        setCentralT(currenteentralT);
    }
    //restart all connected SubMachine
    for(QMap<int,SingleSubMacInfo>::iterator i=submacinfo.begin();i!=submacinfo.end();++i){
        netcontrollor->sendMode(i.key(),workmode);
        i.value().targetT=25;
    }
    waitinglist.clear();
    serveinglist.clear();
}

void MainWindow::subMachineStart(int roomid){
    if(submacinfo.find(roomid)==submacinfo.end()){
        submacinfo.insert(roomid,SingleSubMacInfo());
    }
    netcontrollor->sendMode(roomid,workmode);
}

void MainWindow::subMachineStop(int roomid){
    if(submacinfo.find(roomid)!=submacinfo.end()){
        submacinfo.remove(roomid);
        waitinglist.removeAll(roomid);
        serveinglist.removeAll(roomid);
        datacontrollor->addOpenClose(roomid,QDateTime::currentDateTime());
    }
}

void MainWindow::subMachineRequestWind(int roomid,int targettemp,int windspeed){
    QMap<int,SingleSubMacInfo>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){
        ptr.value().targetT=targettemp;
        ptr.value().speed=windspeed;
        //add to waitting list
        if(waitinglist.contains(roomid)==false&&serveinglist.contains(roomid)==false){
            waitinglist.push_back(roomid);
        }
    }
    else{
        netcontrollor->sendCompelStop(roomid);
    }
}

void MainWindow::subMachineStopWind(int roomid){
    QMap<int,SingleSubMacInfo>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){
        waitinglist.removeAll(roomid);
        if(serveinglist.contains(roomid)){
            serveinglist.removeAll(roomid);
            double usedfee,usedenergy;
            //calculate fee and energy according to start time
            if(ptr.value().speed==LOW_WIND_SPEED){
                usedenergy=((ptr.value().adjuststarttime.time().secsTo(QTime::currentTime()))/60.0+1.0)*0.8;
            }
            else if(ptr.value().speed==MID_WIND_SPEED){
                usedenergy=((ptr.value().adjuststarttime.time().secsTo(QTime::currentTime()))/60.0+1.0)*1.0;
            }
            else{
                usedenergy=((ptr.value().adjuststarttime.time().secsTo(QTime::currentTime()))/60.0+1.0)*1.3;
            }
            usedfee=5.0*usedenergy;
            SingleAdjustData addata(workmode,ptr.value().speed,ptr.value().adjuststarttemp,ptr.value().currentT,ptr.value().adjuststarttime,QDateTime::currentDateTime(),usedfee);
            datacontrollor->addAdjust(roomid,addata);
        }
    }
    else{
        netcontrollor->sendCompelStop(roomid);
    }
}

void MainWindow::updateRoomTemp(int roomid,int roomtemp,int windspeed){
    QMap<int,SingleSubMacInfo>::iterator ptr;
    if((ptr=submacinfo.find(roomid))!=submacinfo.end()){
        ptr.value().currentT=roomtemp;
        ptr.value().speed=windspeed;
        //stop send wind if the room is in target temperature
        if(ptr.value().currentT==ptr.value().targetT){
            subMachineStopWind(roomid);
        }
    }
    else{
        netcontrollor->sendCompelStop(roomid);
    }
}

void MainWindow::openCloseCentralMachine(){
    if(isopen==false){
        openCentralMachine();
    }
    else{
        closeCentralMachine();
    }
}

void MainWindow::setCentralT(int newval){
    //judge legitimacy of the new temperature
    if(workmode==COLDMODE&&newval>=18&&newval<=25){
        currenteentralT=newval;
    }
    else if(workmode==WARMMODE&&newval>=25&&newval<=30){
        currenteentralT=newval;
    }
    //judge legitimacy of current temperature
    if(workmode==COLDMODE){
        if(currenteentralT>25){
            currenteentralT=25;
        }
        else if(currenteentralT<18){
            currenteentralT=18;
        }
    }
    else if(workmode=WARMMODE){
        if(currenteentralT>30){
            currenteentralT=30;
        }
        else if(currenteentralT<25){
            currenteentralT=25;
        }
    }
    ui->TLabel->setText(tr("%1").arg(currenteentralT));
}

void MainWindow::openCentralMachine(){
    submacinfo.clear();
    waitinglist.clear();
    serveinglist.clear();
    //default work mode is depended on current month
    if(QDate::currentDate().month()>=5||QDate::currentDate().month()<=10){
        ui->ColdModeButton->setChecked(true);
        workmode=COLDMODE;
    }
    else{
        ui->WarmModeButton->setChecked(true);
        workmode=WARMMODE;
    }
    setCentralT(25);//default temperature
    ui->ColdModeButton->setEnabled(true);
    ui->WarmModeButton->setEnabled(true);
    ui->ReportButton->setEnabled(true);
    ui->MonitorButton->setEnabled(true);
    ui->IncreaseTButton->setEnabled(true);
    ui->DecreaseTButton->setEnabled(true);
    ui->OpenCloseButton->setText("关机");
    isopen=true;
    netcontrollor->startserver();
    sendwindtimer->start();
}

void MainWindow::closeCentralMachine(){
    for(QMap<int,SingleSubMacInfo>::iterator i=submacinfo.begin();i!=submacinfo.end();++i){
        netcontrollor->sendCompelStop(i.key());
    }
    netcontrollor->stopserver();
    ui->TLabel->setText("--");
    ui->ColdModeButton->setEnabled(false);
    ui->WarmModeButton->setEnabled(false);
    ui->ReportButton->setEnabled(false);
    ui->MonitorButton->setEnabled(false);
    ui->IncreaseTButton->setEnabled(false);
    ui->DecreaseTButton->setEnabled(false);
    ui->OpenCloseButton->setText("开机");
    sendwindtimer->stop();
    isopen=false;
}

void MainWindow::increaseCenralT(){
    setCentralT(currenteentralT+1);
}

void MainWindow::decreaseCenralT(){
    setCentralT(currenteentralT-1);
}

void MainWindow::setMonitorVisible(){
    monitorwindow->setVisible(true);
}

void MainWindow::setReportVisible(){
    //read data from database
    reportwindow->updateData();
    reportwindow->fillTable();
    //make report table visible
    reportwindow->setVisible(true);
}

void MainWindow::sendWindToSubMachine(){
    //if serving list is not full,add new machine from waitting list
    while(waitinglist.size()>0&&serveinglist.size()<3){
        serveinglist.push_back(waitinglist.front());
        submacinfo[waitinglist.front()].adjuststarttemp=submacinfo[waitinglist.front()].currentT;
        submacinfo[waitinglist.front()].adjuststarttime=QDateTime::currentDateTime();
        waitinglist.pop_front();
    }
    //send wind to all machine in serving list
    for(int i=0;i<serveinglist.size();++i){
        netcontrollor->sendWind(serveinglist[i],workmode);
        if(submacinfo[serveinglist[i]].speed==LOW_WIND_SPEED){
            submacinfo[serveinglist[i]].useE+=0.8/60.0;
            submacinfo[serveinglist[i]].useF+=5*0.8/60.0;
        }
        else if(submacinfo[serveinglist[i]].speed==MID_WIND_SPEED){
            submacinfo[serveinglist[i]].useE+=1.0/60.0;
            submacinfo[serveinglist[i]].useF+=5*1.0/60.0;
        }
        else{
            submacinfo[serveinglist[i]].useE+=1.3/60.0;
            submacinfo[serveinglist[i]].useF+=5*1.3/60.0;
        }
    }
}
