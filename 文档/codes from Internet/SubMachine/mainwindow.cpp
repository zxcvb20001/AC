#include "mainwindow.h"
#include "ui_mainwindow.h"
#pragma execution_character_set("utf-8")


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qsrand(time(NULL));
    ui->setupUi(this);
    setWindowTitle("空调");

    isopen=false;
    //use button group to make wind speed buttons exclusive
    windSpeedGroup=new QButtonGroup(this);
    windSpeedGroup->addButton(ui->LowWind);
    windSpeedGroup->addButton(ui->MidWind);
    windSpeedGroup->addButton(ui->HighWind);
    connect(ui->LowWind,&QPushButton::clicked,this,&MainWindow::onWindSpeedClick);
    connect(ui->MidWind,&QPushButton::clicked,this,&MainWindow::onWindSpeedClick);
    connect(ui->HighWind,&QPushButton::clicked,this,&MainWindow::onWindSpeedClick);

    connect(ui->OpenClose,&QPushButton::clicked,this,&MainWindow::handleOpenClose);
    connect(ui->IncreaseTemp,&QPushButton::clicked,this,&MainWindow::increaseTargetTemp);
    connect(ui->DecreaseTemp,&QPushButton::clicked,this,&MainWindow::decreaseTargetTemp);

    regresstemptimer=new QTimer(this);
    regresstemptimer->setInterval(10000);//regress temperature per 10 sec
    connect(regresstemptimer,&QTimer::timeout,this,&MainWindow::regressTemp);

    setdelaytimer=new QTimer(this);
    setdelaytimer->setSingleShot(true);
    setdelaytimer->setInterval(1000);//temperature will update until 1 sec with no operation
    connect(setdelaytimer,&QTimer::timeout,this,&MainWindow::changeTargetTemp);

    judgetimer=new QTimer(this);
    judgetimer->setInterval(1000);
    connect(judgetimer,&QTimer::timeout,this,&MainWindow::judgeTemp);

    //connect with net controller
    connect(&netcon,&netController::recvOpenAck,this,&MainWindow::OpenMachine);
    connect(&netcon,&netController::recvWind,this,&MainWindow::ReceiveWind);
    connect(&netcon,&netController::compelCLose,this,&MainWindow::CloseMachine);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRoomID(int newval){
    roomid=newval;
    ui->RoomID->setText(tr("%1").arg(roomid));
}

void MainWindow::openFailed(){
    QMessageBox::warning(this,"开机失败","无法连接中央空调");
}

void MainWindow::onWindSpeedClick(){
    //change wind speed according to current checked button
    if(windSpeedGroup->checkedId()==-2){
        changeWindSpeed(0);
    }
    else if(windSpeedGroup->checkedId()==-3){
        changeWindSpeed(1);
    }
    else{
        changeWindSpeed(2);
    }
}

void MainWindow::OpenMachine(bool mode){
    currentmode=mode;
    targettemp=temp_targettemp=25;
    ui->SetTemp->setText(tr("%1").arg(targettemp));
    if(currentmode==0){
        ui->CurrentMode->setText("制冷");
    }
    else{
        ui->CurrentMode->setText("供暖");
    }

    if(isopen==false){
        isopen=true;
        ui->OpenClose->setText("关机");
        //enable all display widget
        ui->IncreaseTemp->setEnabled(true);
        ui->DecreaseTemp->setEnabled(true);

        ui->LowWind->setEnabled(true);
        ui->MidWind->setEnabled(true);
        ui->MidWind->setChecked(true);
        ui->HighWind->setEnabled(true);
        changeWindSpeed(1);//change wind speed to middle

        usedenergy=0;
        ui->UsedEnergy->setText("0.0");
        usedfee=0;
        ui->UsedFee->setText("0.0");

        generateRoomTemp();
        receivecnt=1;
        judgetimer->start();
        regresstemptimer->start();
    }
}

void MainWindow::CloseMachine(){
    isopen=false;
    ui->OpenClose->setText("开机");
    //disable all display widget
    ui->IncreaseTemp->setEnabled(false);
    ui->DecreaseTemp->setEnabled(false);
    ui->LowWind->setEnabled(false);
    ui->MidWind->setEnabled(false);
    ui->HighWind->setEnabled(false);
    ui->CurrentMode->setText("-----");
    ui->CurrentTemp->setText("--");
    ui->SetTemp->setText("--");
    ui->UsedEnergy->setText("--");
    ui->UsedFee->setText("--");
    //stop all timer
    judgetimer->stop();
    regresstemptimer->stop();
}

void MainWindow::ReceiveWind(bool mode){
    if(isopen==false)
        return;
    QString showstr;
    receivecnt=(receivecnt+1)%15;
    //1 wind packet represent 1 sec wind
    //high speed change temperature per 1sec,mid speed change per 3sec,and low speed change per 5sec
    if(currentwindspeed==0&&receivecnt%5==0||currentwindspeed==1&&receivecnt%3==0||currentwindspeed==2){
        if(mode==0){
            roomtemp-=1;
        }
        else{
            roomtemp+=1;
        }
        ui->CurrentTemp->setText(showstr.setNum(roomtemp));
    }
    //calculate fee and energy
    if(currentwindspeed==0){
        usedenergy+=0.8/60.0;
        usedfee+=5*0.8/60.0;
    }
    else if(currentwindspeed==1){
        usedenergy+=1.0/60.0;
        usedfee+=5*1.0/60.0;
    }
    else{
        usedenergy+=1.3/60.0;
        usedfee+=5*1.3/60.0;
    }

    ui->UsedEnergy->setText(showstr.setNum(usedenergy,'g',1));
    ui->UsedFee->setText(showstr.setNum(usedfee,'g',1));
    //report room temp to central aircondition
    netcon.sendRoomTemp(roomid,roomtemp,currentwindspeed);
}

void MainWindow::handleOpenClose(){
    if(isopen==false){
        if(netcon.preSendOpenInfo(roomid)==false){
            openFailed();
        }
    }
    else{
        netcon.sendCloseInfo(roomid);
        CloseMachine();
    }
}

void MainWindow::increaseTargetTemp(){
    if(currentmode==0){//cole mode
        if(temp_targettemp<25){
            temp_targettemp++;
            ui->SetTemp->setText(tr("%1").arg(temp_targettemp));
            setdelaytimer->start();
        }
    }
    else{
        if(temp_targettemp<35){
            temp_targettemp++;
            ui->SetTemp->setText(tr("%1").arg(temp_targettemp));
            setdelaytimer->start();
        }
    }
}

void MainWindow::decreaseTargetTemp(){
    if(currentmode==0){//cold mode
        if(temp_targettemp>18){
            temp_targettemp--;
            ui->SetTemp->setText(tr("%1").arg(temp_targettemp));
            setdelaytimer->start();
        }
    }
    else{
        if(temp_targettemp>25){
            temp_targettemp--;
            ui->SetTemp->setText(tr("%1").arg(temp_targettemp));
            setdelaytimer->start();
        }
    }
}

void MainWindow::changeWindSpeed(int newspeed){
    currentwindspeed=newspeed;
}

void MainWindow::generateRoomTemp(){
    //generate room temperature based on current work mode
    if(currentmode==0){
        roomtemp=targettemp+qrand()%5+1;
    }
    else{
        roomtemp=targettemp-qrand()%5-1;
    }
    ui->CurrentTemp->setText(tr("%1").arg(roomtemp));
    netcon.sendRoomTemp(roomid,roomtemp,currentwindspeed);
}

void MainWindow::regressTemp(){
    //regress room temperature based on current work mode
    if(currentmode==0){
        if(roomtemp<=35){
            roomtemp++;
        }
    }
    else{
        if(roomtemp>18){
            roomtemp--;
        }
    }
    ui->CurrentTemp->setText(tr("%1").arg(roomtemp));
    netcon.sendRoomTemp(roomid,roomtemp,currentwindspeed);
}


void MainWindow::changeTargetTemp(){
    targettemp=temp_targettemp;
}

void MainWindow::judgeTemp(){
    //compare current temperature with target temperature to judge the need of wind request
    if(currentmode==0&&roomtemp>targettemp+1||currentmode==1&&roomtemp<targettemp-1){
        netcon.sendWindRequest(roomid,targettemp,currentwindspeed);
    }
}
